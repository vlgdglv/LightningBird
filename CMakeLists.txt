cmake_minimum_required (VERSION 3.12)
project (LightningBird)

find_package(Boost 1.66 COMPONENTS system thread serialization program_options)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/sptag/AnnService)

set(LIBRARYONLY ON CACHE BOOL "Only build libraries, no executables")

# add_subdirectory(sptag)

set(SOURCE_FILES src/main.cpp)
add_executable(LightningBird ${SOURCE_FILES})

target_link_libraries(LightningBird  ${Boost_LIBRARIES}) # SPTAGLibStatic
