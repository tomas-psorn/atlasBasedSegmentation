//
// Created by psorn on 10.5.17.
//

#include "convertor.h"

int main( int argc, char *argv[] )
{

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();

    imageReader -> SetFileName(argv[1]);
    imageWriter -> SetFileName(argv[2]);

    imageWriter -> SetInput( imageReader -> GetOutput() );

    imageWriter -> Update();

}