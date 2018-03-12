
#include "WeightedPerfectMatching.h"
#include <vector>
#include <algorithm>

WPM::WPM(Graph* &G){
	this->num_vertices = G->n;
	this->num_matchedEdges = 0;

	//初始化所有顶点
	for (int i = 0; i < G->n; ++i){
		vertices.push_back(std::unique_ptr<Vertex>(new Vertex(G->headArray[i]->Hvertex)));

		//错误代码？？？
		//Vertex vertex(G->headArray[i]->Hvertex);
		//Vertex *p_vertex = &vertex;
		//vertices.push_back(p_vertex);
	}

	//初始化所有边
	for (int j = 0; j < G->n; ++j){
		edge_vertex *p = G->headArray[j]->first_edge;
		while (p){
			//每条边两个顶点的信息
			Vertex *p_v1 = vertices[(G->headArray[j]->Hvertex) - 1].get();
			Vertex *p_v2 = vertices[(p->Evertex) - 1].get();

			//边的信息
			edges.push_back(std::unique_ptr<Edge>(new Edge(p_v1, p_v2, p->cost)));

			p = p->next;
		}
	}
}

//初始化所有顶点的权重——该顶点所有边中最大权重的1/2
void WPM::initial_charge(){
	for (auto const &p_vertex : vertices){
		//遍历所有该顶点的边
		double max_charge = 0;
		for (auto const &p_edge : edges){
			if (p_edge->getV1()->id == p_vertex->id || p_edge->getV2()->id == p_vertex->id){
				max_charge = std::max(max_charge, p_edge->getWeight());
			}

		}
		//对顶点进行权重赋值
		p_vertex->updateVertexTotal(max_charge / 2.0);

	}
}

//初始化根节点集，所有未匹配的点都作为根节点
void WPM::initial_roots(){
	for (auto const &vertex : vertices){
		if (false == vertex->inMatchedEdge()){
			this->roots.push_back(vertex.get());
		}
	}

}


//在根节点集中删除花节点
void WPM::removeRootFromForest(Blossom *root){
	roots.remove(root);
}

//向根节点集中加入花节点
void WPM::addRootToForest(Blossom *root){
	roots.push_back(root);
}

//找出已与pair匹配的另一节点
Vertex* WPM::getMatchedVertex(Vertex *pair){
	for (auto const &edge : edges){
		if (edge->getV1() == pair){
			if (edge->isMatched() == 1)
				return edge->getV2();
		}
		if (edge->getV2() == pair){
			if (edge->isMatched() == 1)
				return edge->getV1();
		}
	}
	return nullptr;
}


//返回v1 v2之间的匹配边
Edge* WPM::getMatchedEdgeBetween(Vertex *v1, Vertex *v2){
	for (auto const &edge : edges){
		if ((edge->getV1() == v1 && edge->getV2() == v2) || (edge->getV1() == v2 && edge->getV2() == v1)){
			if (edge->isMatched() == 1){
				return edge.get();
			}
		}
	}
	return nullptr;
}

/*
//寻找以花节点parent（或单节点）为父节点的可以grow tree的tight边，且该tight边信息v1的最外层花是parent
void WPM::find_tight(Blossom *parent, std::vector<Edge *>& tight_edges){
for (auto edge : edges){
//if ((edge->getV1()->getOutermostBlossom() == parent&&edge->getV2()->getOutermostBlossom() != parent) || (edge->getV2()->getOutermostBlossom() == parent&&edge->getV1()->getOutermostBlossom() != parent)){
if ((edge->getV1()->getOutermostBlossom() == parent&&edge->getV2()->getOutermostBlossom() != parent)){
if (edge->isTight())
tight_edges.push_back(edge);
}
else{
;
}
}

}
*/


