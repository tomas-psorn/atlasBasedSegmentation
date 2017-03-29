//
// Created by tomas on 30.3.17.
//

#ifndef ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
#define ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H

#include "itkImage.h"
#include "itkImageRegistrationMethodv4.h"
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



typedef float PixelType;
typedef itk::Image<PixelType , 2> Image2D;
typedef itk::CenteredSimilarity2DTransform <double > TransformType;
typedef itk::ImageFileReader <Image2D> ImageReader;

typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
typedef itk::MeanSquaresImageToImageMetricv4< Image2D, Image2D > MetricType;
typedef itk::ImageRegistrationMethodv4< Image2D, Image2D, TransformType > RegistrationType;

void affine2D(const char * fixedImageFilename, char * movingImageFilename, TransformType::Pointer transform);

#endif //ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
