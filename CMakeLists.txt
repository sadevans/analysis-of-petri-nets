cmake_minimum_required(VERSION 3.5)
project(des-model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework)

include_directories(
  include
)

set(LIBRARY_SOURCES
  src/components.cpp
  src/automation.cpp
)

## Library.
add_library(${PROJECT_NAME} SHARED
  ${LIBRARY_SOURCES}
)

## Examples.
add_executable(${PROJECT_NAME}-example
  src/graph_translators.cpp
  main.cpp
)
target_link_libraries(${PROJECT_NAME}-example ${PROJECT_NAME})

if (Boost_FOUND)
  message("Boost Test found, build project tests...")

  set(TEST_SOURCES
    test/components_tests.cpp
    test/state_tests.cpp
    test/event_tests.cpp
    test/automation_tests.cpp
  )

  ## Tests.
  add_executable(${PROJECT_NAME}-tests
    ${LIBRARY_SOURCES}
    ${TEST_SOURCES}
  )
  target_link_libraries(${PROJECT_NAME}-tests
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

  enable_testing()
  add_test(NAME TestsOfDES COMMAND ${PROJECT_NAME}-tests)

endif()
