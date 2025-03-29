cmake_minimum_required(VERSION 3.16)
project(CollisionCraft VERSION 0.1 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set platform flags
message(STATUS "Resolving target platform architecture")
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        message(STATUS "Targeting MacOS Metal (Arm64)")
        set(CMAKE_OSX_ARCHITECTURES arm64)
    else()
        message(STATUS "Targeting MacOS (Intel)")
        set(CMAKE_OSX_ARCHITECTURES x86_64)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        message(STATUS "Targeting Linux x86_64")
        ##set(CMAKE_OSX_ARCHITECTURES arm64)
    else()
        message(STATUS "Targeting Linux x86_32")
        #set(CMAKE_OSX_ARCHITECTURES x86_64)
    endif()
else()
    message(FATAL_ERROR "Unsupported target architecture")
endif()

# Build paths
set(CMAKE_BINARY_DIR ./bin/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ./source/)

set(CMAKE_AUTOMOC ON)

# External dependencies
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    OpenGL
    OpenGLWidgets
)

# Define source and header files
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/logging.cpp
    ${PROJECT_SOURCE_DIR}/appwindow.cpp
    ${PROJECT_SOURCE_DIR}/viewportwidget.cpp
)

# Define resources
qt_add_resources(PROCESSED_RESOURCES resources/resources.qrc)

# Create executable target and link modules
add_executable(CollisionCraft ${SOURCES} ${PROCESSED_RESOURCES})
target_link_libraries(CollisionCraft PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
