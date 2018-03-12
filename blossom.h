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

	//������Ϣ
	Blossom*                treeParentBlossom;
	Edge*                   treeParentEdge;
	std::list<Blossom*>     treeChildren;
	

	//������Ϣ
	std::shared_ptr<Blossom>    outwardBlossom;
	std::vector<Blossom *>       innerChildren;
	Edge*                       nextBlossomEdge;
private:

	double charge_blossom;			//����Ȩ��
	int label;						//�����ڵ������е�λ��	
	//static int num;

	



	
};




#endif