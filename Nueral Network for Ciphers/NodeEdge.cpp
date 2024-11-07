#include "NodeEdge.h"
#include <math.h>

Node::Node()
{
    l_edges = nullptr;
    r_edges = nullptr;
    l_edge_count = 0;
    r_edge_count = 0;
    a = 0;
    z = 0;
    b = 0;
}

void Node::SetNode(Edge* left_edges, Edge* right_edges, int left_edge_count, int right_edge_count)
{
    l_edges = left_edges;
    r_edges = right_edges;
    l_edge_count = left_edge_count;
    r_edge_count = right_edge_count;
}

double Node::activation(double x)
{
    //Sigmoid function
    double e = 2.71828;
    double v = 1 / (1 + pow(e, -1 * x));

    return v;
}

double Node::calcValue()
{
    double sum = 0;
    double weight;
    double l_value;

    for (int i = 0; i < l_edge_count; i++) {
        weight = (l_edges + i)->w;
        l_value = (l_edges + i)->l_node->a;
        sum += weight * l_value;
    }
    z = sum + b;
    a = activation(z);
    return a;
}

void Node::setValue(double value)
{
    a = value;
}

Node::~Node()
{
    delete[] l_edges;
    l_edges = nullptr;

    delete[] r_edges;
    r_edges = nullptr;
}

Edge::Edge()
{
    w = 1;
    l_node = nullptr;
    r_node = nullptr;
}

Edge::Edge(double weight, Node* left_node, Node* right_node)
{
    w = weight;
    l_node = left_node;
    r_node = right_node;
}
