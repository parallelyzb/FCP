#ifndef _EDGE_H_
#define _EDGE_H_

#define Even_Even_InSame		1
#define Even_Even_NotInSame		2
#define Even_Nontree			3


#include "vertex.h"



class Edge{
public:
	Edge(Vertex *_v1, Vertex *_v2, double _weight){
		this->v1 = _v1;
		this->v2 = _v2;
		this->weight = _weight;
		this->matched = false;
	}

	Vertex* getV1(){ return this->v1; }
	Vertex* getV2(){ return this->v2; }
	double getWeight(){ return this->weight; }
	void adjustWeight(double adjust){ this->weight += adjust; }

	bool isMatched(){ return matched; }
	double getSlack();
	bool isTight();	
	int judge();
	void flip();

private:
	Vertex *v1;
	Vertex *v2;
	double weight;

	bool matched;
	


};


#endif