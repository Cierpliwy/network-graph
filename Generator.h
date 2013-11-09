#ifndef GENERATOR_H
#define GENERATOR_H
#include "Graph.h"

class Generator
{
public:
    virtual void setGraph(Graph &graph) = 0;
    virtual void generate() = 0;
};

#endif //GENERATOR_H
