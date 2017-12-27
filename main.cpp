#include <iostream>
#include <utility>

using std::cin;
using std::cout;
using std::pair;

struct Node {
    Node * right = nullptr;
    int value;
};

void free(size_t size, Node * our)
{
    for (int i = 0; i < size; i++)
    {
        auto prev = our;
        our = our->right;
        delete prev;
    }
}

pair<Node*, Node*> split(size_t size, Node * our)
{
    Node* res = our;
    for (size_t i = 0; i < size / 2; i++)
        res = res->right;
    return {our, res};
}

Node* merge(Node* f, size_t s1, Node* s, size_t s2)
{
    Node* res = f;
    if (f->value > s->value)
    {
        res = s;
        s = s->right;
        s2--;
    }
    else
    {
        s1--;
        f = f->right;
    }
    auto mem = res;
    while (s1 > 0 && s2 > 0)
    {
        if (f->value < s->value)
        {
            res->right = f;
            res = res->right;
            f = f->right;
            s1--;
        }
        else
        {
            res->right = s;
            res = res->right;
            s = s->right;
            s2--;
        }
    }
    while (s1 > 0)
    {
        res->right = f;
        res = res->right;
        f = f->right;
        s1--;
    }
    while (s2 > 0)
    {
        res->right = s;
        res = res->right;
        s = s->right;
        s2--;
    }
    return mem;
}

Node* merge_sort(size_t size, Node * our)
{
    if (size <= 1)
        return our;
    auto p = split(size, our);
    p.first = merge_sort(size / 2, p.first);
    p.second = merge_sort(size - size / 2, p.second);
    return merge(p.first, size / 2, p.second, size - size / 2);
}

int main()
{
    size_t n = 0;
    cin >> n;
    Node * our = new Node;
    Node * next = our;
    cin >> our->value;
    for (size_t i = 0; i < n - 1; i++)
    {
        next->right = new Node;
        next = next->right;
        cin >> next->value;
    }
    our = merge_sort(n, our);
    auto rem = our;
    for (size_t i = 0; i < n; i++)
        cout << our->value << " ", our = our->right;
    free(n, rem);
}
