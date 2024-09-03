#include <iostream>
#include <iomanip>
#include <functional>
#include <queue>
#include <string>
#include <vector>
#include <initializer_list>

#include "bst.h"

BST::Node::Node(int value, Node *left, Node *right)
{
    this->value = value;
    this->left = left;
    this->right = right;
}

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {}

BST::Node::Node(const Node &node) = default;

BST::BST() : root(nullptr) {}

BST::BST(const std::initializer_list<int>& elem)
{
    this->root = new Node(*elem.begin(), nullptr, nullptr);
    for(auto i = elem.begin(); i!=elem.end(); i++){
        if(i == elem.begin()) continue;
        add_node(*i);
    }
}

BST::BST(const BST& bst)
{
    // std::cout<< "node......" << std::endl;
    this->root = new Node(bst.root->value, nullptr, nullptr);
    std::vector<int> dta;
    const_cast<BST*>(&bst)->bfs([&dta](BST::Node*& node){
        dta.push_back(node->value);
    });
    for(int i=1; i<dta.size(); i++){
        add_node(dta[i]);
    }

}

BST::BST(BST&& other) noexcept
{
    root = other.root;
    other.root = nullptr;
}

BST &BST::operator=(const BST &other)
{
    if (this == &other)
    {
        return *this;
    }
    this->root = new Node(other.root->value, nullptr, nullptr);
    std::vector<int> dta;
    const_cast<BST*>(&other)->bfs([&dta](BST::Node *&node)
        { dta.push_back(node->value); });
    for(int i=1; i<dta.size(); i++){
        add_node(dta[i]);
    }
    return *this;
}

BST &BST::operator=(BST &&other)
{
    root = other.root;
    other.root = nullptr;
    return *this;
}

BST::~BST()
{
    if(!root) return;
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node *&node)
        { nodes.push_back(node); });
    for (auto &node : nodes)
        delete node;
}

BST::Node *&BST::get_root()
{
    return root;
}

void BST::bfs(std::function<void(BST::Node *&node)> func)
{
    std::queue<Node *> que;
    que.push(root);
    while (!que.empty())
    {
        auto node = que.front();
        que.pop();
        if(node != nullptr){
            func(node);
            if (node->left)
                que.push(node->left);
            if (node->right)
                que.push(node->right);
        }
    }
}

size_t BST::length()
{
    if (!root)
        return 0;

    size_t num = 0;
    bfs([&num](Node *&node)
        {   num++; 
            // std::cout<< node->value << " num:" << num << std::endl;
        });
    return num;
}

bool BST::add_node(int value)
{
    Node **node = &root;
    while (1)
    {
        if (*node == nullptr)
        {
            *node = new Node(value, nullptr, nullptr);
            return true;
        }
        const int _value = (*node)->value;
        if (value > _value)
        {
            node = &((*node)->right);
        }
        else if (value < _value)
        {
            node = &((*node)->left);
        }
        else
        {
            return false;
        }
    }
}

BST::Node **BST::find_node(int value)
{
    auto cur = &root;
    while (1)
    {
        if (!*cur)
            return nullptr;
        auto val = (*cur)->value;
        if (value > val)
        {
            cur = &(*cur)->right;
        }
        else if (value < val)
        {
            cur = &(*cur)->left;
        }
        else
        {
            return cur;
        }
    }
}

BST::Node **BST::find_parrent(int value)
{
    if (!find_node(value))
    {
        return nullptr;
    }

    if (value == root->value)
    {
        return nullptr;
    }

    auto cur = &root;
    while (1)
    {
        if (!*cur)
            return nullptr;
        if (value > (*cur)->value && (*cur)->right->value == value || value < (*cur)->value && (*cur)->left->value == value)
        {
            return cur;
        }

        if (value > (*cur)->value)
            cur = &(*cur)->right;
        if (value < (*cur)->value)
            cur = &(*cur)->left;
    }
}

BST::Node **BST::find_successor(int value)
{
    auto node = find_node(value);
    if (!node)
    {
        return nullptr;
    }

    if (!(*node)->left)
    {
        return nullptr;
    }
    else
    {
        auto cur = &(*node)->left;
        while ((*cur)->right)
        {
            cur = &(*cur)->right;
        }
        return cur;
    }
}

bool BST::delete_node(int value)
{
    auto cur = find_node(value);
    if (cur)
    {
        int cnt = 0;
        cnt += ((*cur)->left != nullptr);
        cnt += ((*cur)->right != nullptr);
        if (cnt == 0)
        {
            *cur = nullptr;
        }
        else if (cnt == 1)
        {
            *cur = ((*cur)->left != nullptr) ? (*cur)->left : (*cur)->right;
        }
        else if (cnt == 2)
        {
            auto successor = find_successor(value);
            (*cur)->value = (*successor)->value;
            *successor = nullptr;
        }
        return true;
    }
    return false;
}

BST &BST::operator++()
{
    bfs([](BST::Node *&node)
        { node->value++; });
    return *this;
}

BST BST::operator++(int)
{
    BST old(*this);
    bfs([](BST::Node *&node)
        { node->value++; });
    return old;
}

std::ostream &operator<<(std::ostream &os, const BST::Node &node)
{
    os << std::left << std::setw(17) << &node;
    os << "=> value:" << std::left << std::setw(10) << node.value;
    os << "left:" << std::left << std::setw(17) << node.left;
    os << "right:" << std::left << std::setw(17) << node.right;
    return os;
}

std::ostream &operator<<(std::ostream &os, const BST &bst)
{
    std::cout << std::string(82, '*');
    std::queue<BST::Node *> que;
    que.push(const_cast<BST &>(bst).get_root());
    while (!que.empty())
    {
        auto node = que.front();
        que.pop();
        std::cout << node;
        if (node->left)
            que.push(node->left);
        if (node->right)
            que.push(node->right);
    }
    return os;
}

bool operator>(const BST::Node &node, int a)
{
    return node.value > a;
}

bool operator>=(const BST::Node &node, int a)
{
    return node.value >= a;
}

bool operator<(const BST::Node &node, int a)
{
    return node.value < a;
}

bool operator<=(const BST::Node &node, int a)
{
    return node.value <= a;
}

bool operator==(const BST::Node &node, int a)
{
    return node.value == a;
}

bool operator>(int a, const BST::Node &node)
{
    return a > node.value;
}

bool operator>=(int a, const BST::Node &node)
{
    return a >= node.value;
}

bool operator<(int a, const BST::Node &node)
{
    return a < node.value;
}

bool operator<=(int a, const BST::Node &node)
{
    return a <= node.value;
}

bool operator==(int a, const BST::Node &node)
{
    return a == node.value;
}