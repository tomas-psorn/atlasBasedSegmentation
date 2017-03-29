#include "atlasBasedSegmentation.h"

void affine2D(const char * fixedImageFilename, char * movingImageFilename, TransformType::Pointer transform){

    ImageReader::Pointer fixedImageReader = ImageReader::New();
    ImageReader::Pointer movingImageReader = ImageReader::New();

    fixedImageReader->SetFileName(fixedImageFilename);
    movingImageReader->SetFileName(movingImageFilename);



    return;
};



