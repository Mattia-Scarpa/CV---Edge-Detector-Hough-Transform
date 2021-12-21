#----------------------------------------------------------------------
# CMakeList.txt: CMake Project

cmake_minimum_required (VERSION 3.8)

project ("roadDetector")

# Add tests and, if necessary, install destinations

find_package(OpenCV REQUIRED)

# Generate the executable
add_executable (${PROJECT_NAME} "src/main.cpp")

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIB_DIR})

add_subdirectory(include)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} include)

