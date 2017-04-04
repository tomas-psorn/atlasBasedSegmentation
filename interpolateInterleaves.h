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

#endif //ATLASBASEDSEGMENTATION_INTERPOLATEINTERLEAVES_H
