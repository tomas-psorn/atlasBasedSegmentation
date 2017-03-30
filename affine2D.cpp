#include "atlasBasedSegmentation.h"

int affine2D(const char * fixedImageFilename, char * movingImageFilename, TransformType::Pointer transform){

    // registration

    ImageReader::Pointer fixedImageReader = ImageReader::New();
    ImageReader::Pointer movingImageReader = ImageReader::New();

    fixedImageReader->SetFileName(fixedImageFilename);
    movingImageReader->SetFileName(movingImageFilename);

    // get registration blocks
    MetricType::Pointer metric = MetricType::New();
    OptimizerType::Pointer optimizer = OptimizerType::New();
    RegistrationType::Pointer registration  = RegistrationType::New();

    // set up the registration
    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);
    registration->SetFixedImage(fixedImageReader->GetOutput());
    registration->SetMovingImage(movingImageReader->GetOutput());

    // set up initializer
    TransformInitializerType::Pointer initializer = TransformInitializerType::New();
    initializer->SetTransform( transform );
    initializer->SetFixedImage( fixedImageReader->GetOutput() );
    initializer->SetMovingImage( movingImageReader->GetOutput() );
    initializer->MomentsOn();
    initializer->InitializeTransform();
    double initialAngle = 0.0;
    double initialScale = 0.0;
    transform->SetScale( initialScale );
    transform->SetAngle( initialAngle );
    registration->SetInitialTransform( transform );
    registration->InPlaceOn();

    // set up optimizer
    OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );
    const double translationScale = 1.0 / 100.0;
    double steplength = 1.0;
    optimizerScales[0] = 10.0;
    optimizerScales[1] =  1.0;
    optimizerScales[2] =  translationScale;
    optimizerScales[3] =  translationScale;
    optimizerScales[4] =  translationScale;
    optimizerScales[5] =  translationScale;
    optimizer->SetScales( optimizerScales );
    optimizer->SetLearningRate( steplength );
    optimizer->SetMinimumStepLength( 0.0001 );
    optimizer->SetNumberOfIterations( 500 );

    // advanced registration setting
    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 1 );
    shrinkFactorsPerLevel[0] = 1;
    const unsigned int numberOfLevels = 1;
    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 1 );
    smoothingSigmasPerLevel[0] = 0;
    registration->SetNumberOfLevels ( numberOfLevels );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );

    // registration
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

    // export outputs
    OptimizerType::ParametersType finalParameters = registration->GetTransform()->GetParameters();
    TransformType::Pointer finalTransform = TransformType::New();

    finalTransform->SetParameters( finalParameters );
    finalTransform->SetFixedParameters( transform->GetFixedParameters() );

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();

    resampler->SetTransform( finalTransform );
    resampler->SetInput( movingImageReader->GetOutput() );

    Image2D::Pointer fixedImage = fixedImageReader->GetOutput();

    resampler->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
    resampler->SetOutputSpacing( fixedImage->GetSpacing() );
    resampler->SetOutputDirection( fixedImage->GetDirection() );
    resampler->SetDefaultPixelValue( 100 );

    ImageWriter::Pointer      writer =  ImageWriter::New();
    CastFilterType::Pointer  caster =  CastFilterType::New();
    RescalerType::Pointer intensityRescaler = RescalerType::New();
    DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

//    caster->SetInput(resampler->GetOutput());
    difference->SetInput1( fixedImageReader->GetOutput() );
    difference->SetInput2( resampler->GetOutput() );
    difference->Update();

    intensityRescaler->SetInput( difference->GetOutput() );
    intensityRescaler->SetOutputMinimum(   0 );
    intensityRescaler->SetOutputMaximum( 255 );

    resampler->SetDefaultPixelValue( 1 );

    ImageWriter::Pointer writer2 = ImageWriter::New();
    writer2->SetInput( intensityRescaler->GetOutput() );
    writer2->SetFileName( "/home/psorn/Data/volumetrie/ED_14Jul01_ARI_120OHR_rat.qx1/3/pdata/1/dicom/test2.dcm" );
    writer2->Update();


    typedef itk::IdentityTransform< double, 2 > IdentityTransformType;
    IdentityTransformType::Pointer identity = IdentityTransformType::New();

    // Compute the difference image between the
    // fixed and moving image before registration.


    resampler->SetTransform( identity );
    writer2->SetFileName( "/home/psorn/Data/volumetrie/ED_14Jul01_ARI_120OHR_rat.qx1/3/pdata/1/dicom/test3.dcm" );
    writer2->Update();


    intensityRescaler->SetInput( resampler->GetOutput() );
    writer->SetInput(intensityRescaler->GetOutput());
    writer->SetFileName( "/home/psorn/Data/volumetrie/ED_14Jul01_ARI_120OHR_rat.qx1/3/pdata/1/dicom/test1.dcm" );
    writer->Update();


    return EXIT_SUCCESS;
};



