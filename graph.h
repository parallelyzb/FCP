#ifndef _GRAPH_
#define _GRAPH_

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#define SATNUM 4  //卫星节点数

#include "reduction.h"

using namespace std;

void readAccess(string filename1, string filename2, vector<vector<int>>& accessArray, vector<vector<int>>& costArray);
void readAccess(string filename1, vector<vector<int>>& accessArray, vector<vector<int>>& costArray);
void displayAccess(vector<vector<int>> accessArray);


//******************带权重无向图的邻接表*******************

//边节点
struct edge_vertex{
	int Evertex;
	double cost;			//边权重
	edge_vertex *next;
};

//头结点
struct head_vertex{
	int Hvertex;
	edge_vertex *first_edge;	//指向第一条边节点
};


//图的邻接表
struct Graph {
	vector<head_vertex *> headArray; //所有表头组成的数组 
	int n, e;				//结点个数，边条数 
};


void createGraph(Graph* &G, vector<vector<int>>& accessArray, vector<vector<int>>& costArray);
void displayGraph(Graph* G);

//vector<int> reach(SATNUM, 0);
//void DFS(Graph* G, int v);

#endif