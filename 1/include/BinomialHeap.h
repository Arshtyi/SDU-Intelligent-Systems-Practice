#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include <vector>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>

struct Node
{
    int key;
    int degree;
    Node *parent;
    Node *child;
    Node *sibling;

    Node(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap
{
public:
    BinomialHeap();
    ~BinomialHeap();

    // Basic Operations
    void make_heap();
    Node *find_min();
    void union_heap(BinomialHeap *rhs);
    Node *insert(int key);
    void extract_min();
    void decrease_key(Node *x, int k);
    void delete_node(Node *x);

    // Helper for visualization
    std::list<Node *> &getRoots();

    // Helper for GUI interaction
    Node *find_node(int key);

    // Helper to clear
    void clear();

private:
    std::list<Node *> roots; // List of roots of binomial trees, sorted by degree
    Node *min_node;

    // Internal helpers
    void link(Node *y, Node *z);
    Node *merge_roots(BinomialHeap *rhs);
    void update_min();
    void clear_recursive(Node *node);
    Node *find_node_recursive(Node *node, int key);
};

#endif // BINOMIALHEAP_H
