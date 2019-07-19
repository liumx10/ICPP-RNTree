#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "index_factory.h"
#include "util.h"

using namespace nvindex;

void test_multi_thread(Index<long long, long long, 64>* btree)
{
    const int nthreads = 4;
    const int MAX_FAILED_TIMES = 10;
    const int test_size = 100000;

    for (int i=0; i<test_size; i++){
        btree->insert(i, i+1);
    }
    std::thread* tid[4];
    volatile bool done = false;

    for (int i=0; i<nthreads; i++){
        tid[i] = new std::thread([&](){
            register_threadinfo();
            int continuous_failed = 0;
            double latency[3];

            while(done!=true){
                ZipfWrapper* workload = new ZipfWrapper(0.5,  test_size);
                int d = workload->Next();

                bool res = btree->insert(d, d+1);
                if (res == false){
                    res = btree->update(d, d+2, latency);
                }

                int r = btree->get(d, latency);
                if (r == -1){
                    continuous_failed++;
                }else{
                    continuous_failed = 0;
                    assert(r == d+1 || r == d+2);
                }

                res = btree->remove(d);
            }
        });
    }
    sleep(5);
    done = true;
    for(int i=0; i<nthreads; i++){
        tid[i]->join();
    }
    std::cout << "tested pass..\n";
}


template<typename K, typename V, int size>
void  test_multi_tree(IndexType type){
    Index<K, V, size>* tree = getIndex<K, V, size>(type);
    test_multi_thread(tree);
    delete tree;
}

TEST(MultiThreadTest, RN_TREE){
    test_multi_tree<long long, long long, 64>(IndexType::RN_TREE);
}

TEST(MultiThreadTest, RN_TREE_R){
    test_multi_tree<long long, long long, 64>(IndexType::RN_TREE_R);
}

TEST(MultiThreadTest, FP_TREE){
    test_multi_tree<long long, long long, 64>(IndexType::FP_TREE);
}

