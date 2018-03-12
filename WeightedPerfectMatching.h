#ifndef _WPM_
#define _WPM_

#include "edge.h"
#include "graph.h"


#include <vector>
#include <algorithm>

class WPM{
public:
	WPM(Graph* &G);

	void initial_charge();
	void initial_roots();


	int getNumVertices(){ return this->num_vertices; }
	int getMatchedEdges(){ return this->num_matchedEdges; }

	void removeRootFromForest(Blossom *root);
	void addRootToForest(Blossom *root);

	
	void increaseMatchedEdges() { num_matchedEdges += 1; }

	//调整权重需要调用的函数（dualChange）
	double getMinDelta();
	void dualChangeVertex(double delta);
	void dualChangeBlossom(double delta,Blossom *root);

	//展开花调用的函数
	void disConnectBlossom(Blossom *p);
	void disConnectTree(Blossom *root);
	//void growTree(int &flag);
	bool process(Edge *edge);



	bool find_perfectmatching();
	void displayResult();
	void exportResult(string exportFilename);
	void costAdjust_and_reInitial();

private:
	int num_vertices;
	int num_matchedEdges;

	std::vector<std::unique_ptr<Vertex>> vertices;
	std::vector<std::unique_ptr<Edge>> edges;
	std::list<Blossom*> roots;



	Vertex *getMatchedVertex(Vertex *pair);
	Edge *getMatchedEdgeBetween(Vertex *v1, Vertex *v2);
	//void find_tight(Blossom *parent, std::vector<Edge *>& tight_edges);
	void connectToTree(Blossom *parent, Vertex *v, Vertex *v_pair, Edge *parentEdge, Edge *pair);

	
	
};

#endif