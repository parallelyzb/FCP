#include "edge.h"

//
double Edge::getSlack(){
	double slack1 = this->getV1()->getVertexCharge() + this->getV2()->getVertexCharge() - this->getWeight();
	//������а��������ߵĻ���Ȩ��
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


//�жϸñ�������������
int Edge::judge(){
	Blossom *out1 = this->v1->getOutermostBlossom();
	Blossom *root1 = out1->getRootBlossom();

	Blossom *out2 = this->v2->getOutermostBlossom();
	Blossom *root2 = out2->getRootBlossom();

	//�ڻ��еı߳�Ϊdead edge,���ٿ���
	if (out1 == out2)
		return 0;

	//�ߵ��������ڵ���ͬһ������ż�ڵ�
	if (root1 == root2&&out1->getLabel() == EVEN&&out2->getLabel() == EVEN)
		return Even_Even_InSame;
	//�ߵ��������ڵ��ǲ�ͬ����ż�ڵ�
	if (root1 != root2&&out1->getLabel() == EVEN&&out2->getLabel() == EVEN)
		return Even_Even_NotInSame;
	//�ߵ��������ڵ������ϵ�ż�ڵ��뻹�������еĽڵ�
	if ((out1->getLabel() == EVEN&&out2->getLabel() == NOT_IN_TREE) || (out2->getLabel() == EVEN&&out1->getLabel() == NOT_IN_TREE))
		return Even_Nontree;
	else
		return 0;
}

//�жϸñ��Ƿ���tight��
bool Edge::isTight(){
	return (0 == this->getSlack());
}

//���ߵ�ƥ��״̬��תһ��
void Edge::flip(){
	this->matched = !matched;
	//�漰����ת�����ߵĽڵ��δƥ����Ϊƥ��㣬���ߴ�ƥ��㻹�Ǳ�Ϊƥ��㣨ƥ����б仯��
	this->getV1()->setMatched(true);
	this->getV2()->setMatched(true);
}

