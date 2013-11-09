#include "GraphImpl.h"
#include "DOTExporter.h"
#include "BuilderGenerator.h"
#include <iostream>
#include <ctime>
using namespace std;

GraphImpl graph;
DOTExporter dot;
BuilderGenerator gen;

void generate(const string& info) {
    cout << info << "...";
    if (gen.generate()) {
        cout << "OK!";
    } else {
        cout << "ERR!";
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2 ) {
        cout << "Set n number!" << endl;
        return EXIT_FAILURE;
    }

    unsigned int n = atoi(argv[1]);

    // Create set of 5 graphs
    // ---------------------
    // 1. Simple chain of X nodes.
    gen.setSeed(time(nullptr));
    gen.setCompleteness(0);
    gen.setEdgesNum(0);
    gen.setNodesNum(n);
    gen.setGraph(graph);
    gen.setMinProb(0.2);
    gen.setMaxProb(1);
    gen.setMaxTreeAdjNodes(2);
    cout << "Nodes: " << graph.getNodesNum() << endl;
    generate("Simple chain of X nodes");

    // 2. Binary tree
    gen.setMaxTreeAdjNodes(3);
    cout << "Nodes: " << graph.getNodesNum() << endl;
    generate("Binary tree");

    // 3. Graph with only one cycle
    gen.setEdgesNum(n);
    generate("Graph with only one cycle");

    // 4. Complete graph
    gen.setCompleteness(1);
    generate("Complete graph");

    // 5. Random tree
    gen.setMaxTreeAdjNodes(n);
    gen.setCompleteness(0);
    gen.setEdgesNum(0);
    generate("Random tree");

    dot.setGraph(graph);
    dot.exportToFile("demo.dot");

    return EXIT_SUCCESS;
}
