cmake_minimum_required(VERSION 3.21)
project(rayproj VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RAYPROJ_BUILD_SHARED "Build shared instead of static" OFF)
option(RAYPROJ_USE_CCACHE "Use ccache if available" ON)
option(RAYPROJ_ENABLE_LTO "Enable link-time optimization" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(Warnings)
include(Dependencies)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/ink.cpp
)

if (WIN32)
  # Static link GCC runtime libraries for distribution (MinGW)
  if (MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++ -static)
    # Hide console window in Release builds
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-mwindows>)
  else()
    # MSVC: Hide console window in Release builds
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>)
  endif()
endif()

target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Link against raylib + Dear ImGui + rlImGui shim
target_link_libraries(${PROJECT_NAME} PRIVATE raylib imgui rlimgui)

rayproj_enable_warnings(${PROJECT_NAME})

if (RAYPROJ_ENABLE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported)
  if (ipo_supported)
    set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
endif()

if (APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation" "-framework CoreVideo" "-framework OpenGL"
  )
endif()

if (UNIX AND NOT APPLE)
  # Nothing required here if raylib is built with X11 (it is by default)
endif()

# Copy assets to binary dir on build (so relative paths work when running from bin/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/src/shaders" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
