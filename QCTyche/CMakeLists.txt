cmake_minimum_required(VERSION 3.15)
project(QCTyche
        VERSION         1.0.0
        DESCRIPTION     "Quality control software ..."
        HOMEPAGE_URL    "https://github.com/thahnen/QCTyche"
        LANGUAGES       CXX)

set(CMAKE_CXX_STANDARD 14)

# Set prefix path for CMake to find libRoyale-CMakeLists.txt
#set(CMAKE_PREFIX_PATH "libroyale/share")


# OpenCV required for image processing
if (WIN32)
    # By now OpenCV has to be installed here for CMake to find it!
    set(OPENCV_DIR C:/opencv/build/x64/vc15/lib)
    find_package(OpenCV REQUIRED)
else()
    find_package(OpenCV REQUIRED)
endif(WIN32)


# Royale library required for image processing
#find_package(royale REQUIRED)
#link_directories(${royale_LIB_DIR})


include_directories(./include
                    ${OPENCV_INCLUDE_DIRS}
                    #./cvui
        )

add_executable(QCTyche
        #include/CameraListener.h
        #src/CameraListener.cpp
        main.cpp)

#COPY_ROYALE_LIBS(QCTYche)

target_link_libraries(QCTyche
        PUBLIC
            ${OPENCV_LIBS})
            #"${royale_LIBS}")
