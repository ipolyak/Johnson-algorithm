#pragma once
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

// ���������� ��������
template <typename T>
T minimum(const T val_1, const T val_2)
{
    return val_1 > val_2 ? val_2 : val_1;
}

template <class T>
class NodeOfLeftHeap
{
public:
    T element; // �������, ������� �������� � ���� ����
    int key;// ���� ���� (���)
    int rank;// ���� ����
    NodeOfLeftHeap<T>* left_child;// ����� �������
    NodeOfLeftHeap<T>* right_child;// ������ �������
    NodeOfLeftHeap<T>* parent;// �������� ����

    NodeOfLeftHeap();
    NodeOfLeftHeap(T _elem, int _key);


    NodeOfLeftHeap<T>& operator=(const NodeOfLeftHeap<T>& Node);
    bool operator==(const NodeOfLeftHeap<T>& Node);
};

// ����������� �� ���������
template <class T>
NodeOfLeftHeap<T>::NodeOfLeftHeap()
{
    left_child = NULL; right_child = NULL; parent = NULL; rank = 1;
}

// �����������
template <class T>
NodeOfLeftHeap<T>::NodeOfLeftHeap(T _elem, int _key)
{
    element = _elem; key = _key; left_child = NULL; right_child = NULL; parent = NULL; rank = 1;
}

// ���������� ��������� ������������
template <class T>
NodeOfLeftHeap<T>& NodeOfLeftHeap<T>::operator =(const NodeOfLeftHeap<T>& Node)
{
    if(Node != NULL)
    {
        element = Node.element; key = Node.key; left_child = Node.left_child; right_child = Node.right_child; parent = Node.parent; rank = Node.rank;
    }
}

// ���������� ��������� ���������
template <class T>
bool NodeOfLeftHeap<T>::operator ==(const NodeOfLeftHeap<T>& Node)
{
    return Node.element == element ? true : false; // � ����� ������ �������� � ���� ����� ���������
}


// ���������� ������������ ������� � ������� ������������� ����
template <class T>
class LeftHeap
{
private:
    int num_of_nodes;
    NodeOfLeftHeap<T>* RootOfHeap;
    NodeOfLeftHeap<T>** KeepOfNodes;// ������ ������� ����������. ���������� ��� �-��� DecreaseKey, ����� �������� ������ ���� � ������ (������� ������ 0(n) ������� � ������� �������� ��������)
public:
    LeftHeap(T* set_of_vertex, int* set_of_key, int num_of_vertex);
    ~LeftHeap();
    NodeOfLeftHeap<T>* MergeHeaps(NodeOfLeftHeap<T>* L_1, NodeOfLeftHeap<T>* L_2);
    void RemoveMinimum(int& appropriate_vertex, int& key_of_minimum);
    void DecreaseKey(T name, int delta);

    /*������ �������*/
    int GetNumOfNodes();
};

// �����������.
// ���������� ���� �� n ��������� ������ (������ �������� ������� ����������)
// set_of_vertex - ��������� ���������
// set_of_key - ��������� ������, ������� �������� ������� ������� ���� ���������� ����� ��
// �������� ������� �� ���� ���������
// num_of_vertex - ����� ������
// ����������� �� ������������, ��� ������������������ ���� ����� ���� ������������ ������ ���������� �� ������ (����), ��
// � ���� ������� �� ����� ��������� ������ ������ �����, �������������� �������������� �����
// ��������� O(n)
template <class T>
LeftHeap<T>::LeftHeap(T* set_of_vertex, int* set_of_key, int num_of_vertex)
{
    list<NodeOfLeftHeap<T>*> ListOfHeap;
    NodeOfLeftHeap<T>* LeftHeapMerge;
    NodeOfLeftHeap<T>* L_1;
    NodeOfLeftHeap<T>* L_2;

    KeepOfNodes = new NodeOfLeftHeap<T>*[num_of_vertex];

    for(int i = 0; i < num_of_vertex; i++)
    {
        NodeOfLeftHeap<T>* HeapWithSingleEl = new NodeOfLeftHeap<T>;
        HeapWithSingleEl->element = set_of_vertex[i];
        HeapWithSingleEl->key = set_of_key[i];
        KeepOfNodes[i] = HeapWithSingleEl;
        ListOfHeap.push_back(HeapWithSingleEl);

        if(num_of_vertex == 1)
        {
            RootOfHeap = HeapWithSingleEl;
            RootOfHeap->parent = RootOfHeap;
            num_of_nodes = 1;
        }
    }

    while(ListOfHeap.size() > 1)
    {
        L_1 = ListOfHeap.front();
        ListOfHeap.pop_front();
        L_2 = ListOfHeap.front();
        ListOfHeap.pop_front();

        LeftHeapMerge = MergeHeaps(L_1, L_2);

        ListOfHeap.push_back(LeftHeapMerge);
    }

    if(num_of_vertex > 1)
    {
        RootOfHeap = LeftHeapMerge;
        RootOfHeap->parent = RootOfHeap;
        num_of_nodes = num_of_vertex;
    }
}


// ����������
template <class T>
LeftHeap<T>::~LeftHeap()
{
}


