#include <QCoreApplication>
#include "rbtree.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{    
    RBTree<int> tree;

    for (int i = 16; i > 0; i --){
        tree.Add(i);
        cout << endl;
    }

    RBTree<int>::iterator iterator(tree, RBTree<int>::BFS);

    while(*iterator != NULL){
        cout << *iterator;
        ++iterator;
    }

    RBTree<int>::iterator iterator1(tree, RBTree<int>::DFS);

    while(*iterator1 != NULL){
        cout << *iterator1;
        ++iterator1;
    }

    int n;
    cin >> n;

    /*
    tree.Delete(0);
    tree.BFS();
    cout << endl;

    tree.Delete(5);
    tree.BFS();
    cout << endl;

    tree.Delete(10);
    tree.BFS();
    cout << endl;

    tree.Delete(6);
    tree.BFS();
    cout << endl;

    tree.Add(52);
    tree.Add(54);
    tree.Add(64);
    tree.Add(10);
    tree.Add(5);
    tree.BFS();
    cout << endl;

    */


}
