//
// Created by psorn on 27.4.17.
//

/* Path to example data
/home/tomas/Dev/ITK/Examples/Data/
*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"

#include "itkTranslationTransform.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkImageRegistrationMethodv4.h"

#include "itkLinearInterpolateImageFunction.h"
#include "itkRegistrationParameterScalesFromPhysicalShift.h"

int main(int argc, char * argv[]){

    const unsigned int Dimension = 2;
    typedef float Pixeltype;
    typedef unsigned char OutputPixelType;

    typedef itk::Image <Pixeltype , Dimension> FixedImageType;
    typedef itk::Image <Pixeltype , Dimension> MovingImageType;
    typedef itk::Image <OutputPixelType , Dimension> OutputImageType;

    typedef itk::ImageFileReader <FixedImageType> FixedImageReaderType;
    typedef itk::ImageFileReader <MovingImageType> MovingImageReaderType;
    typedef itk::ImageFileWriter <OutputImageType > WriterType;

    typedef itk::TranslationTransform <double , Dimension> TransformType;
    typedef itk::RegularStepGradientDescentOptimizerv4 <double> OptimizerType;
    typedef itk::MeanSquaresImageToImageMetricv4 <FixedImageType, MovingImageType> MetricType;
    typedef itk::ImageRegistrationMethodv4 <FixedImageType, MovingImageType, TransformType> RegistrationType;

    typedef itk::LinearInterpolateImageFunction <FixedImageType, double > FixedLinearInterpolatorType;
    typedef itk::LinearInterpolateImageFunction <FixedImageType, double > MovingLinearInterpolatorType;

    typedef itk::RegistrationParameterScalesFromPhysicalShift <MetricType> ScalesEstimatorType;

    typedef itk::CompositeTransform <double, Dimension> CompositeTransformType;

    typedef itk::ResampleImageFilter <FixedImageType, MovingImageType > ResampleFilterType;
    typedef itk::CastImageFilter <FixedImageType, OutputImageType > CastFilterType;

    OptimizerType::Pointer optimizer = OptimizerType::New();
    MetricType::Pointer metric = MetricType::New();
    RegistrationType::Pointer registration = RegistrationType::New();

    registration -> SetMetric( metric );
    registration -> SetOptimizer( optimizer );

    FixedLinearInterpolatorType::Pointer fixedInterpolator = FixedLinearInterpolatorType::New();
    MovingLinearInterpolatorType::Pointer movingInterpolator = MovingLinearInterpolatorType::New();

    metric -> SetFixedInterpolator( fixedInterpolator );
    metric -> SetMovingInterpolator( movingInterpolator );

    FixedImageReaderType::Pointer fixedImageReader = FixedImageReaderType::New();
    MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

    fixedImageReader -> SetFileName(argv[1]);
    movingImageReader -> SetFileName(argv[2]);

    registration -> SetFixedImage( fixedImageReader -> GetOutput() );
    registration -> SetMovingImage( movingImageReader -> GetOutput() );

    TransformType::Pointer movingInitialTransform = TransformType::New();
    TransformType::ParametersType initialParameters (movingInitialTransform -> GetNumberOfParameters() );
    initialParameters[0] = 0.0;
    initialParameters[1] = 0.0;
    movingInitialTransform -> SetParameters( initialParameters );
    registration -> SetMovingInitialTransform( movingInitialTransform );

    TransformType::Pointer fixedInitialTransform = TransformType::New();
    fixedInitialTransform -> SetIdentity();
    registration -> SetFixedInitialTransform( fixedInitialTransform );

    optimizer -> SetLearningRate( 4.0 );
    optimizer -> SetMinimumStepLength( 0.001 );
    optimizer -> SetRelaxationFactor( 0.5 );
    optimizer -> SetNumberOfIterations( 200 );

    ScalesEstimatorType::Pointer scalesEstimator = ScalesEstimatorType::New();

    scalesEstimator -> SetMetric( metric );
    scalesEstimator -> SetTransformForward( true );
    optimizer -> SetScalesEstimator(scalesEstimator);
    optimizer -> SetDoEstimateLearningRateOnce( true );

    const unsigned int numberOfLevels = 1;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 1 );
    shrinkFactorsPerLevel[0] = 0;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 1 );
    smoothingSigmasPerLevel[0] = 1;

    registration -> SetNumberOfLevels( numberOfLevels );
    registration -> SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    registration -> SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );

    std::cout << movingImageReader ->GetOutput()->GetSpacing() << std::endl;

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

    std::cout << "Got here too" << std::endl;

    TransformType::ConstPointer transform = registration -> GetTransform();
    TransformType::ParametersType finalParameters = transform -> GetParameters();
    const double TranslationAlongX = finalParameters[0];
    const double TranslationAlongY = finalParameters[1];
    const unsigned int numberOfIterations = optimizer ->GetNumberOfIterations();
    const double  bestValue = optimizer -> GetValue();

    std::cout << "Result = " << std::endl;
    std::cout << " Translation X = " << TranslationAlongX  << std::endl;
    std::cout << " Translation Y = " << TranslationAlongY  << std::endl;
    std::cout << " Iterations    = " << numberOfIterations << std::endl;
    std::cout << " Metric value  = " << bestValue          << std::endl;

    CompositeTransformType::Pointer outputCompositeTransform = CompositeTransformType::New();

    outputCompositeTransform -> AddTransform( movingInitialTransform );
    outputCompositeTransform -> AddTransform( registration -> GetModifiableTransform() );

    FixedImageType::Pointer fixedImage = fixedImageReader -> GetOutput();

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler -> SetInput( movingImageReader-> GetOutput() );
    resampler -> SetTransform( outputCompositeTransform );
    resampler -> SetSize ( fixedImage -> GetLargestPossibleRegion().GetSize() );
    resampler -> SetOutputOrigin( fixedImage -> GetOrigin() );
    resampler -> SetOutputSpacing( fixedImage -> GetSpacing() );
    resampler -> SetOutputDirection( fixedImage -> GetDirection() );
    resampler -> SetDefaultPixelValue( 100 );

    CastFilterType::Pointer caster = CastFilterType::New();
    WriterType::Pointer writer = WriterType::New();

    writer -> SetFileName( argv[3] );

    caster -> SetInput( resampler -> GetOutput() );
    writer -> SetInput( caster -> GetOutput() );
    writer -> Update();

    return EXIT_SUCCESS;
}
