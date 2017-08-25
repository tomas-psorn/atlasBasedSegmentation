#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkAffineTransform.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkMattesMutualInformationImageToImageMetricv4.h"


#include "itkCenteredTransformInitializer.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkCommand.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkBinaryImageToShapeLabelMapFilter.h"

#include "itkSubtractImageFilter.h"

#include "itkCommand.h"


const unsigned int Dim = 3;

typedef float   FloatPixelType;
typedef unsigned short  IntPixelType;

typedef itk::Image< FloatPixelType , Dim >  FloatImageType;
typedef itk::Image< IntPixelType , Dim >  IntImageType;

// pre/post process
typedef itk::CastImageFilter< IntImageType, FloatImageType > Int2FloatCasterType;
typedef itk::CastImageFilter< FloatImageType, IntImageType > Float2IntCasterType;
typedef itk::ImageFileReader< IntImageType > ReaderType;
typedef itk::ImageFileWriter< IntImageType > WriterType;
typedef itk::ResampleImageFilter <FloatImageType, FloatImageType> ResampleFilterType;


// registration tools
typedef itk::AffineTransform < double > TransformType;
typedef itk::RegularStepGradientDescentOptimizerv4< double > OptimizerType;
typedef itk::MattesMutualInformationImageToImageMetricv4<FloatImageType, FloatImageType> MetricType;
typedef itk::ImageRegistrationMethodv4< FloatImageType, FloatImageType, TransformType > RegistrationType;

typedef itk::IdentityTransform< double, Dim > IdentityTransformType;

typedef itk::SubtractImageFilter <FloatImageType, FloatImageType, FloatImageType> SubtractFilterType;

//typedef itk::BinaryImageToLabelMapFilter <IntImageType, IntImageType> ImageToLabelFilterType;
typedef itk::BinaryThresholdImageFilter <IntImageType, IntImageType> BinaryThresholdFilterType;

typedef itk::RescaleIntensityImageFilter< FloatImageType, IntImageType > RescalerType;

/*
* Multi res stuff
*/
template <typename TRegistration>
class RegistrationInterfaceCommand : public itk::Command
{
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // We then define \code{Self}, \code{Superclass}, \code{Pointer},
    // \code{New()} and a constructor in a similar fashion to the
    // \code{CommandIterationUpdate} class in Section
    // \ref{sec:MonitoringImageRegistration}.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
public:
    typedef  RegistrationInterfaceCommand   Self;
    typedef  itk::Command                   Superclass;
    typedef  itk::SmartPointer<Self>        Pointer;
    itkNewMacro( Self );

protected:
    RegistrationInterfaceCommand() {};

public:
    typedef   TRegistration      RegistrationType;
    typedef   RegistrationType * RegistrationPointer;
    typedef   itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
    typedef   OptimizerType * OptimizerPointer;
    void Execute( itk::Object * object, const itk::EventObject & event) ITK_OVERRIDE
    {
        if( !(itk::MultiResolutionIterationEvent().CheckEvent( &event ) ) )
        {
            return;
        }

        RegistrationPointer registration =
                static_cast<RegistrationPointer>( object );
        OptimizerPointer optimizer =  static_cast< OptimizerPointer >(
                registration->GetModifiableOptimizer() );

        unsigned int currentLevel = registration->GetCurrentLevel();
        typename RegistrationType::ShrinkFactorsPerDimensionContainerType shrinkFactors =
                registration->GetShrinkFactorsPerDimension( currentLevel );
        typename RegistrationType::SmoothingSigmasArrayType smoothingSigmas =
                registration->GetSmoothingSigmasPerLevel();

        std::cout << "-------------------------------------" << std::endl;
        std::cout << " Current level = " << currentLevel << std::endl;
        std::cout << "    shrink factor = " << shrinkFactors << std::endl;
        std::cout << "    smoothing sigma = " << smoothingSigmas[currentLevel] << std::endl;
        std::cout << std::endl;

        if ( registration->GetCurrentLevel() == 0 )
        {
            optimizer->SetLearningRate( 16.00 );
            optimizer->SetMinimumStepLength( 2.5 );
        }
        else
        {
            optimizer->SetLearningRate( optimizer->GetCurrentStepLength() );
            optimizer->SetMinimumStepLength(optimizer->GetMinimumStepLength() * 0.2 );
        }
    }

    void Execute(const itk::Object * , const itk::EventObject & ) ITK_OVERRIDE
    {
        return;
    }
};

/*
 *  Iter update
*/

