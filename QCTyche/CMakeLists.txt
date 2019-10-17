cmake_minimum_required(VERSION 3.14)
project(QCTyche
        VERSION         1.0.0
        DESCRIPTION     "Quality control software ..."
        HOMEPAGE_URL    "https://github.com/thahnen/QCTyche"
        LANGUAGES       CXX)

set(CMAKE_CXX_STANDARD 14)


# OpenCV required for image processing
set(OpenCV_DIR C:/opencv/build/x64/vc15/lib)
set(OpenCV_INCLUDE_DIR C:/opencv/build/include)
find_package(OpenCV REQUIRED)

if (NOT OpenCV_FOUND)
    message("OpenCV was not found using 'OpenCV_DIR': ${OpenCV_DIR}")
    return()
endif()

link_directories(${OpenCV_LIB_DIR})


# Royale library required for camera interface
set(royale_DIR "C:/Program Files (x86)/royale/3.23.0.86/share")
#set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/royale/3.23.0.86/share")
set(royale_INCLUDE_DIR "C:/Program Files (x86)/royale/3.23.0.86/include")
find_package(royale REQUIRED)

link_directories(${royale_LIB_DIR})


include_directories(./include
        ${OpenCV_INCLUDE_DIR}
        ${royale_INCLUDE_DIR})

add_executable(QCTyche
        include/CameraListener.h
        src/CameraListener.cpp
        main.cpp)

COPY_ROYALE_LIBS(QCTyche)

target_link_libraries(QCTyche
        PUBLIC
            ${OPENCV_LIBS}
            ${royale_LIBS})