// ����� 2 ���� � ����, �.�. L1 + L2 = L
// �������� ���������:
// �������� ������ ���� 2�� ���
// ���������� ��� ����, �������� ��������� ���������� ���_������_������� < ���_�������� < ���_������� �������
// � ���������� ���� ��������� ������������ �������� �������
// ��� ����� ��������� � ���������� ���� �� ������ ������� ����� � ����� (�.�. �� ������ ������� ����)
// ��� ������ ������� ������� � ���� ��������� �������� �������:
// 1. ���� ������ ������� �� ������ ����� ������� ������� (� ������ ��������� ����� ������� - ����� ������������ �������� ����)
// 2. ���� �������� ���� = min(����_������_�������, ����_�������_�������) + 1
// ����� ������ �� ����� � ��������� ��� ���� �������� ������� - ����� ����������� �������� �������
// ��������� �������� ���������:
// ������ ������ �� ��������� logn. �������������, �.�. �� ���������� ������ ���� (����. ����� logn1 � logn2) �������� ���, �� � ������������ ������ ������ logn1 + logn2
// ��� ��� ��������� �� ����� ���� �� �����, � �������� ������ ��������� � ��������� ����� ����������� �� ���������, �� ��������� ������� O(logn1 + logn2) = O(logn)
template <class T>
NodeOfLeftHeap<T>* LeftHeap<T>::MergeHeaps(NodeOfLeftHeap<T>* L_1, NodeOfLeftHeap<T>* L_2)
{
    NodeOfLeftHeap<T>* L;

    if(L_1 == NULL)
        return L_2;

    if(L_2 == NULL)
        return L_1;

    NodeOfLeftHeap<T>* L_TMP;
    if(L_1->key > L_2->key)
    {
        L_TMP = L_1;
        L_1 = L_2;
        L_2 = L_TMP;
    }

    L = L_1;
    L_TMP = MergeHeaps(L_1->right_child, L_2);
    L->right_child = L_TMP;
    L_TMP->parent = L;

    if(L->left_child == NULL)
    {
        L_TMP = L->left_child;
        L->left_child = L->right_child;
        L->right_child = L_TMP;
    }
    else
        if(L->left_child->rank < L->right_child->rank)
        {
            L_TMP = L->left_child;
            L->left_child = L->right_child;
            L->right_child = L_TMP;
        }

        if(L->right_child == NULL)
            L->rank = minimum(0, L->left_child->rank) + 1;
        else
            L->rank = minimum(L->right_child->rank, L->left_child->rank) + 1;

        return L;
}

// � ���� ������� ������� �������, �� ����� ���� ��������� ��� ������ (����� ������� � ���������� appropriate_vertex � ��� ���� � key_of_minimum)
// ��������, �������� ��������� �������������� ������, ������� ����� � �����
// ����� ������� ��������, ��������� ����� ����� � ������ �������
// �.�. �������� ������ ����� ����������� �� ���������, � ������� O(logn), �.�. ����� ��������� O(logn)
template <class T>
void LeftHeap<T>::RemoveMinimum(int &appropriate_vertex, int &key_of_minimum)
{
    appropriate_vertex = RootOfHeap->element;
    key_of_minimum = RootOfHeap->key;


    NodeOfLeftHeap<T>* L_RES;
    if(num_of_nodes > 1)
        L_RES = MergeHeaps(RootOfHeap->left_child, RootOfHeap->right_child);

    delete RootOfHeap;

    if(num_of_nodes > 1)
    {
        RootOfHeap = L_RES;
        RootOfHeap->parent = RootOfHeap;
    }

    num_of_nodes -= 1;
}

// �������� ����� �����
template <class T>
int LeftHeap<T>::GetNumOfNodes()
{
    return num_of_nodes;
}

// ��������� � ����, ���������� ������� name, ���� �� delta ������
// �������� ���������:
// �������� ���� P_2 � ������ � ����, ���������� name, �� �������� ����. ������ �������� ���� �� ����������� ������������, � ���������� - �������������
// ��������������� �������� �������: ���� ����� �� ���� �� ���� ����, ����������� name, � ����� �������� ����
// ����� ����, ��� ������������ ��-�� �������, ���������� �������� � ���������� ����
// ����������� �������� �� O(logn) (�������������� ����� ���� logn � ������� logn). � ������ ������ �� O(n), �.�. ������ ����� ���� ������������ � ���� ������ ����.
template <class T>
void LeftHeap<T>::DecreaseKey(T name, int delta)
{
    NodeOfLeftHeap<T>* P = KeepOfNodes[name]; // ����, ������� ����� ��������� ������� name
    NodeOfLeftHeap<T>* P_1;
    NodeOfLeftHeap<T>* P_2;
    NodeOfLeftHeap<T>* TMP;

    int rank_1, rank_2, new_rank;

    P->key = P->key - delta;
    if(P == RootOfHeap)
        return;

    P_1 = P->parent;
    P_2 = P;

    if(P_1->left_child == P)
        P_1->left_child = NULL;
    else
        if(P_1->right_child == P)
            P_1->right_child = NULL;

    while(P_1 != NULL)
    {
        if(P_1->left_child != NULL)
            rank_1 = P_1->left_child->rank;
        else
            rank_1 = 0;

        if(P_1->right_child != NULL)
            rank_2 = P_1->right_child->rank;
        else
            rank_2 = 0;

        new_rank = minimum(rank_1, rank_2) + 1;

        if(rank_1 < rank_2)
        {
            TMP = P_1->left_child;
            P_1->left_child = P_1->right_child;
            P_1->right_child = TMP;
        }

        if(new_rank != P_1->rank)
            P_1->rank = new_rank;
        else
            break;

        P_1 = P_1->parent;
    }

    RootOfHeap = MergeHeaps(RootOfHeap, P_2);
    RootOfHeap->parent = RootOfHeap;
}