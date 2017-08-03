//
// Created by psorn on 12.5.17.
//

#include "transform.h"

int main( int argc, char *argv[] )
{

    if( argc < 8 )
    {
        std::cerr << "Function: " << argv[0] << " usage:" << std::endl;
        std::cerr << "   input file " << std::endl;
        std::cerr << "   output file " << std::endl;
        std::cerr << "   translation [x, y, z] in mm"<< std::endl;
        std::cerr << "   rotation [x, y, z] in deg"<< std::endl;
        return EXIT_FAILURE;
    }

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    ImageWriterType::Pointer imageWriter = ImageWriterType::New();

    imageReader -> SetFileName(argv[1]);
    imageWriter -> SetFileName(argv[2]);


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
//    TransformType::AngleType angleX = atof(argv[6]) * 2 * M_PI / 360.;
    TransformType::AngleType angleX = atof(argv[6]);
    std::cout << "axisX: " << axisX << std::endl;
    transform -> SetRotation( axisX, angleX );



    axisVector[0] = 0.0;
    axisVector[1] = 1.0;
    const TransformType::AxisType & axisY = axisVector;
    TransformType::AngleType angleY = atof(argv[7]) * 2 * M_PI / 360.;
    transform -> SetRotation( axisY, angleY );

    axisVector[1] = 0.0;
    axisVector[2] = 1.0;
    const TransformType::AxisType & axisZ = axisVector;
    TransformType::AngleType angleZ = atof(argv[8]) * 2 * M_PI / 360.;
    transform -> SetRotation( axisZ, angleZ );


    //const TransformType::TranslationType& translation;
    /*
    resampler -> SetTransform( transform );
    resampler->SetSize( imageReader-> GetOutput() -> GetLargestPossibleRegion().GetSize() );
    resampler -> SetOutputOrigin( imageReader -> GetOutput() -> GetOrigin() );
    resampler -> SetOutputSpacing( imageReader -> GetOutput() -> GetSpacing() );
    resampler -> SetOutputDirection( imageReader -> GetOutput() -> GetDirection() );
    */

    short2doubleCaster -> SetInput ( imageReader -> GetOutput() );

//    resampler -> SetInput( short2doubleCaster ->GetOutput() );

    FlipFilterType::Pointer flipFilter = FlipFilterType::New();

    flipFilter -> SetInput(short2doubleCaster -> GetOutput());

    FlipFilterType::FlipAxesArrayType flipAxes;

    flipAxes[0] = 0;
    flipAxes[1] = 0;
    flipAxes[2] = 1;

    flipFilter -> SetFlipAxes( flipAxes );

//    double2shortCaster -> SetInput( resampler ->GetOutput() );

    double2shortCaster -> SetInput( flipFilter -> GetOutput() );

    imageWriter -> SetInput( double2shortCaster -> GetOutput() );

    imageWriter -> Update();

}