#ifndef RBTREE_H
#define RBTREE_H

#include <queue>
#include <stack>
#include <iostream>
#include <math.h>

template<class T>
class RBTree{
    enum FIND_MODE{
        MIN, MAX
    };

    template <class T1 = T>
    struct Node{
        Node<T1>* parent;
        Node<T1>* left;
        Node<T1>* right;
        T value;
        bool red;

        Node<T1>(Node<T1>* parent, Node<T1>* left, Node<T1>* right, T value, bool red){
            this->parent = parent;
            this->left = left;
            this->right = right;
            this->value = value;
            this->red = red;
        }

        ~Node(){}
    };

    Node<T>* root;
    //iterator
protected:
    int Add(Node<T>* node, T value);
    int RecolorUp(Node<T>* node);
    int RecolorDown(Node<T>* node);
    int RotateLeft(Node<T>* node);
    int RotateRight(Node<T>* node);
    int InsertHelper(Node<T>* node);
    int SetRoot(Node<T>* node);
    Node<T>* FindNode(T value);
    Node<T>* FindNode(Node<T>* sub_tree, FIND_MODE mode);
public:
    enum SEARCH_TYPE{
        BFS, DFS
    };

    class iterator {
           Node<T>* pointer;
           SEARCH_TYPE sType;
           std::queue<Node<T>*> queue;
           std::stack<Node<T>*> stack;
           std::stack<Node<T>*> visited;
       public:
           iterator(){
               iterator(root, BFS);
           }
           iterator(RBTree tree, SEARCH_TYPE sType = BFS){
               this->pointer = tree.root;
               this->sType = sType;
               switch(sType){
               case(BFS):
                   queue.push(pointer);
                   break;
               case(DFS):
                   DFSHelper(pointer);
               }
           }

           ~iterator(){}

           iterator& operator++(){
               switch(sType){
               case(BFS):
                   if(queue.empty()){
                       pointer = NULL;
                       return *this;
                   }
                   pointer = queue.front();
                   queue.pop();
                   if (pointer->left != NULL)
                       queue.push(pointer->left);
                   if (pointer->right != NULL)
                       queue.push(pointer->right);
                   return *this;
               case(DFS):
                   if(queue.empty()){
                       pointer = NULL;
                       return *this;
                   }
                   pointer = stack.top();
                   stack.pop();
                   return *this;
               }
           }
           T operator*() const{
               return pointer->value;
           }
           T operator->() const{
               return pointer->value;
           }
        private:
           int DFSHelper(Node<T>* pointer){
               if(pointer->left != NULL){
                   DFSHelper(pointer->left);
               }
               stack.push(pointer);
               if(pointer->right != NULL){
                   DFSHelper(pointer->right);
               }
               return 0;
           }
       };



    RBTree();
    RBTree(const RBTree& tree);
    RBTree(T root);

    ~RBTree();

    int Add(T value);
    int Delete(T value);
    int Find(T value);

    //int BFS();
};

template<class T>
RBTree<T>::RBTree(){
    root = NULL;
}

template<class T>
RBTree<T>::RBTree(const RBTree& tree){
    this->root = tree.root;
}

template<class T>
RBTree<T>::~RBTree(){
    if (root != NULL)
        delete root;
}

template<class T>
int RBTree<T>::SetRoot(Node<T>* node){
    if (node != NULL){
        root = node;
        node->red = false;
        return 0;
    }
    return -1;
}

/*
template<class T>
int RBTree<T>::BFS(){
    std::queue<Node<T>*> q;
    Node<T>* temp;

    q.push(root);
    while (!q.empty()){
        temp = q.front();
        q.pop();
        std::cout << temp->value << " ";

        if(temp->left != NULL){
            q.push(temp->left);
        }
        if(temp->right != NULL){
            q.push(temp->right);
        }
    }
    return 0;
}
*/

