#pragma once
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

template <class T>
class NodeOfSelforganizationHeap
{
public:
    T element; // �������, ������� �������� � ���� ����
    int key;// ���� ���� (���)
    NodeOfSelforganizationHeap<T>* left_child;// ����� �������
    NodeOfSelforganizationHeap<T>* right_child;// ������ �������
    NodeOfSelforganizationHeap<T>* parent;// �������� ����

    NodeOfSelforganizationHeap();
    NodeOfSelforganizationHeap(T _elem, int _key);


    NodeOfSelforganizationHeap<T>& operator=(const NodeOfSelforganizationHeap<T>& Node);
    bool operator==(const NodeOfSelforganizationHeap<T>& Node);
};

// ����������� �� ���������
template <class T>
NodeOfSelforganizationHeap<T>::NodeOfSelforganizationHeap()
{
    left_child = NULL; right_child = NULL; parent = NULL;
}

// �����������
template <class T>
NodeOfSelforganizationHeap<T>::NodeOfSelforganizationHeap(T _elem, int _key)
{
    element = _elem; key = _key; left_child = NULL; right_child = NULL; parent = NULL;
}

// ���������� ��������� ������������
template <class T>
NodeOfSelforganizationHeap<T>& NodeOfSelforganizationHeap<T>::operator =(const NodeOfSelforganizationHeap<T>& Node)
{
    if(Node != NULL)
    {
        element = Node.element; key = Node.key; left_child = Node.left_child; right_child = Node.right_child; parent = Node.parent;
    }
}

// ���������� ��������� ���������
template <class T>
bool NodeOfSelforganizationHeap<T>::operator ==(const NodeOfSelforganizationHeap<T>& Node)
{
    return Node.element == element ? true : false; // � ����� ������ �������� � ���� ����� ���������
}


// ������������������ ����
template <class T>
class SelforganizationHeap
{
private:
    int num_of_nodes;
    NodeOfSelforganizationHeap<T>* RootOfHeap;
    NodeOfSelforganizationHeap<T>** KeepOfNodes;// ������ ������� ����������. ���������� ��� �-��� DecreaseKey, ����� �������� ������ ���� � ������ (������� ������ 0(n) ������� � ������� �������� ��������)
public:
    SelforganizationHeap(T* set_of_vertex, int* set_of_key, int num_of_vertex);
    ~SelforganizationHeap();

