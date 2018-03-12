#include "reduction.h"

void reduce(std::vector<std::vector<int>>& accessArray, std::vector<std::vector<int>>& costArray){
	int size = accessArray[0].size();

	//accessArray
	for (int i = 1; i <= size; ++i){

		for (int j = 1; j <= 2 * size; ++j){
			if (j <= size)
				;
			else{
				if ((j - i) == size)
					accessArray[i - 1].push_back(1);
				else
					accessArray[i - 1].push_back(0);
			}
		}

	}

	for (int i = size + 1; i <= 2 * size; ++i){
		std::vector<int> newRow;

		for (int j = 1; j <= 2 * size; ++j){

			if (j > size){
				newRow.push_back(accessArray[i - size - 1][j - size - 1]);
			}
			else{
				if ((i - j) == size)
					newRow.push_back(1);
				else
					newRow.push_back(0);
			}
		}

		accessArray.push_back(newRow);
	}



	//costArray
	for (int i = 1; i <= size; ++i){

		for (int j = 1; j <= 2 * size; ++j){
			if (j <= size)
				;
			else{
				costArray[i - 1].push_back(0);
			}
		}

	}

	for (int i = size + 1; i <= 2 * size; ++i){
		std::vector<int> newRow;

		for (int j = 1; j <= 2 * size; ++j){

			if (j > size){
				newRow.push_back(costArray[i - size - 1][j - size - 1]);
			}
			else{
				newRow.push_back(0);
			}
		}

		costArray.push_back(newRow);
	}
}