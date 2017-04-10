/*
source: ImageRegistration9.cxx
itk sw guide p. 516
*/

#ifndef ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
#define ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H

#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkCenteredTransformInitializer.h"
#include "itkAffineTransform.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

const    unsigned int    Dimension = 3;
typedef  float           PixelType;

typedef itk::Image< PixelType, Dimension >  FixedImageType;
typedef itk::Image< PixelType, Dimension >  MovingImageType;

typedef itk::AffineTransform<double, Dimension> TransformType;

typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
typedef itk::MeanSquaresImageToImageMetric<FixedImageType, MovingImageType > MetricType;
typedef itk::LinearInterpolateImageFunction< MovingImageType,double>    InterpolatorType;
typedef itk::ImageRegistrationMethod<FixedImageType, MovingImageType> RegistrationType;

typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

typedef itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType >  TransformInitializerType;

typedef OptimizerType::ScalesType  OptimizerScalesType;

typedef itk::ResampleImageFilter<MovingImageType, FixedImageType >    ResampleFilterType;

typedef  unsigned char  OutputPixelType;

typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

typedef itk::CastImageFilter<FixedImageType, OutputImageType > CastFilterType;

typedef itk::ImageFileWriter< OutputImageType>  WriterType;

#endif //ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
