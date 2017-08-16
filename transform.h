//
// Created by psorn on 12.5.17.
//

#ifndef ATLASBASEDSEGMENTATION_TRANSFORM_H
#define ATLASBASEDSEGMENTATION_TRANSFORM_H

#include <math.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimilarity3DTransform.h"
#include "itkResampleImageFilter.h"

#include "itkCastImageFilter.h"

#include "itkFlipImageFilter.h"

const int Dimension = 3;
typedef unsigned short ShortType;

typedef double DoubleType;

typedef itk::Image <ShortType, Dimension > ShortImageType;
typedef itk::Image <DoubleType, Dimension > DoubleImageType;


typedef itk::ImageFileReader <ShortImageType> ImageReaderType;
typedef itk::ImageFileWriter <ShortImageType> ImageWriterType;

typedef itk::CastImageFilter <ShortImageType , DoubleImageType > Short2doubleCasterType;
typedef itk::CastImageFilter <DoubleImageType , ShortImageType > Double2shortCasterType;

typedef itk::ResampleImageFilter <DoubleImageType , DoubleImageType> ResampleImageFilterType;

typedef itk::Similarity3DTransform <DoubleType > TransformType;

typedef itk::FlipImageFilter <DoubleImageType> FlipFilterType;

// Transform bricks
typedef TransformType::VersorType VersorType;
typedef VersorType::VectorType VectorType;



#endif //ATLASBASEDSEGMENTATION_TRANSFORM_H
