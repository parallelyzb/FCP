/*
Blossom Algorithm带花树算法由Edmonds在1965年提出，解决一般图的匹配问题
*/

/*
待优化：在reduce之后，对一个图若有多种最大权匹配方案，
代码更多的选择权重为0的辅助边以及权重最大的实际边，
而不是选择多条权重较大的实际边使得总权重达到最大。
即没有使更多的节点之间建立连接
*/


/*
运行时，需：
1、修改main函数中路径，该路径是可见性表以及相应的权重表
2、设定graph.h中NUMSAT，是总的卫星数
3、设定main函数中num_iteration，为在当前的拓扑关系（可见性，权重）需要进行链路规划的次数
*/


#include <iostream>
#include "WeightedPerfectMatching.h"


int main(void){

	string filename1 = "C:\\Users\\yanzhibo\\Desktop\\state_10.txt";
	string filename2 = "C:\\Users\\yanzhibo\\Desktop\\cost_10.txt";
	vector<vector<int>> accessArray;
	vector<vector<int>> costArray;
	//readAccess(filename1, filename2, accessArray, costArray);
	readAccess(filename1, accessArray, costArray);
	//displayAccess(accessArray);

	Graph *G;
	createGraph(G, accessArray, costArray);
	//displayGraph(G);

	WPM matching(G);

	//在当前的拓扑关系需要进行链路规划的次数
	int num_iteration = 4;
	string exportFilename_1 = "C:\\Users\\yanzhibo\\Desktop\\access";
	string exportFilename_2 = ".txt";
	for (int i = 1; i <= num_iteration; ++i){
		matching.find_perfectmatching();
		matching.displayResult();
		string exportFilename = exportFilename_1 + std::to_string(i) + exportFilename_2;
		matching.exportResult(exportFilename);

		matching.costAdjust_and_reInitial();
	}

	//matching.find_perfectmatching();
	//matching.displayResult();
}
