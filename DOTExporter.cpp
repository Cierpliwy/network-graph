#include "DOTExporter.h"
#include <sstream>
#include <fstream>
#include <iomanip>

void DOTExporter::setGraph(Graph &graph) {
    m_graph = &graph;
}

static std::string hex(unsigned char a) {
    std::string s;
    unsigned char c = a >> 4;
    if (c > 9) s += 'a' + (c-10);
    else s+= '0' + c;
    c = a & 0x0F;
    if (c > 9) s += 'a' + (c-10);
    else s+= '0' + c;
    return s;
}

bool DOTExporter::exportToFile(const std::string &path) {
    std::stringstream ss;
    ss << "graph {" << std::endl;
    auto n = m_graph->getFirstNode();
    while(n) {
        ss << n->id;
        ss << " [fillcolor = \"#" 
           << hex(n->style.bg.r)
           << hex(n->style.bg.g)
           << hex(n->style.bg.b)
           << "\", style=\"filled" 
           << (n->style.dashed ? ",dashed" : "") << "\", color = \"#"
           << hex(n->style.pen.r)
           << hex(n->style.pen.g)
           << hex(n->style.pen.b)
           << "\", penwidth = " << n->style.penWidth << "]"
           << std::endl;

        n = m_graph->getNextNode(n);
    }
    auto e = m_graph->getFirstEdge();
    while(e) {
        auto ns = m_graph->getEdgeSource(e);
        if (!ns) return false;
        auto nt = m_graph->getEdgeTarget(e);
        if (!nt) return false;
        ss << ns->id << " -- " << nt->id
           << " [ label = " << e->reliability
           << ", fillcolor = \"#" 
           << hex(e->style.bg.r)
           << hex(e->style.bg.g)
           << hex(e->style.bg.b)
           << "\", style=\"filled" 
           << (e->style.dashed ? ",dashed" : "") << "\", color = \"#"
           << hex(e->style.pen.r)
           << hex(e->style.pen.g)
           << hex(e->style.pen.b)
           << "\", penwidth = " << e->style.penWidth << "]"
           << std::endl;
        e = m_graph->getNextEdge(e);
    }
    ss << "}" << std::endl;
    std::ofstream out(path, std::ofstream::trunc);
    out << ss.str();
    out.flush();
    out.close(); 

    return true;
}
