#ifndef _GRAPH_
#define _GRAPH_

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#define SATNUM 4  //���ǽڵ���

#include "reduction.h"

using namespace std;

void readAccess(string filename1, string filename2, vector<vector<int>>& accessArray, vector<vector<int>>& costArray);
void readAccess(string filename1, vector<vector<int>>& accessArray, vector<vector<int>>& costArray);
void displayAccess(vector<vector<int>> accessArray);


//******************��Ȩ������ͼ���ڽӱ�*******************

//�߽ڵ�
struct edge_vertex{
	int Evertex;
	double cost;			//��Ȩ��
	edge_vertex *next;
};

//ͷ���
struct head_vertex{
	int Hvertex;
	edge_vertex *first_edge;	//ָ���һ���߽ڵ�
};


//ͼ���ڽӱ�
struct Graph {
	vector<head_vertex *> headArray; //���б�ͷ��ɵ����� 
	int n, e;				//�������������� 
};


void createGraph(Graph* &G, vector<vector<int>>& accessArray, vector<vector<int>>& costArray);
void displayGraph(Graph* G);

//vector<int> reach(SATNUM, 0);
//void DFS(Graph* G, int v);

#endif