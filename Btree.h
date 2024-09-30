#pragma once
int ORDER;
#include <iostream>
#include<fstream>
#include<string>
#include<direct.h>
#include"File.h"
using namespace std;

//====================================
template <typename T>
class BTreeNode
{
public:
    bool isLeaf;
    int count;

    T* keys;
    File** data;

    BTreeNode** children;
    BTreeNode* parent;

    BTreeNode(bool leaf = false, int order = 0)
    {
        keys = new T[order * 2 - 1];
        data = new File * [order * 2 - 1];

        children = new BTreeNode * [order * 2];
        isLeaf = leaf;
        count = 0;

        parent = NULL;
        for (int i = 0; i < order * 2; i++)
        {
            children[i] = NULL;
        }
        for (int i = 0; i < order * 2 - 1; i++)
        {
            keys[i] = 0;
            data[i] = NULL;
        }
    }

};
template <typename T>
class BTree
{
public:
    BTreeNode<T>* root;
    //int min_keys;
    //int max_keys;
    int digree;
    BTree(int deg)
    {
        root = NULL;
        this->digree = deg;

    }

    //=======================================
    void insert(T key)
    {
        if (root == NULL)
        {

            BTreeNode<T>* newnode = new BTreeNode<T>(true, digree);
            newnode->count++;
            newnode->keys[0] = key;
            root = newnode;
        }
        else
        {

            if (root->count == (2 * digree) - 1)
            {
                BTreeNode<T>* newnode = new BTreeNode<T>(false, digree);
                newnode->children[0] = root;

                Split_Node(newnode, 0);

                root = newnode;
                insert_withspace(newnode, key);
            }
            else
            {
                insert_withspace(root, key);
            }


        }
    }

    //=======================================
    void Split_Node(BTreeNode<T>* node, int i)
    {
        BTreeNode<T>* temp_root = node->children[i];
        BTreeNode<T>* new_sibling = new BTreeNode<T>(temp_root->isLeaf, digree);


        //-------------SHIFT KEY-------------------
        for (int j = digree - 1; j >= 0; j--)
        {
            new_sibling->keys[j] = temp_root->keys[j + digree];
            new_sibling->data[j] = temp_root->data[j + digree];
            temp_root->keys[j + digree] = 0;
        }

        new_sibling->count = digree - 1;
        temp_root->count = digree - 1;
        //-------------SHIFT children addresses if interior node-------------------
        if (!temp_root->isLeaf) {
            for (int j = digree; j >= 0; j--)
            {
                new_sibling->children[j] = temp_root->children[j + digree];
                temp_root->children[j + digree] = nullptr;
            }
        }
        //-------------SHIFT children addresses if interior node-------------------
        for (int j = node->count; j > i; j--)
        {
            node->children[j + 1] = node->children[j];
        }
        node->children[i + 1] = new_sibling;
        //-----------key shift in parent-------------------------------------------
        for (int j = node->count - 1; j >= i; j--)
        {
            node->keys[j + 1] = node->keys[j];
            node->data[j + 1] = node->data[j];
        }

        node->keys[i] = temp_root->keys[digree - 1];
        node->data[i] = temp_root->data[digree - 1];
        node->count++;
    }
    //=============================================
    void insert_withspace(BTreeNode<T>* node, T key)
    {
        int index = node->count - 1;

        if (node->isLeaf)
        {
            while (index >= 0 && key < node->keys[index])
            {
                node->keys[index + 1] = node->keys[index];
                node->data[index + 1] = node->data[index];
                index--;

            }
            if (key == node->keys[index])
            {
                cout << "duplicate \n";
                return;
            }
            node->keys[index + 1] = key;
            node->data[index + 1] = NULL;
            node->count++;
        }
        else
        {
            while (index >= 0 && key < node->keys[index])
            {
                index--;
            }
            index++;
            if (node->children[index]->count == 2 * digree - 1)
            {
                Split_Node(node, index);
                if (key > node->keys[index])
                {
                    index++;
                }
            }
            insert_withspace(node->children[index], key);
        }
    }
    //============================================

    //=============================================
    BTreeNode<T>* getRoot() const {
        return root;
    }