class CommandIterationUpdate : public itk::Command
{
public:
    typedef  CommandIterationUpdate   Self;
    typedef  itk::Command             Superclass;
    typedef itk::SmartPointer<Self>   Pointer;
    itkNewMacro( Self );

protected:
    CommandIterationUpdate() {};

public:
    typedef itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
    typedef   const OptimizerType *                             OptimizerPointer;
    void Execute(itk::Object *caller, const itk::EventObject & event) ITK_OVERRIDE
    {
        Execute( (const itk::Object *)caller, event);
    }
    void Execute(const itk::Object * object, const itk::EventObject & event) ITK_OVERRIDE
    {
        OptimizerPointer optimizer = static_cast< OptimizerPointer >( object );
        if( ! itk::IterationEvent().CheckEvent( &event ) )
        {
            return;
        }
        std::cout << optimizer->GetCurrentIteration() << "   ";
        std::cout << optimizer->GetValue() << "   ";
        std::cout << optimizer->GetCurrentPosition() << std::endl;
    }
};

int main( int argc, char *argv[] )
{
    if( argc < 7 )
    {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << " atlasImageFile  movingImageFile ";
        std::cerr << " atlaslabesl  registrationResult ";
        std::cerr << " segmentationResult ";
        std::cerr << " differenceBefore differenceAfter " << std::endl;
        return EXIT_FAILURE;
    }

    /*
     * registration init
    */

    MetricType::Pointer         metric        = MetricType::New();
    metric->SetNumberOfHistogramBins( 64 );
    OptimizerType::Pointer      optimizer     = OptimizerType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();
    registration->SetMetric(        metric        );
    registration->SetOptimizer(     optimizer     );

    /*
     * Image handling init
    */
    ReaderType::Pointer  fixedImageReader  = ReaderType::New();
    ReaderType::Pointer movingImageReader = ReaderType::New();
    Int2FloatCasterType::Pointer fixedImageCaster = Int2FloatCasterType::New();
    Int2FloatCasterType::Pointer movingImageCaster = Int2FloatCasterType::New();
    fixedImageReader->SetFileName(  argv[1] );
    movingImageReader->SetFileName( argv[2] );
    fixedImageCaster -> SetInput(    fixedImageReader->GetOutput()    );
    movingImageCaster -> SetInput(   movingImageReader->GetOutput()   );

    FloatImageType::Pointer fixedImage1 = FloatImageType::New();
    FloatImageType::Pointer movingImage1 = FloatImageType::New();

    fixedImageCaster -> Update();
    movingImageCaster -> Update();

    fixedImage1 = fixedImageCaster -> GetOutput();
    movingImage1 = movingImageCaster -> GetOutput();

    std::cout << " fixedImage1 -> GetOrigin() " << fixedImage1 -> GetOrigin() << std::endl;
    std::cout << " fixedImage1 -> GetSpacing() " << fixedImage1 -> GetSpacing() << std::endl;
    std::cout << " fixedImage1 -> GetDirection() " << fixedImage1 -> GetDirection() << std::endl;
    std::cout << " fixedImage1 -> GetLargestPossibleRegion() " << fixedImage1 -> GetLargestPossibleRegion() << std::endl;
    std::cout << " movingImage1 -> GetOrigin() " << movingImage1 -> GetOrigin() << std::endl;
    std::cout << " movingImage1 -> GetSpacing() " << movingImage1 -> GetSpacing() << std::endl;
    std::cout << " movingImage1 -> GetDirection() " << movingImage1 -> GetDirection() << std::endl;
    std::cout << " movingImage1 -> GetLargestPossibleRegion() " << movingImage1 -> GetLargestPossibleRegion() << std::endl;


    movingImage1 -> SetOrigin( fixedImage1 -> GetOrigin() );
    movingImage1 -> SetDirection( fixedImage1 -> GetDirection() );


    std::cout << " fixedImage1 -> GetOrigin() " << fixedImage1 -> GetOrigin() << std::endl;
    std::cout << " fixedImage1 -> GetSpacing() " << fixedImage1 -> GetSpacing() << std::endl;
    std::cout << " fixedImage1 -> GetDirection() " << fixedImage1 -> GetDirection() << std::endl;
    std::cout << " fixedImage1 -> GetLargestPossibleRegion() " << fixedImage1 -> GetLargestPossibleRegion() << std::endl;
    std::cout << " movingImage1 -> GetOrigin() " << movingImage1 -> GetOrigin() << std::endl;
    std::cout << " movingImage1 -> GetSpacing() " << movingImage1 -> GetSpacing() << std::endl;
    std::cout << " movingImage1 -> GetDirection() " << movingImage1 -> GetDirection() << std::endl;
    std::cout << " movingImage1 -> GetLargestPossibleRegion() " << movingImage1 -> GetLargestPossibleRegion() << std::endl;


    /*
     * Initialize tansform
    */
    TransformType::Pointer  initialTransform = TransformType::New();
    initialTransform -> SetIdentity();

    /*
     * Feed registration with what you have so far
    */
    registration->SetFixedImage(    fixedImage1    );
    registration->SetMovingImage(   movingImage1  );
    movingImageCaster -> GetOutput() -> SetOrigin( fixedImageCaster -> GetOutput() -> GetOrigin()  );
    registration->SetInitialTransform( initialTransform );

    /*
     * Set up the optimizer
    */
    typedef OptimizerType::ScalesType       OptimizerScalesType;
    OptimizerScalesType optimizerScales( initialTransform->GetNumberOfParameters() );
    const double translationScale = 1.0 / 100.0;
    optimizerScales[0] = 1.0;
    optimizerScales[1] = 1.0;
    optimizerScales[2] = 1.0;
    optimizerScales[3] = translationScale;
    optimizerScales[4] = translationScale;
    optimizerScales[5] = translationScale;
    optimizerScales[6] = 0.05;
    optimizer->SetScales( optimizerScales );
    optimizer->SetNumberOfIterations( 200 );
    optimizer->SetRelaxationFactor( 0.5 );
    optimizer->SetMinimumStepLength( 0.001 );
    optimizer->SetReturnBestParametersAndValue(true);
    CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
    optimizer->AddObserver( itk::IterationEvent(), observer );

    /*
     * multires setup
     */
    const unsigned int numberOfLevels = 3;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 3 );
    shrinkFactorsPerLevel[0] = 3;
    shrinkFactorsPerLevel[1] = 2;
    shrinkFactorsPerLevel[2] = 1;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 3 );
    smoothingSigmasPerLevel[0] = 0;
    smoothingSigmasPerLevel[1] = 0;
    smoothingSigmasPerLevel[2] = 0;

    registration->SetNumberOfLevels ( numberOfLevels );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );

    typedef RegistrationInterfaceCommand<RegistrationType> CommandType;
    CommandType::Pointer command = CommandType::New();
    registration->AddObserver( itk::MultiResolutionIterationEvent(), command );


    /*
     * Kick off
     */
    try
    {
        registration->Update();
        std::cout << "Optimizer stop condition: "
                  << registration->GetOptimizer()->GetStopConditionDescription()
                  << std::endl;
    }

    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    const TransformType::ParametersType finalParameters = registration->GetOutput()->Get()->GetParameters();

    /*
     * Get final transform
     */
    TransformType::Pointer finalTransform = TransformType::New();
    finalTransform->SetFixedParameters( registration->GetOutput()->Get()->GetFixedParameters() );
    finalTransform->SetParameters( finalParameters );

    TransformType::MatrixType matrix = finalTransform->GetMatrix();
    TransformType::OffsetType offset = finalTransform->GetOffset();

    /*
     * post proc init
     */
    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetTransform( finalTransform );
    resampler->SetInput( movingImageCaster->GetOutput() );

    FloatImageType::Pointer fixedImage = fixedImageCaster->GetOutput();
    resampler->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
    resampler->SetOutputSpacing( fixedImage->GetSpacing() );
    resampler->SetOutputDirection( fixedImage->GetDirection() );
    resampler->SetDefaultPixelValue( 0 );

    WriterType::Pointer      writer =  WriterType::New();
    Float2IntCasterType::Pointer  float2intCaster =  Float2IntCasterType::New();

    writer->SetFileName( argv[4] );
    float2intCaster->SetInput( resampler->GetOutput() );
    writer->SetInput( float2intCaster->GetOutput()   );
    writer->Update();

    /*
     * transform atlas
     */
    ReaderType::Pointer atlasReader = ReaderType::New();
    Int2FloatCasterType::Pointer atlasInCaster = Int2FloatCasterType::New();
    atlasReader -> SetFileName( argv[3] );
    atlasInCaster -> SetInput( atlasReader -> GetOutput() );

    FloatImageType::Pointer movingImage = FloatImageType::New();
    movingImage = movingImageCaster ->GetOutput();

    resampler->SetTransform( finalTransform -> GetInverseTransform().GetPointer() );
    resampler->SetInput( atlasInCaster->GetOutput() );
    resampler->SetSize(    movingImage -> GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(  movingImage -> GetOrigin());
    resampler->SetOutputSpacing( movingImage -> GetSpacing());
    resampler->SetOutputDirection( movingImage -> GetDirection());
    resampler->SetDefaultPixelValue( 0 );
    writer->SetFileName( argv[5] );
    float2intCaster->SetInput( resampler->GetOutput() );

    BinaryThresholdFilterType::Pointer thresholdFilter = BinaryThresholdFilterType::New();

    thresholdFilter -> SetInput( float2intCaster -> GetOutput() );
    thresholdFilter -> SetLowerThreshold(1);
    thresholdFilter -> SetUpperThreshold( UINT16_MAX );

    writer->SetInput( thresholdFilter -> GetOutput() );
    writer->Update();

    return EXIT_SUCCESS;
}
