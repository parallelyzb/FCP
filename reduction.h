#ifndef _REDUCTION_
#define _REDUCTION_

#include <fstream>
#include <string>
#include <vector>

//��n*n�Ŀɼ��Ծ����Ȩ�ؾ�����չ��2n*2n����ԭ����ͼ��Ӧ�ĸ�����Ȩ�ز��䣬��Ͷ�Ӧ��֮��ĸ�����Ȩ����Ϊ0
void reduce(std::vector<std::vector<int>>& accessArray, std::vector<std::vector<int>>& costArray);

#endif