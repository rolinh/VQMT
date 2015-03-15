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

# defines for installation

# installation prefix
if(DEFINED PREFIX)
	set(PREFIX ${PREFIX} CACHE PATH "install prefix")
	set(CMAKE_INSTALL_PREFIX ${PREFIX})
else()
	set(PREFIX ${CMAKE_INSTALL_PREFIX} CACHE PATH "install prefix")
endif()
mark_as_advanced(CMAKE_INSTALL_PREFIX)

# share directory
if(DEFINED SHAREDIR)
	set(SHAREDIR ${SHAREDIR} CACHE PATH "share directory")
else()
	set(SHAREDIR ${PREFIX}/share CACHE PATH "share directory")
endif()

# man path
# TODO uncomment the following once a manpage has been written
#if(DEFINED MAN_PATH)
#	set(MAN_PATH ${MAN_PATH} CACHE PATH "manpage directory")
#else()
#	set(MAN_PATH ${SHAREDIR}/man CACHE PATH "manpage directory")
#endif()

# documentation path
if(DEFINED DOC_PATH)
    set(VQMT_DOC_PATH ${DFC_DOC_PATH} CACHE PATH "doc directory")
else()
    set(VQMT_DOC_PATH ${SHAREDIR}/doc/${PACKAGE} CACHE PATH "doc directory")
endif()

# set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wdouble-promotion HAS_DOUBLE_PROMOTION)
check_cxx_compiler_flag(-Wsuggest-attribute=const HAS_SUGGEST_ATTRIBUTE_CONST)
check_cxx_compiler_flag(-Wsuggest-attribute=noreturn HAS_SUGGEST_ATTRIBUTE_NORETURN)
check_cxx_compiler_flag(-Wuseless-cast HAS_USELESS_CAST)
check_cxx_compiler_flag(-Wlogical-op HAS_LOGICAL_OP)
check_cxx_compiler_flag(-Wstrict-null-sentinel HAS_STRICT_NULL_SENTINEL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wformat=2 -Winit-self -Wmissing-include-dirs -Wswitch-default -Wfloat-equal -Wundef -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-conversion  -Wmissing-declarations -Wredundant-decls -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -pipe")

if(HAS_DOUBLE_PROMOTION)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion")
endif()
if(HAS_SUGGEST_ATTRIBUTE_CONST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=const")
endif()
if(HAS_SUGGEST_ATTRIBUTE_NORETURN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=noreturn")
endif()
if(HAS_USELESS_CAST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuseless-cast")
endif()
if(HAS_LOGICAL_OP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-op")
endif()
if(HAS_STRICT_NULL_SENTINEL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-null-sentinel")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -flto -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3 -ggdb3 -Wpadded -Wpacked")

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

set(VQMT_DOC_FILES
	AUTHORS.md
    CHANGELOG.md
	LICENSE
	README.md
    readme.txt
)

# rule to generate a source tarball
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION}")
set(CPACK_SOURCE_IGNORE_FILES
	"~$"
    ".directory"
	".swp$"
	"^${PROJECT_SOURCE_DIR}/.git/"
	"^${PROJECT_SOURCE_DIR}/build/"
	"${PROJECT_SOURCE_DIR}/.gitignore$"
	"${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
add_custom_target(dist
	COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# installation
install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)
# TODO uncomment the following once the manpage has been written
#install(FILES ${MAN_DIR}/vqmt.1 DESTINATION ${MAN_PATH}/man1)
install(FILES ${VQMT_DOC_FILES} DESTINATION ${VQMT_DOC_PATH})

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P
	${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

