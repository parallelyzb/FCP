#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "blossom.h"

#define MAX_NUMBER 999

class Vertex:public Blossom{

public:
	Vertex(int _id) :Blossom(),id(_id), totalCharge(0),isMatched(false){};
	const int id;
	
	bool inMatchedEdge(){ return this->isMatched; }
	void setMatched(bool P){ this->isMatched = P; }

	double getVertexCharge(){ return(this->totalCharge); }

	//double getMaxNegativeCharge(){ return (INFINITE); }

	void updateVertexTotal(double delta){ totalCharge = totalCharge + delta; }
	void modifyVertexTotal(double delta){ this->totalCharge = delta; }
	Vertex *getStem(){ return this; }

	

private:
	double totalCharge;		//单节点（非花节点）的权重
	bool isMatched;		//节点是否已有匹配点
};



#endif