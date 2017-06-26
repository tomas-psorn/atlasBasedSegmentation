//
// Created by tomas on 18.6.17.
//

#ifndef ATLASBASEDSEGMENTATION_EVALMASKOVERLAP_H
#define ATLASBASEDSEGMENTATION_EVALMASKOVERLAP_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLabelOverlapMeasuresImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkNormalizeImageFilter.h"

const unsigned int Dimension = 3;

typedef unsigned short  IntPixelType;
typedef float  FloatPixelType;

typedef itk::Image< IntPixelType , Dimension >  IntImageType;

typedef itk::Image< IntPixelType , Dimension >  FloatImageType;


typedef itk::ImageFileReader <IntImageType > ImageReaderType;

typedef itk::ImageFileWriter <IntImageType > ImageWriterType;

typedef itk::LabelOverlapMeasuresImageFilter <IntImageType> LabelOverlapFilterType;

typedef itk::RescaleIntensityImageFilter <FloatImageType> RescaleFilterType;

typedef itk::CastImageFilter <IntImageType, FloatImageType> CastFilterType;

typedef itk::NormalizeImageFilter <IntImageType, IntImageType> NormalizeFilterType;

#endif //ATLASBASEDSEGMENTATION_EVALMASKOVERLAP_H
