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

#endif //ATLASBASEDSEGMENTATION_AFFINE3DREGISTRATION_H
