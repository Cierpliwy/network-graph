#include "GraphImpl.h"
#include "DOTExporter.h"

int main(int argc, char *argv[])
{
    GraphImpl impl;
    DOTExporter dot;
    auto n1 = impl.addNode(Node(1));
    auto n2 = impl.addNode(Node(2));
    auto n3 = impl.addNode(Node(3));
    auto n4 = impl.addNode(Node(4));
    auto n5 = impl.addNode(Node(5));
    auto n6 = impl.addNode(Node(6));
    auto n7 = impl.addNode(Node(7));
    impl.addEdge(n1,n2, Edge(0.99));
    impl.addEdge(n1,n6, Edge(0.32));
    impl.addEdge(n4,n1, Edge(0.1));
    impl.addEdge(n5,n3, Edge(0.2));
    impl.addEdge(n7,n2, Edge(0.6));
    dot.setGraph(impl);
    dot.exportToFile("1.dot");
    impl.addEdge(n3,n7, Edge(0.2));
    dot.exportToFile("2.dot");
    impl.addEdge(n4,n5, Edge(0.2));
    dot.exportToFile("3.dot");
    impl.removeNode(n6);
    dot.exportToFile("4.dot");
    impl.removeNode(n3);
    impl.removeNode(n1);
    dot.exportToFile("5.dot");
    GraphImpl lol;
    impl.clone(lol);
    dot.setGraph(lol);
    dot.exportToFile("5clone.dot");

    return 0;
}
