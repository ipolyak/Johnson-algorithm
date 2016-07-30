#pragma once
#include <iostream>

using namespace std;

// Вычисление минимума
template <typename T>
T calc_minimum(const T val_1, const T val_2)
{
    return val_1 > val_2 ? val_2 : val_1;
}

template <class T>
class DHeap
{
private:
    int num_of_nodes;
    int d; // Показывает, сколько детей должен иметь узел
    T* name;
    int* key;
public:
    DHeap(T* set_of_vertex, int* set_of_key, int* position, int num_of_vertex, int _d);
    ~DHeap();

    void Emersion(int* position, int pos);
    void Diving(int* position, int pos);
    void Delete(int* posiiton, int pos);
    void DeleteMin(int* position, T& _name, int& _key);
    void DecreaseKey(int* position, int pos, int delta);
    int MinChild(int pos);

    // Функции доступа к переменным
    int GetNumOfNodes();
    int GetD();

    void Swap(int i, int j);

    void ShowDHeap();
};

// Конструктор. Образовать d кучу с n узлами (Окучивание) O(n)
template <class T>
DHeap<T>::DHeap(T* set_of_vertex, int* set_of_key, int* position, int num_of_vertex, int _d)
{
    name = new T[num_of_vertex];
    key = new int[num_of_vertex];
    num_of_nodes = num_of_vertex;
    if(_d > 0)
        d = _d;
    else
        d = 2;

    for(int i = 0; i < num_of_vertex; i++)
    {
        name[i] = set_of_vertex[i];
        key[i] = set_of_key[i];
    }

    for(int i = num_of_vertex - 1; i >= 0; i--)
        Diving(position, i);
}

// Деструктор
template <class T>
DHeap<T>::~DHeap()
{
    delete[] name;
    delete[] key;
}

// Всплытие (восстановление свойства кучеобразности от узла с позицией pos, двигаясь вверх) O(log(d)n)
template <class T>
void DHeap<T>::Emersion(int* position, int pos)
{
    int parent = (pos - 1) / d;

    while((pos != 0) && (key[parent] > key[pos]))
    {
        position[name[pos]] = parent;
        Swap(pos, parent);
        position[name[pos]] = pos;
        pos = parent;
        parent = (pos - 1) / d;
    }
}

// Погружение (восстановление свойства кучеобразности от узла с позицией pos, двигаясь вниз) O(d * log(d)n)
template <class T>
void DHeap<T>::Diving(int* position, int pos)
{
    int child = MinChild(pos);

    while((child != 0) && (key[child] < key[pos]))
    {
        position[name[pos]] = child;
        Swap(pos, child);
        position[name[pos]] = pos;
        pos = child;
        child = MinChild(child);
    }
}

// Удаление узла. Сложность O(d * log(d)n)
template <class T>
void DHeap<T>::Delete(int* position, int pos)
{
    position[name[pos]] = num_of_nodes - 1;
    name[pos] = name[num_of_nodes - 1];
    key[pos] = key[num_of_nodes - 1];
    position[name[pos]] = 0;
    num_of_nodes -= 1;
    if((pos != 0) && (key[pos] < key[(pos - 1) / d]))
        Emersion(position, pos);
    else
        Diving(position, pos);
}

// Удаление минимума 
template <class T>
void DHeap<T>::DeleteMin(int* position, T& _name, int& _key)
{
    _name = name[0];
    _key = key[0];
    Delete(position, 0);
}

// Уменьшить ключ 
template <class T>
void DHeap<T>::DecreaseKey(int* position, int pos, int delta)
{
    key[pos] -= delta;
    Emersion(position, pos);
}

// Поиск у узла с позицией pos узла с минимальным ключом
template <class T>
int DHeap<T>::MinChild(int pos)
{
    int minchild;

    if(pos * d + 1 >= num_of_nodes)
        minchild = 0;
    else
    {
        int firstchild = pos * d + 1;
        int lastchild = calc_minimum((pos + 1) * d, num_of_nodes - 1);
        int minkey = key[firstchild];
        minchild = firstchild;

        for(pos = firstchild + 1; pos <= lastchild; pos++)
            if(key[pos] < minkey)
            {
                minkey = key[pos];
                minchild = pos;
            }
    }

    return minchild;
}


// Получить количество вершин
template <class T>
int DHeap<T>::GetNumOfNodes()
{
    return num_of_nodes;
}

// Получить параметр d
template <class T>
int DHeap<T>::GetD()
{
    return d;
}

// Транспонирование элементов
template <class T>
void DHeap<T>::Swap(int i, int j)
{
    T tmp_1;
    int tmp_2;

    tmp_1 = name[i]; name[i] = name[j]; name[j] = tmp_1;
    tmp_2 = key[i]; key[i] = key[j]; key[j] = tmp_2;
}

// Показать d-кучу
template <class T>
void DHeap<T>::ShowDHeap()
{
    for(int i = 0; i < num_of_nodes; i++)
        cout << " ( "<<name[i] << ", " << key[i] << ")";

    cout << endl;
}