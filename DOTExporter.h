#ifndef DOT_EXPORTER_H
#define DOT_EXPORTER_H
#include "Exporter.h"

class DOTExporter : public Exporter
{
public:
    DOTExporter() : m_graph(nullptr) {}

    virtual void setGraph(Graph &graph);
    virtual bool exportToFile(const std::string &path);

private:
    Graph *m_graph;
};

#endif //DOT_EXPORTER_H
