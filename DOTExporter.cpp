#include "DOTExporter.h"
#include <sstream>
#include <fstream>
#include <iostream>

void DOTExporter::setGraph(Graph &graph) {
    m_graph = &graph;
}

bool DOTExporter::exportToFile(const std::string &path) {
    std::stringstream ss;
    ss << "graph {" << std::endl;
    auto n = m_graph->getFirstNode();
    while(n) {
        ss << n->id << std::endl;
        n = m_graph->getNextNode(n);
    }
    auto e = m_graph->getFirstEdge();
    while(e) {
        auto ns = m_graph->getEdgeSource(e);
        if (!ns) return false;
        auto nt = m_graph->getEdgeTarget(e);
        if (!nt) return false;
        ss << ns->id << " -- " << nt->id
           << " [ label = " << e->faultProb << " ]" << std::endl;
        e = m_graph->getNextEdge(e);
    }
    ss << "}" << std::endl;
    std::ofstream out(path, std::ofstream::trunc);
    out << ss.str();
    out.flush();
    out.close(); 

    return true;
}
