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

    maskFilter -> SetMaskImage( maskReader -> GetOutput() );

    maskFilter -> SetInput( imageReader -> GetOutput() );

    imageWriter -> SetInput( maskFilter -> GetOutput() );



    imageWriter -> Update();

}