

cmake_minimum_required(VERSION 3.4.1)

set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")

include_directories(
        RenderContext
        DeferredShading
        util
        glm
        inc
        inc/assimp
        inc/cpuinfo
        model
        opencv_3_0_0
)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/setAffinity/setAffinity.cpp
        ${CMAKE_SOURCE_DIR}/DeferredShading/DeferredShadingGBuffer.cpp
        ${CMAKE_SOURCE_DIR}/DeferredShading/Model3DSample.cpp
        ${CMAKE_SOURCE_DIR}/DeferredShading/TriangleSample.cpp
        ${CMAKE_SOURCE_DIR}/model/model.cpp
        ${CMAKE_SOURCE_DIR}/model/mesh.cpp
        ${CMAKE_SOURCE_DIR}/model/shader.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/RenderContext/*.cpp)

add_library( # Sets the name of the library.
        RangerEngine

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${src-files}
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

add_library(assimp SHARED IMPORTED )
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libassimp.so)

add_library(opencv_java3 SHARED IMPORTED )
set_target_properties(opencv_java3 PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libopencv_java3.so)

set(CPUINFO_LIBRARIES ${jnilibs}/${ANDROID_ABI}/libcpuinfo.a)


set(third-party-libs
        assimp
        opencv_java3)

target_link_libraries( # Specifies the target library.
        RangerEngine

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${third-party-libs}
        ${CPUINFO_LIBRARIES}
        #assimp
        EGL
        GLESv3
        android
        )