#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cctype>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <cmath>

void n_gram(const std::string& doc, int n);

std::vector<int> freq_in_chunk(const std::string& chunk, 
	const std::vector<std::string>& dictonary);

long double dzv_calc(int T, const std::vector<int> di,
	const std::vector <int> dj, int i, int j,
	const std::vector<std::vector<int>> freq_of_robot_ngramm_i,
	const std::vector<std::vector<int>> freq_of_robot_ngramm_j);

long double zv_calc(int T, const std::vector<int> di, int i,
	const std::vector<std::vector<int>> freq_of_robot_ngramm_i_j);

long double spearman_calc(const std::vector<int> di, const std::vector<int> freq_of_robot_ngramm_k);

void prepare(std::string& st);
