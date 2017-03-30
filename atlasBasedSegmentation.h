//
// Created by tomas on 30.3.17.
//

#ifndef ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
#define ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H

#include "itkImage.h"
#include "itkImageRegistrationMethodv4.h"
#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkCenteredTransformInitializer.h"
#include "itkCenteredSimilarity2DTransform.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkIdentityTransform.h"




typedef float DoublePixelType;
typedef unsigned char CharPixelType;


typedef itk::Image<DoublePixelType , 2> Image2D;
typedef itk::Image<CharPixelType , 2> OutputImageType;


typedef itk::CenteredSimilarity2DTransform <double > TransformType;
typedef itk::ImageFileReader <Image2D> ImageReader;
typedef itk::ImageFileWriter <OutputImageType> ImageWriter;

typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
typedef OptimizerType::ScalesType OptimizerScalesType;
typedef itk::MeanSquaresImageToImageMetricv4< Image2D, Image2D > MetricType;
typedef itk::ImageRegistrationMethodv4< Image2D, Image2D, TransformType > RegistrationType;
typedef itk::CenteredTransformInitializer< TransformType, Image2D, Image2D > TransformInitializerType;
typedef itk::ResampleImageFilter< Image2D,Image2D > ResampleFilterType;

typedef itk::RescaleIntensityImageFilter<Image2D, OutputImageType >   RescalerType;
typedef itk::SubtractImageFilter <Image2D,Image2D, Image2D > DifferenceFilterType;
typedef itk::CastImageFilter< Image2D, OutputImageType > CastFilterType;




int affine2D(const char * fixedImageFilename, char * movingImageFilename, TransformType::Pointer transform);
int labelInverseTransform(const char * labelImageFilename, const char * segmentationFilename , TransformType::InverseTransformBasePointer inverseTransform);


#endif //ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
