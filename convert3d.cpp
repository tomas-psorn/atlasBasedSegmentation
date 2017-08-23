#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

const int Dimension = 3;
typedef unsigned short PixelType;

typedef itk::Image <PixelType, Dimension > ImageType;
typedef itk::ImageFileReader <ImageType> ImageReaderType;
typedef itk::ImageFileWriter <ImageType> ImageWriterType;


int main( int argc, char *argv[] )
{

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();

    imageReader -> SetFileName(argv[1]);
    imageWriter -> SetFileName(argv[2]);

    imageWriter -> SetInput( imageReader -> GetOutput() );

    imageWriter -> Update();

}