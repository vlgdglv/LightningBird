cmake_minimum_required (VERSION 3.12)
project (LightningBird)


include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/sptag/AnnService)

set(LIBRARYONLY ON CACHE BOOL "Only build libraries, no executables")

add_subdirectory(sptag)

set(SOURCE_FILES src/main.cpp)
add_executable(LightningBird ${SOURCE_FILES})

# 链接库
target_link_libraries(LightningBird SPTAGLibStatic)
