#include <iostream>
#include <vector>
#include "Network.h"
#include "Bitmap.h"
#include <iomanip>

template <typename T> T* vtoa(std::vector<T>);
template <typename T> T** vto2Da(std::vector<std::vector<T>>);
template <typename T> void delete2Da(T**, int);

void NumberIdentifier();

int main() {
	NumberIdentifier();

	return 0;
}

template <typename T>
T* vtoa(std::vector<T> vec) {
	T* arr = new T[vec.size()];
	for (int i = 0; i < vec.size(); i++) {
		arr[i] = vec[i];
	}
	return arr;
}

template<typename T>
T** vto2Da(std::vector<std::vector<T>> vec)
{
	int numRows = vec.size();
	double** arr = new double* [numRows];
	for (int i = 0; i < numRows; ++i) {
		int numCols = vec[i].size();
		arr[i] = new double[numCols];
		for (int j = 0; j < numCols; ++j) {
			arr[i][j] = vec[i][j];
		}
	}

	return arr;
}

template<typename T>
void delete2Da(T** arr, int size)
{
	for (int i = 0; i < size; i++) {
		delete[] arr[i];
	}

	delete[] arr;
}

void NumberIdentifier()
{
	Bitmap bmp;
	std::vector<std::pair<std::vector<double>, double>> pics;
	pics = bmp.loadbmpsTo2DArray("numbers-data", 10000); //Load all training data into pics
	std::vector<double> test_input;
	test_input = bmp.bmpToArray("numbers-data/1/100.bmp");

	int node_heights[4] = { 256, 16, 16, 10 };
	Network NeuralNetwork(node_heights, 4);
	NeuralNetwork.stepSizeb = 10000;
	NeuralNetwork.stepSizew = 10000;

	std::vector<std::vector<double>> input;
	std::vector<std::vector<double>> output;

	for (int i = 0; i < pics.size(); i++) {
		input.push_back(pics[i].first);
		for (int j = 0; j < 10; j++) {
			output.push_back({0,0,0,0,0,0,0,0,0,0});
		}
		output[i][pics[i].second] = 1;
	}

	double** x = vto2Da(input);
	double** y = vto2Da(output);
	double* x_test = vtoa(test_input);

	NeuralNetwork.loadNetwork("network-test.net");
	NeuralNetwork.saveNetwork("network-prev.net");

	for (int l = 0; l < 1; l++) {
		for (int k = 0; k < 10; k++) {
			//NeuralNetwork.TrainForSingleData(x[k], y[k]);
			NeuralNetwork.TrainWithBatch(x, y, input.size());
			if (k % 1 == 0) {
				std::cout << NeuralNetwork.Accuracy(x, y, input.size()) << "\n";
			}
		}
		NeuralNetwork.saveNetwork("network-test.net");
		
	}

	NeuralNetwork.saveNetwork("network-test.net");

	NeuralNetwork.printOutput(x_test);

	delete2Da(x, input.size());
	delete2Da(y, output.size());
}