    NodeOfSelforganizationHeap<T>* MergeHeaps(NodeOfSelforganizationHeap<T>* S_1, NodeOfSelforganizationHeap<T>* S_2);
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
// ����������� �� ������������, ��� ������������� ���� ����� ���� ������������ ������ ���������� �� ������ (����), ��
// � ���� ������� �� ����� ��������� ������ ������ �����, �������������� �������������� �����
template <class T>
SelforganizationHeap<T>::SelforganizationHeap(T* set_of_vertex, int* set_of_key, int num_of_vertex)
{
    list<NodeOfSelforganizationHeap<T>*> ListOfHeap;
    NodeOfSelforganizationHeap<T>* SelforganizationHeapMerge;
    NodeOfSelforganizationHeap<T>* S_1;
    NodeOfSelforganizationHeap<T>* S_2;

    KeepOfNodes = new NodeOfSelforganizationHeap<T>*[num_of_vertex];

    for(int i = 0; i < num_of_vertex; i++)
    {
        NodeOfSelforganizationHeap<T>* HeapWithSingleEl = new NodeOfSelforganizationHeap<T>;
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
        S_1 = ListOfHeap.front();
        ListOfHeap.pop_front();
        S_2 = ListOfHeap.front();
        ListOfHeap.pop_front();

        SelforganizationHeapMerge = MergeHeaps(S_1, S_2);

        ListOfHeap.push_back(SelforganizationHeapMerge);
    }

    if(num_of_vertex > 1)
    {
        RootOfHeap = SelforganizationHeapMerge;
        RootOfHeap->parent = RootOfHeap;
        num_of_nodes = num_of_vertex;
    }
}

// ����������
template <class T>
SelforganizationHeap<T>::~SelforganizationHeap()
{
}

// ������� 2�� ���. 
// �������� ���������:
// ���� ���� h1 � h2, ������� �� � h. ��� ����� ������� ������ ���� �������� ���, �������� �������� ��������������
// ������������ �������������� ���� ���������� ����� ����� ����� ����, � ����� ���������� �����, �������� � ����, ���������� ������� ������������
// ��������� O(n). ��������������� ������ O(logn)
template <class T>
NodeOfSelforganizationHeap<T>* SelforganizationHeap<T>::MergeHeaps(NodeOfSelforganizationHeap<T>* S_1, NodeOfSelforganizationHeap<T>* S_2)
{
    NodeOfSelforganizationHeap<T>* S;// � ����� ����� ����� �������� ��������� �� ������ �������������� ����

    if(S_1 == NULL)// ���� ���� ���� �������, �� � ���������� ������, ����������� ����� ���� S_2 ��������
    {
        NodeOfSelforganizationHeap<T>* TMP;
        NodeOfSelforganizationHeap<T>* Bypass;
        TMP = S_2->left_child ;
        S_2->left_child = S_2->right_child;
        S_2->right_child = TMP;

        Bypass = S_2->left_child;
        while(Bypass != NULL)
        {
            TMP = Bypass->left_child;
            Bypass->left_child = Bypass->right_child;
            Bypass->right_child = TMP;

            Bypass = Bypass->left_child;
        }
        return S_2;
    }

    if(S_2 == NULL)
        return S_1;

    NodeOfSelforganizationHeap<T>* S_TMP;

    if(S_1->key > S_2->key)
    {
        S_TMP = S_1;
        S_1 = S_2;
        S_2 = S_TMP;
    }

    S = S_1;
    S_TMP = MergeHeaps(S_1->right_child, S_2);

    S->right_child = S->left_child;

    S->left_child = S_TMP;
    S_TMP->parent = S;

    return S;
}

// � ���� ������� ������� �������, �� ����� ���� ��������� ��� ������ (����� ������� � ���������� appropriate_vertex � ��� ���� � key_of_minimum)
// ��������, �������� �������� ��������������, ������� ����� � �����
// ����� ������� ��������, ��������� ����� ����� � ������ �������
// �.�. �������� ������ ����� ����������� �� ���������, � ������� O(n) � ������ ������, � ��������������� O(logn)
template <class T>
void SelforganizationHeap<T>::RemoveMinimum(int &appropriate_vertex, int &key_of_minimum)
{
    appropriate_vertex = RootOfHeap->element;
    key_of_minimum = RootOfHeap->key;


    NodeOfSelforganizationHeap<T>* S_RES;
    if(num_of_nodes > 1)
        S_RES = MergeHeaps(RootOfHeap->left_child, RootOfHeap->right_child);

    delete RootOfHeap;

    if(num_of_nodes > 1)
    {
        RootOfHeap = S_RES;
        RootOfHeap->parent = RootOfHeap;
    }

    num_of_nodes -= 1;
}


// ��������� � ����, ���������� ������� name, ���� �� delta ������
// �������� ���������:
// �������� ���� S_2 � ������ � ����, ���������� name, �� �������� ����.
// ���������� �������� � ���������� ����
// ����������� �������� � ������ ������ �� O(n) (�.�. ������� �� O(n)). ��������������� ������ O(logn)
template <class T>
void SelforganizationHeap<T>::DecreaseKey(T name, int delta)
{
    NodeOfSelforganizationHeap<T>* S = KeepOfNodes[name]; // ����, ������� ����� ��������� ������� name
    NodeOfSelforganizationHeap<T>* S_1;
    NodeOfSelforganizationHeap<T>* S_2;


    S->key = S->key - delta;
    if(S == RootOfHeap)
        return;

    S_1 = S->parent;
    S_2 = S;

    if(S_1->left_child == S_2)
        S_1->left_child = NULL;
    else
        if(S_1->right_child == S_2)
            S_1->right_child = NULL;

    RootOfHeap = MergeHeaps(RootOfHeap, S_2);

    RootOfHeap->parent = RootOfHeap;
}

// �������� ����� �����
template <class T>
int SelforganizationHeap<T>::GetNumOfNodes()
{
    return num_of_nodes;
}