#ifndef GRAPH_ALGORITHMS
#define GRAPH_ALGORITHMS
#include "Graph.h"
#include <random>

class GraphAlgorithms
{
public:
    GraphAlgorithms() :
        m_graph(nullptr),
        m_iterationsPerNode(100),
        m_relativeColors(false),
        m_confidence(0.95),
        m_precision(0.1),
        m_iterationsLimit(100000),
        m_seed(0)
        {}

    void setRelativeColors(bool relative) {
        m_relativeColors = relative;
    }

    void setGraph(Graph &graph) {
        m_graph = &graph;
    }

    void setIterationsPerNode(unsigned int it) {
        m_iterationsPerNode = it;
    }

    void setIterationsLimit(unsigned int it) {
        m_iterationsLimit = it;
    }

    void setPrecision(double precision) {
    	m_precision = precision;
    }

    double getPrecision() {
    	return m_precision;
    }

    void setConfidence(double confidence) {
    	m_confidence = confidence;
    }

    double getConfidence() {
    	return m_confidence;
    }

    double getLastIterations() const {
        return m_lastIterations;
    }

    bool isConnected();
    bool hasBridge();
    float monteCarlo();
    int monteCarloIteration();
    float monteCarlo2();
    void edgeWeakness();
    void nodeWeakness();
    void setSeed(unsigned int seed);
    unsigned int getSeed();

private:
    Graph *m_graph;
    unsigned int m_iterationsPerNode;
    unsigned int m_iterationsLimit;
    double m_confidence;
    double m_precision;
    bool m_relativeColors;
    unsigned int m_seed;
	std::mt19937 m_gen;
	std::uniform_real_distribution<> m_dist;
    double m_lastIterations;

};

#endif //GRAPH_ALGORITHMS
