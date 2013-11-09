#include "BuilderGenerator.h"
#include <list>
#include <cmath>
#include <cstdlib>
using namespace std;

static inline float clamp(float a) {
    return min(max(a,0.f),1.f);
}

bool BuilderGenerator::isValid(unsigned int &i, 
                               Node *node, 
                               const vector<Node*>& nodes) {
    unsigned int oldI = i;
    do {
        if (m_graph->getNodeLevel(nodes[i]) < nodes.size() - 1) {
            if (!node) return true;
            if (!m_graph->getEdge(node, nodes[i])) return true;
        }
        i += 1;
        if (i >= nodes.size()) i = 0;
    } while( i != oldI);

    return false;
}

bool BuilderGenerator::generate() {
    // Check input data
    if (!m_graph) return false;
    m_nodesNum = max(m_nodesNum,1u);
    if (m_edgesNum < m_nodesNum - 1) m_edgesNum = m_nodesNum - 1;
    if (m_edgesNum > m_nodesNum * (m_nodesNum - 1) / 2)
        m_edgesNum = m_nodesNum * (m_nodesNum - 1) / 2;
    if (m_maxTreeAdjNodes < 2 ) m_maxTreeAdjNodes = 2;
    clamp(m_completeness);
    clamp(m_minProb);
    clamp(m_maxProb);
    srand(m_seed);

    // Create simple tree.
    unsigned int createdNodes = 0;
    unsigned int startLabel = m_graph->getNodesNum();
    unsigned int createdEdges = 0;
    list<Node*> openNodes;    
    vector<Node*> closedNodes;

    // Create first node
    unsigned int maxLevel;
    unsigned int neighbours;

    Node *n = m_graph->addNode(Node(createdNodes + startLabel));
    if (!n) return false;
    createdNodes++;
    openNodes.push_back(n);

    // Create other nodes until we reach goal.
    while(createdNodes != m_nodesNum) {
        if (openNodes.empty()) return false;
        n = *openNodes.begin();
        openNodes.pop_front();

        maxLevel = min(m_nodesNum - createdNodes, m_maxTreeAdjNodes - 1);
        neighbours = rand() % (maxLevel + 1);
        if (openNodes.empty() && !neighbours) neighbours = 1;
        
        for (auto i = 0u; i < neighbours; ++i) {
            Node *n2 = m_graph->addNode(Node(createdNodes + startLabel));
            if (!n2) return false;
            createdNodes++;
            float prob = (rand() % 
                          static_cast<unsigned int>((m_maxProb - m_minProb) 
                          * 1000 + 1)) / 1000.0f + m_minProb;
            if (!m_graph->addEdge(n, n2, Edge(prob))) return false;
            createdEdges++;
            openNodes.push_back(n2);
        }
        closedNodes.push_back(n);
    }

    // Add remaining nodes to closed nodes.
    for(auto &n : openNodes) closedNodes.push_back(n);

    // Complete tree to certain level (to add cycles).
    unsigned int maxComplEdges = m_nodesNum * (m_nodesNum - 1) / 2 
                                 * m_completeness;
    unsigned int edgeNum = max(m_edgesNum, max(createdEdges, maxComplEdges));

    unsigned int i,j;
    while (createdEdges != edgeNum) {
        i = rand() % closedNodes.size();
        j = rand() % closedNodes.size();
        if (!isValid(i, nullptr, closedNodes)) return false;
        Node *n1 = closedNodes[i];
        if (!isValid(j, n1, closedNodes)) return false;
        Node *n2 = closedNodes[j];
        float prob = (rand() % 
                      static_cast<unsigned int>((m_maxProb - m_minProb) 
                      * 1000 + 1)) / 1000.0f + m_minProb;
        if (!m_graph->addEdge(n1, n2, Edge(prob))) return false;
        createdEdges++;
    }

    return true;
}
