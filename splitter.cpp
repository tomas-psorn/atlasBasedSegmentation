#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkImage.h"
#include <sstream>

int main( int argc, char ** argv )
{
    // Verify the number of parameters in the command line
    if( argc < 2 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " input3DImageFile  output2DImageFile " << std::endl;
        std::cerr << " sliceNumber " << std::endl;
        return EXIT_FAILURE;
    }

    typedef unsigned int        InputPixelType;
    typedef unsigned int       OutputPixelType;

    typedef itk::Image< InputPixelType,  3 >    InputImageType;
    typedef itk::Image< OutputPixelType, 2 >    OutputImageType;

    typedef itk::ImageFileReader< InputImageType  >  ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >  WriterType;

    const char * inputFilename  = argv[1];
    const char * outputPath = argv[2];

    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();

    reader->SetFileName( inputFilename  );

    typedef itk::ExtractImageFilter< InputImageType,
    OutputImageType > FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->InPlaceOn();
    filter->SetDirectionCollapseToSubmatrix();

    reader->UpdateOutputInformation();
    InputImageType::RegionType inputRegion =
            reader->GetOutput()->GetLargestPossibleRegion();

    InputImageType::SizeType size = inputRegion.GetSize();
    unsigned short numberOfSlices = size[2];
    InputImageType::IndexType start = inputRegion.GetIndex();
    InputImageType::RegionType desiredRegion;
    size[2] = 0;
    desiredRegion.SetSize(  size  );

    std::stringstream outputFilename;

    for (int sliceIndex = 0; sliceIndex < numberOfSlices; sliceIndex++){

        start[2] = sliceIndex;
        desiredRegion.SetIndex( start );

        filter->SetExtractionRegion( desiredRegion );
        filter->SetInput( reader->GetOutput() );

        outputFilename.str(std::string());
        outputFilename << outputPath << sliceIndex << ".dcm";

        writer->SetFileName( outputFilename.str() );
        writer->SetInput( filter->GetOutput() );

        try
        {
            writer->Update();
        }
        catch( itk::ExceptionObject & err )
        {
            std::cerr << "ExceptionObject caught !" << std::endl;
            std::cerr << err << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

