cmake_minimum_required(VERSION 3.20)

project(AI_TextVision VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV CONFIG REQUIRED)
find_package(Tesseract CONFIG REQUIRED)

add_executable(AI-TextVision
    src/main.cpp
)

target_link_libraries(AI-TextVision
    PRIVATE
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
        Tesseract::libtesseract
)