#pragma once
#include <iostream>
#include <ctime>
#include <list>
#include <algorithm>

using namespace std;

// Класс ребро графа (если быть точным, то эта структура характеризует вершину j, которая смежна с вершиной i, являющаяся индексом массива adjacency_list)
// Также, так как известны i и j, то известно ребро (i, j), а значит мы в этой структуре можем задать вес ребра (i, j)
class Edge
{
public:
    int vertB;// Номер смежной вершины
    int weight;// Вес ребра
};


// Для ф-ции EdgeIsExist (приходится так делать из-за ограниченной функциональности MVS2008)
int vertexB = 0;
// Предикат для функции EdgeIsExist
bool Predicat(Edge current_edge)
{
    return (current_edge.vertB == vertexB);
}

// Класс ориентированный граф без петель

/*
Хранить будем в виде списков смежности:
Создается массив, индексы которого ассоциируются с номерами вершин графа. Массив содержит в себе списки, а списки содержат ребра, инцидентные соответствующей вершине,
указанной в индексе массива
*/
class Graph
{
private:
    int num_of_vertex;
    int num_of_edge;
    int max_num_of_edge;
    int lower_limit_of_weight; // Нижняя граница весов ребер графа
    int upper_limit_of_weight;// Верхняя граница весов ребер графа
    double pr_of_occurrence_edge; // Вероятность возникновения ребра (для организации равномерного распределения)

    int curr_vertex;// Для итератора - текущая рассматриваемая вершина в списке adjacency_list
    Edge curr_edge;// Для итератора - текущее рассматриваемое ребро
    list<Edge>::iterator iteratorForBypass;// Итератор для обхода

    list<Edge>* adjacency_list;// Список смежности
public:
    Graph(int _num_of_vertex, int _num_of_edge, int _lower_limit_of_weight, int _upper_limit_of_weight);
    Graph();
    Graph(Graph& CopyGraph);
    ~Graph();
    void AddEdge(int _vertA, int _vertB, int _weight);
    void AddVertex(int _vert);
    bool EdgeIsExist(int _vertA, int _vertB);
    void ShowGraph();

    /* Итератор */
    void Reset();
    bool IsEnd();
    void GoNext();

    /* Методы доступа к приватным переменным */
    int GetCurrVertex();
    Edge GetCurrEdge();
    int GetNumOfVertex();
    int GetNumOfEdge();
    int GetMaxNumOfEdge();
    int GetLowerLimitOfWeight();
    int GetUpperLimitOfWeight();
    double GetPrOfOccurrenceEdge();
    void SetWeightOfEdge(int _vertA, int _vertB, int _weight);

    bool IsEmpty();

    list<Edge>& operator[](int pos);
    Graph& operator=(Graph& Graph_2);
};

// Конструктор - создание графа с заданным числом ребер и произвольными весами, удовлетворяющие неравенству:
// lower_limit_of_weight <= <Вес любого ребра> <= upper_limit_of_weight
Graph::Graph(int _num_of_vertex, int _num_of_edge, int _lower_limit_of_weight, int _upper_limit_of_weight)
{
    if(_num_of_vertex > 0 && _num_of_edge > 0 && _lower_limit_of_weight <= _upper_limit_of_weight)
    {
        int** adjacency_matrix;

        num_of_vertex = _num_of_vertex;
        num_of_edge = 0;
        max_num_of_edge = num_of_vertex * (num_of_vertex - 1);
        lower_limit_of_weight = _lower_limit_of_weight;
        upper_limit_of_weight = _upper_limit_of_weight;



        adjacency_list = new list<Edge>[num_of_vertex];
        adjacency_matrix = new int*[num_of_vertex];
        for(int i = 0; i < num_of_vertex; i++)
            adjacency_matrix[i] = new int[num_of_vertex];

        for(int i = 0; i < num_of_vertex; i++)
            for(int j = 0; j < num_of_vertex; j++)
                adjacency_matrix[i][j] = 0;


        curr_vertex = 0;

        //srand((unsigned int)(time(NULL)));

        // Если пользователь ввел число ребер большее, чем возможно, то...
        if(_num_of_edge > max_num_of_edge)
            _num_of_edge = max_num_of_edge;

        if(_num_of_vertex > 1)
            pr_of_occurrence_edge = (double)(_num_of_edge) / (double)(_num_of_vertex * (_num_of_vertex - 1));
        else
            pr_of_occurrence_edge = 100.0;

        if(_num_of_edge == max_num_of_edge)
            pr_of_occurrence_edge = 100.0;

        // Начинаем создавать граф <==> создавать список смежности
        double rand_value = 0.0; // Для генерации ребер 
        int rand_weight = 0; // Случайный вес ребра

        while(num_of_edge < _num_of_edge)
            for(int i = 0; i < num_of_vertex; i++)
                for(int j = 0; j < num_of_vertex; j++)
                    if(i != j && adjacency_matrix[i][j] == 0 && num_of_edge < _num_of_edge)
                    {
                        rand_value = (double)(rand()) / RAND_MAX;
                        if(rand_value >= 1.0 - pr_of_occurrence_edge)
                        {
                            if(upper_limit_of_weight != lower_limit_of_weight)
                                rand_weight = rand() % (upper_limit_of_weight - lower_limit_of_weight) + lower_limit_of_weight;// Генерация веса ребра на заданном отрезке весов
                            else
                                rand_weight = upper_limit_of_weight;
                            adjacency_matrix[i][j] = 1;
                            AddEdge(i, j, rand_weight);
                        }
                    }
    }
    else // Иначе ничего не делаем
    {
        cout << "Incorrect data, try again" << endl;
        adjacency_list = NULL;
    }
}

