#include "atlasBasedSegmentation.h"

int main (int argc, char *argv[])
{
    if( argc < 3 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << std::endl;
        std::cerr << "fixedFilePath" << std::endl;
        std::cerr << "movingFilePath" << std::endl;
        std::cerr << "outputFile" << std::endl;
        std::cerr << "log directory" << std::endl;
        return EXIT_FAILURE;
    }

    /*
    read and cast input images
    */

    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    IntToDoubleCasterType::Pointer intToDoubleCaster = IntToDoubleCasterType::New();
    DoubleToIntCasterType::Pointer doubleToIntCaster = DoubleToIntCasterType::New();

    reader->SetFileName(argv[1]);
    reader->Update();
    intToDoubleCaster->SetInput(reader->GetOutput());
    DoubleImageType::Pointer fixedImage = intToDoubleCaster->GetOutput();

    reader->SetFileName(argv[2]);
    reader->Update();
    intToDoubleCaster->SetInput(reader->GetOutput());
    DoubleImageType::Pointer movingImage = intToDoubleCaster->GetOutput();

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();

    TransformType::Pointer identityTransform = TransformType::New();

    identityTransform -> SetIdentity();

    resampler -> SetInput( movingImage );
    resampler -> SetOutputOrigin( movingImage -> GetOrigin() );
    resampler -> SetOutputDirection( movingImage -> GetDirection() );
    resampler -> SetSize( fixedImage -> GetLargestPossibleRegion().GetSize() );
    resampler -> SetTransform( identityTransform );



    RegistrationType::Pointer registration = RegistrationType::New();
    InterpolatorType::Pointer interpolation = InterpolatorType::New();
    TransformType::Pointer transform = TransformType::New();
    MetricType::Pointer metric = MetricType::New();
    OptimizerType::Pointer optimizer = OptimizerType::New();

    registration -> SetMetric( metric );
    registration -> SetOptimizer( optimizer );
    registration -> SetFixedImage( fixedImage );
    registration -> SetMovingImage( resampler -> GetOutput() );

    registration -> Update();

    resampler -> SetInput( movingImage );
    resampler -> SetOutputSpacing( movingImage -> GetSpacing() );
    resampler -> SetOutputOrigin( movingImage -> GetOrigin() );
    resampler -> SetOutputDirection( movingImage -> GetDirection() );
    resampler -> SetSize( fixedImage -> GetLargestPossibleRegion().GetSize() );
    resampler -> SetTransform( registration -> GetTransform() );

    writer -> SetFileName(argv[3]);

    doubleToIntCaster -> SetInput( resampler -> GetOutput() );

    writer -> SetInput( doubleToIntCaster -> GetOutput() );

    writer -> Update();

    return EXIT_SUCCESS;
}
