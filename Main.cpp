#include "GraphImpl.h"
#include "DOTExporter.h"
#include "BuilderGenerator.h"
#include "Algorithms.h"
#include <iostream>
#include <ctime>
using namespace std;

GraphImpl graph,graph2;
DOTExporter dot;
BuilderGenerator gen;

int main(int, char **)
{
    GraphImpl graph;
    DOTExporter dot;
    BuilderGenerator gen;
    GraphAlgorithms alg;
    unsigned int x;
    float y;

    cout << "Number of nodes: ";
    cin >> x;
    gen.setNodesNum(x);

    cout << "Number of edges (use 0 for completeness): ";
    cin >> x;
    gen.setEdgesNum(x);

    cout << "Graph completness (0..1 - 0 = use edges num): ";
    cin >> y;
    gen.setCompleteness(y);

    cout << "Set seed (0 = random seed): ";
    cin >> x;
    gen.setSeed(x);

    cout << "Set min. reliability of edge: ";
    cin >> y;
    gen.setMinProb(y);

    cout << "Set max. reliability of edge: ";
    cin >> y;
    gen.setMaxProb(y);

    cout << "Set max. number of ajacent nodes in tree pass: ";
    cin >> x;
    gen.setMaxTreeAdjNodes(x);
    gen.setGraph(graph);

    cout << "Set number of iteration per node in Monte Carlo sim.: ";
    cin >> x;
    alg.setIterationsPerNode(x);
    alg.setGraph(graph);

    cout << "Use relative colors (0 = false): ";
    cin >> x;
    alg.setRelativeColors(x);

    dot.setGraph(graph);

    cout << "Generating graph... ";
    cout.flush();
    cout << (gen.generate() ? "OK" : "ERR");
    cout << endl;
    cout << "Used seed: " << gen.getSeed() << endl;

    cout << "Is connected: ";
    cout.flush();
    cout << (alg.isConnected() ? "Yes" : "No");
    cout << endl;
    dot.exportToFile("graph.dot");

    cout << "Reliability: ";
    cout.flush();
    cout << alg.monteCarlo() * 100 << "%" << endl;

    cout << "Calculating edge & node weakness...";
    cout.flush();
    alg.edgeWeakness();
    alg.nodeWeakness();
    dot.exportToFile("weakness.dot");
    cout << endl;

    return EXIT_SUCCESS;
}
