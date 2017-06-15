//
// Created by tomas on 30.3.17.
//

#ifndef ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
#define ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H

#include <itkSimilarity3DTransform.h>
#include "itkImage.h"
#include "itkImageRegistrationMethodv4.h"
#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkIdentityTransform.h"

#include "itkGradientAnisotropicDiffusionImageFilter.h"

const bool verbose = true;

const int dimension = 3;

// read images for registration

typedef unsigned short InputPixelType;
typedef double DoublePixelType;

typedef itk::Image< InputPixelType, dimension> IntImageType;
typedef itk::Image< DoublePixelType, dimension> DoubleImageType;
typedef itk::ImageFileReader <IntImageType> ReaderType;
typedef itk::ImageFileWriter <IntImageType> WriterType;
typedef itk::CastImageFilter <IntImageType, DoubleImageType> IntToDoubleCasterType;
typedef itk::CastImageFilter <DoubleImageType, IntImageType> DoubleToIntCasterType;

typedef itk::ResampleImageFilter < DoubleImageType, DoubleImageType > ResampleFilterType;
typedef itk::Similarity3DTransform < DoublePixelType > TransformType;
typedef itk::BSplineInterpolateImageFunction< DoubleImageType ,double > InterpolatorType;
typedef itk::MeanSquaresImageToImageMetricv4 < DoubleImageType, DoubleImageType > MetricType;
typedef itk::RegularStepGradientDescentOptimizerv4 < DoublePixelType > OptimizerType;
typedef itk::ImageRegistrationMethodv4 < DoubleImageType, DoubleImageType, TransformType > RegistrationType;

#endif //ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
