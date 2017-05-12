//
// Created by psorn on 12.5.17.
//

#include "transform.h"

int main( int argc, char *argv[] )
{

    if( argc < 8 )
    {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << "   fixedImageFile " << std::endl;
        std::cerr << "   movingImageFile " << std::endl;
        std::cerr << "   translation [x, y, z] "<< std::endl;
        std::cerr << "   rotation [x, y, z] "<< std::endl;
        return EXIT_FAILURE;
    }

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();

    imageReader -> SetFileName(argv[1]);
    imageWriter -> SetFileName(argv[2]);

    imageReader -> Update();

    Short2doubleCasterType::Pointer short2doubleCaster = Short2doubleCasterType::New();
    Double2shortCasterType::Pointer double2shortCaster = Double2shortCasterType::New();

    ResampleImageFilterType::Pointer resampler = ResampleImageFilterType::New();

    TransformType::Pointer transform = TransformType::New();

    // set translation

    double translationVector [3];

    translationVector[0] = atof(argv[3]);
    translationVector[1] = atof(argv[4]);
    translationVector[2] = atof(argv[5]);

    const TransformType::TranslationType & translation = translationVector;

    transform -> SetTranslation( translation );

    // set rotation

    double axisVector [3];

    axisVector[0] = 1.0;
    axisVector[1] = 0.0;
    axisVector[2] = 0.0;
    const TransformType::AxisType & axisX = axisVector;
    TransformType::AngleType angleX = atof(argv[6]);
    angleX = angleX / M_PI;
    angleX = angleX / (2 * M_PI);
    transform -> SetRotation( axisX, angleX );

    axisVector[0] = 0.0;
    axisVector[1] = 1.0;
    const TransformType::AxisType & axisY = axisVector;
    TransformType::AngleType angleY = atof(argv[7]);
    angleY = angleY / (2 * M_PI);
    transform -> SetRotation( axisY, angleY );

    axisVector[1] = 0.0;
    axisVector[2] = 1.0;
    const TransformType::AxisType & axisZ = axisVector;
    TransformType::AngleType angleZ = atof(argv[8]);
    angleZ = angleZ * ( M_PI / 180);
    transform -> SetRotation( axisZ, angleZ );

    //const TransformType::TranslationType& translation;

    resampler -> SetTransform( transform );
    resampler->SetSize( imageReader-> GetOutput() -> GetLargestPossibleRegion().GetSize() );
    resampler -> SetOutputOrigin( imageReader -> GetOutput() -> GetOrigin() );
    resampler -> SetOutputSpacing( imageReader -> GetOutput() -> GetSpacing() );
    resampler -> SetOutputDirection( imageReader -> GetOutput() -> GetDirection() );

    short2doubleCaster -> SetInput ( imageReader -> GetOutput() );

    resampler -> SetInput( short2doubleCaster ->GetOutput() );

    double2shortCaster -> SetInput( resampler ->GetOutput() );

    imageWriter -> SetInput( double2shortCaster -> GetOutput() );

    imageWriter -> Update();

}