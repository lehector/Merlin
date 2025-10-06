project(Tests)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run
        BinaryOpNodesTests.cpp
        TernaryOpNodeTests.cpp
        UnaryOpNodesTests.cpp
        BlazeSemanticsTests.cpp
        HelperTests.cpp
)
target_link_libraries(Google_Tests_run gtest gtest_main merlin_code)