//
// Created by psorn on 5.6.17.
//

#ifndef ATLASBASEDSEGMENTATION_DEFORMABLEREGISTRATION_H
#define ATLASBASEDSEGMENTATION_DEFORMABLEREGISTRATION_H

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"

#include "itkFEMRegistrationFilter.h"

typedef itk::Image<unsigned char, 2>                       DiskImageType;
typedef itk::Image<float, 2>                               ImageType;
typedef itk::fem::Element2DC0LinearQuadrilateralMembrane   ElementType;
typedef itk::fem::Element2DC0LinearTriangularMembrane      ElementType2;
typedef itk::fem::FEMObject<2>                             FEMObjectType;

typedef itk::fem::FEMRegistrationFilter<ImageType,ImageType,FEMObjectType>
        RegistrationType;

typedef itk::ImageFileReader< DiskImageType > FileSourceType;

typedef itk::HistogramMatchingImageFilter<ImageType,ImageType> HEFilterType;

#endif //ATLASBASEDSEGMENTATION_DEFORMABLEREGISTRATION_H
