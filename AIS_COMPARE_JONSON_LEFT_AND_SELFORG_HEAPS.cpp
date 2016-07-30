// AIS_COMPARE_JONSON_LEFT_AND_SELFORG_HEAPS.cpp : main project file.

#include <iostream>
#include <omp.h>
#include "Graph.h"
#include "Algorithms.h"

using namespace std;

// Алгоритм Джонсона с использованием алгоритма Дейкстры на левосторонней куче
void Djonson_LeftHeap(int** dist, Graph ADJ, int num_vert)
{
    Graph AdditionalGraph = ADJ;

    AdditionalGraph.AddVertex(ADJ.GetNumOfVertex() + 1);

    int* dist_Bellman_Ford = new int[AdditionalGraph.GetNumOfVertex()];// Он будет содержать значения не больше нуля

    for(int i = 0; i < AdditionalGraph.GetNumOfVertex() - 1; i++)
        AdditionalGraph.AddEdge(AdditionalGraph.GetNumOfVertex() - 1, i, 0);

    // Применяем Беллмана-Форда для графа с наличием отрицательных весов ребер
    if(Bellman_Ford(dist_Bellman_Ford, AdditionalGraph, AdditionalGraph.GetNumOfVertex(), AdditionalGraph.GetNumOfVertex() - 1))
    {
        for(ADJ.Reset(); !ADJ.IsEnd(); ADJ.GoNext())// Делаем неотрицательные веса для алгоритма Дейкстры
        {
            ADJ.SetWeightOfEdge(ADJ.GetCurrVertex(),
                ADJ.GetCurrEdge().vertB,
                ADJ.GetCurrEdge().weight + dist_Bellman_Ford[ADJ.GetCurrVertex()] - dist_Bellman_Ford[ADJ.GetCurrEdge().vertB]);
        }

        for(int i = 0; i < ADJ.GetNumOfVertex(); i++)
        {
            Dijkstra_LeftHeap(dist[i], ADJ, ADJ.GetNumOfVertex(), i);
            for(int j = 0; j < ADJ.GetNumOfVertex(); j++)
                dist[i][j] = dist[i][j] + dist_Bellman_Ford[j] - dist_Bellman_Ford[i];
        }

        for(ADJ.Reset(); !ADJ.IsEnd(); ADJ.GoNext())// Обратно меняем веса на предыдущие у графа ADJ
        {
            ADJ.SetWeightOfEdge(ADJ.GetCurrVertex(),
                ADJ.GetCurrEdge().vertB,
                ADJ.GetCurrEdge().weight + dist_Bellman_Ford[ADJ.GetCurrEdge().vertB ] - dist_Bellman_Ford[ADJ.GetCurrVertex()]);
        }
    }
    else
        cout << "Graph contains a cycle of negative weight" << endl;
}

// Алгоритм Джонсона с использованием алгоритма Дейкстры на d-куче (d = 2)
void Djonson_DHeap(int** dist, Graph ADJ, int num_vert, int _d)
{
    Graph AdditionalGraph = ADJ;

    AdditionalGraph.AddVertex(ADJ.GetNumOfVertex() + 1);

    int* dist_Bellman_Ford = new int[AdditionalGraph.GetNumOfVertex()];// Он будет содержать значения не больше нуля

    for(int i = 0; i < AdditionalGraph.GetNumOfVertex() - 1; i++)
        AdditionalGraph.AddEdge(AdditionalGraph.GetNumOfVertex() - 1, i, 0);

    // Применяем Беллмана-Форда для графа с наличием отрицательных весов ребер
    if(Bellman_Ford(dist_Bellman_Ford, AdditionalGraph, AdditionalGraph.GetNumOfVertex(), AdditionalGraph.GetNumOfVertex() - 1))
    {
        for(ADJ.Reset(); !ADJ.IsEnd(); ADJ.GoNext())// Делаем неотрицательные веса для алгоритма Дейкстры
        {
            ADJ.SetWeightOfEdge(ADJ.GetCurrVertex(),
                ADJ.GetCurrEdge().vertB,
                ADJ.GetCurrEdge().weight + dist_Bellman_Ford[ADJ.GetCurrVertex()] - dist_Bellman_Ford[ADJ.GetCurrEdge().vertB]);
        }

        for(int i = 0; i < ADJ.GetNumOfVertex(); i++)
        {
            Dijkstra_DHeap(dist[i], ADJ, ADJ.GetNumOfVertex(), i, _d);
            for(int j = 0; j < ADJ.GetNumOfVertex(); j++)
                dist[i][j] = dist[i][j] + dist_Bellman_Ford[j] - dist_Bellman_Ford[i];
        }

        for(ADJ.Reset(); !ADJ.IsEnd(); ADJ.GoNext())// Обратно меняем веса на предыдущие у графа ADJ
        {
            ADJ.SetWeightOfEdge(ADJ.GetCurrVertex(),
                ADJ.GetCurrEdge().vertB,
                ADJ.GetCurrEdge().weight + dist_Bellman_Ford[ADJ.GetCurrEdge().vertB ] - dist_Bellman_Ford[ADJ.GetCurrVertex()]);
        }
    }
    else
        cout << "Graph contains a cycle of negative weight" << endl;
}