//把花节点parent到v之间的未匹配边parentEdge 和 v与v_pair之间的匹配边 加入树中
void WPM::connectToTree(Blossom *parent, Vertex *v, Vertex *v_pair, Edge *parentEdge, Edge *pair){
	parent->treeChildren.push_back(v);
	v->treeChildren.push_back(v_pair);

	v->treeParentBlossom = parent;
	v_pair->treeParentBlossom = v;

	v->treeParentEdge = parentEdge;
	v_pair->treeParentEdge = pair;


	//标记v（EVEN）和与v构成匹配边的节点(ODD)
	v->setLabel(ODD);
	v_pair->setLabel(EVEN);

}


//当所有树不能再延伸时，需要调整权重（dual change），找出最小的delta
double WPM::getMinDelta(){
	double min_delta = MAX_NUMBER;
	for (auto const &edge : edges){
		if (edge->judge() == Even_Nontree){
			if (edge->getSlack() < min_delta)
				min_delta = edge->getSlack();
		}

		else if (edge->judge() == Even_Even_InSame || edge->judge() == Even_Even_NotInSame){
			if ((edge->getSlack() / 2) < min_delta)
				min_delta = (edge->getSlack() / 2);
		}

		else
			;
	}
	return min_delta;
}

//调整权重，对所有在树中的偶节点-delta，奇节点+delta，在树中的最外层花+2delta
void WPM::dualChangeVertex(double delta){
	for (auto const &vertex : vertices){
		if (vertex->getLabel() == EVEN)
			vertex->updateVertexTotal(-delta);
		else if (vertex->getLabel() == ODD)
			vertex->updateVertexTotal(delta);
		else
			;
	}
}

//找出树中是花节点的点（一定是最外层花）并更新权重
void WPM::dualChangeBlossom(double delta, Blossom *root){

	Blossom *p = root;
	if (p->innerChildren.size() != 0){
		p->updateBlossomCharge(2 * delta);
	}

	if (p->treeChildren.size() != 0){
		for (auto child : p->treeChildren){
			dualChangeBlossom(delta, child);
		}
	}


}

//拆开树中花节点
//void WPM::disConnectBlossom(Blossom *blossom){
//	Blossom *p = blossom;
//	if (p->innerChildren.size() != 0){
//		for (auto inner : p->innerChildren){
//			disConnectBlossom(inner);
//		}
//
//	}
//
//	p->setLabel(NOT_IN_TREE);
//
//	p->treeChildren.clear();   //??????????????清空
//	p->treeParentBlossom = nullptr;
//	p->treeParentEdge = nullptr;
//
//
//	//p->outwardBlossom = nullptr;
//	//p->outwardBlossom.reset();
//	p->nextBlossomEdge = nullptr;
//	p->innerChildren.clear();
//}

void WPM::disConnectBlossom(Blossom *blossom){
	Blossom *p = blossom;

	std::vector<Blossom *> innerChildren_p(p->innerChildren);

	p->setLabel(NOT_IN_TREE);
	p->treeChildren.clear();   //??????????????清空
	p->treeParentBlossom = nullptr;
	p->treeParentEdge = nullptr;


	//p->outwardBlossom = nullptr;
	p->outwardBlossom.reset();
	p->nextBlossomEdge = nullptr;
	p->innerChildren.clear();


	if (innerChildren_p.size() != 0){

		for (auto it = innerChildren_p.begin(); it != innerChildren_p.end(); ++it){
			disConnectBlossom(*it);
		}

		vector<Blossom *>().swap(innerChildren_p);
	}


}



//从当前根节点拆开树
void WPM::disConnectTree(Blossom *root){

	if (root->treeChildren.size() != 0){
		for (auto child : root->treeChildren){
			disConnectTree(child);
		}
	}
	disConnectBlossom(root);

}

