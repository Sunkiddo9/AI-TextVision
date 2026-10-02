# AI TextVision

AI TextVision is a C++ OCR (Optical Character Recognition) application that preprocesses images with OpenCV and extracts text using Tesseract OCR.

The application also reports OCR confidence and compares it with an 80% confidence threshold.

## Features

- Load an image from a custom path or use `input/sample.png` by default.
- Convert the image to grayscale.
- Reduce noise with a 5 x 5 Gaussian blur.
- Apply adaptive Gaussian thresholding.
- Extract English text with Tesseract's LSTM OCR engine.
- Report the average OCR confidence.
- Classify results as `HIGH CONFIDENCE` or `LOW CONFIDENCE`.
- Save the processed image and OCR result to the `output/` directory.

## Processing Pipeline

```text
Input image
    |
    v
Load image with OpenCV
    |
    v
Convert to grayscale
    |
    v
Apply Gaussian blur
    |
    v
Apply adaptive thresholding
    |
    v
Recognize text with Tesseract OCR
    |
    v
Calculate confidence
    |
    v
Save processed image and recognized text
```

## Technologies

| Technology | Purpose |
| --- | --- |
| C++17 | Core application language |
| OpenCV 4.x | Image loading and preprocessing |
| Tesseract OCR 5.x | Text recognition |
| CMake 3.20+ | Build configuration |
| vcpkg | C++ dependency management |
| Visual Studio 2022 / MSVC | Windows compiler |

## Requirements

- Windows 10 or Windows 11, x64
- Visual Studio 2022 with the **Desktop development with C++** workload
- CMake 3.20 or later
- Git
- vcpkg
- OpenCV 4.x
- Tesseract OCR 5.x

## Project Structure

```text
AI-TextVision/
├── CMakeLists.txt
├── vcpkg.json
├── README.md
├── src/
│   └── main.cpp
├── input/
│   └── sample.png
├── output/
│   ├── processed.png
│   └── recognized.txt
└── tessdata/
    └── eng.traineddata
```

The `output/` directory and its files are created or overwritten when the application runs.

## Installation

### 1. Install vcpkg

If vcpkg is not already installed:

```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

### 2. Install dependencies

From the project directory, install the dependencies for the x64 Windows triplet:

```powershell
C:\vcpkg\vcpkg.exe install --triplet x64-windows
```

The required packages are declared in `vcpkg.json`:

- `opencv4`
- `tesseract`

### 3. Install the Tesseract language data

Make sure the English trained-data file exists at:

```text
tessdata/eng.traineddata
```

The application loads this file using the relative `tessdata` path, so run the executable from the project root or ensure that the `tessdata` directory is available in the current working directory.

## Build

From the project root, configure the project with CMake:

```powershell
cmake -B build -S . -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
```

Build the Release configuration:

```powershell
cmake --build build --config Release
```

## Run

Run the executable from the project root:

```powershell
.\build\Release\AI-TextVision.exe
```

When prompted:

- Press **Enter** to process `input/sample.png`.
- Enter a relative or absolute path to process a different image, such as `input/my-image.png`.

## Output

After a successful run, the application produces:

```text
output/processed.png
```

The thresholded image passed to the OCR engine.

```text
output/recognized.txt
```

The recognized text, OCR confidence, required confidence, and result status.

## Example Output

```text
=============================================
              RECOGNIZED TEXT
=============================================

Spider-Man: Neighbourhood Hero

=============================================
              OCR ANALYSIS
=============================================
OCR Confidence: 85%
Required Confidence: 80%

STATUS: HIGH CONFIDENCE
OCR result passed the confidence threshold.
```

## Troubleshooting

### Could not open image

Check that the image path is correct, the file exists, and the image format is supported.

### Could not initialize Tesseract

Verify that `tessdata/eng.traineddata` exists and that the application is being run from the project root.

### Low OCR confidence

Use a clear, well-lit image with minimal skew and a readable font. Preprocessing results can vary depending on image quality and layout.

### CMake cannot find OpenCV or Tesseract

Check that vcpkg is installed and that `CMAKE_TOOLCHAIN_FILE` points to the correct `vcpkg.cmake` file.

## Future Improvements

- Add support for additional languages, including Hindi and other regional Indian scripts.
- Add webcam-based real-time text extraction.
- Add perspective correction and automatic deskewing.
- Add a graphical user interface.
- Add batch image processing and PDF export.

## Author

**Vikram Yadav**

GitHub: [@Sunkiddo9](https://github.com/Sunkiddo9)
