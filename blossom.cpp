
#include "edge.h"
#include <iostream>
Blossom::Blossom(){
	charge_blossom = 0;
	label = EVEN;		//�����ڵ�һ����ż�ڵ�
	treeParentBlossom = nullptr;
	treeParentEdge = nullptr;

	outwardBlossom = nullptr;
	//nextBlossomEdge = nullptr;
}

//�õ�����һ����,�Ѿ��������ڵ��Ƿ��� nullptr
Blossom* Blossom::getOutwardBlossom(){
	return outwardBlossom.get();
}
//�õ��ýڵ������Ļ�����surface blossom
Blossom* Blossom::getOutermostBlossom()
{
	Blossom* outermost = this;
	while (outermost->outwardBlossom.get() != nullptr)
		outermost = outermost->outwardBlossom.get();
	return outermost;
}

//�õ�����v���ڵĴ����Ļ�
Blossom* Blossom::getInnerBlossomContaining(Vertex *v)
{
	//Blossom *inner = (Blossom*)v;
	Blossom *inner = v;
	while (inner != nullptr && inner->outwardBlossom.get() != this)
		inner = inner->outwardBlossom.get();
	return inner;
}


//�õ���ǰ�ڵ��������ĸ��ڵ�
Blossom* Blossom::getRootBlossom()
{
	Blossom* root = this;
	while (root->treeParentBlossom != nullptr)
		root = root->treeParentBlossom;
	return root;

}


//�õ�������ײ�lca
Vertex* Blossom::getStem()
{
	return this->innerChildren[0]->getStem();
}



//pairEdge�������������е�EVEN������
Blossom* Blossom::makeBlossomWith(Blossom *pair, Edge *pairEdge){

	std::list<Blossom *> ourPath;	//���ŵ�ǰ���ڵ㵽���ڵ�ķ���·��
	std::list<Blossom *> pairPath;	//������һ�ڵ�����ڵ�ķ���·��

	ourPath.push_back(this);
	pairPath.push_back(pair);
	while (ourPath.back()->treeParentBlossom != nullptr)
		ourPath.push_back(ourPath.back()->treeParentBlossom);
	while (pairPath.back()->treeParentBlossom != nullptr)
		pairPath.push_back(pairPath.back()->treeParentBlossom);


	//�ҳ�lowest common ancestor�������У������ǴӸ��ڵ㿪ʼ�����һ������·�����еĻ��ڵ�
	Blossom *LCA = nullptr;
	while (ourPath.size() > 0 && pairPath.size() > 0 &&ourPath.back() == pairPath.back()){
		LCA = ourPath.back();
		ourPath.erase(std::prev(ourPath.end()));
		pairPath.erase(std::prev(pairPath.end()));
	}

	//�������������µ�Blossom
	std::shared_ptr<Blossom> newBlossom = std::make_shared<Blossom>();

	
	newBlossom->treeParentBlossom = LCA->treeParentBlossom;
	newBlossom->treeParentEdge = LCA->treeParentEdge;
	//��������Ը��ڵ�Ϊ��������������֮��û��ڵ㲻�Ǹ��ڵ㣩����Ѹû��ĸ��ڵ���ӽڵ��滻����
	if (newBlossom->treeParentBlossom != nullptr)
	{
		newBlossom->treeParentBlossom->treeChildren.remove(LCA);
		newBlossom->treeParentBlossom->treeChildren.push_back(newBlossom.get());
	}

	//���µ�Blossom��innerChildren��ֵ������nextBlossomEdge��¼�»��еıߵ���Ϣ
	LCA->label = EVEN;
	newBlossom->innerChildren.push_back(LCA);

	Blossom *previous = LCA;
	for (auto it = ourPath.rbegin(); it != ourPath.rend(); ++it)
	{
		(*it)->label = EVEN;
		newBlossom->innerChildren.push_back(*it);
		previous->nextBlossomEdge = (*it)->treeParentEdge;
		previous = *it;
	}
	previous->nextBlossomEdge = pairEdge;

	for (auto it = pairPath.begin(); it != pairPath.end(); ++it)
	{
		(*it)->label = EVEN;
		newBlossom->innerChildren.push_back(*it);
		(*it)->nextBlossomEdge = (*it)->treeParentEdge;
	}

	//����Blossom�е�innerChildren��outwardBlossom��ֵ
	for (auto const &inner : newBlossom->innerChildren)
		inner->outwardBlossom = newBlossom;

	//���»����Ѿ������treeChildren�������е���Ϊ�˻��нڵ㣬�е���Ϊ���»���treeChildren
	for (auto const &inner : newBlossom->innerChildren)
	{
		for (auto const &child : inner->treeChildren)
		{
			if (child->outwardBlossom == nullptr)
			{
				newBlossom->treeChildren.push_back(child);
				child->treeParentBlossom = newBlossom.get();
			}
		}
	}

	//newBlossom��outwardBlossom��ֵ
	newBlossom->outwardBlossom = nullptr;

	//���LCA�Ǹ��ڵ�Ļ������صõ��Ļ����Ա���¸��ڵ�
	if (LCA->treeParentEdge == nullptr)
		return newBlossom.get();
	return nullptr;
}


