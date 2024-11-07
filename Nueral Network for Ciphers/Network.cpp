#include "Network.h"
#include "NodeEdge.h"
#include <string>
#include <iostream>
#include <math.h>
#include <fstream>
#include <random>

Network::Network(int* node_sizes, int node_width)
{
	//add nodes
	stepSizeb = 1;
	stepSizew = 1;
	heights = node_sizes;
	width = node_width;
	nodes = new Node*[width];
	for (int i = 0; i < width; i++) {
		nodes[i] = new Node[heights[i]];
	}

	//connect nodes with edges
	for (int i = 0; i < width - 1; i++) {
		ConnectWithEdges(nodes[i], nodes[i + 1], heights[i], heights[i + 1]);
	}
}

void Network::saveNetworkValue(std::string filename, networkValue* network)
{
	std::ofstream networkFile(filename, std::ios::trunc);

	for (int i = 0; i < width; i++) {
		networkFile << heights[i] << " ";
	}
	networkFile << std::endl;

	networkFile << stepSizeb << " " << stepSizew << std::endl;

	for (int L = 0; L < width - 1; L++) {
		for (int i = 0; i < heights[L + 1]; i++) {
			networkFile << network->B[L][i] << " ";
			for (int j = 0; j < heights[L]; j++) {
				networkFile << network->W[L][i][j] << " ";
			}
			networkFile << std::endl;
		}
	}

	networkFile.close();
}

networkValue* Network::BackPropagation(double* x, double* y)
{
	//create networkValue pointers
	networkValue* v = CreateValueStuct();

	SetInput(x);
	Solve();

	//solve all partial derivative Cost/a
	//solve all partial derivatives a/z
	double* currentProduct = new double[heights[width-1]];

	for (int i = 0; i < heights[width-1]; i++) {
		currentProduct[i] = dcostOa(nodes[width-1][i].a, y[i]);
	}

	for (int l = 0; l < width - 1; l++) {
		double** wij = new double*[heights[width - 1 - l]]; //list of weights from i to j
		for (int i = 0; i < heights[width - 1 - l]; i++) {
			currentProduct[i] *= daOz(nodes[width - 1 - l][i].z);
			v->B[width - 1 - l - 1][i] = currentProduct[i]; //solve all b

			wij[i] = new double[heights[width - 1 - l - 1]]; //create second array for j

			for (int j = 0; j < heights[width - 1 - l - 1]; j++) {
				v->W[width - 1 - l - 1][i][j] = nodes[width - 1 - l - 1][j].a * currentProduct[i]; //solve all of w

				wij[i][j] = nodes[width - 1 - l][i].l_edges[j].w; //set weight values
			}
		}
		double* newCurrentProduct = new double[heights[width - 1 - l - 1]]; //prepare for next layer
		//solve for all new a/z
		for (int j = 0; j < heights[width - 1 - l - 1]; j++) {
			newCurrentProduct[j] = dzLOaL1(currentProduct, wij, heights[width - 1 - l], j);
		}
		
		for (int i = 0; i < heights[width - 1 - l]; i++) {
			delete[] wij[i];
		}
		delete[] wij;

		//switch to new layer
		delete[] currentProduct;
		currentProduct = newCurrentProduct;
	}

	delete[] currentProduct;

	return v;
}

