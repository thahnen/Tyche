cmake_minimum_required(VERSION 3.15)
project(QCTyche
        VERSION         1.0.0
        DESCRIPTION     "Quality control software ..."
        HOMEPAGE_URL    "https://github.com/thahnen/QCTyche"
        LANGUAGES       CXX)

set(CMAKE_CXX_STANDARD 14)

# Set prefix path for CMake to find libRoyale-CMakeLists.txt
set(CMAKE_PREFIX_PATH "libroyale/share")

# OpenCV required for image processing
find_package(OpenCV REQUIRED)

# Royale libarary required for image processing
find_package(royale REQUIRED)
link_directories(${royale_LIB_DIR})


include_directories(./include
                    ./cvui)

add_executable(QCTyche main.cpp)

COPY_ROYALE_LIBS(QCTYche)

target_link_libraries(QCTyche
        PUBLIC
            ${OPENCV_LIBS}
            "${royale_LIBS}")
