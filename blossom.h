#ifndef _BLOSSOM_H_
#define _BLOSSOM_H_


#include <memory>
#include <list>
#include <vector>

#define EVEN			-1
#define ODD				1
#define NOT_IN_TREE		0

class Vertex;
class Edge;

class Blossom{
public:
	Blossom();

	double getBlossomCharge(){ return charge_blossom; }
	int getLabel(){ return label; }
	void setLabel(int _label){ this->label = _label; }
	void updateBlossomCharge(double delta){ charge_blossom = charge_blossom + delta; }


	Blossom* getOutwardBlossom();
	Blossom* getOutermostBlossom();
	Blossom* getInnerBlossomContaining(Vertex *v);
	Blossom* getRootBlossom();
	virtual Vertex* getStem();
	

	Blossom* makeBlossomWith(Blossom *pair, Edge *pairEdge);
	void flipPathBetween(Vertex *v1, Vertex *v2);
	void arguementPath(Vertex *v);

	//树的信息
	Blossom*                treeParentBlossom;
	Edge*                   treeParentEdge;
	std::list<Blossom*>     treeChildren;
	

	//花中信息
	std::shared_ptr<Blossom>    outwardBlossom;
	std::vector<Blossom *>       innerChildren;
	Edge*                       nextBlossomEdge;
private:

	double charge_blossom;			//花的权重
	int label;						//缩花节点在树中的位置	
	//static int num;

	



	
};




#endif