// Конструктор по умолчанию
Graph::Graph()
{
    adjacency_list = NULL;
    num_of_vertex = 0;
    num_of_edge = 0;
    max_num_of_edge = 0;
}

// Конструктор копирования
Graph::Graph(Graph& CopyGraph)
{
    adjacency_list = new list<Edge>[CopyGraph.GetNumOfVertex()];
    num_of_vertex = CopyGraph.GetNumOfVertex();
    num_of_edge = 0;
    max_num_of_edge = CopyGraph.GetMaxNumOfEdge();
    lower_limit_of_weight = CopyGraph.GetLowerLimitOfWeight();
    upper_limit_of_weight = CopyGraph.GetUpperLimitOfWeight();
    pr_of_occurrence_edge = CopyGraph.GetPrOfOccurrenceEdge();
    curr_edge.vertB = 0;
    curr_edge.weight = lower_limit_of_weight;
    curr_vertex = 0;


    for(CopyGraph.Reset(); !CopyGraph.IsEnd(); CopyGraph.GoNext())
        AddEdge(CopyGraph.GetCurrVertex(), CopyGraph.GetCurrEdge().vertB, CopyGraph.GetCurrEdge().weight);


}
// Деструктор
Graph::~Graph()
{
    if(adjacency_list != NULL)
        delete[] adjacency_list;
}

// Добавить ребро
void Graph::AddEdge(int _vertA, int _vertB, int _weight)
{
    if(_vertA >= 0 && _vertA < num_of_vertex && _vertB >= 0 && _vertB < num_of_vertex)
    {
        Edge new_edge;

        new_edge.vertB = _vertB;
        new_edge.weight = _weight;

        // Добавляем ребро в соответсвующий список
        adjacency_list[_vertA].push_back(new_edge);

        num_of_edge++;
    }
}

// Добавить вершину
void Graph::AddVertex(int _vert)
{
    if(_vert > num_of_vertex) // Если вершина уникальна, то ...
    {
        list<Edge>* new_arr = new list<Edge>[_vert];

        for(Reset(); !IsEnd(); GoNext())
            new_arr[curr_vertex].push_back(curr_edge); 

        delete [] adjacency_list;

        adjacency_list = new_arr;

        num_of_vertex++;
    }
}

// Существует ли ребро?
bool Graph::EdgeIsExist(int _vertA, int _vertB)
{
    vertexB = _vertB;
    list<Edge>::iterator findIter = find_if(adjacency_list[_vertA].begin(), adjacency_list[_vertA].end(), &Predicat);
    if(findIter != adjacency_list[_vertA].end())
        return true;
    return false;
}

// Показать граф (список смежностей) в консоли
void Graph::ShowGraph()
{
    if(adjacency_list != NULL)
    {
        Edge current_edge;
        cout << "Adjacency list:" << endl;
        cout << endl;
        for(int i = 0; i < num_of_vertex; i++)
        {
            list<Edge>::iterator Iter = adjacency_list[i].begin();
            cout << i << ": ";

            while(Iter != adjacency_list[i].end())
            {
                current_edge = *Iter;
                cout << "(" << i << ", " << current_edge.vertB<<", "<< current_edge.weight <<") ";
                Iter++;
            }

            cout << endl;
        }
    }
}

