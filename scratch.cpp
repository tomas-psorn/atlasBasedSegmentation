//
// Created by psorn on 27.4.17.
//

/* Path to example data
/home/tomas/Dev/ITK/Examples/Data/
*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"

#include "itkTranslationTransform.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkImageRegistrationMethodv4.h"

#include "itkLinearInterpolateImageFunction.h"
#include "itkRegistrationParameterScalesFromPhysicalShift.h"

int main(int argc, char * argv[]){

    if( argc < 4 )
    {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << " fixedImageFile  movingImageFile ";
        std::cerr << "outputImagefile [differenceImageAfter]";
        std::cerr << "[differenceImageBefore] [useEstimator]" << std::endl;
        return EXIT_FAILURE;
    }

    const    unsigned int    Dimension = 2;
    typedef  float           PixelType;

    typedef itk::Image< PixelType, Dimension >  FixedImageType;
    typedef itk::Image< PixelType, Dimension >  MovingImageType;

    typedef itk::TranslationTransform< double, Dimension > TransformType;

    typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;

    typedef itk::MeanSquaresImageToImageMetricv4<
            FixedImageType,
            MovingImageType >    MetricType;

    typedef itk::ImageRegistrationMethodv4<
            FixedImageType,
            MovingImageType,
            TransformType   >    RegistrationType;

    MetricType::Pointer         metric        = MetricType::New();
    OptimizerType::Pointer      optimizer     = OptimizerType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();

    registration->SetMetric(        metric        );
    registration->SetOptimizer(     optimizer     );



    typedef itk::ImageFileReader< FixedImageType  >   FixedImageReaderType;
    typedef itk::ImageFileReader< MovingImageType >   MovingImageReaderType;
    FixedImageReaderType::Pointer   fixedImageReader     = FixedImageReaderType::New();
    MovingImageReaderType::Pointer  movingImageReader    = MovingImageReaderType::New();

    fixedImageReader->SetFileName(  argv[1] );
    movingImageReader->SetFileName( argv[2] );

    registration->SetFixedImage(    fixedImageReader->GetOutput()    );
    registration->SetMovingImage(   movingImageReader->GetOutput()   );

    TransformType::Pointer movingInitialTransform = TransformType::New();

    TransformType::ParametersType initialParameters(
            movingInitialTransform->GetNumberOfParameters() );
    initialParameters[0] = 0.0;  // Initial offset in mm along X
    initialParameters[1] = 0.0;  // Initial offset in mm along Y

    movingInitialTransform->SetParameters( initialParameters );

    registration->SetMovingInitialTransform( movingInitialTransform );

    TransformType::Pointer   identityTransform = TransformType::New();
    identityTransform->SetIdentity();

    registration->SetFixedInitialTransform( identityTransform );

    optimizer->SetLearningRate( 4 );
    optimizer->SetMinimumStepLength( 0.001 );
    optimizer->SetRelaxationFactor( 0.5 );

    bool useEstimator = false;
    if( argc > 6 )
    {
        useEstimator = atoi(argv[6]) != 0;
    }

    std::cout <<"use? " << useEstimator << std::endl;

    if( useEstimator )
    {
        typedef itk::RegistrationParameterScalesFromPhysicalShift<MetricType> ScalesEstimatorType;
        ScalesEstimatorType::Pointer scalesEstimator = ScalesEstimatorType::New();
        scalesEstimator->SetMetric( metric );
        scalesEstimator->SetTransformForward( true );
        optimizer->SetScalesEstimator( scalesEstimator );
        optimizer->SetDoEstimateLearningRateOnce( true );
    }


    //  Software Guide : BeginLatex
    //
    //  In case the optimizer never succeeds reaching the desired
    //  precision tolerance, it is prudent to establish a limit on the number of
    //  iterations to be performed. This maximum number is defined with the
    //  method \code{SetNumberOfIterations()}.
    //
    //  \index{itk::Gradient\-Descent\-Optimizerv4\-Template!SetNumberOfIterations()}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    optimizer->SetNumberOfIterations( 200 );
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  ITKv4 facilitates a multi-level registration framework whereby each stage is
    //  different in the resolution of its virtual space and the smoothness of the
    //  fixed and moving images.
    //  These criteria need to be defined before registration starts. Otherwise,
    //  the default values will be used.
    //  In this example, we run a simple registration in one level with no
    //  space shrinking or smoothing on the input data.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    const unsigned int numberOfLevels = 1;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 1 );
    shrinkFactorsPerLevel[0] = 1;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 1 );
    smoothingSigmasPerLevel[0] = 0;

    registration->SetNumberOfLevels ( numberOfLevels );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  The registration process is triggered by an invocation of the
    //  \code{Update()} method. If something goes wrong during the
    //  initialization or execution of the registration an exception will be
    //  thrown. We should therefore place the \code{Update()} method
    //  inside a \code{try/catch} block as illustrated in the following lines.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    try
    {
        registration->Update();
        std::cout << "Optimizer stop condition: "
                  << registration->GetOptimizer()->GetStopConditionDescription()
                  << std::endl;
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  In a real life application, you may attempt to recover from the error by
    //  taking more effective actions in the catch block. Here we are simply
    //  printing out a message and then terminating the execution of the program.
    //
    //
    //  The result of the registration process is obtained using the \code{GetTransform()}
    //  method that returns a constant pointer to the output transform.
    //
    //  \index{itk::ImageRegistrationMethodv4!GetTransform()}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    TransformType::ConstPointer transform = registration->GetTransform();
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  In the case of the \doxygen{TranslationTransform}, there is a
    //  straightforward interpretation of the parameters.  Each element of the
    //  array corresponds to a translation along one spatial dimension.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    TransformType::ParametersType finalParameters = transform->GetParameters();
    const double TranslationAlongX = finalParameters[0];
    const double TranslationAlongY = finalParameters[1];
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  The optimizer can be queried for the actual number of iterations
    //  performed to reach convergence.  The \code{GetCurrentIteration()}
    //  method returns this value. A large number of iterations may be an
    //  indication that the learning rate has been set too small, which
    //  is undesirable since it results in long computational times.
    //
    //  \index{itk::Gradient\-Descent\-Optimizerv4\-Template!GetCurrentIteration()}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  The value of the image metric corresponding to the last set of parameters
    //  can be obtained with the \code{GetValue()} method of the optimizer.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    const double bestValue = optimizer->GetValue();
    // Software Guide : EndCodeSnippet


    // Print out results
    //
    std::cout << "Result = " << std::endl;
    std::cout << " Translation X = " << TranslationAlongX  << std::endl;
    std::cout << " Translation Y = " << TranslationAlongY  << std::endl;
    std::cout << " Iterations    = " << numberOfIterations << std::endl;
    std::cout << " Metric value  = " << bestValue          << std::endl;


    //  Software Guide : BeginLatex
    //
    //  Let's execute this example over two of the images provided in
    //  \code{Examples/Data}:
    //
    //  \begin{itemize}
    //  \item \code{BrainProtonDensitySliceBorder20.png}
    //  \item \code{BrainProtonDensitySliceShifted13x17y.png}
    //  \end{itemize}
    //
    //  The second image is the result of intentionally translating the first
    //  image by $(13,17)$ millimeters. Both images have unit-spacing and
    //  are shown in Figure \ref{fig:FixedMovingImageRegistration1}. The
    //  registration takes 20 iterations and the resulting transform parameters are:
    //
    //  \begin{verbatim}
    //  Translation X = 13.0012
    //  Translation Y = 16.9999
    //  \end{verbatim}
    //
    //  As expected, these values match quite well the misalignment that we
    //  intentionally introduced in the moving image.
    //
    // \begin{figure}
    // \center
    // \includegraphics[width=0.44\textwidth]{BrainProtonDensitySliceBorder20}
    // \includegraphics[width=0.44\textwidth]{BrainProtonDensitySliceShifted13x17y}
    // \itkcaption[Fixed and Moving images in registration framework]{Fixed and
    // Moving image provided as input to the registration method.}
    // \label{fig:FixedMovingImageRegistration1}
    // \end{figure}
    //
    //
    //  Software Guide : EndLatex


    //  Software Guide : BeginLatex
    //
    //  It is common, as the last step of a registration task, to use the
    //  resulting transform to map the moving image into the fixed image space.
    //
    //  Before the mapping process, notice that we have not used the direct initialization
    //  of the output transform in this example, so the parameters of the moving initial
    //  transform are not reflected in the output parameters of the registration filter.
    //  Hence, a composite transform is needed to concatenate both initial and output
    //  transforms together.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef itk::CompositeTransform<
            double,
            Dimension > CompositeTransformType;
    CompositeTransformType::Pointer outputCompositeTransform =
            CompositeTransformType::New();
    outputCompositeTransform->AddTransform( movingInitialTransform );
    outputCompositeTransform->AddTransform(
            registration->GetModifiableTransform() );
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  Now the mapping process is easily done with the \doxygen{ResampleImageFilter}.
    //  Please refer to Section~\ref{sec:ResampleImageFilter} for details on the use
    //  of this filter.  First, a ResampleImageFilter type is instantiated
    //  using the image types. It is convenient to use the fixed image type as
    //  the output type since it is likely that the transformed moving image
    //  will be compared with the fixed image.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef itk::ResampleImageFilter<
            MovingImageType,
            FixedImageType >    ResampleFilterType;
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  A resampling filter is created and the moving image is connected as
    //  its input.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetInput( movingImageReader->GetOutput() );
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  The created output composite transform is also passed as input to the
    //  resampling filter.
    //
    //  \index{itk::ImageRegistrationMethod!Resampling image}
    //  \index{itk::ImageRegistrationMethod!Pipeline}
    //  \index{itk::ImageRegistrationMethod!DataObjectDecorator}
    //  \index{itk::ImageRegistrationMethod!GetOutput()}
    //  \index{itk::DataObjectDecorator!Use in Registration}
    //  \index{itk::DataObjectDecorator!Get()}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    resampler->SetTransform( outputCompositeTransform );
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  As described in Section \ref{sec:ResampleImageFilter}, the
    //  ResampleImageFilter requires additional parameters to be specified, in
    //  particular, the spacing, origin and size of the output image. The default
    //  pixel value is also set to a distinct gray level in order to highlight
    //  the regions that are mapped outside of the moving image.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
    resampler->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin(  fixedImage->GetOrigin() );
    resampler->SetOutputSpacing( fixedImage->GetSpacing() );
    resampler->SetOutputDirection( fixedImage->GetDirection() );
    resampler->SetDefaultPixelValue( 100 );
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    // \begin{figure}
    // \center
    // \includegraphics[width=0.32\textwidth]{ImageRegistration1Output}
    // \includegraphics[width=0.32\textwidth]{ImageRegistration1DifferenceBefore}
    // \includegraphics[width=0.32\textwidth]{ImageRegistration1DifferenceAfter}
    // \itkcaption[HelloWorld registration output images]{Mapped moving image and its
    // difference with the fixed image before and after registration}
    // \label{fig:ImageRegistration1Output}
    // \end{figure}
    //
    //  Software Guide : EndLatex


    //  Software Guide : BeginLatex
    //
    //  The output of the filter is passed to a writer that will store the
    //  image in a file. An \doxygen{CastImageFilter} is used to convert the
    //  pixel type of the resampled image to the final type used by the
    //  writer. The cast and writer filters are instantiated below.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef unsigned char                            OutputPixelType;

    typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

    typedef itk::CastImageFilter<
            FixedImageType,
            OutputImageType >          CastFilterType;

    typedef itk::ImageFileWriter< OutputImageType >  WriterType;
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  The filters are created by invoking their \code{New()}
    //  method.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    WriterType::Pointer      writer =  WriterType::New();
    CastFilterType::Pointer  caster =  CastFilterType::New();
    // Software Guide : EndCodeSnippet


    writer->SetFileName( argv[3] );


    //  Software Guide : BeginLatex
    //
    //  The filters are connected together and the \code{Update()} method of the
    //  writer is invoked in order to trigger the execution of the pipeline.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    caster->SetInput( resampler->GetOutput() );
    writer->SetInput( caster->GetOutput()   );
    writer->Update();
    return EXIT_SUCCESS;
}
