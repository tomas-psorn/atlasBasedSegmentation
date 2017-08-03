
/**
 *  @file    applyMask3d.cpp
 *  @author  Tomas Psorn (nnocae10)
 *  @date    08/17/2017
 *  @version 1.0
 *
 *  @brief apply binnary mask to an image (3D)
 *
 *  @section DESCRIPTION
 *
 *  A utility for mask binnary mask application
 */

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMaskImageFilter.h"


const int Dimension = 3;
typedef unsigned short ShortType;

typedef itk::Image <ShortType, Dimension > ShortImageType;
typedef itk::ImageFileReader <ShortImageType> ImageReaderType;
typedef itk::ImageFileWriter <ShortImageType> ImageWriterType;
typedef itk::MaskImageFilter <ShortImageType, ShortImageType, ShortImageType> MaskFilterType;

int help(char *argv[]){
    std::cerr << "Program apllyMask3d located in: " << argv[0] << " usage:" << std::endl;
    std::cerr << "   input file" << std::endl;
    std::cerr << "   mask file" << std::endl;
    std::cerr << "   output file" << std::endl;
    std::cerr << "   [spacing tolerance]" << std::endl;
    return EXIT_FAILURE;
}


int main( int argc, char *argv[] )
{
    if (argc < 3) help(argv);

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageReaderType::Pointer maskReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();
    MaskFilterType::Pointer maskFilter = MaskFilterType::New();

    imageReader -> SetFileName(argv[1]);
    maskReader  -> SetFileName(argv[2]);
    imageWriter -> SetFileName(argv[3]);

    // update call needs to be done here
    imageReader -> Update();
    maskReader -> Update();

    ShortImageType::Pointer image = imageReader -> GetOutput();
    ShortImageType::Pointer mask = maskReader -> GetOutput();

    // ad-hoc modification
    mask -> SetOrigin( image -> GetOrigin());
    mask -> SetDirection( image -> GetDirection());

    maskFilter -> SetMaskImage( mask );
    maskFilter -> SetInput( image );

    // ad-hoc modification
    double tolerance = 1e3;
    if (argc < 4) tolerance = atof(argv[4]);

    maskFilter -> SetCoordinateTolerance(tolerance);

    imageWriter -> SetInput( maskFilter -> GetOutput() );

    imageWriter -> Update();

    return EXIT_SUCCESS;
}