// Алгоритм Джонсона с использованием алгоритма Дейкстры на самоорганизующейся куче
void Djonson_SelfOrganizationHeap(int** dist, Graph ADJ, int num_vert)
{
    Graph AdditionalGraph = ADJ;

    AdditionalGraph.AddVertex(ADJ.GetNumOfVertex() + 1);

    int* dist_Bellman_Ford = new int[AdditionalGraph.GetNumOfVertex()];// Он будет содержать значения не больше нуля

    for(int i = 0; i < AdditionalGraph.GetNumOfVertex() - 1; i++)
        AdditionalGraph.AddEdge(AdditionalGraph.GetNumOfVertex() - 1, i, 0);

    // Применяем Беллмана-Форда для графа с наличием отрицательных весов ребер
    if(Bellman_Ford(dist_Bellman_Ford, AdditionalGraph, AdditionalGraph.GetNumOfVertex(), AdditionalGraph.GetNumOfVertex() - 1))
    {
        for(ADJ.Reset(); !ADJ.IsEnd(); ADJ.GoNext())// Делаем неотрицательные веса для алгоритма Дейкстры
        {
            ADJ.SetWeightOfEdge(ADJ.GetCurrVertex(),
                ADJ.GetCurrEdge().vertB,
                ADJ.GetCurrEdge().weight + dist_Bellman_Ford[ADJ.GetCurrVertex()] - dist_Bellman_Ford[ADJ.GetCurrEdge().vertB]);
        }

        for(int i = 0; i < ADJ.GetNumOfVertex(); i++)
        {
            Dijkstra_SelfOrganizationHeap(dist[i], ADJ, ADJ.GetNumOfVertex(), i);
            for(int j = 0; j < ADJ.GetNumOfVertex(); j++)
                dist[i][j] = dist[i][j] + dist_Bellman_Ford[j] - dist_Bellman_Ford[i];
        }

        for(ADJ.Reset(); !ADJ.IsEnd(); ADJ.GoNext())// Обратно меняем веса на предыдущие у графа ADJ
        {
            ADJ.SetWeightOfEdge(ADJ.GetCurrVertex(),
                ADJ.GetCurrEdge().vertB,
                ADJ.GetCurrEdge().weight + dist_Bellman_Ford[ADJ.GetCurrEdge().vertB ] - dist_Bellman_Ford[ADJ.GetCurrVertex()]);
        }
    }
    else
        cout << "Graph contains a cycle of negative weight" << endl;
}

