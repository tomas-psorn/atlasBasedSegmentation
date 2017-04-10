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

// interpolate
const int anisoDiffusionIterations = 5;
const float anisoDiffusionTimeStep = 0.00485938;
const float anisoDiffusionConductance = 0.5;

typedef itk::GradientAnisotropicDiffusionImageFilter<DoubleImageType, DoubleImageType > AnisoFilterType;
typedef itk::ResampleImageFilter <DoubleImageType, DoubleImageType> ResampleFilterType;
typedef itk::AffineTransform<double, dimension> TransformType;
typedef itk::LinearInterpolateImageFunction< DoubleImageType,double> InterpolatorType;
typedef DoubleImageType::SizeType::SizeValueType SizeValueType;


#endif //ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