template<class T>
int RBTree<T>::RecolorDown(Node<T>* node){
    if (node->parent == NULL){
        node->red = false;
    } else {
        node->red = !node->parent->red;
    }
    if (node->left != NULL){
        RecolorDown(node->left);
    }
    if (node->right != NULL){
        RecolorDown(node->right);
    }
    return 0;
}

template<class T>
int RBTree<T>::RecolorUp(Node<T>* node){ // delete double red
    if (node->red && node->parent->red){
        node->parent->parent->right->red = false;
        node->parent->parent->left->red = false;
        if (node->parent->parent != NULL &&
                node->parent->parent->parent != NULL){
            node->parent->parent->red = true;
            return InsertHelper(node->parent->parent);
        }
    }
    return 0;
}

template<class T>
int RBTree<T>::RotateRight(Node<T>* node){
    if (node->left == NULL){
        return -1;
    }

    if (node->parent != NULL){
        if (node->parent->left == node){
            node->parent->left  = node->left;
        }else{
            node->parent->right = node->left;
        }
    }       
    node->left->parent = node->parent;
    node->parent = node->left;
    if (node->left->parent == NULL){
        SetRoot(node->left);
    }

    if(node->left->right != NULL){
        Node<T>* left = node->left;
        node->left->right->parent = node;
        node->left = node->left->right;
        left->right = node;
        return 0;
    }
    node->left->right = node;
    node->left = NULL;
    return 0;
}

template<class T>
int RBTree<T>::RotateLeft(Node<T>* node){
    if (node->right == NULL){
        return -1;
    }

    if (node->parent != NULL){
        if (node->parent->right == node){
            node->parent->right  = node->right;
        }else{
            node->parent->left = node->right;
        }
    }
    node->right->parent = node->parent;
    node->parent = node->right;
    if (node->right->parent == NULL){
        SetRoot(node->right);
    }

    if(node->right->left != NULL){
        Node<T>* right = node->right;
        node->right->left->parent = node;
        node->right = node->right->left;
        right->left = node;
        return 0;
    }
    node->right->left = node;
    node->right = NULL;
    return 0;
}

template<class T>
int RBTree<T>::Add(T value){
    if(root == NULL){
        root = new Node<T>(NULL, NULL, NULL, value, false);
        return 0;
    }
    if(Find(value) == 0){
        return 0;
    }
    return Add(root, value);
}

template<class T>
int RBTree<T>::Add(Node<T>* node, T value){
    if (value > node->value){
        if (node->right == NULL){
            node->right = new Node<T>(node, NULL, NULL, value, true);
            InsertHelper(node->right);
            return 0;
        }
        return Add(node->right, value);
    }
    if (value < node->value){
        if (node->left == NULL){
            node->left = new Node<T>(node, NULL, NULL, value, true);
            InsertHelper(node->left);
            return 0;
        }
        return Add(node->left, value);
    }
    return -1;
}

template<class T>
int RBTree<T>::InsertHelper(Node<T>* node){    
    if (!node->parent->red)
        return 0;
    //parent is red
    if (node->parent->parent->left == node->parent){
        if (node->parent->parent->right == NULL ||
                !node->parent->parent->right->red){
            //uncle is black or NULL
            if (node->parent->right == node){
                RotateLeft(node->parent);
                RotateRight(node->parent);
                RecolorDown(node);
                return 0;
            }
            //node is left left child
            RotateRight(node->parent->parent);
            RecolorDown(node->parent);
            return 0;
        }
        //uncle is red
        RecolorUp(node);
        return 0;

    }
    //node->parent is right child
    if (node->parent->parent->left == NULL ||
            !node->parent->parent->left->red){
        //uncle is black or NULL
        if (node->parent->left == node){
            RotateRight(node->parent);
            RotateLeft(node->parent);
            RecolorDown(node->parent);
            return 0;
        }
        //node is right right child
        RotateLeft(node->parent->parent);
        RecolorDown(node);
        return 0;
    }
    //uncle is red
    RecolorUp(node);
    return 0;
}

