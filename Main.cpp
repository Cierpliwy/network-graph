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
    if (gen.generate()) cout << "OK!";
    else cout << "ERR!";
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
    generate("Simple chain of X nodes");

    // 2. Binary tree
    gen.setMaxTreeAdjNodes(3);
    generate("Binary tree");

    // 3. Complete graph
    gen.setCompleteness(1);
    generate("Complete graph");
    
    // 4. Graph with only two cycles
    gen.setEdgesNum(n+1);
    gen.setCompleteness(0);
    generate("Graph with only two cycles");

    // 5. Half complete graph
    gen.setEdgesNum(0);
    gen.setCompleteness(0.5);
    gen.setMaxTreeAdjNodes(n/4);
    generate("Half complete graph");

    dot.setGraph(graph);
    cout << "Saving to 'demo.dot'...";
    if (dot.exportToFile("demo.dot")) cout << "OK!";
    else cout << "ERR!";
    cout << endl;

    return EXIT_SUCCESS;
}
