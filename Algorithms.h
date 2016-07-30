#pragma once
#include <iostream>
#include "Graph.h"
#include "LeftHeap.h"
#include "DHeap.h"
#include "SelfOrganizationHeap.h"

using namespace std;

#define MAX_DIST 2137483646
#define MAX_WEIGHT 2137483646

// �������� ��������-����� ��������� �� ������ ����� ���������� ���� �� ����� ������� �� ���� ���������, �� � ������� ������� ������ �������������� ����
bool Bellman_Ford(int* dist, Graph ADJ, int num_vert, int curr_vert)
{
    int curr_name;
    for(int i = 0; i < num_vert; i++)
        dist[i] = MAX_DIST;

    dist[curr_vert] = 0;

    for(int i = 0; i < num_vert; i++)// ���������� �� num_vert - 1 ������� �������� �����, � num_vert, ����� ���������� ������� �������������� ����� (��� ��� ����������)
        for(int j = 0; j < num_vert; j++)
        {
            list<Edge>::iterator Iter = ADJ[j].begin();

            while(Iter != ADJ[j].end())
            {
                curr_name = Iter._Ptr->_Myval.vertB;
                if((curr_vert != curr_name) && (dist[curr_name] > dist[j] + Iter._Ptr->_Myval.weight))
                {
                    dist[curr_name] = dist[j] + Iter._Ptr->_Myval.weight;
                    if(i == num_vert - 1) // ��� ��� �� ����� � ��� ����� if (������� ����) � ��� ��������� ������� ��������, �� ����� ����������� ���� �� �����-���� �������
                        // ������ ����������. ��� ��������������� � ������� �������������� �����
                        return false;
                }

                Iter++;
            }
        }
        return true;
}

// �������� �� ������������� ����
void Dijkstra_LeftHeap(int* dist, Graph ADJ, int num_vert, int curr_vert)
{
    int* key;
    int* name;

    key = new int[num_vert];
    name = new int[num_vert];

    for(int i = 0; i < num_vert; i++)
    {
        dist[i] = MAX_DIST;
        key[i] = MAX_WEIGHT;
        name[i] = i;
    }

    key[curr_vert] = 0;
    LeftHeap<int> LH(name, key, num_vert);
    while(LH.GetNumOfNodes() > 0)
    {
        int curr_name_of_min;
        int curr_key_of_min;

        LH.RemoveMinimum(curr_name_of_min, curr_key_of_min);

        dist[curr_name_of_min] = curr_key_of_min;

        list<Edge>::iterator Iter = ADJ[curr_name_of_min].begin();

        while(Iter != ADJ[curr_name_of_min].end())
        {
            if(key[Iter._Ptr->_Myval.vertB] > dist[curr_name_of_min] + Iter._Ptr->_Myval.weight)
            {
                int delta = key[Iter._Ptr->_Myval.vertB] - (dist[curr_name_of_min] + Iter._Ptr->_Myval.weight);
                key[Iter._Ptr->_Myval.vertB] = dist[curr_name_of_min] + Iter._Ptr->_Myval.weight;
                LH.DecreaseKey(Iter._Ptr->_Myval.vertB, delta);// ��������, ��� �� ������ ����� ��������� ���� ��-�� ������� � if'�
            }

            Iter++;
        }

    }
}

// �������� �� d-����
void Dijkstra_DHeap(int* dist, Graph ADJ, int num_vert, int curr_vert, int _d)
{
    int* key;
    int* name;
    int* position;

    key = new int[num_vert];
    name = new int[num_vert];
    position = new int[num_vert];

    for(int i = 0; i < num_vert; i++)
    {
        dist[i] = MAX_DIST;
        key[i] = MAX_WEIGHT;
        name[i] = i;
        position[i] = i;
    }

    key[curr_vert] = 0;
    position[curr_vert] = 0;
    DHeap<int> DH(name, key, position, num_vert, _d);
    while(DH.GetNumOfNodes() > 0)
    {
        int curr_name_of_min;
        int curr_key_of_min;

        DH.DeleteMin(position, curr_name_of_min, curr_key_of_min);

        dist[curr_name_of_min] = curr_key_of_min;

        list<Edge>::iterator Iter = ADJ[curr_name_of_min].begin();

        while(Iter != ADJ[curr_name_of_min].end())
        {
            int pos_of_vert_in_heap = position[Iter._Ptr->_Myval.vertB];
            if(key[Iter._Ptr->_Myval.vertB] > (dist[curr_name_of_min] + Iter._Ptr->_Myval.weight))
            {
                int delta = key[Iter._Ptr->_Myval.vertB] - (dist[curr_name_of_min] + Iter._Ptr->_Myval.weight);
                key[Iter._Ptr->_Myval.vertB] = dist[curr_name_of_min] + Iter._Ptr->_Myval.weight;
                DH.DecreaseKey(position, pos_of_vert_in_heap, delta);
            }

            Iter++;
        }

    }
}

// �������� �� ������������������ ����
void Dijkstra_SelfOrganizationHeap(int* dist, Graph ADJ, int num_vert, int curr_vert)
{
    int* key;
    int* name;

    key = new int[num_vert];
    name = new int[num_vert];

    for(int i = 0; i < num_vert; i++)
    {
        dist[i] = MAX_DIST;
        key[i] = MAX_WEIGHT;
        name[i] = i;
    }

    key[curr_vert] = 0;
    SelforganizationHeap<int> SH(name, key, num_vert);
    while(SH.GetNumOfNodes() > 0)
    {
        int curr_name_of_min;
        int curr_key_of_min;

        SH.RemoveMinimum(curr_name_of_min, curr_key_of_min);

        dist[curr_name_of_min] = curr_key_of_min;

        list<Edge>::iterator Iter = ADJ[curr_name_of_min].begin();

        while(Iter != ADJ[curr_name_of_min].end())
        {
            if(key[Iter._Ptr->_Myval.vertB] > dist[curr_name_of_min] + Iter._Ptr->_Myval.weight)
            {
                int delta = key[Iter._Ptr->_Myval.vertB] - (dist[curr_name_of_min] + Iter._Ptr->_Myval.weight);
                key[Iter._Ptr->_Myval.vertB] = dist[curr_name_of_min] + Iter._Ptr->_Myval.weight;
                SH.DecreaseKey(Iter._Ptr->_Myval.vertB, delta);// ��������, ��� �� ������ ����� ��������� ���� ��-�� ������� � if'�
            }

            Iter++;
        }

    }
}