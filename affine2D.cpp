#include "atlasBasedSegmentation.h"

int affine2D(const char * fixedImageFilename, char * movingImageFilename, TransformType::Pointer transform){

    ImageReader::Pointer fixedImageReader = ImageReader::New();
    ImageReader::Pointer movingImageReader = ImageReader::New();

    fixedImageReader->SetFileName(fixedImageFilename);
    movingImageReader->SetFileName(movingImageFilename);

    MetricType::Pointer metric = MetricType::New();
    OptimizerType::Pointer optimizer = OptimizerType::New();
    RegistrationType::Pointer registration  = RegistrationType::New();

    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);

    registration->SetFixedImage(fixedImageReader->GetOutput());
    registration->SetMovingImage(movingImageReader->GetOutput());

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

    OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );
    const double translationScale = 1.0 / 100.0;
    optimizerScales[0] = 10.0;
    optimizerScales[1] =  1.0;
    optimizerScales[2] =  translationScale;
    optimizerScales[3] =  translationScale;
    optimizerScales[4] =  translationScale;
    optimizerScales[5] =  translationScale;
    optimizer->SetScales( optimizerScales );

    double steplength = 1.0;
    optimizer->SetLearningRate( steplength );
    optimizer->SetMinimumStepLength( 0.0001 );
    optimizer->SetNumberOfIterations( 500 );

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



    return EXIT_SUCCESS;
};



