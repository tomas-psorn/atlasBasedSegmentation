#include "atlasBasedSegmentation.h"

int main (int argc, char *argv[])
{
    if( argc < 3 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " fixedFilePath  movingFilePath logDirectory" << std::endl;
        std::cerr << " sliceNumber " << std::endl;
        return EXIT_FAILURE;
    }

    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    IntToDoubleCasterType::Pointer intToDoubleCaster = IntToDoubleCasterType::New();
    DoubleToIntCasterType::Pointer doubleToIntCaster = DoubleToIntCasterType::New();

    //
    //
    //read and cast input images
    //
    //

    reader->SetFileName(argv[1]);
    reader->Update();
    intToDoubleCaster->SetInput(reader->GetOutput());
    intToDoubleCaster->Update();
    DoubleImageType::Pointer fixedImage = intToDoubleCaster->GetOutput();

    reader->SetFileName(argv[2]);
    reader->Update();
    intToDoubleCaster->SetInput(reader->GetOutput());

    IntImageType::Pointer movIm = reader->GetOutput();
    DoubleImageType::Pointer movingImage = intToDoubleCaster->GetOutput();

    //
    //
    // interpolate to isotropic datasets
    //
    //

    // set up anisotropic smoother
    AnisoFilterType::Pointer anisoSmoother = AnisoFilterType::New();
    anisoSmoother->SetInput( fixedImage );
    anisoSmoother->SetNumberOfIterations(anisoDiffusionIterations);
    anisoSmoother->SetTimeStep(anisoDiffusionTimeStep);
    anisoSmoother->SetConductanceParameter(anisoDiffusionConductance);

    // prepare resampler
    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    TransformType::Pointer transform = TransformType::New();
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    DoubleImageType::SpacingType spacing;
    DoubleImageType::SizeType   inputSize = fixedImage->GetLargestPossibleRegion().GetSize();
    DoubleImageType::SizeType   size;
    double isoSpacing;
    double dx, dy, dz;

    interpolator -> SetSplineOrder(3);

    //
    //  resample fixed image
    //

    // calculate necessary values
    const DoubleImageType::SpacingType& fixedImageSpacing = fixedImage->GetSpacing();
    isoSpacing = std::sqrt( fixedImageSpacing[2] * fixedImageSpacing[0] );
    spacing[0] = isoSpacing;
    spacing[1] = isoSpacing;
    spacing[2] = isoSpacing;
    dx = inputSize[0] * fixedImageSpacing[0] / isoSpacing;
    dy = inputSize[1] * fixedImageSpacing[1] / isoSpacing;
    dz = (inputSize[2] - 1 ) * fixedImageSpacing[2] / isoSpacing;
    size[0] = static_cast<SizeValueType>( dx );
    size[1] = static_cast<SizeValueType>( dy );
    size[2] = static_cast<SizeValueType>( dz );

    // set up resampler
    transform->SetIdentity();
    resampler->SetTransform( transform );
    resampler->SetInterpolator( interpolator );
    resampler->SetOutputSpacing( spacing );
    resampler->SetOutputOrigin( fixedImage->GetOrigin() );
    resampler->SetOutputDirection( fixedImage->GetDirection() );
    resampler->SetSize( size );

    // resample
    anisoSmoother->SetInput(fixedImage);
    resampler->SetInput( anisoSmoother->GetOutput() );
    resampler->Update();
    fixedImage= resampler->GetOutput();



    //
    // now the same for moving image
    //

    // calculate necessary values

    const DoubleImageType::SpacingType& movingImageSpacing = movingImage->GetSpacing();
    isoSpacing = std::sqrt( movingImageSpacing[2] * movingImageSpacing[0] );
    spacing[0] = isoSpacing;
    spacing[1] = isoSpacing;
    spacing[2] = isoSpacing;
    dx = inputSize[0] * movingImageSpacing[0] / isoSpacing;
    dy = inputSize[1] * movingImageSpacing[1] / isoSpacing;
    dz = (inputSize[2] - 1 ) * movingImageSpacing[2] / isoSpacing;
    size[0] = static_cast<SizeValueType>( dx );
    size[1] = static_cast<SizeValueType>( dy );
    size[2] = static_cast<SizeValueType>( dz );

    // set up resampler
    transform->SetIdentity();
    resampler->SetTransform( transform );
    resampler->SetInterpolator( interpolator );
    resampler->SetOutputSpacing( spacing );
    resampler->SetOutputOrigin( movingImage->GetOrigin() );
    resampler->SetOutputDirection( movingImage->GetDirection() );
    resampler->SetSize( size );

    // resample
    anisoSmoother->SetInput(movingImage);
    resampler->SetInput( anisoSmoother->GetOutput() );
    resampler->Update();
    movingImage = resampler->GetOutput();


    //
    // verbose
    //
    std::string logDir(argv[3]);
    std::string fixedIsotropicPath = logDir + "fixedIsotropic.dcm";
    doubleToIntCaster->SetInput(fixedImage);
    doubleToIntCaster->Update();
    writer->SetFileName(fixedIsotropicPath.c_str());

    writer->SetInput(doubleToIntCaster -> GetOutput());
    if(verbose) std::cout<< "Writing isotropic fixed image to: " << fixedIsotropicPath << std::endl;
    writer->Update();
    if(verbose) std::cout<< "Done " << std::endl;

    std::string movingIsotropicPath = logDir + "movingIsotropic.dcm";
    doubleToIntCaster->SetInput(movingImage);
    doubleToIntCaster->Update();
    writer->SetFileName(movingIsotropicPath.c_str());
    writer->SetInput(doubleToIntCaster -> GetOutput());
    if(verbose) std::cout<< "Writing isotropic moving image to: " << movingIsotropicPath << std::endl;
    writer->Update();
    if(verbose) std::cout<< "Done " << std::endl;

    return EXIT_SUCCESS;
}