void Network::AdjustValues(networkValue** d, int size)
{
	//get average adjustments for all weights and biases
	networkValue* avg = CreateValueStuct();

	double w_sum = 0;
	double b_sum = 0;

	for (int L = 0; L < width - 1; L++) {
		for (int i = 0; i < heights[L + 1]; i++) {
			for (int dataSet = 0; dataSet < size; dataSet++) {
				b_sum += d[dataSet]->B[L][i];
			}
			avg->B[L][i] = b_sum / size;
			b_sum = 0;

			for (int j = 0; j < heights[L]; j++) {
				for (int dataSet = 0; dataSet < size; dataSet++) {
					w_sum += d[dataSet]->W[L][i][j];
				}
				avg->W[L][i][j] = w_sum / size;
				w_sum = 0;
			}
		}
	}

	saveNetworkValue("network_last_adjustment.net", avg);

	//adjust all weights and biases * stepSize

	for (int L = 0; L < width - 1; L++) {
		for (int i = 0; i < heights[L + 1]; i++) {
			nodes[L + 1][i].b = nodes[L + 1][i].b - stepSizeb * avg->B[L][i];
			//std::cout << nodes[L + 1][i].b;
			for (int j = 0; j < heights[L]; j++) {
				nodes[L + 1][i].l_edges[j].w = nodes[L + 1][i].l_edges[j].w - stepSizew * avg->W[L][i][j];
			}
		}
	}
	
	DeleteNetworkValue(avg);
}

networkValue* Network::CreateValueStuct()
{
	double*** W = new double** [width-1];
	double** B = new double* [width-1];

	for (int i = 0; i < width-1; i++) {
		W[i] = new double* [heights[i + 1]];
		B[i] = new double[heights[i + 1]];
		for (int j = 0; j < heights[i + 1]; j++)
			W[i][j] = new double[heights[i]];
	}

	networkValue* newValue = new networkValue;
	newValue->W = W;
	newValue->B = B;

	return newValue;
}

void Network::ConnectWithEdges(Node* nodes1, Node* nodes2, int node1_size, int node2_size)
{
	// Create a random number generator
	std::random_device rd;   // Seed generator
	std::mt19937 gen(rd());  // Mersenne Twister engine

	// Define a distribution between 0 and 1
	std::uniform_real_distribution<> dis(0.0, 1.0);

	// Generate a random number
	double random_number = dis(gen);

	Edge* edgeArr_l;
	Edge** edgeArr_r;
	double weight = 0.0000000000000000000001;

	edgeArr_r = new Edge * [node2_size];
	for (int i = 0; i < node2_size; i++) {
		edgeArr_r[i] = new Edge[node1_size];
	}
	for (int i = 0; i < node1_size; i++) {
		edgeArr_l = new Edge[node2_size];
		for (int j = 0; j < node2_size; j++) {
			edgeArr_l[j] = Edge(weight * random_number, nodes1 + i, nodes2 + j);
			edgeArr_r[j][i] = edgeArr_l[j];
		}
		(nodes1 + i)->r_edges = edgeArr_l;
		(nodes1 + i)->r_edge_count = node2_size;
	}
	for (int i = 0; i < node2_size; i++) {
		(nodes2 + i)->l_edges = edgeArr_r[i];
		(nodes2 + i)->l_edge_count = node1_size;
	}
}

double Network::dcostOa(double a, double y)
{
	return 2*(a-y);
}

double Network::daOz(double z)
{
	//sigmoid
	double e = 2.71828;
	double enegx = pow(e, -1 * z);
	double v = enegx / pow(1 + enegx,2);

	return v * (1 - v);
}

double Network::dzLOaL1(double* products, double** w, int size, int j)
{
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += products[i] * w[i][j];
	}
	return sum;
}

void Network::DeleteNetworkValue(networkValue* v)
{
	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < heights[i + 1]; j++)
			delete[] v->W[i][j];
	}

	for (int i = 0; i < width - 1; i++) {
		delete[] v->W[i];
		delete[] v->B[i];
	}

	delete[] v->W;
	delete[] v->B;

	delete v;
}

void Network::SetInput(double* x)
{
	for (int i = 0; i < heights[0]; i++) {
		nodes[0][i].a = x[i];
	}
}

void Network::Solve()
{
	for (int i = 1; i < width; i++) {
		for (int j = 0; j < heights[i]; j++) {
			nodes[i][j].calcValue();
		}
	}
}

