//
// source: ResampleVolumesToBeIsotropic.cxx
//

#include <itkBSplineInterpolateImageFunction.h>
#include "interpolateInterleaves.h"

int main(int argc, char * argv[]){
    if( argc < 3 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << "  inputImageFile  outputImageFile  lower upper " << std::endl;
        return EXIT_FAILURE;
    }

    ReaderType::Pointer reader = ReaderType::New();

    reader->SetFileName( argv[1] );

    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject & excep )
    {
        std::cerr << "Exception caught!" << std::endl;
        std::cerr << excep << std::endl;
    }

    IntensityFilterType::Pointer intensityWindowing = IntensityFilterType::New();

    intensityWindowing->SetOutputMinimum(   0.0 );
    intensityWindowing->SetOutputMaximum( 255.0 ); // floats but in the range of chars.

    intensityWindowing->SetInput( reader->GetOutput() );

    GaussianFilterType::Pointer smootherX = GaussianFilterType::New();
    GaussianFilterType::Pointer smootherY = GaussianFilterType::New();

    smootherX->SetInput( intensityWindowing->GetOutput() );
    smootherY->SetInput( smootherX->GetOutput() );

    InputImageType::ConstPointer inputImage = reader->GetOutput();

    const InputImageType::SpacingType& inputSpacing = inputImage->GetSpacing();

    const double isoSpacing = std::sqrt( inputSpacing[2] * inputSpacing[0] );

    smootherX->SetSigma( isoSpacing );
    smootherY->SetSigma( isoSpacing );

    smootherX->SetDirection( 0 );
    smootherY->SetDirection( 1 );

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();

    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();

    resampler->SetTransform( transform );

    InterpolatorType::Pointer interpolator = InterpolatorType::New();

    resampler->SetInterpolator( interpolator );

    resampler->SetDefaultPixelValue( 255 ); // highlight regions without source

    OutputImageType::SpacingType spacing;

    spacing[0] = isoSpacing;
    spacing[1] = isoSpacing;
    spacing[2] = isoSpacing;

    resampler->SetOutputSpacing( spacing );

    resampler->SetOutputOrigin( inputImage->GetOrigin() );
    resampler->SetOutputDirection( inputImage->GetDirection() );

    InputImageType::SizeType   inputSize =
            inputImage->GetLargestPossibleRegion().GetSize();

    typedef InputImageType::SizeType::SizeValueType SizeValueType;

    const double dx = inputSize[0] * inputSpacing[0] / isoSpacing;
    const double dy = inputSize[1] * inputSpacing[1] / isoSpacing;

    const double dz = (inputSize[2] - 1 ) * inputSpacing[2] / isoSpacing;

    InputImageType::SizeType   size;

    size[0] = static_cast<SizeValueType>( dx );
    size[1] = static_cast<SizeValueType>( dy );
    size[2] = static_cast<SizeValueType>( dz );

    resampler->SetSize( size );


    resampler->SetInput( smootherY->GetOutput() );
    resampler->Update();

    WriterType::Pointer writer = WriterType::New();

    writer->SetFileName( argv[2] );
    writer->SetInput( resampler->GetOutput() );

    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & excep )
    {
        std::cerr << "Exception caught !" << std::endl;
        std::cerr << excep << std::endl;
    }

    return EXIT_SUCCESS;

}
