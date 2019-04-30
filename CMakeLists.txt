cmake_minimum_required(VERSION 3.5)

project(hdf5_incremental_output)
find_package(HDF5 COMPONENTS CXX REQUIRED)
include_directories(${HDF5_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 17)

add_executable(hdf5_incremental_output main.cpp)
target_link_libraries(hdf5_incremental_output LINK_PUBLIC ${HDF5_LIBRARIES})