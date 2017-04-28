/*
source: ImageRegistration9.cxx
itk sw guide p. 516
*/


#include "affine3dRegistration.h"

int main( int argc, char *argv[] )
{
    if( argc < 6 )
    {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << "   fixedImageFile " << std::endl;
        std::cerr << "   movingImageFile " << std::endl;
        std::cerr << "   outputImagefile" << std::endl;
        std::cerr << "   [differenceBeforeRegistration] [differenceAfterRegistration] " << std::endl;
        std::cerr << "   [stepLength] [maxNumberOfIterations] "<< std::endl;
        return EXIT_FAILURE;
    }

    ImageReaderType::Pointer fixedImageReader = ImageReaderType::New();
    ImageReaderType::Pointer movingImageReader = ImageReaderType::New();

    fixedImageReader -> SetFileName( argv[1] );
    movingImageReader -> SetFileName( argv[2] );

    InToIntraCasterType::Pointer inToIntraFixedCaster = InToIntraCasterType::New();
    InToIntraCasterType::Pointer inToIntraMovingCaster = InToIntraCasterType::New();

    inToIntraFixedCaster -> SetInput( fixedImageReader -> GetOutput() );
    inToIntraMovingCaster -> SetInput( movingImageReader -> GetOutput() );


    // metric will use default linear interpolation
    MetricType::Pointer         metric        = MetricType::New();
    TransformType::Pointer      transform     = TransformType::New();
    OptimizerType::Pointer      optimizer     = OptimizerType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();


    registration -> SetMetric( metric );
    registration -> SetOptimizer( optimizer );

    // initialization of centered transform
    TransformType::Pointer initialTransform = TransformType::New();
    TransformInitializerType::Pointer initializer = TransformInitializerType::New();

    initializer -> SetTransform( initialTransform );
    initializer -> SetFixedImage( inToIntraFixedCaster -> GetOutput() );

    initializer -> SetMovingImage( inToIntraMovingCaster -> GetOutput() );

    initializer -> MomentsOn();

    initializer -> InitializeTransform();

    VersorType rotation;
    VectorType axis;

    axis[0] = 0.0;
    axis[1] = 0.0;
    axis[2] = 1.0;
    const double angle = 0.0;
    rotation.Set(axis, angle);
    initialTransform -> SetRotation( rotation );

    registration -> SetInitialTransform( initialTransform );


    optimizer->SetLearningRate( 4 );
    optimizer->SetMinimumStepLength( 0.0001 );
    optimizer->SetRelaxationFactor( 0.5 );
    optimizer->SetNumberOfIterations( 100 );

    registration -> SetFixedImage( inToIntraFixedCaster -> GetOutput() );
    registration -> SetMovingImage( inToIntraMovingCaster -> GetOutput() );

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

    std::cout << optimizer -> GetNumberOfIterations() << std::endl;

    std::cout << registration -> GetTransform()->GetParameters() << std::endl;

    CompositeTransformType::Pointer outputCompositeTransform = CompositeTransformType::New();
    outputCompositeTransform->AddTransform( initialTransform );
    outputCompositeTransform->AddTransform( registration->GetModifiableTransform() );

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetInput( inToIntraMovingCaster->GetOutput() );

    resampler->SetTransform( outputCompositeTransform );

    IntraImageType::Pointer fixedImage = inToIntraFixedCaster -> GetOutput();
    resampler->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
    resampler->SetOutputSpacing( fixedImage->GetSpacing() );
    resampler->SetOutputDirection( fixedImage->GetDirection() );
    resampler->SetDefaultPixelValue( 0 );

    WriterType::Pointer      writer =  WriterType::New();

    writer -> SetFileName( argv[3] );

    IntraToOutCasterType::Pointer intraToOutCaster = IntraToOutCasterType::New();

    intraToOutCaster -> SetInput( resampler -> GetOutput() );
    writer -> SetInput( intraToOutCaster -> GetOutput() );
    writer -> Update();

    return EXIT_SUCCESS;
}
