#include "edge.h"

//
double Edge::getSlack(){
	double slack1 = this->getV1()->getVertexCharge() + this->getV2()->getVertexCharge() - this->getWeight();
	//求出所有包含这条边的花的权和
	double slack2 = 0;
	Blossom *out1 = this->getV1()->getOutermostBlossom();
	Blossom *out2 = this->getV2()->getOutermostBlossom();
	while (out1 == out2){
		slack2 += out1->getBlossomCharge();
		out1 = out1->getInnerBlossomContaining(v1);
		out2 = out2->getInnerBlossomContaining(v2);
	}

	double slack = slack1 + slack2;
	return slack;

}


//判断该边属于哪种类型
int Edge::judge(){
	Blossom *out1 = this->v1->getOutermostBlossom();
	Blossom *root1 = out1->getRootBlossom();

	Blossom *out2 = this->v2->getOutermostBlossom();
	Blossom *root2 = out2->getRootBlossom();

	//在花中的边成为dead edge,不再考虑
	if (out1 == out2)
		return 0;

	//边的两个单节点是同一颗树上偶节点
	if (root1 == root2&&out1->getLabel() == EVEN&&out2->getLabel() == EVEN)
		return Even_Even_InSame;
	//边的两个单节点是不同树上偶节点
	if (root1 != root2&&out1->getLabel() == EVEN&&out2->getLabel() == EVEN)
		return Even_Even_NotInSame;
	//边的两个单节点在树上的偶节点与还不在树中的节点
	if ((out1->getLabel() == EVEN&&out2->getLabel() == NOT_IN_TREE) || (out2->getLabel() == EVEN&&out1->getLabel() == NOT_IN_TREE))
		return Even_Nontree;
	else
		return 0;
}

//判断该边是否是tight边
bool Edge::isTight(){
	return (0 == this->getSlack());
}

//将边的匹配状态反转一下
void Edge::flip(){
	this->matched = !matched;
	//涉及到反转边两边的节点从未匹配点变为匹配点，或者从匹配点还是变为匹配点（匹配边有变化）
	this->getV1()->setMatched(true);
	this->getV2()->setMatched(true);
}

