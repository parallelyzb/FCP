#ifndef _REDUCTION_
#define _REDUCTION_

#include <fstream>
#include <string>
#include <vector>

//将n*n的可见性矩阵和权重矩阵扩展到2n*2n，和原连接图对应的辅助边权重不变，点和对应点之间的辅助边权重设为0
void reduce(std::vector<std::vector<int>>& accessArray, std::vector<std::vector<int>>& costArray);

#endif