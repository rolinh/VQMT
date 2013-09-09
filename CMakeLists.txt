cmake_minimum_required(VERSION 2.8.4)

# project information
project(vqmt CXX)
set(WEBSITE "http:/mmspg.epfl.ch/vqmt")
set(COPYRIGHT "(c) 2013 Philippe Hanhart")
set(DESCRIPTION "Compute PSNR, SSIM, MS-SSIM, VIFp, PSNR-HVS and PSNR-HVS-M metrics between raw YUV videos using OpenCV")

set(PACKAGE ${CMAKE_PROJECT_NAME})
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# useful defines
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/${CMAKE_BUILD_TYPE})

add_definitions(-O2)
find_package(OpenCV REQUIRED)
set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})
set(SRCS
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Metric.cpp
    ${SOURCE_DIR}/MSSSIM.cpp
    ${SOURCE_DIR}/PSNR.cpp
    ${SOURCE_DIR}/PSNRHVS.cpp
    ${SOURCE_DIR}/SSIM.cpp
    ${SOURCE_DIR}/VideoYUV.cpp
    ${SOURCE_DIR}/VIFP.cpp
)
add_executable(
    ${EXECUTABLE_NAME}
    ${SRCS}
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
