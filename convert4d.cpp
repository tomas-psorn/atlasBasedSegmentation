#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

const int Dimension = 4;
typedef unsigned short PixelType;

typedef itk::Image <PixelType, Dimension > ImageType;
typedef itk::ImageFileReader <ImageType> ImageReaderType;
typedef itk::ImageFileWriter <ImageType> ImageWriterType;

int help(char *argv[]){
    std::cerr << "Function: " << argv[0] << " usage:" << std::endl;
    std::cerr << "   input file " << std::endl;
    std::cerr << "   output file " << std::endl;
    return EXIT_FAILURE;
}

int main( int argc, char *argv[] )
{

    if (argc < 2) help(argv);

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();

    imageReader -> SetFileName(argv[1]);
    imageWriter -> SetFileName(argv[2]);

    imageWriter -> SetInput( imageReader -> GetOutput() );

    imageWriter -> Update();

}

