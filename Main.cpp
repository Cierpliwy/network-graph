#include "GraphImpl.h"
#include "DOTExporter.h"
#include "BuilderGenerator.h"
#include "Algorithms.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
using namespace std;

GraphImpl graph,graph2;
DOTExporter dot;
BuilderGenerator gen;
bool inTerminal = false;

template<class T> void getInput(const std::string &msg, T &value)
{
    cout << " - " << msg << ": ";
    cin >> value;
    if (!inTerminal) {
        cout << value << endl;
    }
}

int main(int, char **)
{
    GraphImpl graph;
    DOTExporter dot;
    BuilderGenerator gen;
    GraphAlgorithms alg;
    unsigned int x;
    float y;
    double d;
    bool zeroSeed = false;

    // Check if we are using terminal as an input
    if (isatty(fileno(stdin)))
        inTerminal = true;

    // Get parameters for generating
    cout << "GENERATOR PARAMS:" << endl;
    getInput("Number of nodes", x);
    gen.setNodesNum(x);

    getInput("Number of edges (use 0 for completeness)", x);
    gen.setEdgesNum(x);

    getInput("Graph completeness (0..1 - 0 = use edges num)", y);
    gen.setCompleteness(y);

    getInput("Set seed for generation (0 = random seed)", x);
    gen.setSeed(x);
    if (x == 0) zeroSeed = true;

    getInput("Set seed for computation (0 = random seed)", x);
    alg.setSeed(x);

    getInput("Set min. reliability of an edge", y);
    gen.setMinProb(y);

    getInput("Set max. reliability of an edge", y);
    gen.setMaxProb(y);

    getInput("Set max. number of adjacent nodes in tree pass", x);
    gen.setMaxTreeAdjNodes(x);

    // Set graph for generation
    gen.setGraph(graph);

    // Set monte carlo paramteres
    cout << "MONTE-CARLO PARAMS:" << endl;
	getInput("Set limit of iteration in Monte Carlo sim.", x);
	alg.setIterationsLimit(x);

	getInput("Set confidence in Monte Carlo sim. as fraction", d);
	alg.setConfidence(d);

	getInput("Set precision in Monte Carlo sim.", d);
	alg.setPrecision(d);

    // Set graph used by algorithms
    alg.setGraph(graph);

    // Additional settings
    cout << "VISUAL SETTINGS:" << endl;
    getInput("Use relative colors (0 = false)", x);
    alg.setRelativeColors(x);

    // Set graph for exporter
    dot.setGraph(graph);

    cout << "GENERATING GRAPH:" << endl;
    cout.flush();
    const unsigned int genLimit = 10000;
    unsigned int i;
    for (i = 0; i < genLimit; ++i) {
        graph.reset();
        if (!gen.generate()) {
            cout << "ERROR: Generation error..." << endl;
            return 1;
        }
        if (!alg.hasBridge()) break;
        if (zeroSeed) gen.setSeed(0);
        else {
            cout << "ERROR: Unable to generate graph with specified seed" << endl;
            return 1;
        }
    }
    if (i == genLimit) {
        cout << "ERROR: Generation limit was exceeded" << endl;
        return 1;
    }
    cout << "- Total generations: " << i + 1 << endl;
    cout << "- Used seed: " << gen.getSeed() << endl;

    string fileName = to_string(gen.getSeed());
    fileName += ".dot";

    cout << "- File saved as: " << fileName << endl;
    dot.exportToFile(fileName);

    cout << "CALCULATING MONTE CARLO:" << endl;

    float res = alg.monteCarlo2();
    cout << "- Result: " << res * 100 << " %" << " -+" << alg.getPrecision() 
         << " % (" << alg.getConfidence() * 100 << "%" << " confidence)" << endl;
    cout << "- Iterations: " << alg.getLastIterations() << endl;
    cout << "- Algorithm seed: " << alg.getSeed() << endl;

    cout << "Calculating edge & node weakness...";
    cout.flush();
    alg.edgeWeakness();
    alg.nodeWeakness();
    dot.exportToFile("weakness.dot");
    cout << endl;

    return EXIT_SUCCESS;
}
