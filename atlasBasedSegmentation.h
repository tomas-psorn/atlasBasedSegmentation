//
// Created by tomas on 30.3.17.
//

#ifndef ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
#define ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H

#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkVersorRigid3DTransform.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

#include "itkCenteredTransformInitializer.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkCommand.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkBinaryImageToShapeLabelMapFilter.h"

#include "itkSubtractImageFilter.h"

#include "itkCommand.h"


const unsigned int Dimension = 3;

typedef float   FloatPixelType;
typedef unsigned short  IntPixelType;

typedef itk::Image< FloatPixelType , Dimension >  FloatImageType;
typedef itk::Image< IntPixelType , Dimension >  IntImageType;


typedef itk::CastImageFilter< IntImageType, FloatImageType > Int2FloatCasterType;
typedef itk::CastImageFilter< FloatImageType, IntImageType > Float2IntCasterType;
typedef itk::ImageFileReader< IntImageType > ReaderType;
typedef itk::ImageFileWriter< IntImageType > WriterType;

typedef itk::VersorRigid3DTransform < double > TransformType;
typedef itk::RegularStepGradientDescentOptimizerv4< double > OptimizerType;
typedef itk::MeanSquaresImageToImageMetricv4< FloatImageType, FloatImageType> MetricType;
typedef itk::ImageRegistrationMethodv4< FloatImageType, FloatImageType, TransformType > RegistrationType;
typedef itk::ResampleImageFilter <FloatImageType, FloatImageType> ResampleFilterType;
typedef itk::CenteredTransformInitializer<TransformType, FloatImageType, FloatImageType >  TransformInitializerType;

typedef TransformType::VersorType  VersorType;
typedef VersorType::VectorType     VectorType;


typedef itk::IdentityTransform< double, Dimension > IdentityTransformType;

typedef itk::SubtractImageFilter <FloatImageType, FloatImageType, FloatImageType> SubtractFilterType;

//typedef itk::BinaryImageToLabelMapFilter <IntImageType, IntImageType> ImageToLabelFilterType;
typedef itk::BinaryThresholdImageFilter <IntImageType, IntImageType> BinaryThresholdFilterType;

typedef itk::RescaleIntensityImageFilter< FloatImageType, IntImageType > RescalerType;


#endif //ATLASBASEDSEGMENTATION_ATLASBASEDSEGMENTATION_H