template<class T>
int RBTree<T>::Delete(T value){
    Node<T>* target = FindNode(value);

    if (target == NULL){
        return -1;
    }

    if (target->parent == NULL || //delete root
            (target->left != NULL && target->right != NULL)){ // delete "core"
        Node<T>* tmp;
        if ((tmp = FindNode(target->right, MIN)) == NULL &&
                (tmp = FindNode(target->left, MAX)) == NULL){
            root = NULL;
            delete target;
            return 0;
        }
        target->value = tmp->value;
        target = tmp;
    }

    if (target->left != NULL){ //delete branch
        target->value = target->left->value;
        target->red = false;
        delete target->left;
        target->left = NULL;
        return 0;
    }

    if (target->right != NULL){ //delete branch
        target->value = target->right->value;
        target->red = false;
        delete target->right;
        target->right = NULL;
        return 0;
    }

    if (target->red != target->parent->red){ //delete leaf + remove one red
        target->parent->red = false;
        if (target->parent->left == target){
            target->parent->left = NULL;
            delete target;
            return 0;
        }
        target->parent->right = NULL;
        delete target;
        return 0;
    }

    if (target->parent->left == target){
        if (target->parent->right->red){
            target->parent->right->red = false;
            RotateLeft(target->parent);
            target->parent->right->red = true;
            target->parent->left = NULL;
            delete target;
            return 0;
        }
        //sibling is black
        if (target->parent->right->right != NULL ) { //right (or both) child of sibling is red
            target->parent->right->right->red = false;
            target->parent->left = NULL;
            RotateLeft(target->parent);
            delete target;
            return 0;
        }
        if (target->parent->right->left != NULL){ //left child of sibling is red
            target->parent->left = NULL;
            RotateRight(target->parent->right);
            target->parent->right->right->red = false;
            RotateLeft(target->parent);
            delete target;
            return 0;
        }
        target->parent->right->red = true;
        target->parent->left = NULL;
        delete target;
        return 0;
    }

    //target is right child
    if (target->parent->left->red){
        target->parent->right = NULL;
        target->parent->left->red = false;
        RotateRight(target->parent);
        target->parent->left->red = true;
        delete target;
        return 0;
    }
    //sibling is black
    if (target->parent->left->left != NULL ) { //left (or both) child of sibling is red
        target->parent->left = NULL;
        target->parent->left->left->red = false;
        RotateRight(target->parent);
        delete target;
        return 0;
    }
    if (target->parent->left->right != NULL){ //right child of sibling is red
        target->parent->left = NULL;
        RotateLeft(target->parent->left);
        target->parent->left->left->red = false;
        RotateRight(target->parent);
        delete target;
        return 0;
    }
    target->parent->left->red = true;
    target->parent->left = NULL;
    delete target;
    return 0;
}

template<class T>
int RBTree<T>::Find(T value){
    Node<T>* temp = FindNode(value);
    if (temp == NULL){
        return 1;
    }
    return 0;

}

template<class T>
RBTree<T>::Node<T>* RBTree<T>::FindNode(T value){
    if (root == NULL){
        return NULL;
    }
    Node<T>* temp = root;

    for (;;){
        if (temp == NULL){
            return NULL;
        }
        if (value < temp->value){            
            temp = temp->left;
            continue;
        }
        if (value > temp->value){
            temp = temp->right;
            continue;
        }
        return temp;
    }
}

template<class T>
RBTree<T>::Node<T>* RBTree<T>::FindNode(Node<T>* sub_tree, FIND_MODE mode){
    if (sub_tree == NULL){
        return NULL;
    }
    Node<T>* temp;
    switch (mode) {
    case MIN:
        if(sub_tree->left == NULL){
            return sub_tree;
        }
        temp = sub_tree->left;
        while (temp->left != NULL){
            temp = temp->left;
        }
        return temp;
    case MAX:
        if(sub_tree->right == NULL){
            return sub_tree;
        }
        temp = sub_tree->right;
        while (temp->right != NULL){
            temp = temp->right;
        }
        return temp;
    default:
        return NULL;
    }
}

#endif // RBTREE_H
