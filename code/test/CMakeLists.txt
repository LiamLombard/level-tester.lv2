enable_testing()

add_executable(${TEST_EXECUTABLE_NAME} ${CMAKE_CURRENT_LIST_DIR}/test.cpp ${program_SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE
  project_warnings --coverage
)

add_test(Tester ${TEST_EXECUTABLE_NAME})