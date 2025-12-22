#include "BinomialHeap.h"

BinomialHeap::BinomialHeap() : min_node(nullptr) {}

BinomialHeap::~BinomialHeap()
{
    clear();
}

void BinomialHeap::make_heap()
{
    clear();
}

void BinomialHeap::clear()
{
    for (Node *root : roots)
    {
        clear_recursive(root);
    }
    roots.clear();
    min_node = nullptr;
}

void BinomialHeap::clear_recursive(Node *node)
{
    if (!node)
        return;
    clear_recursive(node->child);
    clear_recursive(node->sibling);
    delete node;
}

Node *BinomialHeap::find_min()
{
    return min_node;
}

// Link tree y to tree z (y becomes child of z)
// Precondition: y->degree == z->degree
void BinomialHeap::link(Node *y, Node *z)
{
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

// Merge two lists of roots, sorted by degree
Node *BinomialHeap::merge_roots(BinomialHeap *rhs)
{
    std::list<Node *> new_roots;
    auto it1 = roots.begin();
    auto it2 = rhs->roots.begin();

    while (it1 != roots.end() && it2 != rhs->roots.end())
    {
        if ((*it1)->degree <= (*it2)->degree)
        {
            new_roots.push_back(*it1);
            it1++;
        }
        else
        {
            new_roots.push_back(*it2);
            it2++;
        }
    }
    while (it1 != roots.end())
    {
        new_roots.push_back(*it1);
        it1++;
    }
    while (it2 != rhs->roots.end())
    {
        new_roots.push_back(*it2);
        it2++;
    }

    roots = new_roots;
    return nullptr; // Return value not really used in this implementation style
}

void BinomialHeap::union_heap(BinomialHeap *rhs)
{
    merge_roots(rhs);

    if (roots.empty())
    {
        rhs->roots.clear();
        rhs->min_node = nullptr;
        min_node = nullptr;
        return;
    }

    std::list<Node *> new_roots;
    auto it = roots.begin();
    Node *prev = nullptr;
    Node *x = *it;
    Node *next = (std::next(it) != roots.end()) ? *std::next(it) : nullptr;

    while (x != nullptr)
    {
        next = (std::next(it) != roots.end()) ? *std::next(it) : nullptr;

        if ((next != nullptr && x->degree != next->degree) ||
            (next != nullptr && std::next(it, 2) != roots.end() && (*std::next(it, 2))->degree == x->degree))
        {
            // Case 1 & 2: Degrees distinct or 3 trees with same degree
            prev = x;
            it++;
            x = next;
        }
        else if (next != nullptr && x->key <= next->key)
        {
            // Case 3: x is smaller, link next to x
            roots.erase(std::next(it)); // Remove next from list
            link(next, x);
            // x remains same, check again with new next
        }
        else if (next != nullptr)
        {
            // Case 4: next is smaller, link x to next
            // We need to remove x from list, but since we are iterating,
            // we can just update the pointer in the list if we were using raw pointers array.
            // With std::list, we remove x.
            it = roots.erase(it); // Remove x, it now points to next
            link(x, next);
            x = next;
            // prev remains same
        }
        else
        {
            it++;
            x = nullptr;
        }
    }

    rhs->roots.clear();
    rhs->min_node = nullptr;
    update_min();
}

Node *BinomialHeap::insert(int key)
{
    BinomialHeap *temp = new BinomialHeap();
    Node *x = new Node(key);
    temp->roots.push_back(x);
    temp->min_node = x;
    union_heap(temp);
    // temp is now empty but we need to delete the object, not the nodes (which are now in this heap)
    // Since union_heap clears rhs->roots, temp destructor won't delete our nodes.
    delete temp;
    return x;
}

void BinomialHeap::extract_min()
{
    if (!min_node)
        return;

    // Find min node in roots list and remove it
    Node *x = min_node;
    roots.remove(x);

    // Create a new heap from x's children
    BinomialHeap *temp = new BinomialHeap();
    Node *child = x->child;
    std::list<Node *> child_list;
    while (child)
    {
        child_list.push_front(child); // Reverse order for correct degree sorting
        Node *next = child->sibling;
        child->sibling = nullptr;
        child->parent = nullptr;
        child = next;
    }
    temp->roots = child_list;

    // Union
    union_heap(temp);
    delete temp;

    // Delete the extracted node
    delete x;

    update_min();
}

void BinomialHeap::decrease_key(Node *x, int k)
{
    if (k > x->key)
    {
        // Error: new key is greater
        return;
    }
    x->key = k;
    Node *y = x;
    Node *z = y->parent;
    while (z != nullptr && y->key < z->key)
    {
        std::swap(y->key, z->key);
        // 仅仅交换了键值
        y = z;
        z = y->parent;
    }
    update_min();
}

void BinomialHeap::delete_node(Node *x)
{
    decrease_key(x, std::numeric_limits<int>::min());
    extract_min();
}

void BinomialHeap::update_min()
{
    if (roots.empty())
    {
        min_node = nullptr;
        return;
    }
    min_node = roots.front();
    for (Node *node : roots)
    {
        if (node->key < min_node->key)
        {
            min_node = node;
        }
    }
}

std::list<Node *> &BinomialHeap::getRoots()
{
    return roots;
}

Node *BinomialHeap::find_node(int key)
{
    for (Node *root : roots)
    {
        Node *res = find_node_recursive(root, key);
        if (res)
            return res;
    }
    return nullptr;
}

Node *BinomialHeap::find_node_recursive(Node *node, int key)
{
    if (!node)
        return nullptr;
    if (node->key == key)
        return node;

    Node *res = find_node_recursive(node->child, key);
    if (res)
        return res;

    return find_node_recursive(node->sibling, key);
}
