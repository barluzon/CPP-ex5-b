#pragma once
#include <iostream>
#include <queue> 

using namespace std;

namespace ariel{

    const int POSTORDER = 0;
    const int INORDER = 1;
    const int PREORDER = 2;

    template<typename T>
    class BinaryTree {
    private:
    struct Node {
        T value;
        Node* right;
        Node* left;
        Node(const T& val, Node* le = nullptr, Node* ri = nullptr)
                : value(val), left(le),right(ri){}
       
    };

    class iterator {
        Node* currNodeP;
        queue<Node*> itOrder; 
            
        public:
        iterator(Node* n,int choosenOrder);

        T& operator*() const {
            return currNodeP->value;
        }
        T* operator->() const {
            return &(currNodeP->value);
        }
        //pre
        iterator &operator++(){
        currNodeP = itOrder.front();
        itOrder.pop();
        return *this;
        }
        //post
        iterator operator++(int){
        iterator temp = *this;
        currNodeP = itOrder.front();
        itOrder.pop();
        return temp;
        }

        bool operator==(const iterator &other) const{
            return currNodeP == other.currNodeP;
        }
        bool operator!=(const iterator &other) const{
            return !(currNodeP == other.currNodeP);
        }
    };

        static void iterOrder(Node** nP,queue<Node*> &itQue, int order);
        
        Node* root = nullptr;

        public:
        BinaryTree(){};
        ~BinaryTree(){
            if(root != nullptr){
                delete root;
            }
        }

        BinaryTree<T>& add_root(T root);
        BinaryTree<T>& add_left(T exist ,T toAdd);
        BinaryTree<T>& add_right(T exist ,T toAdd);
        Node *searchNode(Node *root, T toFind); 

        BinaryTree &operator=(const BinaryTree<T> tree) {
            if (this == &tree) {
                return *this;
            }
            if (root != nullptr) {
                delete root;
            }
            if (tree.root != nullptr) {
                root = new Node(tree.root->value);
                copyTree(root, tree.root);
            }
            return *this;
        }

        void copyTree(Node *copyTo, const Node *copyFrom) {
            if (copyFrom->right != nullptr) {
                copyTo->right = new Node(copyFrom->right->value);
                copyTree(copyTo->right, copyFrom->right);
            }
            if (copyFrom->left != nullptr) {
                copyTo->left = new Node(copyFrom->left->value);
                copyTree(copyTo->left, copyFrom->left);
            }
        } 
        
        BinaryTree(const BinaryTree &tree) {
            if (tree.root != nullptr) {
                this->root = new Node(tree.root->value);
                copyTree(root, tree.root);
            }
        }

        BinaryTree& operator=(BinaryTree<T>&& tree) noexcept {
            if (root){delete root;
            }
            root  = tree.root;
            tree.root = nullptr;
            return *this;
        }

        BinaryTree(BinaryTree &&tree) noexcept {
            this->root = tree.root;
            tree.root=nullptr;
        }
        
        iterator begin(){
            return  iterator(root,INORDER);
        }
        iterator end(){
            return iterator(nullptr,INORDER);
        }
        iterator begin_preorder(){
            return iterator(root,PREORDER);
        }
        iterator end_preorder(){
            return iterator(nullptr,PREORDER);
        }
        iterator begin_inorder(){
            return  iterator(root,INORDER);
        }
        iterator end_inorder(){
            return iterator(nullptr,INORDER);
        }
        iterator begin_postorder(){
            return iterator(root,POSTORDER);
        }
        iterator end_postorder(){
            return iterator(nullptr,POSTORDER);
        }

        //from: https://stackoverflow.com/questions/13484943/print-a-binary-tree-in-a-pretty-way

        static ostream& printTree(std::ostream& out, Node* root, const string& space){
            if (root == NULL){
                out << "Exc: Root doesn't exist\n";
                return out;
            }

            bool hasLeft = (root->left != NULL);
            bool hasRight = (root->right != NULL);

            if (!hasLeft && !hasRight){
                out << root->value;
                return out;
            }
            out << space;
            out << ((hasLeft  && hasRight) ? "├─── " : "");
            out << ((!hasLeft && hasRight) ? "└─── " : "");

            if (hasRight){
                    bool printStrand = (hasLeft && hasRight && (root->right->right != NULL || root->right->left != NULL));
                    string newSpace = space + (printStrand ? "│   " : "    ");
                    out << root->right->value << endl;
                    printTree(out,root->right, newSpace);
            }

            if (hasLeft){
                    out << (hasRight ? space : "") << "└─── " << root->left->value << endl;
                    printTree(out,root->left, space + "    ");
            }
            return out;
        }
    
        friend std::ostream& operator<<(std::ostream& out, const BinaryTree<T>& tree) {
            out<<"\nTree: \n\n"<<endl;
            printTree(out,tree.root, "");
            out << endl;
            return out;
        }
    };

    //Implement func

    template<typename T>
    void BinaryTree<T>::iterOrder(Node **nP,queue<Node*> &itQue, int order) {
        if (*nP == nullptr){
            return;
        }
        if(order == INORDER){
           iterOrder(&(*nP)->left, itQue, INORDER);
           itQue.push(*nP);
           iterOrder(&(*nP)->right, itQue, INORDER);
        }
    
        else if(order == POSTORDER){
            iterOrder(&(*nP)->left, itQue,POSTORDER);
            iterOrder(&(*nP)->right, itQue,POSTORDER);
            itQue.push(*nP);
        }

        else if(order == PREORDER){
            itQue.push(*nP);
            iterOrder(&(*nP)->left, itQue, PREORDER);
            iterOrder(&(*nP)->right, itQue, PREORDER);
        }
    }
    
    template<typename T>
    BinaryTree<T>::iterator::iterator(Node* nP, int choosenOrder){
        iterOrder(&nP,itOrder,choosenOrder);
        itOrder.push(nullptr);
        currNodeP = itOrder.front();
        itOrder.pop();
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_root(T newRoot){
        if (root == nullptr){
            root = new Node(newRoot);
        }
        root->value = newRoot;
        return *this;
    }

        template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_left(T exist, T toAdd){
     Node *nP = searchNode(root, exist);
        if (nP == nullptr) {
            throw std::invalid_argument("Exc: Node doesn't exist\n");
        }
        if (nP->left == nullptr) {
            nP->left = new Node(toAdd);
        } else {
            nP->left->value = toAdd;
        }
        return *this;
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_right(T exist ,T toAdd){
        if (root == nullptr){throw invalid_argument("Exc: Root doesn't exist\n");
        }
        Node *nP = searchNode(root, exist);
        if (nP == nullptr) {
            throw std::invalid_argument("Exc: Node doesn't exist\n");
        }
        if (nP->right == nullptr) {
            nP->right = new Node{toAdd};
            
        } else {
            nP->right->value = toAdd;
        }
        return *this;
    }

    template <typename T>
    typename BinaryTree<T>::Node* BinaryTree<T>:: searchNode(BinaryTree::Node *root ,T toFind) {
        if(!root) {return nullptr;
        }
        if(root->value == toFind) {
            return root;
        }

        Node* retNodeP = searchNode(root->right,toFind);
        if(retNodeP) {return retNodeP;
        }
        return searchNode(root->left,toFind);
    }
}