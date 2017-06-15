//
// Created by tomas on 14.6.17.
//

#include "subtract3d.h"

int main( int argc, char *argv[] ) {

    ImageReaderType::Pointer imageReader1   = ImageReaderType::New();
    ImageReaderType::Pointer imageReader2  = ImageReaderType::New();

    DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

    WriterType::Pointer imageWriter = WriterType::New();

    imageReader1 -> SetFileName( argv[1] );
    imageReader2 -> SetFileName( argv[2] );

    imageWriter -> SetFileName(argv[3]);

    difference->SetInput1( imageReader1->GetOutput() );
    difference->SetInput2( imageReader2->GetOutput() );

    imageWriter -> SetInput(difference -> GetOutput());

    imageWriter -> Update();

    return 0;


}
