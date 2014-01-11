#include "Algorithms.h"
#include <queue>
#include <cstdlib>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <random>

using namespace std;

static Color getColor(float prob)
{
    Color c;
    c.r = (1-prob)*255;
    c.g = prob*255;
    c.b = 0;
    return c;
}

static float relVal(float min, float max, float n) {
    if (max == min) return n;
    return (n - min) / (max - min);
}

bool GraphAlgorithms::isConnected() {
    unsigned int unconnectedNum = 0;
    Node *n = m_graph->getFirstNode();
    while(n) {
        n->visited = false;
        if (!n->connected) unconnectedNum++;
        n = m_graph->getNextNode(n);
    }

    queue<Node*> visitedNodes;

    n = m_graph->getFirstNode();
    while(n && !n->connected) {
        n = m_graph->getNextNode(n);
    }
    if (!n) return false;

    n->visited = true;
    visitedNodes.push(n);
    unsigned int totalVisited = 1;

    while(!visitedNodes.empty()) {
        Node *n = visitedNodes.front();
        visitedNodes.pop();
        
        pair<Node*, Edge*> p = m_graph->getFirstAdjNode(n);
        while(p.first) {
            if (p.first->connected && p.second->connected && 
                !p.second->broken && !p.first->visited) {
                p.first->visited = true;
                visitedNodes.push(p.first);
                totalVisited++;
            }
            p = m_graph->getNextAdjNode(n);
        }
    }
    
    if (totalVisited == m_graph->getNodesNum() - unconnectedNum) return true;
    return false;

}

bool GraphAlgorithms::hasBridge()
{
    // Every edge is connected and not broken on start-up
    Edge *e = m_graph->getFirstEdge();
    while (e) {
        e->broken = false;
        e->connected = true;
        e = m_graph->getNextEdge(e);
    }

    // Check if graph is connected even if one edge is disconnected
    e = m_graph->getFirstEdge();
    while (e) {
        e->connected = false;
        if (!isConnected()) return true;
        e->connected = true;
        e = m_graph->getNextEdge(e);
    }

    return false;
}

float GraphAlgorithms::monteCarlo()
{
    if (!m_seed) {
        srand(time(nullptr));
        m_seed = rand();
    }

    unsigned long long acc = 0;
    for (unsigned int i = 0; i < m_iterationsPerNode; ++i) {
        Edge *e = m_graph->getFirstEdge();
        while(e) {
            e->broken = ((rand() % 1001) / 1000.0f) > e->reliability; 
            e = m_graph->getNextEdge(e);
        }
        if (isConnected()) ++acc;
    }

    return acc/static_cast<float>(m_iterationsPerNode);
}

float GraphAlgorithms::monteCarlo2()
{
    if (!m_seed) {
    	std::random_device rd;
    	m_gen = std::mt19937(rd());
    	m_seed = m_dist(m_gen) * 1000;
    	srand(m_seed);
    }

    int acc = 0;

    boost::math::normal_distribution<> nd(0.0,1.0);
    double w = m_precision / 100.0;
    double t = boost::math::quantile(nd, 1.0 - (1.0-m_confidence)/2.0);
    unsigned int n = 1;
    int x = monteCarloIteration();
	acc = x;
    double p = x;
    while ((n < 1000) || (t*sqrt(p*(1-p)) > w * sqrt(n) )) {
    	if (n >= m_iterationsLimit) {
    		cout << "DEBUG: Exceeded iterations limit." << endl;
    		//TODO recalculate precision and confidence
    		break;
    	}

//      //DEBUG
//    	cout << "n=" << n << endl;
//    	cout << "t=" << t << " p=" << p <<" x=" << x <<endl;
//    	cout << "t*sqrt(p*(1-p)) = " << (t*sqrt(p*(1-p))) << " w * sqrt(n) =" << (w * sqrt(n)) << endl <<endl;
    	++n;
    	x = monteCarloIteration();

    	acc += x;
    	p = p + (x-p)/n;
    }

    //DEBUG
	cout << "n=" << n << endl;
	cout << "t=" << t << " p=" << p <<" x=" << x <<endl;
	cout << "t*sqrt(p*(1-p)) = " << (t*sqrt(p*(1-p))) << " w * sqrt(n) =" << (w * sqrt(n)) << endl <<endl;

	cout << "Calculated in " << n << " iterations." << endl;
	cout << "Graph complete in " << acc << " iterations." << endl;
	return acc/(float)n;
}

int GraphAlgorithms::monteCarloIteration() {
    Edge *e = m_graph->getFirstEdge();
    while(e) {
        e->broken = m_dist(m_gen) > e->reliability;
        e = m_graph->getNextEdge(e);
    }
    if (isConnected()) return 1;
    else return 0;
}

void GraphAlgorithms::edgeWeakness()
{
    Edge *lastEdge = nullptr;
    Edge *e = m_graph->getFirstEdge();
    Edge *er = nullptr;
    float minRel = 1, maxRel = 0;
    float minRelS = 1, maxRelS = 0;
    vector<pair<Edge*, float>> edges;

    while(e) {
       if (lastEdge) lastEdge->connected = true;
       e->connected = false;
       minRel = min(minRel, e->reliability);
       maxRel = max(maxRel, e->reliability);

       float rel = monteCarlo();
       if (rel < minRelS) er = e;
       minRelS = min(minRelS, rel);
       maxRelS = max(maxRelS, rel);

       edges.push_back(make_pair(e,rel));
       lastEdge = e;
       e = m_graph->getNextEdge(e);
    }
    if (lastEdge) lastEdge->connected = true;

    // Set style
    for(auto &e : edges) {
        e.first->style.penWidth = 
            3 * relVal(minRel, maxRel, e.first->reliability) + 1;

        if (m_relativeColors) {
            e.first->style.pen = getColor(relVal(minRelS, maxRelS, e.second));
        } else e.first->style.pen = getColor(e.second);

        if (e.first == er) e.first->style.dashed = true;
        else e.first->style.dashed = false;
    }
}

void GraphAlgorithms::nodeWeakness()
{
    Node *lastNode = nullptr;
    Node *n = m_graph->getFirstNode();
    float minRel = 1, maxRel = 0;
    Node *ns = nullptr;
    vector<pair<Node*, float>> nodes;

    while(n) {
        if (lastNode) lastNode->connected = true;
        n->connected = false;

        float rel = monteCarlo();
        if (rel < minRel) ns = n;
        minRel = min(minRel, rel);
        maxRel = max(maxRel, rel);

        nodes.push_back(make_pair(n,rel));
        lastNode = n;
        n = m_graph->getNextNode(n);
    }
    if (lastNode) lastNode->connected = true;

    //Set style
    for(auto &i : nodes) {
        if (i.first == ns) i.first->style.dashed = true;
        else i.first->style.dashed = false;

        i.first->style.penWidth = 
            3 * (1 - relVal(minRel, maxRel, i.second)) + 1;

        if (m_relativeColors)
            i.first->style.bg = getColor(relVal(minRel, maxRel, i.second));
        else
            i.first->style.bg = getColor(i.second);

    }
}

void GraphAlgorithms::setSeed(unsigned int seed) {
    m_seed = seed;
}

unsigned int GraphAlgorithms::getSeed() {
    return m_seed;
}
