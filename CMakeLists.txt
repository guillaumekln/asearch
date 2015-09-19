# Setup
cmake_minimum_required(VERSION 2.8.11)

project(asearch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic -Wextra -std=c++11")

# Binaries
include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_executable(compiler
  ${PROJECT_SOURCE_DIR}/src/compiler/ptrie.cc
  ${PROJECT_SOURCE_DIR}/src/compiler/main.cc
  )

add_executable(approx
  ${PROJECT_SOURCE_DIR}/src/approx/ptrie.cc
  ${PROJECT_SOURCE_DIR}/src/approx/dl-row.cc
  ${PROJECT_SOURCE_DIR}/src/approx/approx.cc
  ${PROJECT_SOURCE_DIR}/src/approx/main.cc
  )

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
    @ONLY
    )
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
    COMMENT "Generating documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
