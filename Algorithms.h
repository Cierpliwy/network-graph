#ifndef GRAPH_ALGORITHMS
#define GRAPH_ALGORITHMS
#include "Graph.h"

class GraphAlgorithms
{
public:
    GraphAlgorithms() :
        m_graph(nullptr),
        m_iterationsPerNode(100),
        m_relativeColors(false) {}

    void setRelativeColors(bool relative) {
        m_relativeColors = relative;
    }

    void setGraph(Graph &graph) {
        m_graph = &graph;
    }

    void setIterationsPerNode(unsigned int it) {
        m_iterationsPerNode = it;
    }

    bool isConnected();
    float monteCarlo();
    void edgeWeakness();
    void nodeWeakness();

private:
    Graph *m_graph;
    unsigned int m_iterationsPerNode;
    bool m_relativeColors;

};

#endif //GRAPH_ALGORITHMS
