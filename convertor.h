//
// Created by psorn on 10.5.17.
//

#ifndef ATLASBASEDSEGMENTATION_CONVERTOR_H
#define ATLASBASEDSEGMENTATION_CONVERTOR_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

const int Dimension = 3;
typedef unsigned short PixelType;

typedef itk::Image <PixelType, Dimension > ImageType;
typedef itk::ImageFileReader <ImageType> ImageReaderType;
typedef itk::ImageFileWriter <ImageType> ImageWriterType;



#endif //ATLASBASEDSEGMENTATION_CONVERTOR_H
