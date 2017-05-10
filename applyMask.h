//
// Created by psorn on 10.5.17.
//

#ifndef ATLASBASEDSEGMENTATION_APPLYMASK_H
#define ATLASBASEDSEGMENTATION_APPLYMASK_H


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


#endif //ATLASBASEDSEGMENTATION_APPLYMASK_H
