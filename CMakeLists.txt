cmake_minimum_required(VERSION 3.14)
project(ForestAudioClustering VERSION 1.0 LANGUAGES CXX)

# Automatically generate compile_commands.json for Vim/clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enforce C++17 for standard library features (like std::filesystem if needed later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Python and NumPy development libraries required by matplotlibcpp
find_package(Python3 COMPONENTS Development NumPy REQUIRED)

# Explicitly list source files (Best practice over GLOB)
set(SOURCES
    main.cpp
    AudioIO/wav_parser.cpp
    AudioIO/csv_io.cpp
    DSP/dsp.cpp
    Features/mfcc.cpp
    Clustering/Kmeans/kmeans.cpp
    Clustering/Utilities/utility.cpp
    Visualization/plotter.cpp
)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Tell the compiler where to look for header files (.hpp and .h)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}                # Allows #include "DSP/dsp.hpp" from anywhere
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party    # Allows #include "matplotlibcpp.h"
    ${Python3_INCLUDE_DIRS}                    # Python headers for matplotlibcpp
)

# Link the executable against Python to allow matplotlibcpp to call the Python C-API
target_link_libraries(${PROJECT_NAME} PRIVATE
    Python3::Python
)
