#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "prototypes.h"
#include "kmeans.h"
#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

const int T = 10, number_of_gr = 10;
const int chunk_size = 1000;
int chunk_count_human, chunk_count_robot;

int main() {
	using std::string;
	using std::vector;

	vector <string> robot_docs, human_docs;
	std::ifstream file;
	for (int i = 0; i < number_of_gr; ++i) {
		file.open("C:\\Users\\k_and\\source\\repos\\Detection of artifical texts\\Detection of artifical texts\\RobotTexts\\R" + std::to_string(i + 1) + ".txt", std::ifstream::binary);
		std::string rob_doc((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		prepare(rob_doc);
		robot_docs.push_back(rob_doc);
		file.open("C:\\Users\\k_and\\source\\repos\\Detection of artifical texts\\Detection of artifical texts\\HumanTexts\\H" + std::to_string(i + 1) + ".txt", std::ifstream::binary);
		std::string hum_doc((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		prepare(hum_doc);
		human_docs.push_back(hum_doc);
	}
	/*file.open("C:\\Users\\k_and\\source\\repos\\Detection of artifical texts\\Detection of artifical texts\\RobotTexts\\H1.txt", std::ifstream::binary);
	std::string target_doc((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	prepare(target_doc);*/

	vector<int> robot_chunks_count, human_chunks_count;
	vector<vector<string>> robot_chunks(number_of_gr), human_chunks(number_of_gr);
	for (int i = 0; i < number_of_gr; ++i) {
		robot_chunks_count.push_back(robot_docs[i].size() / chunk_size);
		human_chunks_count.push_back(human_docs[i].size() / chunk_size);
		string temp;
		string::iterator it;
		int j;
		for (it = robot_docs[i].begin(), j = 1; it != robot_docs[i].end(); ++it, ++j) {
			temp += *it;
			if (!(j % chunk_size)) {
				robot_chunks[i].push_back(temp);
				temp.clear();
			}
		}
		temp.clear();
		for (it = human_docs[i].begin(), j = 1; it != human_docs[i].end(); ++it, ++j) {
			temp += *it;
			if (!(j % chunk_size)) {
				human_chunks[i].push_back(temp);
				temp.clear();
			}
		}
	}

	int total_robot_chunks = 0, total_human_chunks = 0;
	for (int i : robot_chunks_count) total_robot_chunks += i;
	for (int i : human_chunks_count) total_human_chunks += i;

	vector <int> real_lable(total_robot_chunks, 1);
	for (int i = 0; i < total_human_chunks; ++i) 
		real_lable.push_back(2);

	string doc;
	for (const std::string& st : robot_docs) 	doc += st;
	for (const std::string& st : human_docs)	doc += st;
	//prepare(doc);
	//n_gram(doc, 5);

	std::ifstream dictionary_file("dictionary.txt");
	vector<string> dictionary;
	string temp;
	while (std::getline(dictionary_file, temp)) {
		dictionary.push_back(temp);
	}
	dictionary_file.close();

	std::vector<vector<std::vector<int>>>freq_of_robot_ngramm(number_of_gr), freq_of_human_ngramm(number_of_gr);
	for (int i = 0; i < number_of_gr; ++i) {
		std::vector<std::string>::iterator it;

		for (it = robot_chunks[i].begin(); it != robot_chunks[i].end(); ++it) {
			freq_of_robot_ngramm[i].push_back(freq_in_chunk(*it, dictionary));
		}
		for (it = human_chunks[i].begin(); it != human_chunks[i].end(); ++it) {
			freq_of_human_ngramm[i].push_back(freq_in_chunk(*it, dictionary));
		}
	}

	//vector <vector<double>> dzv_matrix;
	//vector <double> row;

	///*for the robot docs 1*/
	//for (int i = 0; i < number_of_gr; ++i) {
	//	for (int j = T; j < robot_chunks_count[i]; ++j) {
	//		for (int k = 0; k < number_of_gr; ++k)
	//			if (i != k)
	//				for (int n = T; n < robot_chunks_count[k]; ++n)
	//					row.push_back(
	//						dzv_calc(
	//							T, freq_of_robot_ngramm[i][j],
	//							freq_of_robot_ngramm[k][n], 
	//							j, n,
	//							freq_of_robot_ngramm[i],
	//							freq_of_robot_ngramm[k]
	//						));
	//				
	//		/*for all the other docs*/
	//		for (int k = 0; k < number_of_gr; ++k)
	//			for (int n = T; n < human_chunks_count[k]; ++n)
	//				row.push_back(
	//					dzv_calc(
	//						T, freq_of_robot_ngramm[i][j],
	//						freq_of_human_ngramm[k][n],
	//						j, n,
	//						freq_of_robot_ngramm[i],
	//						freq_of_human_ngramm[k]
	//					));	
	//		dzv_matrix.push_back(row);
	//		row.clear();
	//	}
	//}
	///*for the human docs*/
	//for (int i = 0; i < number_of_gr; ++i) {
	//	for (int j = T; j < human_chunks_count[i]; ++j) {
	//		for (int k = 0; k < number_of_gr; ++k)
	//			if (i != k)
	//				for (int n = T; n < human_chunks_count[k]; ++n)
	//					row.push_back(
	//						dzv_calc(
	//							T, freq_of_human_ngramm[i][j],
	//							freq_of_human_ngramm[k][n],
	//							j, n,
	//							freq_of_human_ngramm[i],
	//							freq_of_human_ngramm[k]
	//						));

	//		/*for all the other docs*/
	//		for (int k = 0; k < number_of_gr; ++k)
	//			for (int n = T; n < robot_chunks_count[k]; ++n)
	//				row.push_back(
	//					dzv_calc(
	//						T, freq_of_human_ngramm[i][j],
	//						freq_of_robot_ngramm[k][n],
	//						j, n,
	//						freq_of_human_ngramm[i],
	//						freq_of_robot_ngramm[k]
	//					));
	//		dzv_matrix.push_back(row);
	//		row.clear();
	//	}
	//}

	///*write matrix with const columns 1*/

	//std::ofstream write_matrix("matrix.txt");
	//int colums = dzv_matrix[0].size();
	//std::for_each(dzv_matrix.begin(), dzv_matrix.end(),
	//	[&colums](vector<double>& col) mutable {
	//		if (col.size() > colums) colums = col.size();
	//	});
	//for (vector<double> v : dzv_matrix) {
	//	for (int i = 0; i < v.size(); ++i) {
	//		write_matrix << v[i];
	//		if (i != (v.size() - 1))
	//			write_matrix << ' ';
	//	}
	//	for (int j = 0; j < colums - v.size(); ++j) {
	//		write_matrix << ' ' << 0;
	//		if (j != (colums - v.size() - 1))
	//			write_matrix << ' ';
	//	}
	//	write_matrix << '\n';
	//}
	//write_matrix.close();

	/*write matrix 1*/

	//for (vector<double> v : dzv_matrix) {
	//	for (int i = 0; i < v.size(); ++i) {
	//		write_matrix << v[i];
	//		if (i != (v.size() - 1))
	//			write_matrix << ' ';
	//	}
	//	write_matrix << '\n';
	//}
	//write_matrix.close();


	/*reading from text 1*/

	vector<vector<double>> dzv;
	std::ifstream infile("matrix.txt");
	std::string line;
	while (getline(infile, line)) {
		std::istringstream is(line);
		dzv.push_back(
			std::vector<double>(std::istream_iterator<double>(is),
				std::istream_iterator<double>()));
	}
	infile.close();


	///*calculating clasters*/

	//int K = atoi("2");
	//std::ifstream infile("matrix.txt");
	//int pointId = 1;
	//vector<Point> all_points;
	//string line;

	//while (getline(infile, line)) {
	//	Point point(pointId, line);
	//	all_points.push_back(point);
	//	pointId++;
	//}
	//infile.close();

	////Return if number of clusters > number of points
	//if (all_points.size() < K) {
	//	std::cout << "Error: Number of clusters greater than number of points." << std::endl;
	//	return 1;
	//}
	////Running K-Means Clustering
	//int iters = 100;

	//KMeans kmeans(K, iters);
	//kmeans.run(all_points);

	/*using boost*/
	namespace bn = boost::python::numpy;
	namespace bp = boost::python;
	_putenv_s("PYTHONPATH", ".");
	Py_Initialize();
	bn::initialize();
	try
	{
		bp::object my_python_class_module = bp::import("mymodule");

		/*for testing... 1*/

		int n_rows = dzv.size();
		
		bp::list total;
		for (int i = 0; i < n_rows; ++i) {
			bp::tuple shape = bp::make_tuple(dzv[i].size());
			bn::dtype dtype = bn::dtype::get_builtin<double>();
			bn::ndarray py_mas = bn::zeros(shape, dtype);
			vector<double>::iterator it = dzv[i].begin();
			for (int j = 0; j < dzv[i].size(); ++j)
				py_mas[j] = dzv[i][j];
			total.append(py_mas);
		}
		bn::ndarray array = bn::from_object(total);


		/*direct calculation 2*/
		/*vector<vector<double>> correct_matrix;
		int colums = dzv_matrix[0].size();
		std::for_each(dzv_matrix.begin(), dzv_matrix.end(),
			[&colums](vector<double>& col) mutable {
				if (col.size() < colums) colums = col.size();
			});
		for (vector<double> v : dzv_matrix) {
			vector<double> temp;
			for (int i = 0; i < colums; ++i) {
				temp.push_back(v[i]);
			}
			correct_matrix.push_back(temp);
		}*/
		/*dzv_matrix = correct_matrix;
		int n_rows = dzv_matrix.size();

		bp::list list_mas;
		for (int i = 0; i < n_rows; ++i) {
			bp::tuple shape = bp::make_tuple(dzv_matrix[i].size());
			bn::dtype dtype = bn::dtype::get_builtin<double>();
			bn::ndarray py_row = bn::zeros(shape, dtype);
			for (int j = 0; j < dzv_matrix[i].size(); ++j)
				py_row[j] = dzv_matrix[i][j];
			list_mas.append(py_row);
		}
		bn::ndarray array = bn::from_object(list_mas);*/


		my_python_class_module.attr("MyFunc")(array);
		my_python_class_module.attr("SVM")(array);
	}
	catch (const bp::error_already_set&)
	{
		PyErr_Print();
		return 1;
	}

	Py_Finalize();
}
