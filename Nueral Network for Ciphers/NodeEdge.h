#ifndef NODEEDGE_H
#define NODEEDGE_H

class Edge;

class Node
{
public:
	Node();
	void SetNode(Edge* left_edges, Edge* right_edges, int left_edge_count, int right_edge_count); //constructor
	double activation(double x); //activation function
	double calcValue(); //solves for value
	void setValue(double); //sets value for input nodes
	Edge* l_edges; //edges leading to this node
	Edge* r_edges; //edges going from this node
	double b; //offset
	double a; //value
	double z; //value before activation function
	int l_edge_count;
	int r_edge_count;
	~Node(); //delete edges
};

class Edge {
public:
	Edge();
	Edge(double, Node* left_node, Node* right_node);
	Node* l_node;
	Node* r_node;
	double w; //weight
};

#endif
