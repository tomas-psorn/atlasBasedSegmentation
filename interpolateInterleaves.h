/*
source: ResampleVolumesToBeIsotropic.cxx
itk sw guide p. 454
*/


#ifndef ATLASBASEDSEGMENTATION_INTERPOLATEINTERLEAVES_H
#define ATLASBASEDSEGMENTATION_INTERPOLATEINTERLEAVES_H


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"

#include "itkIdentityTransform.h"

#include "itkIntensityWindowingImageFilter.h"

const     unsigned int    Dimension = 3;

typedef   signed short  InputPixelType;
typedef   signed short  OutputPixelType;
typedef   float         InternalPixelType;

typedef itk::Image< InputPixelType,    Dimension >   InputImageType;
typedef itk::Image< InternalPixelType, Dimension >   InternalImageType;

typedef itk::ImageFileReader< InputImageType  >  ReaderType;

typedef itk::IntensityWindowingImageFilter<InputImageType, InternalImageType >  IntensityFilterType;

typedef itk::RecursiveGaussianImageFilter<InternalImageType, InternalImageType > GaussianFilterType;

typedef itk::GradientAnisotropicDiffusionImageFilter<InternalImageType, InternalImageType > AnisoFilterType;



typedef itk::Image< OutputPixelType,   Dimension >   OutputImageType;

typedef itk::ResampleImageFilter<InternalImageType, OutputImageType >  ResampleFilterType;

typedef itk::IdentityTransform< double, Dimension >  TransformType;

typedef itk::BSplineInterpolateImageFunction<InternalImageType, double >  InterpolatorType;

typedef InputImageType::SizeType::SizeValueType SizeValueType;

typedef itk::ImageFileWriter< OutputImageType >  WriterType;

#endif //ATLASBASEDSEGMENTATION_INTERPOLATEINTERLEAVES_H