// Установить начало обхода
void Graph::Reset()
{
    curr_vertex = 0;
    iteratorForBypass = adjacency_list[curr_vertex].begin();

    while(iteratorForBypass == adjacency_list[curr_vertex].end() && curr_vertex < num_of_vertex - 1)
    {
        curr_vertex++;
        iteratorForBypass = adjacency_list[curr_vertex].begin();
    }

    if(iteratorForBypass != adjacency_list[curr_vertex].end())
        curr_edge = *iteratorForBypass;
}

// Определение конца обхода
bool Graph::IsEnd()
{
    return ((curr_vertex == num_of_vertex - 1) && (iteratorForBypass == adjacency_list[curr_vertex].end()));
}

// Переместиться дальше
void Graph::GoNext()
{
    if(iteratorForBypass != adjacency_list[curr_vertex].end())
        iteratorForBypass++;
    if(iteratorForBypass != adjacency_list[curr_vertex].end())
        curr_edge = *iteratorForBypass;
    else
    {
        while(iteratorForBypass == adjacency_list[curr_vertex].end() && curr_vertex < num_of_vertex - 1)
        {
            curr_vertex++;
            iteratorForBypass = adjacency_list[curr_vertex].begin();
        }

        if(iteratorForBypass != adjacency_list[curr_vertex].end())
            curr_edge = *iteratorForBypass;
    }
}

// Получить текущую вершину
int Graph::GetCurrVertex()
{
    return curr_vertex;
}

// Получить текущее ребро
Edge Graph::GetCurrEdge()
{
    return curr_edge;
}

// Получить число вершин в графе
int Graph::GetNumOfVertex()
{
    return num_of_vertex;
}

// Получить число ребер в графе
int Graph::GetNumOfEdge()
{
    return num_of_edge;
}

// Получить максимально возможное число ребер в графе
int Graph::GetMaxNumOfEdge()
{
    return max_num_of_edge;
}

// Получить нижнюю границу весов ребер в графе
int Graph::GetLowerLimitOfWeight()
{
    return lower_limit_of_weight;
}

// Получить верхнюю границу весов ребер в графе
int Graph::GetUpperLimitOfWeight()
{
    return upper_limit_of_weight;
}

// Получить вероятность возникновения ребра в графе
double Graph::GetPrOfOccurrenceEdge()
{
    return pr_of_occurrence_edge;
}

// Изменить вес текущего ребра (по умолчанию, если вес не удовлетворяет требованиям, устанавливается среднее арифметическое между верхней и нижней границами весов)
void Graph::SetWeightOfEdge(int _vertA, int _vertB, int _weight)
{
    if(_weight < lower_limit_of_weight || _weight > upper_limit_of_weight)
        _weight = (upper_limit_of_weight + upper_limit_of_weight) / 2;

    vertexB = _vertB; // Для предиката
    list<Edge>::iterator findIter = find_if(adjacency_list[_vertA].begin(), adjacency_list[_vertA].end(), &Predicat);

    if(findIter != adjacency_list[_vertA].end())
        findIter._Ptr->_Myval.weight = _weight;
}

// Граф пустой?
bool Graph::IsEmpty()
{
    return adjacency_list == NULL ? true : false;
}

// Перегрузка оператора индексации
list<Edge>& Graph::operator [](int pos)
{
    return adjacency_list[pos];
}

// Перегрузка оператора присваивания
Graph& Graph::operator =(Graph &Graph_2)
{
    if(!Graph_2.IsEmpty())
    {

        adjacency_list = new list<Edge>[Graph_2.GetNumOfVertex()];
        num_of_vertex = Graph_2.GetNumOfVertex();
        max_num_of_edge = Graph_2.GetMaxNumOfEdge();
        lower_limit_of_weight = Graph_2.GetLowerLimitOfWeight();
        upper_limit_of_weight = Graph_2.GetUpperLimitOfWeight();
        pr_of_occurrence_edge = Graph_2.GetPrOfOccurrenceEdge();

        for(Graph_2.Reset(); !Graph_2.IsEnd(); Graph_2.GoNext())
            AddEdge(Graph_2.GetCurrVertex(), Graph_2.GetCurrEdge().vertB, Graph_2.GetCurrEdge().weight);

    }

    return *this;
}