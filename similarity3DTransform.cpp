#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkSimilarity3DTransform.h"
#include "itkResampleImageFilter.h"
#include "itkCenteredTransformInitializer.h"

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

typedef itk::Similarity3DTransform < double > TransformType;

typedef itk::CenteredTransformInitializer <TransformType, FloatImageType, FloatImageType> CenteredTransformInitializerType;

int main( int argc, char *argv[] ) {

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    Short2floatCasterType::Pointer short2floatCaster = Short2floatCasterType::New();
    TransformType::Pointer transform = TransformType::New();
    ResampleImageFilterType::Pointer resampler = ResampleImageFilterType::New();
    Float2shortCasterType::Pointer float2shortCasterType = Float2shortCasterType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();
    CenteredTransformInitializerType::Pointer centeredTransformInitializer = CenteredTransformInitializerType::New();


    // TRANSLATION
    TransformType::OutputVectorType translation;
    translation[0] = atof(argv[3]);
    translation[1] = atof(argv[4]);
    translation[2] = atof(argv[5]);
    transform -> SetTranslation( translation );

    // ROTATION
    TransformType::VersorType rotationX;
    TransformType::VersorType rotationY;
    TransformType::VersorType rotationZ;
    TransformType::VersorType rotation;


    rotationX.SetRotationAroundX( 2 * M_PI * (atof(argv[6]) / 360)  );
    rotationY.SetRotationAroundY( 2 * M_PI * (atof(argv[7]) / 360) );
    rotationZ.SetRotationAroundZ( 2 * M_PI * (atof(argv[8]) / 360) );

    rotation *= rotationX;
    rotation *= rotationY;
    rotation *= rotationZ;

    transform -> SetRotation ( rotation );


    // SCALE
    transform -> SetScale(atof(argv[9]));





    // PREPROC

    imageReader -> SetFileName( argv[1] );
    imageReader -> Update();
    short2floatCaster -> SetInput( imageReader -> GetOutput() );
    short2floatCaster -> Update();


    // CENTER OF ROTATION
    centeredTransformInitializer -> SetFixedImage( short2floatCaster -> GetOutput() );
    centeredTransformInitializer -> SetMovingImage( short2floatCaster -> GetOutput() );
    centeredTransformInitializer -> SetTransform( transform );
    centeredTransformInitializer -> GeometryOn();
    centeredTransformInitializer -> InitializeTransform();


    // PERFORM
    resampler -> SetTransform( transform );
    resampler -> SetInput( short2floatCaster -> GetOutput() );
    resampler->SetSize( short2floatCaster-> GetOutput() -> GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin( short2floatCaster-> GetOutput() -> GetOrigin() );
    resampler->SetOutputSpacing( short2floatCaster-> GetOutput() -> GetSpacing() );
    resampler->SetOutputDirection( short2floatCaster-> GetOutput() -> GetDirection() );

    // POSTPROC
    float2shortCasterType -> SetInput( resampler -> GetOutput() );
    imageWriter -> SetFileName( argv[2] );
    imageWriter -> SetInput( float2shortCasterType -> GetOutput() );
    imageWriter -> Update();

    return EXIT_SUCCESS;
}
