/*
source: ImageRegistration9.cxx
itk sw guide p. 516
*/

#ifndef ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
#define ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H

#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkCenteredTransformInitializer.h"
#include "itkAffineTransform.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkImageMaskSpatialObject.h"

#include "itkCastImageFilter.h"

const    unsigned int Dimension = 3;
typedef  unsigned int           PixelType;
typedef  unsigned char           MaskPixelType;


typedef itk::ImageMaskSpatialObject< Dimension > MaskType;

typedef itk::Image< PixelType, Dimension >  FixedImageType;
typedef itk::Image< PixelType, Dimension >  MovingImageType;
typedef itk::Image< MaskPixelType, Dimension >  MaskImageType;


typedef itk::AffineTransform<double, Dimension> TransformType;

typedef itk::RegularStepGradientDescentOptimizerv4<double > OptimizerType;
typedef itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType > MetricType;
typedef itk::LinearInterpolateImageFunction< MovingImageType,double>    InterpolatorType;
typedef itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType> RegistrationType;

typedef itk::ImageFileReader< FixedImageType  > ImageReaderType;
typedef itk::ImageFileReader< MaskImageType  > MaskReaderType;

typedef itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType >  TransformInitializerType;

typedef OptimizerType::ScalesType  OptimizerScalesType;

typedef itk::ResampleImageFilter<MovingImageType, FixedImageType >    ResampleFilterType;

typedef  unsigned char  OutputPixelType;

typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

typedef itk::CastImageFilter<FixedImageType, OutputImageType > CastFilterType;

typedef itk::ImageFileWriter< OutputImageType>  WriterType;

typedef itk::CastImageFilter <FixedImageType , MaskType > UnsignedShortToChar;

#endif //ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
