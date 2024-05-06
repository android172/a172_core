cmake_minimum_required(VERSION 3.23.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

project(a172_core)

# add sources
file(GLOB_RECURSE SOURCES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/**/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/**/*.h
    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/include/**/*.hpp)
# add_executable(${PROJECT_NAME}
#     ${SOURCES})
add_library(${PROJECT_NAME}
    ${SOURCES})


# include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
    include
)

# link
target_link_directories(${PROJECT_NAME}
    PRIVATE
    src
)
