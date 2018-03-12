
#include "edge.h"
#include <iostream>
Blossom::Blossom(){
	charge_blossom = 0;
	label = EVEN;		//缩花节点一定是偶节点
	treeParentBlossom = nullptr;
	treeParentEdge = nullptr;

	outwardBlossom = nullptr;
	//nextBlossomEdge = nullptr;
}

//得到外层第一个花,已经是最外层节点是返回 nullptr
Blossom* Blossom::getOutwardBlossom(){
	return outwardBlossom.get();
}
//得到该节点最外层的花——surface blossom
Blossom* Blossom::getOutermostBlossom()
{
	Blossom* outermost = this;
	while (outermost->outwardBlossom.get() != nullptr)
		outermost = outermost->outwardBlossom.get();
	return outermost;
}

//得到顶点v所在的次外层的花
Blossom* Blossom::getInnerBlossomContaining(Vertex *v)
{
	//Blossom *inner = (Blossom*)v;
	Blossom *inner = v;
	while (inner != nullptr && inner->outwardBlossom.get() != this)
		inner = inner->outwardBlossom.get();
	return inner;
}


//得到当前节点所在树的根节点
Blossom* Blossom::getRootBlossom()
{
	Blossom* root = this;
	while (root->treeParentBlossom != nullptr)
		root = root->treeParentBlossom;
	return root;

}


//得到花的最底层lca
Vertex* Blossom::getStem()
{
	return this->innerChildren[0]->getStem();
}



//pairEdge连接了两棵树中的EVEN，缩花
Blossom* Blossom::makeBlossomWith(Blossom *pair, Edge *pairEdge){

	std::list<Blossom *> ourPath;	//存着当前花节点到根节点的反序路径
	std::list<Blossom *> pairPath;	//存着另一节点带根节点的反序路径

	ourPath.push_back(this);
	pairPath.push_back(pair);
	while (ourPath.back()->treeParentBlossom != nullptr)
		ourPath.push_back(ourPath.back()->treeParentBlossom);
	while (pairPath.back()->treeParentBlossom != nullptr)
		pairPath.push_back(pairPath.back()->treeParentBlossom);


	//找出lowest common ancestor——花托，花托是从根节点开始的最后一个两条路径都有的花节点
	Blossom *LCA = nullptr;
	while (ourPath.size() > 0 && pairPath.size() > 0 &&ourPath.back() == pairPath.back()){
		LCA = ourPath.back();
		ourPath.erase(std::prev(ourPath.end()));
		pairPath.erase(std::prev(pairPath.end()));
	}

	//缩花——产生新的Blossom
	std::shared_ptr<Blossom> newBlossom = std::make_shared<Blossom>();

	
	newBlossom->treeParentBlossom = LCA->treeParentBlossom;
	newBlossom->treeParentEdge = LCA->treeParentEdge;
	//如果不是以根节点为花托缩花（缩花之后该花节点不是根节点），则把该花的父节点的子节点替换更新
	if (newBlossom->treeParentBlossom != nullptr)
	{
		newBlossom->treeParentBlossom->treeChildren.remove(LCA);
		newBlossom->treeParentBlossom->treeChildren.push_back(newBlossom.get());
	}

	//对新的Blossom的innerChildren赋值，并用nextBlossomEdge记录下花中的边的信息
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

	//对新Blossom中的innerChildren的outwardBlossom赋值
	for (auto const &inner : newBlossom->innerChildren)
		inner->outwardBlossom = newBlossom;

	//对新花的已经延伸的treeChildren做处理，有的作为了花中节点，有的作为了新花的treeChildren
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

	//newBlossom的outwardBlossom赋值
	newBlossom->outwardBlossom = nullptr;

	//如果LCA是根节点的花，返回得到的花，以便更新根节点
	if (LCA->treeParentEdge == nullptr)
		return newBlossom.get();
	return nullptr;
}


//给出最外层的花，给出花的最内层花托节点v1,和连向下一花节点的节点v2，找出偶路径并反转匹配信息
void Blossom::flipPathBetween(Vertex *v1, Vertex *v2){
	//确保v1是最内层花托节点
	if (getStem() != v1)
		std::swap(v1, v2);
	//如果要展开的路径上的节点是非花节点，则不需要做任何操作，返回
	if (innerChildren.size() == 0){ return; }

	Blossom *b2 = getInnerBlossomContaining(v2);
	Blossom *first = innerChildren[0];
	Blossom *last = b2;

	//保证展开的路径是偶数路径
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
	//找出最终路径并改变匹配信息后，花的花托位置变化
	//std::rotate(innerChildren.begin(), b2_pos, innerChildren.end());
	
}


//给出连接到花外的Vertex顶点，沿着父节点求出所有花的路径，并反转匹配信息
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

	//节点pv是树中父Blossom中，与本花节点连接的 非花Vertex顶点
	Vertex *pv = parentEdge->getV1();
	if (pv->getOutermostBlossom() == this)
		pv = parentEdge->getV2();

	parentEdge->flip();
	parent->arguementPath(pv);
}


