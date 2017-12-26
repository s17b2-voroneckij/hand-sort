#include <iostream>

using std::cin;
using std::cout;

struct Node {
    Node * right = nullptr;
    int value;
};

void free(int size, Node * our)
{
    for (int i = 0; i < size; i++)
    {
        auto prev = our;
        our = our->right;
        delete prev;
    }
}

Node* merge_sort(int size, Node * our)
{
    if (size <= 1)
        return our;
    Node * second = our;
    for (int i = 0; i < size / 2; i++)
    {
        second = second->right;
    }
    auto s1 = size / 2;
    auto s2 = size - s1;
    our = merge_sort(s1, our);
    second = merge_sort(s2, second);
    auto rem1 = our, rem2 = second;
    Node * write = new Node;
    auto res = write;
    while (s1 > 0 || s2 > 0)
    {
        if (s2 == 0 || (s1 != 0 && our->value < second->value))
        {
            write->value = our->value;
            our = our->right;
            s1--;
            write->right = new Node;
            write = write->right;
        }
        else
        {
            write->value = second->value;
            second = second->right;
            s2--;
            write->right = new Node;
            write = write->right;
        }
    }
    free(size / 2, rem1);
    free(size - size / 2, rem2);
    free(1, write);
    return res;
}

int main()
{
    int n;
    cin >> n;
    Node * our = new Node;
    Node * next = our;
    cin >> our->value;
    for (int i = 0; i < n - 1; i++)
    {
        next->right = new Node;
        next = next->right;
        cin >> next->value;
    }
    our = merge_sort(n, our);
    auto rem = our;
    for (int i = 0; i < n; i++)
        cout << our->value << " ", our = our->right;
    free(n, rem);
}
