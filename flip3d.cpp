#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFlipImageFilter.h"

const int Dimension = 3;
typedef unsigned short ShortType;

typedef itk::Image <ShortType, Dimension > ShortImageType;
typedef itk::ImageFileReader <ShortImageType> ImageReaderType;
typedef itk::ImageFileWriter <ShortImageType> ImageWriterType;
typedef itk::FlipImageFilter <ShortImageType> FlipFilterType;


int main( int argc, char *argv[] ) {

    if (argc < 3) {
        std::cerr << "Function: " << argv[0] << " usage:" << std::endl;
        std::cerr << "   input file " << std::endl;
        std::cerr << "   output file " << std::endl;
        std::cerr << "   dimension to flip around 0 - x, 1 - y, 2 - z" << std::endl;
        return EXIT_FAILURE;
    }

    ImageReaderType::Pointer    imageReader = ImageReaderType::New();
    ImageWriterType::Pointer    imageWriter = ImageWriterType::New();
    FlipFilterType::Pointer     flipFilter = FlipFilterType::New();
    FlipFilterType::FlipAxesArrayType flipAxes;

    int axis = atoi(argv[3]);

    if (axis == 0 || axis == 1 || axis == 2 ){
        flipAxes[axis] = 1;
    } else
    {
        std::cerr << "axis value invalid, see usage" << std::endl;
        return EXIT_FAILURE;
    }

    imageReader -> SetFileName(argv[1]);
    imageWriter-> SetFileName(argv[2]);
    flipFilter -> SetFlipAxes( flipAxes );
    flipFilter -> SetInput( imageReader -> GetOutput() );
    imageWriter -> SetInput( flipFilter -> GetOutput() );
    imageWriter -> Update();

    return EXIT_SUCCESS;
}