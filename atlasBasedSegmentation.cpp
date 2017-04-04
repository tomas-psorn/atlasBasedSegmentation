#include "atlasBasedSegmentation.h"

int main (int argc, char *argv[])
{
    if( argc < 3 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " fixedFilePath  movingFilePath labelFilePath segmentationFilePath" << std::endl;
        std::cerr << " sliceNumber " << std::endl;
        return EXIT_FAILURE;
    }


    TransformType::Pointer transform = TransformType::New();

    affine2D(argv[1], argv[2], transform);

    TransformType::InverseTransformBasePointer inverseTransform = transform->GetInverseTransform();

    std::cout << inverseTransform->GetParameters() << std::endl;
    std::cout << transform->GetParameters() << std::endl;

//  labelInverseTransform(argv[3], argv[4], inverseTransform);

    return 0;
}
