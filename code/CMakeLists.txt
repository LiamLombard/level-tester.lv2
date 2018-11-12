cmake_minimum_required(VERSION 3.8)
# Link this 'library' to use the following warnings

project(program)

add_library(project_warnings INTERFACE)

set(EXECUTABLE_NAME "level-tester")
set(TEST_EXECUTABLE_NAME "levelTesterTester")

if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

  if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
  endif()
endif()

if(MSVC)
  target_compile_options(project_warnings INTERFACE /W4)
else()
  target_compile_options(project_warnings
                         INTERFACE
                         -Wall
                         -Wextra # reasonable and standard
                         -Wshadow # warn the user if a variable declaration shadows one from a
                                  # parent context
                         -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                                            # non-virtual destructor. This helps catch hard to
                                            # track down memory errors
                         -Wold-style-cast # warn for c-style casts
                         -Wcast-align # warn for potential performance problem casts
                         -Wunused # warn on anything being unused
                         -Woverloaded-virtual # warn if you overload (not override) a virtual
                                              # function
                         -Wpedantic # warn if non-standard C++ is used
                         -Wconversion # warn on type conversions that may lose data
                         -Wsign-conversion # warn on sign conversions
                         -Wmisleading-indentation # warn if identation implies blocks where blocks
                                                  # do not exist
                         -Wlogical-op # warn about logical operations being used where bitwise were
                                      # probably wanted
                         -Wnull-dereference # warn if a null dereference is detected
                         -Wuseless-cast # warn if you perform a cast to the same type
                         -Wdouble-promotion # warn if float is implicit promoted to double
                         -Wformat=2 # warn on security issues around functions that format output
                                    # (ie printf)
                         )
endif()

include(src/CMakeLists.txt)
include(test/CMakeLists.txt)


add_library(${EXECUTABLE_NAME} SHARED ${program_SOURCES})

target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  project_warnings --coverage
)

add_custom_command(TARGET ${EXECUTABLE_NAME}
  DEPENDS ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E rename lib${EXECUTABLE_NAME}.so ${EXECUTABLE_NAME}.so
  COMMAND ${CMAKE_COMMAND} -E copy ${EXECUTABLE_NAME}.so ../../
)
