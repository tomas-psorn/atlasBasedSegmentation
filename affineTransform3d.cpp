#include <itkAffineTransform.h>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkAffineTransform.h.h"
#include "itkResampleImageFilter.h"

const int Dimension = 3;
typedef unsigned short ShortType;
typedef float  FloatType;

typedef itk::Image <ShortType, Dimension > ShortImageType;
typedef itk::Image <FloatType, Dimension > FloatImageType;


typedef itk::ImageFileReader <ShortImageType> ImageReaderType;
typedef itk::ImageFileWriter <ShortImageType> ImageWriterType;

typedef itk::CastImageFilter <ShortImageType , FloatImageType > Short2floatCasterType;
typedef itk::CastImageFilter <FloatImageType , ShortImageType > Float2shortCasterType;

typedef itk::ResampleImageFilter <FloatImageType , FloatImageType> ResampleImageFilterType;

typedef itk::AffineTransform<FloatType > TransformType;


int main( int argc, char *argv[] ) {



    return 0;
}
