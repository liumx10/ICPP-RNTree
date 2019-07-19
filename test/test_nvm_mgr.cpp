#pragma once

#include <gtest/gtest.h>
#include "nvm_mgr.h"

#include <stdio.h>
#include <iostream>

TEST(NVMMgrTest, Open){
    void* threads_info;
    int threads;
    bool safe;

    void * first_alloc, *second_alloc;

    {
        NVMMgr mgr;
        mgr.init(threads_info, threads, safe);
        std::cout << threads << " " << threads_info << std::endl;
        ASSERT_EQ(safe, true) << "Initial not safe\n";

        const int nthreads = 5;
        for(int i=0; i<nthreads; i++){
            void* addr = mgr.alloc_thread_info();
            *(int*)(addr) = i;
        }
        first_alloc = mgr.alloc_block(1);

        mgr.init(threads_info, threads, safe);
        ASSERT_EQ(threads, nthreads) << "Invalid threads number\n";
        ASSERT_EQ(safe, false) << "Should call recovery\n";
    }
    {
        // 重启
        NVMMgr mgr;
        mgr.init(threads_info, threads, safe);
        ASSERT_EQ(safe, true) << "Initial not safe\n";

        second_alloc = mgr.alloc_block(1);
        ASSERT_EQ((size_t)second_alloc - (size_t)first_alloc, 4096) << "allocate wrong position\n";
    }
}
