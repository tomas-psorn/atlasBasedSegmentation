//
// Created by tomas on 14.6.17.
//

#ifndef ATLASBASEDSEGMENTATION_SUBTRACT3D_H
#define ATLASBASEDSEGMENTATION_SUBTRACT3D_H

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImage.h"

#include "itkSquaredDifferenceImageFilter.h"

const unsigned int Dimension = 3;
typedef unsigned short PixelType;

typedef itk::Image< PixelType, Dimension >  ImageType;

typedef itk::ImageFileReader< ImageType  > ImageReaderType;

typedef itk::ImageFileWriter< ImageType >  WriterType;

typedef itk::SquaredDifferenceImageFilter<
        ImageType, ImageType, ImageType> DifferenceFilterType;

#endif //ATLASBASEDSEGMENTATION_SUBTRACT3D_H
