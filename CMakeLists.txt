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

if(APPLE)
    message(STATUS "Targeting MacOS dependencies")
    set(USD_PATH lib/openusd/macos)
    set(VHACD_PATH lib/vhacd/macos)
    # TMP flag to avoid compile issues with OpenUSD with older versions of TBB
    add_compile_options(-Wno-error=changes-meaning)
elseif(UNIX)
    message(STATUS "Targeting Unix dependencies")
    set(USD_PATH lib/openusd/linux)
    set(VHACD_PATH lib/vhacd/linux)
    # TMP flag to avoid compile issues with OpenUSD with older versions of TBB
    add_compile_options(-Wno-error=changes-meaning)
elseif(WIN32)
    message(STATUS "Targeting Windows dependencies")
    set(VHACD_PATH ${CMAKE_SOURCE_DIR}/lib/vhacd/windows)
else()
    message(FATAL_ERROR "Unsupported target architecture")
endif()


set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/logging.cpp
    ${PROJECT_SOURCE_DIR}/graphics.cpp
    ${PROJECT_SOURCE_DIR}/mesh.cpp
    ${PROJECT_SOURCE_DIR}/rendermesh.cpp
    ${PROJECT_SOURCE_DIR}/gridrendermesh.cpp
    ${PROJECT_SOURCE_DIR}/scenemodel.cpp
    ${PROJECT_SOURCE_DIR}/modelloader.cpp
    ${PROJECT_SOURCE_DIR}/collisiongen.cpp
    ${PROJECT_SOURCE_DIR}/appwindow.cpp
    ${PROJECT_SOURCE_DIR}/viewportwidget.cpp
    ${PROJECT_SOURCE_DIR}/viewportcamera.cpp
    ${PROJECT_SOURCE_DIR}/logwidget.cpp
    ${PROJECT_SOURCE_DIR}/propertywidgets.cpp
    ${PROJECT_SOURCE_DIR}/propertypanel.cpp
    ${PROJECT_SOURCE_DIR}/expanderwidget.cpp
)

# Define resources
qt_add_resources(PROCESSED_RESOURCES resources/resources.qrc)

set(VHACD_INCLUDES ${VHACD_PATH}/include)
set(VHACD_LIB ${VHACD_PATH}/lib)

# Create executable target and link modules
add_executable(CollisionCraft ${SOURCES} ${PROCESSED_RESOURCES})

# Add preprocessor flags depending on build flavour
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(CollisionCraft PRIVATE 
        QT_NO_KEYWORDS
        DEBUG
    )
else()
    target_compile_definitions(CollisionCraft PRIVATE 
        QT_NO_KEYWORDS
    )
endif()

if(APPLE OR UNIX)
    set(USD_INCLUDES ${USD_PATH}/include)
    set(USD_LIB ${USD_PATH}/lib)
    target_include_directories(CollisionCraft PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${Python3_INCLUDE_DIRS}
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
elseif(WIN32)
    target_include_directories(CollisionCraft PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${Python3_INCLUDE_DIRS}
        ${VHACD_INCLUDES}
    )

    target_link_directories(CollisionCraft PRIVATE
        ${VHACD_LIB}
    )

    find_package(PXR CONFIG REQUIRED)
    message(STATUS "pxr_DIR = ${pxr_DIR}")
    get_directory_property(_all_targets BUILDSYSTEM_TARGETS)
    message(STATUS "🗂️ All CMake targets: ${_all_targets}")

    target_link_libraries(CollisionCraft PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        
        # OpenUSD Modules
        usd
        usdGeom
        usdUtils
        tf
        gf
        sdf
        vt

        # CGAL Modules & its dependencies
        Boost::boost
        Eigen3::Eigen
        CGAL::CGAL

        # VHACD dependency
        ${VHACD_LIB}/vhacd.lib
        opengl32
    )
endif()