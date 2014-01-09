#include "GraphImpl.h"
#include "DOTExporter.h"
#include "BuilderGenerator.h"
#include "Algorithms.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

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
    double d;

    cout << "Number of nodes: ";
    cin >> x;
    gen.setNodesNum(x);

    cout << "Number of edges (use 0 for completeness): ";
    cin >> x;
    gen.setEdgesNum(x);

    cout << "Graph completness (0..1 - 0 = use edges num): ";
    cin >> y;
    gen.setCompleteness(y);

    cout << "Set seed for generation (0 = random seed): ";
    cin >> x;
    gen.setSeed(x);

    cout << "Set seed for computation (0 = random seed): ";
    cin >> x;
    alg.setSeed(x);

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

//    cout << "Set number of iteration per node in Monte Carlo sim.: ";
//    cin >> x;
//    alg.setIterationsPerNode(x);
	cout << "Set limit of iteration in Monte Carlo sim.: ";
	cin >> x;
	alg.setIterationsLimit(x);
	cout << "Set confidence in Monte Carlo sim.: ";
	cin >> d;
	alg.setConfidence(d);
	cout << "Set precision in Monte Carlo sim.: ";
	cin >> d;
	alg.setPrecision(d);
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

    cout << "### DONE ### " << endl;
    cout.flush();
    float res = alg.monteCarlo2();
    cout << "Result: " << res * 100 << " %" << " -+" << alg.getPrecision() <<
    		" (" << alg.getConfidence() * 100 << " 	% confidence)" << endl;

    cout << "Alg seed: " << alg.getSeed() << endl;

    cout << "Calculating edge & node weakness...";
    cout.flush();
    alg.edgeWeakness();
    alg.nodeWeakness();
    dot.exportToFile("weakness.dot");
    cout << endl;

    return EXIT_SUCCESS;
}
