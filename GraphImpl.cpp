#include "GraphImpl.h"
#include <map>
#include <cassert>

Node* GraphImpl::addNode(const Node &node) {
    NodeImpl *n = new NodeImpl(node);
    if (!n) return nullptr;
   
    if (m_nodeHead) {
        n->nextNode = m_nodeHead;
        m_nodeHead->prevNode = n;
    }
    m_nodeHead = n; 
    m_nodesNum++;

    return n;
}

bool GraphImpl::removeNode(Node *node) {
    if (!node) return false;
    NodeImpl* n = static_cast<NodeImpl*>(node);

    // Update pointers
    if (n->prevNode) n->prevNode->nextNode = n->nextNode;
    if (n->nextNode) n->nextNode->prevNode = n->prevNode;
    if (m_nodeHead == n) m_nodeHead = n->nextNode;
   
    // Remove edges
    while(!n->adjNodes.empty())
        assert(removeEdge(n->adjNodes.begin()->second));

    // Remove node
    delete n;
    m_nodesNum--;

    return true;
}

Node* GraphImpl::getFirstNode() {
    return m_nodeHead;
}

Node* GraphImpl::getNextNode(Node *node) {
    if (!node) return nullptr;
    return static_cast<NodeImpl*>(node)->nextNode;
}

std::pair<Node*, Edge*> GraphImpl::getFirstAdjNode(Node *node) {
    if (!node) return std::make_pair(nullptr, nullptr);
    NodeImpl *n = static_cast<NodeImpl*>(node);
    n->it = n->adjNodes.begin();
    if (n->adjNodes.empty()) return std::make_pair(nullptr, nullptr);
    return *n->it;
}

std::pair<Node*, Edge*> GraphImpl::getNextAdjNode(Node *node) {
    if (!node) return std::make_pair(nullptr, nullptr);
    NodeImpl *n = static_cast<NodeImpl*>(node);
    if (++n->it == n->adjNodes.end()) return std::make_pair(nullptr, nullptr);

    return *n->it;
}

unsigned int GraphImpl::getNodeLevel(Node *node)
{
    if (!node) return 0;
    NodeImpl *n = static_cast<NodeImpl*>(node);
    return n->adjNodes.size();
}

unsigned int GraphImpl::getNodesNum() {
    return m_nodesNum;
}

Edge* GraphImpl::addEdge(Node *source, Node *target, const Edge &edge)
{
    if (!source || !target) return nullptr;
    NodeImpl *n1 = static_cast<NodeImpl*>(source);
    NodeImpl *n2 = static_cast<NodeImpl*>(target);

    EdgeImpl *e = new EdgeImpl(edge);
    if (!e) return nullptr;

    if (m_edgeHead) {
        e->nextEdge = m_edgeHead;
        m_edgeHead->prevEdge = e;
    }
    m_edgeHead = e;

    //Add adjacent nodes and edges
    n1->adjNodes.push_back(std::make_pair(n2,e));
    if (n1->adjNodes.end() == n1->it) n1->it = --n1->adjNodes.end();
    n2->adjNodes.push_back(std::make_pair(n1,e));
    if (n2->adjNodes.end() == n2->it) n2->it = --n2->adjNodes.end();

    e->source = n1;
    e->target = n2;

    return e;
}

bool GraphImpl::removeEdge(Edge *edge)
{
    if (!edge) return false;
    EdgeImpl *e = static_cast<EdgeImpl*>(edge);

    //Update pointers
    if (e->prevEdge) e->prevEdge->nextEdge = e->nextEdge;
    if (e->nextEdge) e->nextEdge->prevEdge = e->prevEdge;
    if (m_edgeHead == e) m_edgeHead = e->nextEdge;

    //Remove edges and adjacent nodes from lists
    if (e->source->it->first == e->target) ++e->source->it;
    e->source->adjNodes.remove(std::make_pair(e->target, e));

    if (e->target->it->first == e->source) ++e->target->it;
    e->target->adjNodes.remove(std::make_pair(e->source, e));

    //Remove edge
    delete e;

    return true;
}

Edge* GraphImpl::getEdge(Node *source, Node *target) 
{
    if (!source || !target) return nullptr;
    auto n1 = static_cast<NodeImpl*>(source);
    auto n2 = static_cast<NodeImpl*>(target);

    for(auto &e : n1->adjNodes)
        if (e.second->source == n2 || e.second->target == n2)
            return e.second;

    return nullptr;
}

Node* GraphImpl::getEdgeSource(Edge* edge) {
    if (!edge) return nullptr;
    EdgeImpl *e = static_cast<EdgeImpl*>(edge);
    return e->source;
}

Node* GraphImpl::getEdgeTarget(Edge* edge) {
    if (!edge) return nullptr;
    EdgeImpl *e = static_cast<EdgeImpl*>(edge);
    return e->target;
}

Edge* GraphImpl::getFirstEdge() {
    return m_edgeHead;
}

Edge* GraphImpl::getNextEdge(Edge *edge) {
    if (!edge) return nullptr;
    return static_cast<EdgeImpl*>(edge)->nextEdge;
}

void GraphImpl::reset() {
    while(auto e = getFirstEdge()) removeEdge(e);
    while(auto n = getFirstNode()) removeNode(n);
    assert(!m_nodeHead);
    assert(!m_edgeHead);
    m_nodesNum = 0;
}

bool GraphImpl::clone(Graph& graph) {
    graph.reset();
    std::map<unsigned int, Node*> newNodes;
    Node *n = getFirstNode();
    while(n) {
        Node* node = graph.addNode(Node(n->id));
        if (!node) return false;
        auto res = newNodes.insert(std::make_pair(n->id, node));
        if (!res.second) return false;
        n = getNextNode(n);
    }

    EdgeImpl *e = static_cast<EdgeImpl*>(getFirstEdge());
    while(e) {
        auto it = newNodes.find(e->source->id);
        if (it == newNodes.end()) return false;
        auto it2 = newNodes.find(e->target->id);
        if (it2 == newNodes.end()) return false;
        if (!graph.addEdge(it->second, it2->second, Edge(e->reliability))) 
            return false;
        e = static_cast<EdgeImpl*>(getNextEdge(e));
    }

    return true;
}
