#ifndef GENERATOR_H
#define GENERATOR_H
#include "Graph.h"

class Generator
{
public:
    virtual void setSeed(unsigned int seed) = 0;
    virtual unsigned int getSeed() = 0;
    virtual void setGraph(Graph &graph) = 0;
    virtual bool generate() = 0;
};

#endif //GENERATOR_H
