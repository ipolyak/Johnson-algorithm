#pragma once
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

// Вычисление минимума
template <typename T>
T minimum(const T val_1, const T val_2)
{
    return val_1 > val_2 ? val_2 : val_1;
}

template <class T>
class NodeOfLeftHeap
{
public:
    T element; // Элемент, который хранится в этом узле
    int key;// ключ узла (вес)
    int rank;// ранг узла
    NodeOfLeftHeap<T>* left_child;// левый потомок
    NodeOfLeftHeap<T>* right_child;// правый потомок
    NodeOfLeftHeap<T>* parent;// родитель узла

    NodeOfLeftHeap();
    NodeOfLeftHeap(T _elem, int _key);


    NodeOfLeftHeap<T>& operator=(const NodeOfLeftHeap<T>& Node);
    bool operator==(const NodeOfLeftHeap<T>& Node);
};

// Конструктор по умолчанию
template <class T>
NodeOfLeftHeap<T>::NodeOfLeftHeap()
{
    left_child = NULL; right_child = NULL; parent = NULL; rank = 1;
}

// Конструктор
template <class T>
NodeOfLeftHeap<T>::NodeOfLeftHeap(T _elem, int _key)
{
    element = _elem; key = _key; left_child = NULL; right_child = NULL; parent = NULL; rank = 1;
}

// Перегрузка оператора присваивания
template <class T>
NodeOfLeftHeap<T>& NodeOfLeftHeap<T>::operator =(const NodeOfLeftHeap<T>& Node)
{
    if(Node != NULL)
    {
        element = Node.element; key = Node.key; left_child = Node.left_child; right_child = Node.right_child; parent = Node.parent; rank = Node.rank;
    }
}

// Перегрузка оператора сравнения
template <class T>
bool NodeOfLeftHeap<T>::operator ==(const NodeOfLeftHeap<T>& Node)
{
    return Node.element == element ? true : false; // В нашем случае элементы в куче будут уникальны
}


// Реализация приоритетной очереди с помощью левосторонней кучи
template <class T>
class LeftHeap
{
private:
    int num_of_nodes;
    NodeOfLeftHeap<T>* RootOfHeap;
    NodeOfLeftHeap<T>** KeepOfNodes;// Массив адресов указателей. Необходимо для ф-ции DecreaseKey, чтобы избежать поиска узла в дереве (которое займет 0(n) времени и ухудшит алгоритм Дейкстры)
public:
    LeftHeap(T* set_of_vertex, int* set_of_key, int num_of_vertex);
    ~LeftHeap();
    NodeOfLeftHeap<T>* MergeHeaps(NodeOfLeftHeap<T>* L_1, NodeOfLeftHeap<T>* L_2);
    void RemoveMinimum(int& appropriate_vertex, int& key_of_minimum);
    void DecreaseKey(T name, int delta);

    /*Методы доступа*/
    int GetNumOfNodes();
};

// Конструктор.
// Образовать кучу из n элементов списка (список является входным параметром)
// set_of_vertex - множество элементов
// set_of_key - множество ключей, которые являются текущей оценкой длин кратчайших путей от
// заданной вершины до всех остальных
// num_of_vertex - число вершин
// Основываясь на высказывании, что самоорганизующаяся куча может быть представлена просто указателем на корень (узел), то
// в этой функции мы будем создавать просто список узлов, соответсвующих одноэлементным кучам
// Сложность O(n)
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


// Деструктор
template <class T>
LeftHeap<T>::~LeftHeap()
{
}


// Слить 2 кучи в одну, т.е. L1 + L2 = L
// Алгоритм следующий:
// Выделяем правые пути 2ух куч
// Объединяем эти пути, соблюдая следующее требование вес_левого_потомка < вес_родителя < вес_правого потомка
// В полученной кучи требуется восстановить свойство левизны
// Для этого двигаемся в полученной кучи от самого правого листа к корню (т.е. по новому правому пути)
// Для каждой текущей вершины в пути проверяем свойство левизны:
// 1. Ранг левого потомка не меньше ранга правого потомка (в случае нарушения этого условия - обмен поддеревьями текущего узла)
// 2. Ранг текущего узла = min(ранг_левого_потомка, ранг_правого_потомка) + 1
// Когда дойдем до корня и установим для него свойство левизны - будет соблюдаться свойство левизны
// Сложность очевидно следующая:
// Высота дерева не превышает logn. Следовательно, т.к. мы объединили правые пути (макс. длины logn1 и logn2) исходных куч, то в получившемся дереве высота logn1 + logn2
// Так как двигаемся по этому пути до корня, а операция обмена деревьями и повышения ранга выполняются за константу, то сложность СЛИЯНИЯ O(logn1 + logn2) = O(logn)
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

// В этой функции удаляем минимум, но перед этим сохраняем его данные (номер вершины в переменной appropriate_vertex и его ключ в key_of_minimum)
// Очевидно, согласно свойствам левостороннего дерева, минимум будет в корне
// Когда минимум удалился, требуется слить левые и правые подкучи
// Т.е. удаление самого корня выполняется за константу, а слияние O(logn), т.е. общая сложность O(logn)
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

// Получить число узлов
template <class T>
int LeftHeap<T>::GetNumOfNodes()
{
    return num_of_nodes;
}

// Уменьшить у узла, содержащий элемент name, ключ на delta единиц
// Алгоритм следующий:
// Отделяем кучу P_2 с корнем в узле, содержащим name, от основной кучи. Теперь основная куча не обязатально левостороняя, а отделенная - левосторонняя
// Восстанавливаем свойство левизны: идем вверх по пути от отца узла, содержащего name, к корню основную кучи
// После того, как восстановили св-во левизны, объединяем основную и отделенную кучи
// Выполняется очевидно за O(logn) (предполагаемая длина пути logn и СЛИЯНИЕ logn). В худшем случае за O(n), т.к. дерево может быть представлено в виде одного пути.
template <class T>
void LeftHeap<T>::DecreaseKey(T name, int delta)
{
    NodeOfLeftHeap<T>* P = KeepOfNodes[name]; // узел, который будет содержать элемент name
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