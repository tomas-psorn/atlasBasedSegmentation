//
// Created by psorn on 10.5.17.
//

#include "applyMask.h"

int main( int argc, char *argv[] )
{

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageReaderType::Pointer maskReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();
    MaskFilterType::Pointer maskFilter = MaskFilterType::New();

    imageReader -> SetFileName(argv[1]);
    maskReader  -> SetFileName(argv[2]);
    imageWriter -> SetFileName(argv[3]);

    imageReader -> Update();
    maskReader -> Update();

    ImageType::Pointer image = imageReader -> GetOutput();
    ImageType::Pointer mask = maskReader -> GetOutput();

    // ad-hoc modification
    mask -> SetOrigin( image -> GetOrigin());

    maskFilter -> SetMaskImage( mask );

    maskFilter -> SetInput( image );

    imageWriter -> SetInput( maskFilter -> GetOutput() );

    imageWriter -> Update();

}