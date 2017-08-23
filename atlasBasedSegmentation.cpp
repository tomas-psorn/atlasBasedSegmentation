#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkVersorRigid3DTransform.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

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


const unsigned int Dimension = 3;

typedef float   FloatPixelType;
typedef unsigned short  IntPixelType;

typedef itk::Image< FloatPixelType , Dimension >  FloatImageType;
typedef itk::Image< IntPixelType , Dimension >  IntImageType;


typedef itk::CastImageFilter< IntImageType, FloatImageType > Int2FloatCasterType;
typedef itk::CastImageFilter< FloatImageType, IntImageType > Float2IntCasterType;
typedef itk::ImageFileReader< IntImageType > ReaderType;
typedef itk::ImageFileWriter< IntImageType > WriterType;

typedef itk::VersorRigid3DTransform < double > TransformType;
typedef itk::RegularStepGradientDescentOptimizerv4< double > OptimizerType;
typedef itk::MeanSquaresImageToImageMetricv4< FloatImageType, FloatImageType> MetricType;
typedef itk::ImageRegistrationMethodv4< FloatImageType, FloatImageType, TransformType > RegistrationType;
typedef itk::ResampleImageFilter <FloatImageType, FloatImageType> ResampleFilterType;
typedef itk::CenteredTransformInitializer<TransformType, FloatImageType, FloatImageType >  TransformInitializerType;

typedef TransformType::VersorType  VersorType;
typedef VersorType::VectorType     VectorType;


typedef itk::IdentityTransform< double, Dimension > IdentityTransformType;

typedef itk::SubtractImageFilter <FloatImageType, FloatImageType, FloatImageType> SubtractFilterType;

//typedef itk::BinaryImageToLabelMapFilter <IntImageType, IntImageType> ImageToLabelFilterType;
typedef itk::BinaryThresholdImageFilter <IntImageType, IntImageType> BinaryThresholdFilterType;

typedef itk::RescaleIntensityImageFilter< FloatImageType, IntImageType > RescalerType;




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

    MetricType::Pointer         metric        = MetricType::New();
    OptimizerType::Pointer      optimizer     = OptimizerType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();

    registration->SetMetric(        metric        );
    registration->SetOptimizer(     optimizer     );

    ReaderType::Pointer  fixedImageReader  = ReaderType::New();
    ReaderType::Pointer movingImageReader = ReaderType::New();

    Int2FloatCasterType::Pointer fixedImageCaster = Int2FloatCasterType::New();
    Int2FloatCasterType::Pointer movingImageCaster = Int2FloatCasterType::New();

    fixedImageReader->SetFileName(  argv[1] );
    movingImageReader->SetFileName( argv[2] );

    fixedImageCaster -> SetInput(    fixedImageReader->GetOutput()    );
    movingImageCaster -> SetInput(   movingImageReader->GetOutput()   );

    registration->SetFixedImage(    fixedImageCaster->GetOutput()    );
    registration->SetMovingImage(   movingImageCaster ->GetOutput()   );

    movingImageCaster -> GetOutput() -> SetOrigin( fixedImageCaster -> GetOutput() -> GetOrigin()  );

    TransformType::Pointer  initialTransform = TransformType::New();
    TransformInitializerType::Pointer initializer = TransformInitializerType::New();

    initializer->SetTransform(   initialTransform );
    initializer->SetFixedImage(  fixedImageCaster->GetOutput() );
    initializer->SetMovingImage( movingImageCaster->GetOutput() );
    initializer->MomentsOn();
    initializer->InitializeTransform();

    VersorType     rotation;
    VectorType     axis;
    axis[0] = 0.0;
    axis[1] = 0.0;
    axis[2] = 1.0;
    const double angle = 0;
    rotation.Set(  axis, angle  );

    initialTransform->SetRotation( rotation );


    registration->SetInitialTransform( initialTransform );


    typedef OptimizerType::ScalesType       OptimizerScalesType;
    OptimizerScalesType optimizerScales( initialTransform->GetNumberOfParameters() );
    const double translationScale = 1.0 / 100.0;
    optimizerScales[0] = 1.0;
    optimizerScales[1] = 1.0;
    optimizerScales[2] = 1.0;
    optimizerScales[3] = translationScale;
    optimizerScales[4] = translationScale;
    optimizerScales[5] = translationScale;
    optimizer->SetScales( optimizerScales );
    optimizer->SetNumberOfIterations( 200 );
    optimizer->SetLearningRate( 0.2 );
    optimizer->SetMinimumStepLength( 0.001 );
    optimizer->SetReturnBestParametersAndValue(true);

    CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
    optimizer->AddObserver( itk::IterationEvent(), observer );

    // One level registration process without shrinking and smoothing.
    //
    const unsigned int numberOfLevels = 1;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 1 );
    shrinkFactorsPerLevel[0] = 1;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 1 );
    smoothingSigmasPerLevel[0] = 0;

    registration->SetNumberOfLevels( numberOfLevels );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );

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

    const double versorX              = finalParameters[0];
    const double versorY              = finalParameters[1];
    const double versorZ              = finalParameters[2];
    const double finalTranslationX    = finalParameters[3];
    const double finalTranslationY    = finalParameters[4];
    const double finalTranslationZ    = finalParameters[5];
    const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
    const double bestValue = optimizer->GetValue();

    // Print out results
    //
    std::cout << std::endl << std::endl;
    std::cout << "Result = " << std::endl;
    std::cout << " versor X      = " << versorX  << std::endl;
    std::cout << " versor Y      = " << versorY  << std::endl;
    std::cout << " versor Z      = " << versorZ  << std::endl;
    std::cout << " Translation X = " << finalTranslationX  << std::endl;
    std::cout << " Translation Y = " << finalTranslationY  << std::endl;
    std::cout << " Translation Z = " << finalTranslationZ  << std::endl;
    std::cout << " Iterations    = " << numberOfIterations << std::endl;
    std::cout << " Metric value  = " << bestValue          << std::endl;


    TransformType::Pointer finalTransform = TransformType::New();

    finalTransform->SetFixedParameters( registration->GetOutput()->Get()->GetFixedParameters() );
    finalTransform->SetParameters( finalParameters );

    TransformType::MatrixType matrix = finalTransform->GetMatrix();
    TransformType::OffsetType offset = finalTransform->GetOffset();
    std::cout << "Matrix = " << std::endl << matrix << std::endl;
    std::cout << "Offset = " << std::endl << offset << std::endl;

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

    // subtract before

    SubtractFilterType::Pointer subtractFilter = SubtractFilterType::New();
    subtractFilter -> SetInput1( fixedImage );
    subtractFilter -> SetInput2( movingImageCaster -> GetOutput() );
    RescalerType::Pointer intensityRescaler = RescalerType::New();
    intensityRescaler -> SetInput( subtractFilter->GetOutput() );
    intensityRescaler -> SetOutputMinimum(   0 );
    intensityRescaler -> SetOutputMaximum( 255 );
    writer -> SetFileName(argv[6]);
    writer -> SetInput( intensityRescaler -> GetOutput() );
    writer -> Update();

    // subtract after
    subtractFilter -> SetInput1( fixedImage );
    subtractFilter -> SetInput2( resampler -> GetOutput() );
    resampler ->SetDefaultPixelValue(0);
    intensityRescaler->SetInput( subtractFilter->GetOutput() );
    intensityRescaler->SetOutputMinimum(   0 );
    intensityRescaler->SetOutputMaximum( 255 );
    writer -> SetFileName(argv[7]);
    writer -> SetInput( intensityRescaler -> GetOutput() );
    writer -> Update();



    // transform atlas
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
