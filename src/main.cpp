#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

namespace fs = std::filesystem;

int main()
{
    std::cout << "=============================================\n";
    std::cout << "             AI TextVision\n";
    std::cout << "        Intelligent OCR System\n";
    std::cout << "=============================================\n\n";

    // -------------------------------------------------
    // 1. Create required directories
    // -------------------------------------------------

    fs::create_directories("input");
    fs::create_directories("output");

    // -------------------------------------------------
    // 2. Ask user for image path
    // -------------------------------------------------

    std::string inputPath;

    std::cout << "Enter image path\n";
    std::cout << "(Press Enter to use input/sample.png): ";

    std::getline(std::cin, inputPath);

    if (inputPath.empty())
    {
        inputPath = "input/sample.png";
    }

    std::cout << "\nInput image: " << inputPath << "\n";

    // -------------------------------------------------
    // 3. Load image
    // -------------------------------------------------

    cv::Mat image = cv::imread(inputPath);

    if (image.empty())
    {
        std::cerr << "\nERROR: Could not open image.\n";
        std::cerr << "Please check the image path.\n";
        return 1;
    }

    std::cout << "[1] Image loaded successfully.\n";
    std::cout << "    Width  : " << image.cols << " pixels\n";
    std::cout << "    Height : " << image.rows << " pixels\n";

    // -------------------------------------------------
    // 4. Convert to grayscale
    // -------------------------------------------------

    cv::Mat gray;

    cv::cvtColor(
        image,
        gray,
        cv::COLOR_BGR2GRAY
    );

    std::cout << "[2] Converted to grayscale.\n";

    // -------------------------------------------------
    // 5. Apply Gaussian blur
    // -------------------------------------------------

    cv::Mat blurred;

    cv::GaussianBlur(
        gray,
        blurred,
        cv::Size(5, 5),
        0
    );

    std::cout << "[3] Gaussian blur applied.\n";

    // -------------------------------------------------
    // 6. Adaptive thresholding
    // -------------------------------------------------

    cv::Mat thresholded;

    cv::adaptiveThreshold(
        blurred,
        thresholded,
        255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY,
        31,
        11
    );

    std::cout << "[4] Adaptive threshold applied.\n";

    // -------------------------------------------------
    // 7. Save processed image
    // -------------------------------------------------

    std::string processedPath = "output/processed.png";

    if (!cv::imwrite(processedPath, thresholded))
    {
        std::cerr << "WARNING: Could not save processed image.\n";
    }
    else
    {
        std::cout << "[5] Processed image saved to:\n";
        std::cout << "    " << processedPath << "\n";
    }

    // -------------------------------------------------
    // 8. Initialize Tesseract
    // -------------------------------------------------

    tesseract::TessBaseAPI ocr;

    if (ocr.Init(
            "tessdata",
            "eng",
            tesseract::OEM_LSTM_ONLY) != 0)
    {
        std::cerr << "\nERROR: Could not initialize Tesseract.\n";
        std::cerr << "Make sure tessdata/eng.traineddata exists.\n";
        return 1;
    }

    std::cout << "[6] Tesseract initialized successfully.\n";

    // -------------------------------------------------
    // 9. Configure OCR
    // -------------------------------------------------

    ocr.SetPageSegMode(
        tesseract::PSM_AUTO
    );

    ocr.SetImage(
        thresholded.data,
        thresholded.cols,
        thresholded.rows,
        1,
        static_cast<int>(thresholded.step)
    );

    // -------------------------------------------------
    // 10. Perform OCR
    // -------------------------------------------------

    if (ocr.Recognize(nullptr) != 0)
    {
        std::cerr << "\nERROR: OCR processing failed.\n";
        ocr.End();
        return 1;
    }

    // -------------------------------------------------
    // 11. Get recognized text
    // -------------------------------------------------

    char* outputText = ocr.GetUTF8Text();

    if (outputText == nullptr)
    {
        std::cerr << "\nERROR: No text detected.\n";
        ocr.End();
        return 1;
    }

    // -------------------------------------------------
    // 12. Get confidence
    // -------------------------------------------------

    int confidence = ocr.MeanTextConf();

    // -------------------------------------------------
    // 13. Display OCR result
    // -------------------------------------------------

    std::cout << "\n=============================================\n";
    std::cout << "              RECOGNIZED TEXT\n";
    std::cout << "=============================================\n\n";

    std::cout << outputText << "\n";

    // -------------------------------------------------
    // 14. Display confidence
    // -------------------------------------------------

    std::cout << "=============================================\n";
    std::cout << "              OCR ANALYSIS\n";
    std::cout << "=============================================\n";

    std::cout << "OCR Confidence: "
              << confidence
              << "%\n";

    // -------------------------------------------------
    // 15. Confidence validation
    // -------------------------------------------------

    const int REQUIRED_CONFIDENCE = 80;

    std::cout << "Required Confidence: "
              << REQUIRED_CONFIDENCE
              << "%\n\n";

    if (confidence >= REQUIRED_CONFIDENCE)
    {
        std::cout << "STATUS: HIGH CONFIDENCE\n";
        std::cout << "OCR result passed the confidence threshold.\n";
    }
    else
    {
        std::cout << "STATUS: LOW CONFIDENCE\n";
        std::cout << "OCR result may require manual verification.\n";
    }

    // -------------------------------------------------
    // 16. Save recognized text
    // -------------------------------------------------

    std::string textPath = "output/recognized.txt";

    std::ofstream outputFile(textPath);

    if (outputFile.is_open())
    {
        outputFile << "AI TextVision - OCR Result\n";
        outputFile << "=====================================\n\n";

        outputFile << outputText;

        outputFile << "\n\n=====================================\n";
        outputFile << "OCR Confidence: "
                   << confidence
                   << "%\n";

        outputFile << "Required Confidence: "
                   << REQUIRED_CONFIDENCE
                   << "%\n";

        if (confidence >= REQUIRED_CONFIDENCE)
        {
            outputFile << "Status: HIGH CONFIDENCE\n";
        }
        else
        {
            outputFile << "Status: LOW CONFIDENCE\n";
        }

        outputFile.close();

        std::cout << "\n[7] OCR result saved to:\n";
        std::cout << "    " << textPath << "\n";
    }
    else
    {
        std::cerr << "\nWARNING: Could not save OCR result.\n";
    }

    // -------------------------------------------------
    // 17. Cleanup
    // -------------------------------------------------

    delete[] outputText;

    ocr.End();

    std::cout << "\n=============================================\n";
    std::cout << "       OCR PROCESSING COMPLETED\n";
    std::cout << "=============================================\n";

    return 0;
}