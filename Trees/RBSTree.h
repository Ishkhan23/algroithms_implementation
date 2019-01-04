#pragma once

#include <iostream>
#include "SearchTree.h"

/*
 * Հա, ուրեմն 1. Արմատն ու տերևները, այսինքն նիլ գագաթները սև են
 * 2.Կարմիրի որդին սև ա
 * 3. Արմատից տերև գնացող ամեն ճանապարհի սև գագաթների քանակը նույնն ա
 * */

template<typename T>
class RBSTree : public SearchTree<T> {
    template<typename E>
    friend std::ostream &operator<<(std::ostream &, const RBSTree<E> &);


public:
    RBSTree();

    virtual ~RBSTree();

    RBSTree(const RBSTree &);

    RBSTree &operator=(const RBSTree &);

    virtual void elegant_insert(const T &);

    virtual void elegant_remove(const T &);

private:
    RBNode<T> *m_root;

    void balance(RBNode<T> *);

    void rotate_left(RBNode<T> *);

    void rotate_right(RBNode<T> *);

    void push_darkness(RBNode<T> *);

    bool obeyRed(RBNode<T> *);
};

template<typename T>
RBSTree<T>::RBSTree()
        : SearchTree<T>() {}

template<typename T>
RBSTree<T>::~RBSTree() {
    this->clear();
}

template<typename T>
RBSTree<T>::RBSTree(const RBSTree &tree)
        : SearchTree<T>(tree) {}

template<typename T>
RBSTree<T> &RBSTree<T>::operator=(const RBSTree &tree) {
    if (this != &tree) {
        this->clear();
        this->m_size = tree.m_size;
        copy(tree);
    }
    return *this;
}

template<typename T>
void RBSTree<T>::elegant_insert(const T &x) {
    if (m_root == nullptr) {
        m_root = new RBNode<T>(x, RBNode<T>::Black);
        return;
    }

    STNode<T> *child = m_root;
    STNode<T> *tempo = nullptr;
    while (child != nullptr) {
        tempo = child;
        if (x < (child)->m_info)
            child = (child)->m_left;
        else
            child = (child)->m_right;
    }

    auto *parent = dynamic_cast<RBNode<T> *>(tempo);
    auto *leaf = new RBNode<T>(parent, x, RBNode<T>::Red);

    if (x < parent->m_info)
        parent->m_left = leaf;
    else
        parent->m_right = leaf;

    balance(leaf);

    ++this->m_size;
}

template<typename T>
void RBSTree<T>::elegant_remove(const T &x) {
    SearchTree<T>::elegant_remove(x);
}

template<typename T>
void RBSTree<T>::balance(RBNode<T> *child) {
    if (child->m_parent == nullptr)
        return;

    RBNode<T> *father = child->m_parent;
    RBNode<T> *grandpa = father->m_parent;

    if (child->m_race == RBNode<T>::Red && obeyRed(child)) {
        if (grandpa->m_left != nullptr && grandpa->m_right != nullptr)
            push_darkness(grandpa);
        else if (child->m_info < father->m_info)
            rotate_right((father->m_info < grandpa->m_info) ? father : child);
        else
            rotate_left((father->m_info < grandpa->m_info) ? child : father);
    }
}

template<typename T>
bool RBSTree<T>::obeyRed(RBNode<T> *child) {
    if (child->m_parent->m_race == RBNode<T>::Red)
        return true;

    auto *left = dynamic_cast<RBNode<T> *>(child->m_left);
    auto *right = dynamic_cast<RBNode<T> *>(child->m_right);

    return (left != nullptr && left->m_race == RBNode<T>::Red) ||
           (right != nullptr && right->m_race == RBNode<T>::Red);
}

template<typename T>
void RBSTree<T>::rotate_left(RBNode<T> *child) {
    RBNode<T> *father = child->m_parent;
    std::swap(father->m_info, child->m_info);
    father->m_left = child;
    father->m_right = child->m_right;
    if (child->m_right != nullptr) {
        dynamic_cast<RBNode<T> * >(child->m_right)->m_parent = father;
        child->m_right = nullptr;
    }
    balance(child);
}

template<typename T>
void RBSTree<T>::rotate_right(RBNode<T> *child) {
    RBNode<T> *father = child->m_parent;
    std::swap(father->m_info, child->m_info);
    father->m_right = child;
    father->m_left = child->m_left;
    if (child->m_left != nullptr) {
        dynamic_cast<RBNode<T> * >(child->m_left)->m_parent = father;
        child->m_left = nullptr;
    }
    balance(child);
}

template<typename T>
void RBSTree<T>::push_darkness(RBNode<T> *grandpa) {
    grandpa->m_race = RBNode<T>::Red;
    dynamic_cast<RBNode<T> * >(grandpa->m_left)->m_race = RBNode<T>::Black;
    dynamic_cast<RBNode<T> * >(grandpa->m_right)->m_race = RBNode<T>::Black;

    if (m_root->m_race == RBNode<T>::Red)
        m_root->m_race = RBNode<T>::Black;

    balance(grandpa);
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const RBSTree<T> &searchTree) {
    out << "[ ";
    if (searchTree.m_root == nullptr)
        return out;

    std::queue<RBNode<T> *> leafs;
    leafs.push(searchTree.m_root);
    RBNode<T> *temp = nullptr;

    while (!leafs.empty()) {
        temp = leafs.front();

        if (temp->m_left != nullptr)
            leafs.push(dynamic_cast<RBNode<T> *>(temp->m_left));
        if (temp->m_right != nullptr)
            leafs.push(dynamic_cast<RBNode<T> *>(temp->m_right));

        leafs.pop();
        out << (temp->m_race == RBNode<T>::Red) ? "\033[31;1m" : "\033[30;1m"
                << temp->m_info << "\033[0m, ";
    }

    return out << "] \n";
}

