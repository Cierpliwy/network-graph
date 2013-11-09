#ifndef GRAPH_IMPL_H
#define GRAPH_IMPL_H
#include "Graph.h"
#include <list>
#include <vector>

struct NodeImpl;

struct EdgeImpl : public Edge
{
    EdgeImpl(const Edge& edge) :
        Edge(edge.faultProb), source(nullptr), target(nullptr),
        prevEdge(nullptr), nextEdge(nullptr) {}

    NodeImpl *source;
    NodeImpl *target;

    EdgeImpl *prevEdge;
    EdgeImpl *nextEdge;
};

struct NodeImpl : public Node
{
    NodeImpl(const Node& node) :
        Node(node.id, node.state), 
        it(adjNodes.begin()), 
        nextNode(nullptr), 
        prevNode(nullptr) {}

    std::list<NodeImpl*> adjNodes;
    std::list<NodeImpl*>::iterator it;
    std::list<EdgeImpl*> adjEdges;

    NodeImpl *nextNode;
    NodeImpl *prevNode;
};

class GraphImpl : public Graph
{
public:
    GraphImpl() : m_nodeHead(nullptr), 
                  m_edgeHead(nullptr),
                  m_nodesNum(0) {}

    virtual ~GraphImpl() { reset(); }

    virtual Node* addNode(const Node &node); //O(1)
    virtual bool removeNode(Node *node); //O(d)
    virtual Node* getFirstNode(); //O(1)
    virtual Node* getNextNode(Node *node); //O(1)
    virtual Node* getFirstAdjNode(Node *node); //O(1)
    virtual Node* getNextAdjNode(Node *node); //O(1)
    virtual unsigned int getNodeLevel(Node *node); //O(1)
    virtual unsigned int getNodesNum(); //O(1)

    virtual Edge* addEdge(Node *source, Node *target, const Edge &edge); //O(1)
    virtual bool removeEdge(Edge *edge); //O(d)
    virtual Edge* getEdge(Node *source, Node *target); //O(d)
    virtual Node* getEdgeSource(Edge *edge); //O(1)
    virtual Node* getEdgeTarget(Edge *edge); //O(1)
    virtual Edge* getFirstEdge(); //O(1)
    virtual Edge* getNextEdge(Edge *edge); //O(1)

    virtual void reset(); //O(n+e)
    virtual bool clone(Graph& graph); //O(n+e)

private:
    NodeImpl *m_nodeHead;
    EdgeImpl *m_edgeHead;
    unsigned int m_nodesNum;
};

#endif //GRAPH_IMPL_H
