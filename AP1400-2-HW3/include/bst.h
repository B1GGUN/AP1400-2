#ifndef BST_H
#define BST_H

#include <functional>

class BST
{

public:
    class Node
    {

    public:
        Node(int value, Node *left, Node *right);
        Node();
        Node(const Node &node);

        int value;
        Node *left;
        Node *right;
    };

    BST();
    BST(const std::initializer_list<int>&);
    BST(const BST&);
    BST(BST&&) noexcept;
    ~BST();
    Node *&get_root();
    void bfs(std::function<void(Node *&node)> func);
    size_t length();
    bool add_node(int value);
    Node **find_node(int value);
    Node **find_parrent(int value);
    Node **find_successor(int value);
    bool delete_node(int value);

    BST& operator++();
    BST operator++(int);
    BST& operator=(const BST& other);
    BST& operator=(BST&& other);

private:
    Node *root;
};

std::ostream &operator<<(std::ostream &os, const BST::Node &node);

std::ostream &operator<<(std::ostream &os, const BST &bst);

bool operator>(const BST::Node &node, int a);
bool operator>=(const BST::Node &node, int a);
bool operator<(const BST::Node &node, int a);
bool operator<=(const BST::Node &node, int a);
bool operator==(const BST::Node &node, int a);

bool operator>(int a, const BST::Node &node);
bool operator>=(int a, const BST::Node &node);
bool operator<(int a, const BST::Node &node);
bool operator<=(int a, const BST::Node &node);
bool operator==(int a, const BST::Node &node);

#endif // BST_H