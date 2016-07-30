#pragma once
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

template <class T>
class NodeOfSelforganizationHeap
{
public:
    T element; // Элемент, который хранится в этом узле
    int key;// ключ узла (вес)
    NodeOfSelforganizationHeap<T>* left_child;// левый потомок
    NodeOfSelforganizationHeap<T>* right_child;// правый потомок
    NodeOfSelforganizationHeap<T>* parent;// родитель узла

    NodeOfSelforganizationHeap();
    NodeOfSelforganizationHeap(T _elem, int _key);


    NodeOfSelforganizationHeap<T>& operator=(const NodeOfSelforganizationHeap<T>& Node);
    bool operator==(const NodeOfSelforganizationHeap<T>& Node);
};

// Конструктор по умолчанию
template <class T>
NodeOfSelforganizationHeap<T>::NodeOfSelforganizationHeap()
{
    left_child = NULL; right_child = NULL; parent = NULL;
}

// Конструктор
template <class T>
NodeOfSelforganizationHeap<T>::NodeOfSelforganizationHeap(T _elem, int _key)
{
    element = _elem; key = _key; left_child = NULL; right_child = NULL; parent = NULL;
}

// Перегрузка оператора присваивания
template <class T>
NodeOfSelforganizationHeap<T>& NodeOfSelforganizationHeap<T>::operator =(const NodeOfSelforganizationHeap<T>& Node)
{
    if(Node != NULL)
    {
        element = Node.element; key = Node.key; left_child = Node.left_child; right_child = Node.right_child; parent = Node.parent;
    }
}

// Перегрузка оператора сравнения
template <class T>
bool NodeOfSelforganizationHeap<T>::operator ==(const NodeOfSelforganizationHeap<T>& Node)
{
    return Node.element == element ? true : false; // В нашем случае элементы в куче будут уникальны
}


// Самоорганизующаяся куча
template <class T>
class SelforganizationHeap
{
private:
    int num_of_nodes;
    NodeOfSelforganizationHeap<T>* RootOfHeap;
    NodeOfSelforganizationHeap<T>** KeepOfNodes;// Массив адресов указателей. Необходимо для ф-ции DecreaseKey, чтобы избежать поиска узла в дереве (которое займет 0(n) времени и ухудшит алгоритм Дейкстры)
public:
    SelforganizationHeap(T* set_of_vertex, int* set_of_key, int num_of_vertex);
    ~SelforganizationHeap();

    NodeOfSelforganizationHeap<T>* MergeHeaps(NodeOfSelforganizationHeap<T>* S_1, NodeOfSelforganizationHeap<T>* S_2);
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
// Основываясь на высказывании, что левосторонняя куча может быть представлена просто указателем на корень (узел), то
// в этой функции мы будем создавать просто список узлов, соответсвующих одноэлементным кучам
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

// Деструктор
template <class T>
SelforganizationHeap<T>::~SelforganizationHeap()
{
}

// Слияние 2ух куч. 
// Алгоритм следующий:
// Есть кучи h1 и h2, сливаем их в h. Для этого сливаем правые пути исходных куч, соблюдая свойство кучеобразности
// Получившийся результирующий путь становится левым путем новой кучи, а левые поддеревья узлов, попавших в путь, становятся правыми поддеревьями
// Сложность O(n). Амортизационная оценка O(logn)
template <class T>
NodeOfSelforganizationHeap<T>* SelforganizationHeap<T>::MergeHeaps(NodeOfSelforganizationHeap<T>* S_1, NodeOfSelforganizationHeap<T>* S_2)
{
    NodeOfSelforganizationHeap<T>* S;// В итоге здесь будет хранится указатель на корень результирующей кучи

    if(S_1 == NULL)// Если один узел нулевой, то и объединять нечего, результатом будет узел S_2 очевидно
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

// В этой функции удаляем минимум, но перед этим сохраняем его данные (номер вершины в переменной appropriate_vertex и его ключ в key_of_minimum)
// Очевидно, согласно свойству кучеобразности, минимум будет в корне
// Когда минимум удалился, требуется слить левые и правые подкучи
// Т.е. удаление самого корня выполняется за константу, а слияние O(n) в худшем случае, а амортизационная O(logn)
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


// Уменьшить у узла, содержащий элемент name, ключ на delta единиц
// Алгоритм следующий:
// Отделяем кучу S_2 с корнем в узле, содержащим name, от основной кучи.
// Объединяем основную и отделенную кучи
// Выполняется очевидно в худшем случае за O(n) (т.к. СЛИЯНИЕ за O(n)). Амортизационная оценка O(logn)
template <class T>
void SelforganizationHeap<T>::DecreaseKey(T name, int delta)
{
    NodeOfSelforganizationHeap<T>* S = KeepOfNodes[name]; // узел, который будет содержать элемент name
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

// Получить число узлов
template <class T>
int SelforganizationHeap<T>::GetNumOfNodes()
{
    return num_of_nodes;
}