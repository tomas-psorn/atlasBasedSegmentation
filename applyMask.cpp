//
// Created by psorn on 10.5.17.
//
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMaskImageFilter.h"


const int Dimension = 3;
typedef unsigned short PixelType;

typedef itk::Image <PixelType, Dimension > ImageType;
typedef itk::ImageFileReader <ImageType> ImageReaderType;
typedef itk::ImageFileWriter <ImageType> ImageWriterType;
typedef itk::MaskImageFilter <ImageType, ImageType, ImageType> MaskFilterType;

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

    maskFilter -> SetCoordinateTolerance(1e3);
    
    imageWriter -> SetInput( maskFilter -> GetOutput() );

    imageWriter -> Update();

}