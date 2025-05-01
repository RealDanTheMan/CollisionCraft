cmake_minimum_required(VERSION 3.16)
project(CollisionCraft VERSION 0.1 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TMP flag to avoid compile issues with OpenUSD with older versions of TBB
add_compile_options(-Wno-error=changes-meaning)

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
set(PROJECT_SOURCE_DIR source/)

set(CMAKE_AUTOMOC ON)

# External dependencies
find_package(Boost REQUIRED)
find_package(CGAL REQUIRED)
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    OpenGL
    OpenGLWidgets
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Targeting MacOS OpenUSD libraries")
    set(USD_PATH lib/openusd/macos)
    set(VHACD_PATH lib/vhacd/macos)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Targeting Linux OpenUSD libraries")
    set(USD_PATH lib/openusd/linux)
    set(VHACD_PATH lib/vhacd/linux)
else()
    message(FATAL_ERROR "Unsupported target architecture for USD or VHACD")
endif()

set(EIGEN_INCLUDES "${CMAKE_SOURCE_DIR}/lib/eigen")
set(CGAL_INCLUDES "${CMAKE_SOURCE_DIR}/lib/cgal")
set(USD_INCLUDES ${USD_PATH}/include)
set(USD_LIB ${USD_PATH}/lib)
set(VHACD_INCLUDES ${VHACD_PATH}/include)
set(VHACD_LIB ${VHACD_PATH}/lib)
include_directories(BEFORE "${EIGEN_INCLUDES}")


set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/logging.cpp
    ${PROJECT_SOURCE_DIR}/graphics.cpp
    ${PROJECT_SOURCE_DIR}/mesh.cpp
    ${PROJECT_SOURCE_DIR}/rendermesh.cpp
    ${PROJECT_SOURCE_DIR}/scenemodel.cpp
    ${PROJECT_SOURCE_DIR}/modelloader.cpp
    ${PROJECT_SOURCE_DIR}/collisiongen.cpp
    ${PROJECT_SOURCE_DIR}/appwindow.cpp
    ${PROJECT_SOURCE_DIR}/viewportwidget.cpp
    ${PROJECT_SOURCE_DIR}/viewportsettingswidget.cpp
    ${PROJECT_SOURCE_DIR}/viewportcamera.cpp
    ${PROJECT_SOURCE_DIR}/logwidget.cpp
    ${PROJECT_SOURCE_DIR}/propertypanel.cpp
)


# Define resources
qt_add_resources(PROCESSED_RESOURCES resources/resources.qrc)

# Create executable target and link modules
add_executable(CollisionCraft ${SOURCES} ${PROCESSED_RESOURCES})
target_compile_definitions(CollisionCraft PRIVATE QT_NO_KEYWORDS)

target_include_directories(CollisionCraft PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${Python3_INCLUDE_DIRS}
    ${EIGEN_INCLUDES}
    ${CGAL_INCLUDES}
    ${USD_INCLUDES}
    ${VHACD_INCLUDES}
)

target_link_directories(CollisionCraft PRIVATE
    ${USD_LIB}
    ${VHACD_LIB}
)

target_link_libraries(CollisionCraft PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    
    # OpenUSD Modules
    usd_usd    
    usd_usdGeom
    usd_usdUtils
    usd_tf
    usd_gf
    usd_sdf
    usd_vt

    # CGAL Modules & its dependencies
    Boost::boost
    Eigen3::Eigen
    CGAL::CGAL

    # VHACD dependency
    libvhacd
)