//对tight边进行处理
bool WPM::process(Edge *edge){
	if (edge->isTight() == false)
		return false;

	auto out1 = edge->getV1()->getOutermostBlossom();
	auto out2 = edge->getV2()->getOutermostBlossom();

	//在花中的边成为dead edge,不再考虑
	if (out1 == out2)
		return false;

	auto root1 = out1->getRootBlossom();
	auto root2 = out2->getRootBlossom();

	//该tight的另一节点是不在树中节点,一定是已匹配点，单独的不匹配点已经被当作树根标记为EVEN
	if (edge->judge() == Even_Nontree){
		if (out1->getLabel() == EVEN){
			Vertex *v = edge->getV2();
			Vertex *v_pair = getMatchedVertex(v);
			Edge *parentEdge = edge;
			Edge *pair = getMatchedEdgeBetween(v, v_pair);
			connectToTree(out1, v, v_pair, parentEdge, pair);
		}
		else{
			Vertex *v = edge->getV1();
			Vertex *v_pair = getMatchedVertex(v);
			Edge *parentEdge = edge;
			Edge *pair = getMatchedEdgeBetween(v, v_pair);
			connectToTree(out2, v, v_pair, parentEdge, pair);
		}
		return true;
	}

	//如果是同一棵树中EVEN节点，则缩花——Blossom
	if (edge->judge() == Even_Even_InSame){
		Blossom *newRoot = out1->makeBlossomWith(out2, edge);
		//如果新的花形成了根节点，则更新根节点
		if (newRoot != nullptr)
		{
			this->removeRootFromForest(root1);
			this->addRootToForest(newRoot);
		}
		return true;
	}

	//如果是不同棵树中EVEN节点，则直接在两个根节点中形成増广路径
	if (edge->judge() == Even_Even_NotInSame){
		//分别有两个端点花向各自的root找出路径
		out1->arguementPath(edge->getV1());
		out2->arguementPath(edge->getV2());

		//把由这两个树根延伸的树释放
		//Blossom *root1 = out1->getRootBlossom();
		//Blossom *root2 = out2->getRootBlossom();
		disConnectTree(root1);
		disConnectTree(root2);
		//从根节点里移除这两个树根
		removeRootFromForest(root1);
		removeRootFromForest(root2);
		//连接两个偶花的tight边设为匹配边
		edge->flip();

		this->increaseMatchedEdges();
		return true;
	}
}





//从根节点grow tree,不能再grow tree时返回0
/*
void WPM::growTree(int &flag){
for (auto root : roots){


Blossom *p = root;
if (!p->treeChildren.empty()){
for (auto child : p->treeChildren){
growTree(child, flag);
}
}
else{
std::vector<Edge *> tight_edges;
find_tight(p, tight_edges);

if (!tight_edges.empty()){
for (auto tight : tight_edges){
auto out1 = tight->getV1()->getOutermostBlossom();
auto out2 = tight->getV2()->getOutermostBlossom();
//该tight的另一节点是不在树中节点,一定是已匹配点，单独的不匹配点已经被当作树根标记为EVEN
if (out2->getLabel() == NOT_IN_TREE){

flag += 1;
//节点已有匹配点，延伸树，一条未匹配边，一条匹配边
Vertex *v = tight->getV1();
Vertex *v_pair = getMatchedVertex(v);
Edge *parentEdge = tight;
Edge *pair = getMatchedEdgeBetween(v, v_pair);
connectToTree(p, v, v_pair, parentEdge, pair);

}


//该tight的另一节点是在树中节点
else{
//如果是ODD节点，不做任何事
if (out2->getLabel() == ODD)
;
else{
flag += 1;
//如果是同一棵树中EVEN节点，则缩花——Blossom
if (out1->getRootBlossom() == out2->getRootBlossom()){
Blossom *newRoot = out1->makeBlossomWith(out2, tight);
//如果新的花形成了根节点，则更新根节点
if (newRoot != nullptr)
{
this->removeRootFromForest(tight->getV1()->getOutermostBlossom()->getRootBlossom());
this->addRootToForest(newRoot);
}
}
//如果是不同棵树中EVEN节点，则直接在两个根节点中形成増广路径
else{
//分别有两个端点花向各自的root找出路径
out1->arguementPath(tight->getV1());
out2->arguementPath(tight->getV2());
//把由这两个树根延伸的树释放
out1->getRootBlossom()->disConnectTree();
out2->getRootBlossom()->disConnectTree();
//从根节点里移除这两个树根
removeRootFromForest(out1->getRootBlossom());
removeRootFromForest(out2->getRootBlossom());
//连接两个偶花的tight边设为匹配边
tight->flip();

}

}


}
}
//对树中的叶子节点找出所有的tight边，对所有tight边处理完之后返回1
//return 1;
}
else
;

}
}
}
*/

