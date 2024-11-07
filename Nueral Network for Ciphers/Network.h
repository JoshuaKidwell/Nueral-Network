#ifndef NETWORK_H
#define NETWORK_H

#include "NodeEdge.h"
#include <string>

struct networkValue {
	double*** W;
	double** B;
};

class Network
{
public:
	Network(int* node_sizes, int node_width);
	void SetInput(double*);
	void Solve();
	void TrainForSingleData(double* x, double* y);
	void TrainWithBatch(double** x, double** y, int batch_size);
	void saveNetwork(std::string filename);
	void loadNetwork(std::string filename);
	double Accuracy(double** x, double** y, int batch_size);
	void printOutput(double*);
	void printOutput();
	void printNetwork();
	void print(Node*);
	Node** nodes; //all nodes;
	int width; //nodes dimensions
	int* heights; //nodes heights
	double stepSizeb;
	double stepSizew;
	~Network();
private:
	void saveNetworkValue(std::string filename, networkValue* network);
	networkValue* BackPropagation(double* y, double* x);
	void AdjustValues(networkValue**, int size);
	networkValue* CreateValueStuct();
	void ConnectWithEdges(Node* nodes1, Node* nodes2, int node1_size, int node2_size);
	double dcostOa(double a, double y);
	double daOz(double z);
	double dzLOaL1(double*, double** w, int size, int);
	void DeleteNetworkValue(networkValue*);
};

#endif