    void display(BTreeNode<T>* x, int level = 0) {
        if (x != nullptr) {
            std::cout << "Level " << level << ": ";
            for (int i = 0; i < x->count; i++)
            {
                std::cout << x->keys[i] << " ";
                if (x->data[i])
                {
                    cout << "DATA (";
                    x->data[i]->printData();
                    cout << " )";
                }
            }
            cout << " Count :" << x->count << "leaf :" << x->isLeaf;
            std::cout << std::endl;

            if (!x->isLeaf) {
                for (int i = 0; i <= x->count; i++) {
                    display(x->children[i], level + 1);
                }
            }
        }
    }
    //========================DELETION=======================
    void deletion(T key)
    {
        if (root == NULL)
        {
            cout << "Tree is empty";
            return;
        }
        delete_node(root, key);
        if (root->count == 0)
        {
            root = root->children[0];
        }

    }
    void delete_node(BTreeNode<T>* node, T key)
    {

        int index = 0;
        while ((index < node->count && node->keys[index] < key))
        {
            index++;
        }
        //cout << "index :" << index << endl;
        if (node->keys[index] == key && index < node->count)
        {
            if (node->isLeaf)  //found key, node is leaf,
            {
                for (int i = index + 1; i < node->count; i++)
                {
                    node->keys[i - 1] = node->keys[i];
                    node->data[i - 1] = node->data[i];
                }

                node->count -= 1;
            }
            else       //found key, node is not leaf 
            {
                //left child of node have keys greater or equal to digree   --> ((CASE 2(a)))
                if (node->children[index]->count >= digree)
                {
                    BTreeNode<T>* current = node->children[index];
                    while (current->isLeaf == false)
                    {
                        current = current->children[current->count];
                    }
                    int maximum = current->keys[current->count - 1];
                    node->keys[index] = maximum;
                    node->data[index] = current->data[current->count - 1];
                    delete_node(node->children[index], maximum);


                }       //--right child of node have keys greater or equal to digree   --> ((CASE 2(b))) onlt if 2(a) nor fulfill--
                else if (node->children[index + 1]->count >= digree)
                {

                    BTreeNode<T>* current = node->children[index + 1];
                    while (current->isLeaf == false)
                    {
                        current = current->children[0];
                    }
                    int minimum = current->keys[0];
                    node->keys[index] = minimum;
                    node->data[index] = current->data[0];
                    delete_node(node->children[index + 1], minimum);

                }       //--right and left child of node have keys less or equal to digree --> ((CASE 2(c)))
                else
                {
                    BTreeNode<T>* left_child = node->children[index];
                    BTreeNode<T>* right_child = node->children[index + 1];

                    left_child->keys[digree - 1] = node->keys[index];
                    left_child->data[digree - 1] = node->data[index];

                    for (int i = 0; i < right_child->count; i++)
                    {
                        left_child->keys[i + digree] = right_child->keys[i];
                        left_child->data[i + digree] = right_child->data[i];
                    }

                    if (!right_child->isLeaf)
                    {
                        for (int i = 0; i <= right_child->count; ++i)
                            left_child->children[i + digree] = right_child->children[i];
                    }

                    for (int i = index + 1; i < node->count; i++)
                    {
                        node->keys[i - 1] = node->keys[i];
                        node->data[i - 1] = node->data[i];
                    }

                    for (int i = index + 2; i <= node->count; i++)
                    {
                        node->children[i - 1] = node->children[i];
                    }

                    left_child->count += right_child->count;
                    left_child->count += 1;
                    node->count -= 1;

                    for (int i = 0; i < right_child->count; i++)
                    {
                        right_child->keys[i] = 0;
                        right_child->data[i] = NULL;
                    }
                    right_child->count = 0;
                    delete right_child;
                    // node->children[index + 1] = NULL;

                    delete_node(node->children[index], key);

                    return;
                }
                return;
            }

        }
        else
        {
            if (node->isLeaf)  //Not found key and on leaf, its mean key is not in tree.
            {
                cout << "KEY is not in Tree!!";
                return;
            }
            else       //not found key still and node is not leaf (interior node)
            {

                bool flag = ((index == node->count) ? true : false);

                if (node->children[index]->count < digree)      //----CASE 3(a)----- recursively node has digree-1 child
                {
                    BTreeNode<T>* mid_child = node->children[index];

                    if (index > 0 && node->children[index - 1]->count >= digree)
                    {
                        cout << " CARRY FROM LEFT SIBLING" << endl;
                        BTreeNode<T>* left_child = node->children[index - 1];

                        for (int i = mid_child->count - 1; i >= 0; i--)
                        {
                            mid_child->keys[i + 1] = mid_child->keys[i];
                            mid_child->data[i + 1] = mid_child->data[i];
                        }

                        mid_child->keys[0] = node->keys[index - 1];
                        mid_child->data[0] = node->data[index - 1];
                        node->keys[index - 1] = left_child->keys[left_child->count - 1];
                        node->data[index - 1] = left_child->data[left_child->count - 1];

                        if (mid_child->isLeaf == false) {
                            for (int i = mid_child->count; i >= 0; i--)
                            {
                                mid_child->children[i + 1] = mid_child->children[i];
                            }
                            mid_child->children[0] = left_child->children[left_child->count];
                        }
                        mid_child->count++;
                        left_child->keys[left_child->count - 1] = 0;
                        left_child->data[left_child->count - 1] = NULL;
                        left_child->count--;
                    }
                    else if (index < node->count && node->children[index + 1]->count >= digree)
                    {
                        cout << " CARRY FROM RIGHT SIBLING" << endl;
                        BTreeNode<T>* right_child = node->children[index + 1];

                        mid_child->keys[mid_child->count] = node->keys[index];
                        mid_child->data[mid_child->count] = node->data[index];

                        node->keys[index] = right_child->keys[0];
                        node->data[index] = right_child->data[0];

                        for (int i = 0; i < right_child->count - 1; i++)
                        {
                            right_child->keys[i] = right_child->keys[i + 1];
                            right_child->data[i] = right_child->data[i + 1];
                        }
                        mid_child->count++;
                        if (mid_child->isLeaf == false)
                        {
                            mid_child->children[mid_child->count] = right_child->children[0];

                            for (int i = 0; i < right_child->count; i++)
                            {
                                right_child->children[i] = right_child->children[i + 1];
                            }
                        }

                        right_child->count--;

                    }
                    else if (index >= 0 && index <= node->count)
                    {
                        cout << " CARRY FROM MERGE SIBLING" << endl;

                        if (index < node->count)
                        {
                            BTreeNode<T>* left_child = node->children[index];
                            BTreeNode<T>* right_child = node->children[index + 1];

                            left_child->keys[digree - 1] = node->keys[index];
                            left_child->data[digree - 1] = node->data[index];

                            for (int i = 0; i < right_child->count; i++)
                            {
                                left_child->keys[i + digree] = right_child->keys[i];
                                left_child->data[i + digree] = right_child->data[i];
                            }

                            if (!right_child->isLeaf)
                            {
                                for (int i = 0; i <= right_child->count; ++i)
                                    left_child->children[i + digree] = right_child->children[i];
                            }

                            for (int i = index + 1; i < node->count; i++)
                            {
                                node->keys[i - 1] = node->keys[i];
                                node->data[i - 1] = node->data[i];
                            }

                            for (int i = index + 2; i <= node->count; i++)
                            {
                                node->children[i - 1] = node->children[i];
                            }

                            left_child->count += right_child->count;
                            left_child->count += 1;
                            node->count -= 1;

                            for (int i = 0; i < right_child->count; i++)
                            {
                                right_child->keys[i] = 0;
                                right_child->data[i] = NULL;
                            }
                            right_child->count = 0;
                            delete right_child;
                            // node->children[index + 1] = NULL;

                        }
                        else
                        {

                            BTreeNode<T>* left_child = node->children[index - 1];
                            BTreeNode<T>* right_child = node->children[index];

                            left_child->keys[digree - 1] = node->keys[index - 1];
                            left_child->data[digree - 1] = node->data[index - 1];

                            for (int i = 0; i < right_child->count; i++)
                            {
                                left_child->keys[i + digree] = right_child->keys[i];
                                left_child->data[i + digree] = right_child->data[i];
                            }

                            if (!right_child->isLeaf)
                            {
                                for (int i = 0; i <= right_child->count; ++i)
                                    left_child->children[i + digree] = right_child->children[i];
                            }

                            for (int i = index; i < node->count; i++)
                            {
                                node->keys[i - 1] = node->keys[i];
                                node->data[i - 1] = node->data[i];
                            }

                            for (int i = index + 1; i <= node->count; i++)
                            {
                                node->children[i - 1] = node->children[i];
                            }

                            left_child->count += right_child->count;
                            left_child->count += 1;
                            node->count -= 1;

                            for (int i = 0; i < right_child->count; i++)
                            {
                                right_child->keys[i] = 0;
                                right_child->data[i] = 0;
                            }
                            right_child->count = 0;
                            delete right_child;
                            //node->children[index ] = NULL;

                        }
                    }

                }

                if (flag && index > node->count)
                {
                    delete_node(node->children[index - 1], key);

                }
                else
                {
                    delete_node(node->children[index], key);

                }


            }

        }
        return;
    }
    BTreeNode<T>* search_node(T k)
    {
        return search(root, k);
    }

    BTreeNode<T>* search(BTreeNode<T>* current, T k)
    {
        int i = 0;
        while (i < current->count && k > current->keys[i])
            i++;

        if (current->keys[i] == k)
            return current;

        if (current->isLeaf == true)
            return NULL;

        return search(current->children[i], k);
    }
};

