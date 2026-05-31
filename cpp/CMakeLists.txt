cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(TrafficSignRecognition)

include_directories(include)
add_executable(traffic-signs-recognition
        src/ImageLoader.cpp
        src/ImageLoader.cpp
        src/Preprocessor.cpp
        main.cpp
        src/Classifier.cpp
        src/Postprocessor.cpp
        include/Postprocessor.h
        src/BatchRunner.cpp
        include/BatchRunner.h
)

set(ONNXRUNTIME_DIR "${CMAKE_SOURCE_DIR}/../libs/onnxruntime-linux-x64-1.26.0")

include_directories(include)
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/lib)
find_library(ONNXRUNTIME_LIB NAMES onnxruntime PATHS "${ONNXRUNTIME_DIR}/lib")
target_link_libraries(traffic-signs-recognition PRIVATE ${ONNXRUNTIME_LIB})
#target_link_libraries(traffic-signs-recognition PRIVATE onnxruntime)