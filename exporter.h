#ifndef EXPORTER_H
#define EXPORTER_H
#include "graph.h"
#include <string>

class Exporter
{
public:
    virtual void setGraph(Graph &graph) = 0;
    virtual bool exportToFile(const std::string &path) = 0;
};

#endif //EXPORTER_H
