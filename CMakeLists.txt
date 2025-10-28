cmake_minimum_required(VERSION 3.15)
project(Checkers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Print vcpkg path for sanity
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# define path for assets
add_definitions(-DASSET_PATH="${CMAKE_SOURCE_DIR}/assets/")


# Create the executable
add_executable(Checkers ${SOURCES})

# Now link libraries
find_package(glm CONFIG REQUIRED)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(Checkers PRIVATE glfw OpenGL::GL imgui::imgui glad::glad glm::glm)
