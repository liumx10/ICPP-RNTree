#pragma once

#include <gtest/gtest.h>
#include <iostream>

#include "index_factory.h"

using namespace nvindex;

void test_single_thread(Index<long long, long long, 64>* btree)
{
    register_threadinfo();
    RandomGenerator rdm;
    rdm.reset();

    int test_size = 100000;

    for (int i = 0; i < test_size; i++)
    {
        int d = rdm.Next() % test_size;
        btree->insert(d, d + 1);
    }

    rdm.reset();
    double time_breaks[3];
    for (int i = 0; i < test_size; i++)
    {
        int d = rdm.Next() % (test_size);
        int x = btree->get(d, time_breaks);
        assert(x == d+1);
    }
    rdm.reset();

    static int scan_values = 0;
    static int scan_length = 100;
    static int last_key = -1;

    auto scan_func = [](long long key, long long value) -> bool {
        assert(key > last_key);
        last_key = key;

        scan_values++;
        if (scan_values > scan_length)
        {
            return true;
        }
        return false;
    };

    for (int i = 0; i < 100; i++)
    {
        int d = rdm.Next() % (test_size);
        int x;

        x = btree->get(d, time_breaks);
        assert(x == d + 1);

        btree->update(d, d + 2, time_breaks);
        x = btree->get(d, time_breaks);
        assert(x == d+2);

        btree->remove(d);
        x = btree->get(d, time_breaks);
        assert(x == -1);

        btree->insert(d, d + 1);
        x = btree->get(d, time_breaks);
        assert(x == d + 1);

        btree->scan(d, scan_func);
        scan_values = 0;
        last_key = -1;
    }

    printf("===== test passed.. ====\n");
    unregister_threadinfo();
}

template<typename K, typename V, int size>
void  test_single_tree(IndexType type){
    Index<K, V, size>* tree = getIndex<K, V, size>(type);
    test_single_thread(tree);
    delete tree;
}

TEST(SingleThreadTest, NV_TREE){
    test_single_tree<long long, long long, 64>(IndexType::NV_TREE);
}

TEST(SingleThreadTest, FP_TREE){
    test_single_tree<long long, long long, 64>(IndexType::FP_TREE);
}
TEST(SingleThreadTest, RN_TREE){
    test_single_tree<long long, long long, 64>(IndexType::RN_TREE);
}
TEST(SingleThreadTest, RN_TREE_R){
    test_single_tree<long long, long long, 64>(IndexType::RN_TREE_R);
}
TEST(SingleThreadTest, WB_TREE){
    test_single_tree<long long, long long, 64>(IndexType::WB_TREE);
}
TEST(SingleThreadTest, WB_TREE_2){
    test_single_tree<long long, long long, 64>(IndexType::WB_TREE_2);
}
