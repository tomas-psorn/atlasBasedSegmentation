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
        std::cerr << "   fixedImageFile  fixedImageMask " << std::endl;
        std::cerr << "   movingImageFile  movingImageMask " << std::endl;
        std::cerr << "   outputImagefile" << std::endl;
        std::cerr << "   [differenceBeforeRegistration] [differenceAfterRegistration] " << std::endl;
        std::cerr << "   [stepLength] [maxNumberOfIterations] "<< std::endl;
        return EXIT_FAILURE;
    }

    MetricType::Pointer         metric        = MetricType::New();
    OptimizerType::Pointer      optimizer     = OptimizerType::New();
    InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();


    ImageReaderType::Pointer  fixedImageReader  = ImageReaderType::New();
    ImageReaderType::Pointer fixedMaskReader = ImageReaderType::New();
    ImageReaderType::Pointer  movingImageReader  = ImageReaderType::New();
    ImageReaderType::Pointer movingMaskReader = ImageReaderType::New();

    fixedImageReader->SetFileName(  argv[1] );
    fixedMaskReader->SetFileName(  argv[2] );
    movingImageReader->SetFileName(  argv[3] );
    movingMaskReader->SetFileName( argv[4] );

    UnsignedShortToChar::Pointer fixedMaskCaster = UnsignedShortToChar::New();
    UnsignedShortToChar::Pointer movingMaskCaster = UnsignedShortToChar::New();

    fixedMaskCaster->SetInput( fixedMaskReader->GetOutput() );
    movingMaskCaster->SetInput( movingMaskReader->GetOutput() );

    fixedMaskCaster->Update();
    movingMaskCaster->Update();

    MaskType::Pointer fixedMask = MaskType::New();
    MaskType::Pointer movingMask = MaskType::New();

    fixedMask->SetImage( fixedImageReader->GetOutput() );
    movingMask->SetImage( movingMaskCaster->GetOutput() );

    metric->SetFixedImageMask(fixedMask);
//    metric->SetMovingImageMask(movingMask);

    registration->SetMetric(        metric        );
    registration->SetOptimizer(     optimizer     );
    registration->SetInterpolator(  interpolator  );


    TransformType::Pointer  transform = TransformType::New();
    registration->SetTransform( transform );

    registration->SetFixedImage(    fixedImageReader->GetOutput()    );
    registration->SetMovingImage(   movingImageReader->GetOutput()   );
    fixedImageReader->Update();

    registration->SetFixedImageRegion(
            fixedImageReader->GetOutput()->GetBufferedRegion() );

    TransformInitializerType::Pointer initializer = TransformInitializerType::New();
    initializer->SetTransform(   transform );
    initializer->SetFixedImage(  fixedImageReader->GetOutput() );
    initializer->SetMovingImage( movingImageReader->GetOutput() );


    registration->SetInitialTransformParameters(transform->GetParameters() );

    double translationScale = 1.0 / 1000.0;
    if( argc > 10 )
    {
        translationScale = atof( argv[10] );
    }

    OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );
    optimizerScales[0] =  1.0;
    optimizerScales[1] =  1.0;
    optimizerScales[2] =  1.0;
    optimizerScales[3] =  1.0;
    optimizerScales[4] =  1.0;
    optimizerScales[5] =  1.0;
    optimizerScales[6] =  1.0;
    optimizerScales[7] =  1.0;
    optimizerScales[8] =  1.0;
    optimizerScales[9]  =  translationScale;
    optimizerScales[10] =  translationScale;
    optimizerScales[11] =  translationScale;
    optimizer->SetScales( optimizerScales );

    double steplength = 0.1;
    if( argc > 6 )
    {
        steplength = atof( argv[8] );
    }
    unsigned int maxNumberOfIterations = 300;
    if( argc > 7 )
    {
        maxNumberOfIterations = atoi( argv[9] );
    }

    optimizer->SetMaximumStepLength( steplength );
    optimizer->SetMinimumStepLength( 0.0001 );
    optimizer->SetNumberOfIterations( maxNumberOfIterations );
    optimizer->MinimizeOn();

    std::cout << "Got here " << std::endl;


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

    OptimizerType::ParametersType finalParameters =
            registration->GetLastTransformParameters();

    const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
    const double bestValue = optimizer->GetValue();

    std::cout << "Result = " << std::endl;
    std::cout << " Iterations    = " << numberOfIterations << std::endl;
    std::cout << " Metric value  = " << bestValue          << std::endl;

    TransformType::Pointer finalTransform = TransformType::New();

    finalTransform->SetParameters( finalParameters );
    finalTransform->SetFixedParameters( transform->GetFixedParameters() );

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();

    resampler->SetTransform( finalTransform );
    resampler->SetInput( movingImageReader->GetOutput() );

    FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

    resampler->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
    resampler->SetOutputSpacing( fixedImage->GetSpacing() );
    resampler->SetOutputDirection( fixedImage->GetDirection() );
    resampler->SetDefaultPixelValue( 100 );

    WriterType::Pointer      writer =  WriterType::New();
    CastFilterType::Pointer  caster =  CastFilterType::New();


    writer->SetFileName( argv[5] );


    caster->SetInput( resampler->GetOutput() );
    writer->SetInput( caster->GetOutput()   );
    writer->Update();


    typedef itk::SubtractImageFilter<
            FixedImageType,
            FixedImageType,
            FixedImageType > DifferenceFilterType;

    DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

    difference->SetInput1( fixedImageReader->GetOutput() );
    difference->SetInput2( resampler->GetOutput() );

    WriterType::Pointer writer2 = WriterType::New();

    typedef itk::RescaleIntensityImageFilter<
            FixedImageType,
            OutputImageType >   RescalerType;

    RescalerType::Pointer intensityRescaler = RescalerType::New();

    intensityRescaler->SetInput( difference->GetOutput() );
    intensityRescaler->SetOutputMinimum(   0 );
    intensityRescaler->SetOutputMaximum( 255 );

    writer2->SetInput( intensityRescaler->GetOutput() );
    resampler->SetDefaultPixelValue( 1 );

    // Compute the difference image between the
    // fixed and resampled moving image.
    if( argc > 7 )
    {
        writer2->SetFileName( argv[7] );
        writer2->Update();
    }


    typedef itk::IdentityTransform< double, Dimension > IdentityTransformType;
    IdentityTransformType::Pointer identity = IdentityTransformType::New();

    // Compute the difference image between the
    // fixed and moving image before registration.
    if( argc > 6 )
    {
        resampler->SetTransform( identity );
        writer2->SetFileName( argv[6] );
        writer2->Update();
    }

    return EXIT_SUCCESS;
}
