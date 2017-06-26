//
// Created by tomas on 18.6.17.
//

#include "evalMaskOverlap.h"

int main( int argc, char *argv[] ) {
    if (argc < 3) {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << " fixedImageFile  movingImageFile ";
        return EXIT_FAILURE;
    }

    ImageReaderType::Pointer reader1 = ImageReaderType::New();
    ImageReaderType::Pointer reader2 = ImageReaderType::New();

    CastFilterType::Pointer caster1 = CastFilterType::New();
    CastFilterType::Pointer caster2 = CastFilterType::New();

    reader1 -> SetFileName( argv[1] );
    reader2 -> SetFileName( argv[2] );

    LabelOverlapFilterType::Pointer labelOverlapFilter = LabelOverlapFilterType::New();

    caster1 -> SetInput(reader1 -> GetOutput());
    caster2 -> SetInput(reader2 -> GetOutput());



    labelOverlapFilter -> SetSourceImage( caster1 -> GetOutput() );
    labelOverlapFilter -> SetTargetImage( caster2 -> GetOutput() );

    std::cout << "Jaccard coef: " << labelOverlapFilter -> GetJaccardCoefficient() << std::endl;


    return EXIT_SUCCESS;

}