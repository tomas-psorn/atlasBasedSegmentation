//
// Created by tomas on 12.6.17.
//

#ifndef ATLASBASEDSEGMENTATION_SUBTRACT2D_H
#define ATLASBASEDSEGMENTATION_SUBTRACT2D_H

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImage.h"

#include "itkSquaredDifferenceImageFilter.h"

const unsigned int Dimension = 2;
typedef unsigned short PixelType;

typedef itk::Image< PixelType, Dimension >  ImageType;

typedef itk::ImageFileReader< ImageType  > ImageReaderType;

typedef itk::ImageFileWriter< ImageType >  WriterType;

typedef itk::SquaredDifferenceImageFilter<
        ImageType, ImageType, ImageType> DifferenceFilterType;

#endif //ATLASBASEDSEGMENTATION_SUBTRACT2D_H
