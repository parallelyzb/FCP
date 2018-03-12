
#include "graph.h"

//将邻接矩阵表与权重表分别读取到二维容器中
void readAccess(string filename1, string filename2, vector<vector<int>>& accessArray, vector<vector<int>>& costArray){
	ifstream infile1(filename1, ifstream::in);
	ifstream infile2(filename2, ifstream::in);

	assert(infile1.is_open());
	assert(infile2.is_open());

	int access = 0;

	for (int i = 0; i < SATNUM; ++i){
		vector<int> temp;
		for (int j = 0; j < SATNUM; ++j){
			infile1 >> access;
			temp.push_back(access);
		}

		accessArray.push_back(temp);
	}

	int cost = 0;

	for (int i = 0; i < SATNUM; ++i){
		vector<int> temp;
		for (int j = 0; j < SATNUM; ++j){
			infile2 >> cost;
			temp.push_back(cost);
		}

		costArray.push_back(temp);
	}


	infile1.close();
	infile2.close();

	reduce(accessArray, costArray);
}

//cost表由access表生成，初始权重置为1
void readAccess(string filename1, vector<vector<int>>& accessArray, vector<vector<int>>& costArray){
	ifstream infile1(filename1, ifstream::in);
	assert(infile1.is_open());

	int access = 0;

	for (int i = 0; i < SATNUM; ++i){
		vector<int> temp;
		for (int j = 0; j < SATNUM; ++j){
			infile1 >> access;
			temp.push_back(access);
		}

		accessArray.push_back(temp);
	}

	for (int i = 0; i < SATNUM; ++i){
		vector<int> temp;
		for (int j = 0; j < SATNUM; ++j){
			if (accessArray[i][j] == 1)
				temp.push_back(1);
			else
				temp.push_back(0);
		}

		costArray.push_back(temp);
	}

	infile1.close();
	reduce(accessArray, costArray);
}
//输出邻接矩阵
void displayAccess(vector<vector<int>> accessArray){
	if (!accessArray.empty()){
		int num = accessArray[0].size();
		for (int i = 0; i < num; ++i){
			for (int j = 0; j < num; ++j){

				cout << accessArray[i][j];
			}
			cout << '\n';
		}
		cout << endl;
	}

	else{
		cout << "error: accessArray is empty! " << endl;
	}

}







//由 邻接矩阵 及 权重矩阵 生成邻接表
void createGraph(Graph* &G, vector<vector<int>>& accessArray, vector<vector<int>>& costArray){
	if (!accessArray.empty()){
		int num = accessArray[0].size();
		G = new Graph();
		G->n = num;
		G->e = 0;


		//头结点数组，节点编号从1开始
		for (int i = 1; i <= num; ++i){
			head_vertex *pHead = new head_vertex();
			pHead->Hvertex = i;
			pHead->first_edge = NULL;
			G->headArray.push_back(pHead);
		}


		//根据邻接矩阵输入边节点信息,当做有向图考虑
		for (int m = 0; m < num; ++m){
			for (int n = num - 1; n >= m; --n){
				if (accessArray[m][n] == 1){

					edge_vertex *pEdge = new edge_vertex();
					pEdge->Evertex = n + 1;
					pEdge->cost = costArray[m][n];

					//在头结点后插入边节点信息
					pEdge->next = G->headArray[m]->first_edge;
					G->headArray[m]->first_edge = pEdge;

					G->e++;
				}
				else{
					;
				}
			}
		}
		//G->e = (int)G->e / 2;	//无向图里边数被多统计了一倍

	}
	else{
		cout << "error: accessArray is empty! " << endl;
	}
}


//输出邻接表
void displayGraph(Graph* G){
	cout << "The number of total edges is " << G->e << '\n';
	for (int i = 0; i < G->n; ++i){
		cout << G->headArray[i]->Hvertex << "->";
		edge_vertex *p = G->headArray[i]->first_edge;
		while (p){
			cout << p->Evertex;
			cout << "->";
			p = p->next;
		}
		cout << "null\n";
	}
	cout << '\n';
}


////深度优先遍历DFS
//void DFS(Graph* G, int v){
//	reach[v - 1] = 1;	//从节点v能到达的节点置为1,节点编号从1开始
//	//cout << v << " ";
//	edge_vertex *p = G->headArray[v - 1]->first_edge;
//	while (p){
//		if (reach[p->Evertex - 1] == 0){
//			DFS(G, p->Evertex);
//		}
//		else{
//			;
//		}
//		p = p->next;
//	}
//}