//���������Ļ��������������ڲ㻨�нڵ�v1,��������һ���ڵ�Ľڵ�v2���ҳ�ż·������תƥ����Ϣ
void Blossom::flipPathBetween(Vertex *v1, Vertex *v2){
	//ȷ��v1�����ڲ㻨�нڵ�
	if (getStem() != v1)
		std::swap(v1, v2);
	//���Ҫչ����·���ϵĽڵ��Ƿǻ��ڵ㣬����Ҫ���κβ���������
	if (innerChildren.size() == 0){ return; }

	Blossom *b2 = getInnerBlossomContaining(v2);
	Blossom *first = innerChildren[0];
	Blossom *last = b2;

	//��֤չ����·����ż��·��
	auto b2_pos = std::find(innerChildren.begin(), innerChildren.end(), b2);
	long pos = std::distance(innerChildren.begin(), b2_pos);
	if (pos % 2 == 1)
	{
		std::swap(v1, v2);
		std::swap(first, last);
	}


	
	Blossom *current = first;
	while (current != last)
	{
		Vertex *b_v1 = current->nextBlossomEdge->getV1();
		Vertex *b_v2 = current->nextBlossomEdge->getV2();
		if (getInnerBlossomContaining(b_v1) != current)
			std::swap(b_v1, b_v2);

		current->flipPathBetween(v1, b_v1);
		current->nextBlossomEdge->flip();

		current = getInnerBlossomContaining(b_v2);
		v1 = b_v2;
	}
	last->flipPathBetween(v1, v2);
	//�ҳ�����·�����ı�ƥ����Ϣ�󣬻��Ļ���λ�ñ仯
	//std::rotate(innerChildren.begin(), b2_pos, innerChildren.end());
	
}


//�������ӵ������Vertex���㣬���Ÿ��ڵ�������л���·��������תƥ����Ϣ
void Blossom::arguementPath(Vertex *v){
	Vertex *v2 = getStem();
	/*
	if (getLabel() == ODD)
	{
		v2 = treeParentEdge->getV1();
		if (v2->getOutermostBlossom() != this)
			v2 = treeParentEdge->getV2();
	}
	*/
	flipPathBetween(v, v2);

	Blossom *parent = treeParentBlossom;
	Edge *parentEdge = treeParentEdge;

	if (parent == nullptr)
		return;

	//�ڵ�pv�����и�Blossom�У��뱾���ڵ����ӵ� �ǻ�Vertex����
	Vertex *pv = parentEdge->getV1();
	if (pv->getOutermostBlossom() == this)
		pv = parentEdge->getV2();

	parentEdge->flip();
	parent->arguementPath(pv);
}


