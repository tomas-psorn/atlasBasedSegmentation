/*
source: ImageRegistration9.cxx
itk sw guide p. 516
*/

#ifndef ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
#define ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H

#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkSimilarity3DTransform.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkCenteredTransformInitializer.h"

#include "itkImageMaskSpatialObject.h"

#include "itkCastImageFilter.h"

// dimensions
const    unsigned int Dimension = 3;

// pixel types
typedef  unsigned int   InputPixelType;
typedef  double         IntraPixelType;
typedef  unsigned char  MaskPixelType;
typedef  unsigned char  OutputPixelType;

// Image types
typedef itk::ImageMaskSpatialObject< Dimension > MaskType;
typedef itk::Image< InputPixelType, Dimension >  InputImageType;
typedef itk::Image< MaskPixelType, Dimension >  MaskImageType;
typedef itk::Image< IntraPixelType , Dimension >  IntraImageType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

// Registration bricks
typedef itk::Similarity3DTransform < double > TransformType;
typedef itk::RegularStepGradientDescentOptimizerv4 < double > OptimizerType;
typedef itk::MeanSquaresImageToImageMetricv4< IntraImageType, IntraImageType > MetricType;
typedef itk::ImageRegistrationMethodv4< IntraImageType, IntraImageType , TransformType> RegistrationType;
typedef OptimizerType::ScalesType  OptimizerScalesType;
typedef itk::CenteredTransformInitializer <TransformType, IntraImageType, IntraImageType> TransformInitializerType;
typedef itk::CompositeTransform< double, Dimension > CompositeTransformType;

// Transform bricks
typedef TransformType::VersorType VersorType;
typedef VersorType::VectorType VectorType;

// Filters and interpolators
typedef itk::LinearInterpolateImageFunction< IntraImageType , double >    InterpolatorType;
typedef itk::ResampleImageFilter<IntraImageType, IntraImageType >    ResampleFilterType;

// Readers
typedef itk::ImageFileReader< InputImageType  > ImageReaderType;
typedef itk::ImageFileReader< MaskImageType  > MaskReaderType;

// Writer
typedef itk::ImageFileWriter< OutputImageType>  WriterType;

//casters
typedef itk::CastImageFilter<InputImageType, IntraImageType > InToIntraCasterType;
typedef itk::CastImageFilter < IntraImageType , OutputImageType > IntraToOutCasterType;

#endif //ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
