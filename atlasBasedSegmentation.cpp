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
    intToDoubleCaster->SetInput(reader->GetOutput());
    intToDoubleCaster->Update();
    DoubleImageType::Pointer fixedImage = intToDoubleCaster->GetOutput();

    std::cout << "Fixed image LPR" << fixedImage -> GetLargestPossibleRegion().GetSize() << std::endl;
    std::cout << "Fixed image spacing" << fixedImage -> GetSpacing() << std::endl;
    std::cout << "Fixed image origin" << fixedImage -> GetOrigin() << std::endl;

    reader->SetFileName(argv[2]);
    intToDoubleCaster->SetInput(reader->GetOutput());
    intToDoubleCaster->Update();
    DoubleImageType::Pointer movingImage = intToDoubleCaster->GetOutput();

    std::cout << "Moving image LPR" << movingImage -> GetLargestPossibleRegion().GetSize() << std::endl;
    std::cout << "Moving image spacing" << movingImage -> GetSpacing() << std::endl;
    std::cout << "Moving image origin" << movingImage -> GetOrigin() << std::endl;





    return EXIT_SUCCESS;

    //

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();

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