bool WPM::find_perfectmatching(){
	WPM::initial_charge();
	WPM::initial_roots();
	while (1){





		while (1){
			int flag = 0;
			for (auto const &edge : edges){
				if (this->process(edge.get()) == true){
					flag = 1;
					break;
				}
			}
			//若有边进行了操作，继续操作
			if (flag == 1)
				continue;
			//若没有边进行了操作，跳出次循环
			if (flag == 0){

				break;
			}

		}
		//如果还没有达到完美匹配，则进行dualChange 改变权重
		if (this->num_matchedEdges * 2 != this->num_vertices){
			double min_delta = this->getMinDelta();
			this->dualChangeVertex(min_delta);
			for (auto root : roots)
				this->dualChangeBlossom(min_delta, root);


		}
		else
			return true;

	}
}

void WPM::displayResult(){
	std::cout << "最大权完美匹配结果是：" << '\n';
	for (auto const &edge : edges){
		if (edge->isMatched() == 1 && edge->getV1()->id <= SATNUM && edge->getV2()->id <= SATNUM){
			std::cout << edge->getV1()->id << "->" << edge->getV2()->id << '\t' << edge->getWeight()<< " " << edge->getV1()->getVertexCharge() << " "<<edge->getV2()->getVertexCharge() <<'\n' ;
		}
	}
	std::cout << endl;
}

//向txt文件输出最后的链路分配结果
void WPM::exportResult(string exportFilename){
	std::ofstream outFile(exportFilename);
	
	assert(outFile.is_open());
	std::vector<int> access(SATNUM, 0);
	for (auto const &edge : edges){
		if (edge->isMatched() == 1 && edge->getV1()->id <= SATNUM && edge->getV2()->id <= SATNUM){
			access[edge->getV1()->id - 1] = edge->getV2()->id;
			access[edge->getV2()->id - 1] = edge->getV1()->id;
		}
	}

	for (auto it = access.begin(); it != access.end(); ++it){
		outFile << *it << '\t';
	}
	outFile.close();
}


//经过一次最大权匹配之后（链路分配），把没有建立连接的边权重加1
void WPM::costAdjust_and_reInitial(){

	for (auto const &edge : edges){
		//if (edge->getV2()->id - edge->getV1()->id == SATNUM)
		//	;
		//else{
		//	if (edge->isMatched() == 0 && edge->getV1()->id <= SATNUM && edge->getV2()->id <= SATNUM)
		//		edge->adjustWeight(1);
		//	else if (edge->isMatched() == 0 && edge->getV1()->id > SATNUM && edge->getV2()->id > SATNUM)
		//		edge->adjustWeight(1);
		//	else
		//		;
		//}
		if (edge->isMatched() == 1)
			edge->flip();
			
		else{
			if (edge->getV1()->id <= SATNUM && edge->getV2()->id <= SATNUM)
				edge->adjustWeight(1);
			else if (edge->getV1()->id > SATNUM && edge->getV2()->id > SATNUM)
				edge->adjustWeight(1);
			else
				;
		}
	}
	this->num_matchedEdges = 0;
	for (auto const &vertex : vertices){
		vertex->modifyVertexTotal(0);
		vertex->setMatched(false);
		vertex->setLabel(EVEN);
	}


}