void main()
{
    Graph test;
    int num_of_vert, num_of_edge, low_lim_weight, up_lim_weight;	

    double work_time_DJ_LeftHeap = 0.0;
    double start_time_DJ_LeftHeap = 0.0;
    double end_time_DJ_LeftHeap = 0.0;

    double work_time_DJ_DHeap = 0.0;
    double start_time_DJ_DHeap = 0.0;
    double end_time_DJ_DHeap = 0.0;

    double work_time_DJ_SelforganizationHeap = 0.0;
    double start_time_DJ_SelforganizationHeap = 0.0;
    double end_time_DJ_SelforganizationHeap = 0.0;

    bool Exit = false;
    int Option;

    while(!Exit)
    {
        cout << "Enter 1 to create a graph: " << endl;
        cout << "Enter 2 to show a graph: " << endl;
        cout << "Enter 3 to run the algorithm Johnson which uses Dijkstra's algorithm on the left heap: " << endl;
        cout << "Enter 4 to run the algorithm Johnson which uses Dijkstra's algorithm on the d-heap: " << endl;
        cout << "Enter 5 to run the algorithm Johnson which uses Dijkstra's algorithm on the selforganization heap: " << endl;
        cout << "Enter 6 to run the experiments: " << endl;
        cout << "Enter 7 to clear the output window: " << endl;
        cout << "Enter 0 to close this program: " << endl;
        cout << endl;

        cin >> Option;
        switch(Option)
        {
        case 1:
            {
                cout << "Input number of vertexs: " <<endl;
                cin >> num_of_vert;

                cout << "Input number of edges: " <<endl;
                cin >> num_of_edge;

                cout << "Input lower limit of weight: " <<endl;
                cin >> low_lim_weight;

                cout << "Input upper limit of weight: " <<endl;
                cin >> up_lim_weight;

                Graph test_1(num_of_vert, num_of_edge, low_lim_weight, up_lim_weight);

                test = test_1;

                break;
            }
        case 2:
            {
                if(!test.IsEmpty())
                    test.ShowGraph();
                else
                    cout << "Graph is empty! " << endl;

                cout << endl;

                break;
            }
        case 3:
            {
                if(!test.IsEmpty())
                {
                    int** table_of_dists;
                    int Show_table;

                    table_of_dists = new int*[test.GetNumOfVertex()];
                    for(int i = 0; i < test.GetNumOfVertex(); i++)
                        table_of_dists[i] = new int[test.GetNumOfVertex()];

                    start_time_DJ_LeftHeap = omp_get_wtime();
                    Djonson_LeftHeap(table_of_dists, test, test.GetNumOfVertex());
                    end_time_DJ_LeftHeap = omp_get_wtime();
                    work_time_DJ_LeftHeap = end_time_DJ_LeftHeap - start_time_DJ_LeftHeap;

                    cout << "Show table of shortest distances? (1 - yes, 0 - no)" << endl;
                    cin >> Show_table;
                    cout << endl;

                    if(Show_table)
                    {
                        for(int i = 0; i < test.GetNumOfVertex(); i++)
                        {
                            cout << i << ":";
                            for(int j = 0; j < test.GetNumOfVertex(); j++)
                                cout << " " <<table_of_dists[i][j];
                            cout << endl;

                        }
                    }

                    cout << endl;
                    // Выдать время работы

                    cout << "Algorithm of Djonson, which uses Dijkstra's algorithm on the left heap is worked: " << work_time_DJ_LeftHeap << "seconds" << endl;

                    cout << endl;

                }
                else
                {
                    cout << "Graph is empty! " << endl;
                    cout << endl;
                }

                break;
            }
        case 4:
            {
                if(!test.IsEmpty())
                {
                    int** table_of_dists;
                    int Show_table;
                    int _d;

                    table_of_dists = new int*[test.GetNumOfVertex()];
                    for(int i = 0; i < test.GetNumOfVertex(); i++)
                        table_of_dists[i] = new int[test.GetNumOfVertex()];

                    cout << "Input the value of d in d-heap:" << endl;
                    cin >> _d;

                    start_time_DJ_DHeap = omp_get_wtime();
                    Djonson_DHeap(table_of_dists, test, test.GetNumOfVertex(), _d);
                    end_time_DJ_DHeap = omp_get_wtime();
                    work_time_DJ_DHeap = end_time_DJ_DHeap - start_time_DJ_DHeap;

                    cout << "Show table of shortest distances? (1 - yes, 0 - no)" << endl;
                    cin >> Show_table;
                    cout << endl;

                    if(Show_table)
                    {
                        for(int i = 0; i < test.GetNumOfVertex(); i++)
                        {
                            cout << i << ":";
                            for(int j = 0; j < test.GetNumOfVertex(); j++)
                                cout << " " <<table_of_dists[i][j];
                            cout << endl;

                        }
                    }

                    cout << endl;
                    // Выдать время работы

                    cout << "Algorithm of Djonson, which uses Dijkstra's algorithm on the d-heap ( d = " << _d << ") is worked: " << work_time_DJ_DHeap << "seconds" << endl;

                    cout << endl;

                }
                else
                {
                    cout << "Graph is empty! " << endl;
                    cout << endl;
                }

                break;
            }
        case 5:
            {
                if(!test.IsEmpty())
                {
                    int** table_of_dists;
                    int Show_table;

                    table_of_dists = new int*[test.GetNumOfVertex()];
                    for(int i = 0; i < test.GetNumOfVertex(); i++)
                        table_of_dists[i] = new int[test.GetNumOfVertex()];

                    start_time_DJ_SelforganizationHeap = omp_get_wtime();
                    Djonson_SelfOrganizationHeap(table_of_dists, test, test.GetNumOfVertex());
                    end_time_DJ_SelforganizationHeap = omp_get_wtime();
                    work_time_DJ_SelforganizationHeap = end_time_DJ_SelforganizationHeap - start_time_DJ_SelforganizationHeap;

                    cout << "Show table of shortest distances? (1 - yes, 0 - no)" << endl;
                    cin >> Show_table;
                    cout << endl;

                    if(Show_table)
                    {
                        for(int i = 0; i < test.GetNumOfVertex(); i++)
                        {
                            cout << i << ":";
                            for(int j = 0; j < test.GetNumOfVertex(); j++)
                                cout << " " <<table_of_dists[i][j];
                            cout << endl;

                        }
                    }

                    cout << endl;
                    // Выдать время работы

                    cout << "Algorithm of Djonson, which uses Dijkstra's algorithm on the selforganization heap is worked: " << work_time_DJ_SelforganizationHeap << "seconds" << endl;

                    cout << endl;

                }
                else
                {
                    cout << "Graph is empty! " << endl;
                    cout << endl;
                }
                break;
            }
        case 6:
            {
                double start_time = 0.0;
                double end_time = 0.0;
                double time_work = 0.0;

                int n, m, min_weight = 1, max_weight = 1000000;


                // Тестирование алгоритмов при использовании полного графа. У алгоритмы Дейкстры используется d = 2

                for(int n = 1; n <= 501; n+=50)
                {
                    int _d = 2;
                    int** dists;
                    dists = new int*[n];
                    for(int i = 0; i < n; i++)
                        dists[i] = new int[n];

                    if(n > 1)
                        m = (n - 1) * n;
                    else
                        m = 1;

                    Graph test_graph(n, m, min_weight, max_weight);

                    start_time = omp_get_wtime();
                    Djonson_LeftHeap(dists, test_graph, test_graph.GetNumOfVertex());
                    end_time = omp_get_wtime();
                    time_work = end_time - start_time;

                    cout << "LeftHeap: " << time_work << endl;

                    start_time = omp_get_wtime();
                    Djonson_DHeap(dists, test_graph, test_graph.GetNumOfVertex(), _d);
                    end_time = omp_get_wtime();
                    time_work = end_time - start_time;

                    cout << "DHeap: " << time_work << endl;

                    start_time = omp_get_wtime();
                    Djonson_SelfOrganizationHeap(dists, test_graph, test_graph.GetNumOfVertex());
                    end_time = omp_get_wtime();
                    time_work = end_time - start_time;

                    cout << "SelfOrganizationHeap: " << time_work << endl;

                    cout << endl;

                }

                break;
            }
        case 7:
            {
                system("cls");
                break;
            }
        case 0:
            {
                Exit = true;
                break;
            }
        default: continue;
        }
    }
}