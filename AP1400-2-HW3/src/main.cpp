#include <iostream>
#include <gtest/gtest.h>
#include "bst.h"

int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        // debug section
        BST bst1{5, 1, 10, 2, 8, 50, 4, 60};
        BST bst2{bst1++};

        // std::vector<int> values1;
        // bst1.bfs([&values1](BST::Node *&node)
        //          { values1.push_back(node->value); });

        // std::vector<int> values2;
        // bst2.bfs([&values2](BST::Node *&node)
        //          { values2.push_back(node->value); });
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}