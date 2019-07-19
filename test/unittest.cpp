#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>

//#include "test_nvm_mgr.cpp"
#include "test_single_thread.cpp"
#include "test_multi_thread.cpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    // Runs all tests using Google Test.
    return RUN_ALL_TESTS();
}
