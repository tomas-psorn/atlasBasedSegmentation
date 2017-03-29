#include "atlasBasedSegmentation.h"

int main( int argc, char ** argv )
{
    if( argc < 2 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " fixedFilePath  movingFilePath " << std::endl;
        std::cerr << " sliceNumber " << std::endl;
        return EXIT_FAILURE;
    }

    TransformType::Pointer transform = TransformType::New();

    affine2D(argv[1], argv[2], transform);

    std::cout << transform->GetParameters() << std::endl;

    return 0;
}
