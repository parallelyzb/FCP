
#include "WeightedPerfectMatching.h"
#include <vector>
#include <algorithm>

WPM::WPM(Graph* &G){
	this->num_vertices = G->n;
	this->num_matchedEdges = 0;

	//��ʼ�����ж���
	for (int i = 0; i < G->n; ++i){
		vertices.push_back(std::unique_ptr<Vertex>(new Vertex(G->headArray[i]->Hvertex)));

		//������룿����
		//Vertex vertex(G->headArray[i]->Hvertex);
		//Vertex *p_vertex = &vertex;
		//vertices.push_back(p_vertex);
	}

	//��ʼ�����б�
	for (int j = 0; j < G->n; ++j){
		edge_vertex *p = G->headArray[j]->first_edge;
		while (p){
			//ÿ���������������Ϣ
			Vertex *p_v1 = vertices[(G->headArray[j]->Hvertex) - 1].get();
			Vertex *p_v2 = vertices[(p->Evertex) - 1].get();

			//�ߵ���Ϣ
			edges.push_back(std::unique_ptr<Edge>(new Edge(p_v1, p_v2, p->cost)));

			p = p->next;
		}
	}
}

//��ʼ�����ж����Ȩ�ء����ö������б������Ȩ�ص�1/2
void WPM::initial_charge(){
	for (auto const &p_vertex : vertices){
		//�������иö���ı�
		double max_charge = 0;
		for (auto const &p_edge : edges){
			if (p_edge->getV1()->id == p_vertex->id || p_edge->getV2()->id == p_vertex->id){
				max_charge = std::max(max_charge, p_edge->getWeight());
			}

		}
		//�Զ������Ȩ�ظ�ֵ
		p_vertex->updateVertexTotal(max_charge / 2.0);

	}
}

//��ʼ�����ڵ㼯������δƥ��ĵ㶼��Ϊ���ڵ�
void WPM::initial_roots(){
	for (auto const &vertex : vertices){
		if (false == vertex->inMatchedEdge()){
			this->roots.push_back(vertex.get());
		}
	}

}


//�ڸ��ڵ㼯��ɾ�����ڵ�
void WPM::removeRootFromForest(Blossom *root){
	roots.remove(root);
}

//����ڵ㼯�м��뻨�ڵ�
void WPM::addRootToForest(Blossom *root){
	roots.push_back(root);
}

//�ҳ�����pairƥ�����һ�ڵ�
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


//����v1 v2֮���ƥ���
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
//Ѱ���Ի��ڵ�parent���򵥽ڵ㣩Ϊ���ڵ�Ŀ���grow tree��tight�ߣ��Ҹ�tight����Ϣv1������㻨��parent
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


//�ѻ��ڵ�parent��v֮���δƥ���parentEdge �� v��v_pair֮���ƥ��� ��������
void WPM::connectToTree(Blossom *parent, Vertex *v, Vertex *v_pair, Edge *parentEdge, Edge *pair){
	parent->treeChildren.push_back(v);
	v->treeChildren.push_back(v_pair);

	v->treeParentBlossom = parent;
	v_pair->treeParentBlossom = v;

	v->treeParentEdge = parentEdge;
	v_pair->treeParentEdge = pair;


	//���v��EVEN������v����ƥ��ߵĽڵ�(ODD)
	v->setLabel(ODD);
	v_pair->setLabel(EVEN);

}


//������������������ʱ����Ҫ����Ȩ�أ�dual change�����ҳ���С��delta
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

//����Ȩ�أ������������е�ż�ڵ�-delta����ڵ�+delta�������е�����㻨+2delta
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

//�ҳ������ǻ��ڵ�ĵ㣨һ��������㻨��������Ȩ��
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

//�����л��ڵ�
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
//	p->treeChildren.clear();   //??????????????���
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
	p->treeChildren.clear();   //??????????????���
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



//�ӵ�ǰ���ڵ����
void WPM::disConnectTree(Blossom *root){

	if (root->treeChildren.size() != 0){
		for (auto child : root->treeChildren){
			disConnectTree(child);
		}
	}
	disConnectBlossom(root);

}

