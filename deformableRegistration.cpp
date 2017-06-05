//
// Created by psorn on 5.6.17.
//

#include "deformableRegistration.h"

int main(int argc, char *argv[]) {

    const char *fixedImageName, *movingImageName, *outputImageName;
    if ( argc < 3 )
    {
        std::cout << "Image file names missing" << std::endl;
        std::cout << "Usage: " << argv[0] << " fixedImageFile movingImageFile"
                  << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        fixedImageName = argv[1];
        movingImageName = argv[2];
        outputImageName = argv[3];
    }

    RegistrationType::Pointer registrationFilter = RegistrationType::New();
    registrationFilter->SetMaxLevel(1);
    registrationFilter->SetUseNormalizedGradient( true );
    registrationFilter->ChooseMetric( 0 );

    unsigned int maxiters = 2000;
    float        E = 100;
    float        p = 1;
    registrationFilter->SetElasticity(E, 100);
    registrationFilter->SetRho(p, 0);
    registrationFilter->SetGamma(1., 0);
    registrationFilter->SetAlpha(1.0);
    registrationFilter->SetMaximumIterations( maxiters, 0 );
    registrationFilter->SetMeshPixelsPerElementAtEachResolution(4, 0);
    registrationFilter->SetWidthOfMetricRegion(1, 0);
    registrationFilter->SetNumberOfIntegrationPoints(100, 0);
    registrationFilter->SetDoLineSearchOnImageEnergy( 0 );
    registrationFilter->SetTimeStep(2.);
    registrationFilter->SetEmployRegridding(false);
    registrationFilter->SetUseLandmarks(false);

    FileSourceType::Pointer movingfilter = FileSourceType::New();
    movingfilter->SetFileName( movingImageName );
    FileSourceType::Pointer fixedfilter = FileSourceType::New();
    fixedfilter->SetFileName( fixedImageName );
    std::cout << " reading moving " << movingImageName << std::endl;
    std::cout << " reading fixed " << fixedImageName << std::endl;

    try
    {
        movingfilter->Update();
    }
    catch( itk::ExceptionObject & e )
    {
        std::cerr << "Exception caught during reference file reading " << std::endl;
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }
    try
    {
        fixedfilter->Update();
    }
    catch( itk::ExceptionObject & e )
    {
        std::cerr << "Exception caught during target file reading " << std::endl;
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }

    typedef itk::RescaleIntensityImageFilter<DiskImageType,ImageType> FilterType;
    FilterType::Pointer movingrescalefilter = FilterType::New();
    FilterType::Pointer fixedrescalefilter = FilterType::New();

    movingrescalefilter->SetInput(movingfilter->GetOutput());
    fixedrescalefilter->SetInput(fixedfilter->GetOutput());

    const double desiredMinimum =  0.0;
    const double desiredMaximum =  255.0;

    movingrescalefilter->SetOutputMinimum( desiredMinimum );
    movingrescalefilter->SetOutputMaximum( desiredMaximum );
    movingrescalefilter->UpdateLargestPossibleRegion();
    fixedrescalefilter->SetOutputMinimum( desiredMinimum );
    fixedrescalefilter->SetOutputMaximum( desiredMaximum );
    fixedrescalefilter->UpdateLargestPossibleRegion();

    HEFilterType::Pointer IntensityEqualizeFilter = HEFilterType::New();

    IntensityEqualizeFilter->SetReferenceImage( fixedrescalefilter->GetOutput() );
    IntensityEqualizeFilter->SetInput( movingrescalefilter->GetOutput() );
    IntensityEqualizeFilter->SetNumberOfHistogramLevels( 100);
    IntensityEqualizeFilter->SetNumberOfMatchPoints( 15);
    IntensityEqualizeFilter->ThresholdAtMeanIntensityOn();
    IntensityEqualizeFilter->Update();

    // Set the images for registration filter
    registrationFilter->SetFixedImage(fixedrescalefilter->GetOutput());
    registrationFilter->SetMovingImage(IntensityEqualizeFilter->GetOutput());

    itk::ImageFileWriter<ImageType>::Pointer writer;
    writer = itk::ImageFileWriter<ImageType>::New();
    writer->SetFileName(argv[3]); // fixed
    writer->SetInput(registrationFilter->GetFixedImage() );

    try
    {
        writer->Write();
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }

    itk::ImageFileWriter<ImageType>::Pointer writer2;
    writer2 =  itk::ImageFileWriter<ImageType>::New();
    writer2->SetFileName(argv[4]); // moving
    writer2->SetInput(registrationFilter->GetMovingImage() );

    try
    {
        writer2->Write();
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }

    itk::fem::MaterialLinearElasticity::Pointer m;
    m = itk::fem::MaterialLinearElasticity::New();
    m->SetGlobalNumber(0);
    // Young's modulus of the membrane
    m->SetYoungsModulus(registrationFilter->GetElasticity());
    m->SetCrossSectionalArea(1.0);  // Cross-sectional area
    m->SetThickness(1.0);           // Thickness
    m->SetMomentOfInertia(1.0);     // Moment of inertia
    m->SetPoissonsRatio(0.);        // Poisson's ratio -- DONT CHOOSE 1.0!!
    m->SetDensityHeatProduct(1.0);  // Density-Heat capacity product

    // Create the element type
    ElementType::Pointer e1=ElementType::New();
    e1->SetMaterial(m.GetPointer());
    registrationFilter->SetElement(e1.GetPointer());
    registrationFilter->SetMaterial(m);

    registrationFilter->RunRegistration();

    itk::ImageFileWriter<ImageType>::Pointer warpedImageWriter;
    warpedImageWriter = itk::ImageFileWriter<ImageType>::New();
    warpedImageWriter->SetInput( registrationFilter->GetWarpedImage() );
    warpedImageWriter->SetFileName(argv[5]); // wrapped
    try
    {
        warpedImageWriter->Update();
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }

    typedef itk::ImageFileWriter<RegistrationType::FieldType> DispWriterType;
    DispWriterType::Pointer dispWriter = DispWriterType::New();
    dispWriter->SetInput( registrationFilter->GetDisplacementField() );
    dispWriter->SetFileName(argv[6]); // displacement
    try
    {
        dispWriter->Update();
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;

}