void Network::TrainForSingleData(double* x, double* y)
{
	networkValue* v;
	v = BackPropagation(x, y);
	AdjustValues(&v,1);

	DeleteNetworkValue(v);
}

void Network::TrainWithBatch(double** x, double** y, int batch_size)
{
	networkValue** data = new networkValue*[batch_size];

	for (int i = 0; i < batch_size; i++) {
		data[i] = BackPropagation(x[i], y[i]);
	}

	AdjustValues(data, batch_size);

	for (int i = 0; i < batch_size; i++) {
		DeleteNetworkValue(data[i]);
	}
	delete[] data;
}

void Network::saveNetwork(std::string filename)
{
	std::ofstream networkFile(filename, std::ios::trunc);

	for (int i = 0; i < width; i++) {
		networkFile << heights[i] << " ";
	}
	networkFile << std::endl;

	networkFile << stepSizeb << " " << stepSizew << std::endl;

	for (int L = 0; L < width - 1; L++) {
		for (int i = 0; i < heights[L + 1]; i++) {
			networkFile << nodes[L + 1][i].b << " ";
			for (int j = 0; j < heights[L]; j++) {
				networkFile << nodes[L + 1][i].l_edges[j].w << " ";
			}
			networkFile << std::endl;
		}
	}

	networkFile.close();
}

void Network::loadNetwork(std::string filename)
{
	std::ifstream networkFile(filename);
	bool properNetwork = true;
	int height;
	for (int i = 0; i < width; i++) {
		networkFile >> height;
		if (height != heights[i]) {
			properNetwork = false;
		}
	}
	if (!properNetwork) {
		std::cout << std::endl << "Improper Newtork, could not load..." << std::endl;
		networkFile.close();
		return;
	}

	networkFile >> stepSizeb;
	networkFile >> stepSizew;

	for (int L = 0; L < width - 1; L++) {
		for (int i = 0; i < heights[L + 1]; i++) {
			networkFile >> nodes[L + 1][i].b;
			for (int j = 0; j < heights[L]; j++) {
				networkFile >> nodes[L + 1][i].l_edges[j].w;
			}
		}
	}

	networkFile.close();
}

double Network::Accuracy(double** x, double** y, int batch_size)
{
	double accuracy;
	double little_sum = 0;
	double little_avg;
	double sum = 0;

	for (int i = 0; i < batch_size; i++) {
		SetInput(x[i]);
		Solve();

		little_sum = 0;
		for (int j = 0; j < heights[width - 1]; j++) {
			little_sum += (y[i][j] - nodes[width-1][j].a) * (y[i][j] - nodes[width-1][j].a);
		}
		little_avg = little_sum / heights[width - 1];
		sum += little_avg;
	}
	accuracy = sum / batch_size;

	return accuracy;
}

void Network::printOutput(double* x)
{
	SetInput(x);
	Solve();
	printOutput();
}

void Network::printOutput()
{
	std::cout << "{";
	for (int i = 0; i < heights[width - 1]; i++) {
		std::cout << nodes[width - 1][i].a << ",";
	}
	std::cout << "}";
}

void Network::printNetwork()
{
	for (int L = 0; L < width - 1; L++) {
		for (int i = 0; i < heights[L + 1]; i++) {
			for (int j = 0; j < heights[L]; j++) {
				Node* n = &nodes[L + 1][i];
				std::cout << "-" << nodes[L + 1][i].l_edges[j].w << "-";
			}
			std::cout << "(b=" << nodes[L + 1][i].b << ")";
			std::cout << std::endl;
		}
		std::cout << std::endl << std::endl;
	}
}

void Network::print(Node* n)
{
	std::cout << n->a << "->";
	for (int i = 0; i < n->r_edge_count; i++) {
		print((n->r_edges + i)->r_node);
	}
}

Network::~Network()
{
	for (int i = 0; i < width; i++) {
		delete[] nodes[i];
	}
	//deleting a node deletes the edges
}
