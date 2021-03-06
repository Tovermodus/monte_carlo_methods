cmake_minimum_required(VERSION 3.10)
project(MikadoMonteCarlo)

set(CMAKE_CXX_STANDARD 14)

add_executable(MikadoMonteCarlo main.cpp Rod.cpp Rod.h Cell.cpp Cell.h Medium.cpp Medium.h MediumParameters.h MonteCarloLoop.cpp MonteCarloLoop.h math_functions.h)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)


add_executable(RodTest Rod.cpp Rod.h Cell.cpp Cell.h Test/rodtest.cpp)
target_link_libraries(RodTest gtest gtest_main)
add_test(NAME rod_test COMMAND RodTest)


add_executable(OverlapTest Rod.cpp Rod.h Cell.cpp Cell.h Medium.cpp Medium.h MediumParameters.h MonteCarloLoop.cpp MonteCarloLoop.h math_functions.h Test/overlaptest.cpp)
target_link_libraries(OverlapTest gtest gtest_main)
add_test(NAME overlap_test COMMAND OverlapTEST)

add_executable(LongTimeTest Rod.cpp Rod.h Cell.cpp Cell.h Medium.cpp Medium.h MediumParameters.h MonteCarloLoop.cpp MonteCarloLoop.h math_functions.h Test/longtimetest.cpp Test/longtimetest.cpp)
target_link_libraries(LongTimeTest gtest gtest_main)
add_test(NAME longtime_test COMMAND LongTimeTest)