#ifndef BUILDER_GENERATOR_H
#define BUILDER_GENERATOR_H
#include "Generator.h"
#include <vector>

class BuilderGenerator : public Generator
{
public:

    BuilderGenerator() :
        m_graph(nullptr),
        m_nodesNum(10),
        m_edgesNum(15),
        m_maxTreeAdjNodes(5),
        m_minProb(0.5),
        m_maxProb(0.5),
        m_completeness(0.3) {}

    virtual void setGraph(Graph &graph) {
        m_graph = &graph;
    }

    virtual void setSeed(unsigned int seed) {
        m_seed = seed;
    }

    void setNodesNum(unsigned int nodesNum) {
        m_nodesNum = nodesNum;
    }

    void setEdgesNum(unsigned int edgesNum) {
        m_edgesNum = edgesNum;
    }

    void setMaxTreeAdjNodes(unsigned int maxTreeAdjNodes) {
        m_maxTreeAdjNodes = maxTreeAdjNodes;
    }

    void setMinProb(float minProb) {
        m_minProb = minProb;
    }

    void setMaxProb(float maxProb) {
        m_maxProb = maxProb;
    }

    void setCompleteness(float completeness) {
        m_completeness = completeness;
    }

    virtual bool generate();

private:

    bool isValid(unsigned int &i, 
                 Node *node, 
                 const std::vector<Node*>& nodes);

    Graph *m_graph;
    unsigned int m_seed;
    unsigned int m_nodesNum;
    unsigned int m_edgesNum;
    unsigned int m_maxTreeAdjNodes;
    float m_minProb;
    float m_maxProb;
    float m_completeness;
};

#endif //BUILDER_GENERATOR_H