//��tight�߽��д���
bool WPM::process(Edge *edge){
	if (edge->isTight() == false)
		return false;

	auto out1 = edge->getV1()->getOutermostBlossom();
	auto out2 = edge->getV2()->getOutermostBlossom();

	//�ڻ��еı߳�Ϊdead edge,���ٿ���
	if (out1 == out2)
		return false;

	auto root1 = out1->getRootBlossom();
	auto root2 = out2->getRootBlossom();

	//��tight����һ�ڵ��ǲ������нڵ�,һ������ƥ��㣬�����Ĳ�ƥ����Ѿ��������������ΪEVEN
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

	//�����ͬһ������EVEN�ڵ㣬����������Blossom
	if (edge->judge() == Even_Even_InSame){
		Blossom *newRoot = out1->makeBlossomWith(out2, edge);
		//����µĻ��γ��˸��ڵ㣬����¸��ڵ�
		if (newRoot != nullptr)
		{
			this->removeRootFromForest(root1);
			this->addRootToForest(newRoot);
		}
		return true;
	}

	//����ǲ�ͬ������EVEN�ڵ㣬��ֱ�����������ڵ����γɉ���·��
	if (edge->judge() == Even_Even_NotInSame){
		//�ֱ��������˵㻨����Ե�root�ҳ�·��
		out1->arguementPath(edge->getV1());
		out2->arguementPath(edge->getV2());

		//����������������������ͷ�
		//Blossom *root1 = out1->getRootBlossom();
		//Blossom *root2 = out2->getRootBlossom();
		disConnectTree(root1);
		disConnectTree(root2);
		//�Ӹ��ڵ����Ƴ�����������
		removeRootFromForest(root1);
		removeRootFromForest(root2);
		//��������ż����tight����Ϊƥ���
		edge->flip();

		this->increaseMatchedEdges();
		return true;
	}
}





//�Ӹ��ڵ�grow tree,������grow treeʱ����0
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
//��tight����һ�ڵ��ǲ������нڵ�,һ������ƥ��㣬�����Ĳ�ƥ����Ѿ��������������ΪEVEN
if (out2->getLabel() == NOT_IN_TREE){

flag += 1;
//�ڵ�����ƥ��㣬��������һ��δƥ��ߣ�һ��ƥ���
Vertex *v = tight->getV1();
Vertex *v_pair = getMatchedVertex(v);
Edge *parentEdge = tight;
Edge *pair = getMatchedEdgeBetween(v, v_pair);
connectToTree(p, v, v_pair, parentEdge, pair);

}


//��tight����һ�ڵ��������нڵ�
else{
//�����ODD�ڵ㣬�����κ���
if (out2->getLabel() == ODD)
;
else{
flag += 1;
//�����ͬһ������EVEN�ڵ㣬����������Blossom
if (out1->getRootBlossom() == out2->getRootBlossom()){
Blossom *newRoot = out1->makeBlossomWith(out2, tight);
//����µĻ��γ��˸��ڵ㣬����¸��ڵ�
if (newRoot != nullptr)
{
this->removeRootFromForest(tight->getV1()->getOutermostBlossom()->getRootBlossom());
this->addRootToForest(newRoot);
}
}
//����ǲ�ͬ������EVEN�ڵ㣬��ֱ�����������ڵ����γɉ���·��
else{
//�ֱ��������˵㻨����Ե�root�ҳ�·��
out1->arguementPath(tight->getV1());
out2->arguementPath(tight->getV2());
//����������������������ͷ�
out1->getRootBlossom()->disConnectTree();
out2->getRootBlossom()->disConnectTree();
//�Ӹ��ڵ����Ƴ�����������
removeRootFromForest(out1->getRootBlossom());
removeRootFromForest(out2->getRootBlossom());
//��������ż����tight����Ϊƥ���
tight->flip();

}

}


}
}
//�����е�Ҷ�ӽڵ��ҳ����е�tight�ߣ�������tight�ߴ�����֮�󷵻�1
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
			//���б߽����˲�������������
			if (flag == 1)
				continue;
			//��û�б߽����˲�����������ѭ��
			if (flag == 0){

				break;
			}

		}
		//�����û�дﵽ����ƥ�䣬�����dualChange �ı�Ȩ��
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
	std::cout << "���Ȩ����ƥ�����ǣ�" << '\n';
	for (auto const &edge : edges){
		if (edge->isMatched() == 1 && edge->getV1()->id <= SATNUM && edge->getV2()->id <= SATNUM){
			std::cout << edge->getV1()->id << "->" << edge->getV2()->id << '\t' << edge->getWeight()<< " " << edge->getV1()->getVertexCharge() << " "<<edge->getV2()->getVertexCharge() <<'\n' ;
		}
	}
	std::cout << endl;
}

//��txt�ļ����������·������
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


//����һ�����Ȩƥ��֮����·���䣩����û�н������ӵı�Ȩ�ؼ�1
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