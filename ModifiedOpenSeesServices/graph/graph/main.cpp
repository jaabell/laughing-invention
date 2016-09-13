#include <Graph.h>
#include <Vertex.h>

#include <ID.h>
#include <OPS_Globals.h>
#include <ConsoleErrorHandler.h>
#include <StandardStream.h>

StandardStream sserr;
ostream& cerr = sserr;

int main()
{
    Vertex* v1 = new Vertex(1, 1);
    Vertex* v2 = new Vertex(2, 2);
    Vertex* v3 = new Vertex(3, 3);

    Graph* g1 = new Graph();
    g1->addVertex(v1);
    g1->addVertex(v2);
    g1->addVertex(v3);
    g1->addEdge(1, 2);
    g1->addEdge(3, 1);

    cerr << "Graph G1\n" << *g1;
    Vertex* v4 = new Vertex(2, 2);
    Vertex* v5 = new Vertex(3, 3);
    Vertex* v6 = new Vertex(4, 4);

    Graph* g2 = new Graph();

    g2->addVertex(v4);
    g2->addVertex(v5);
    g2->addVertex(v6);
    g2->addEdge(2, 4);
    g2->addEdge(3, 4);
    g2->addEdge(3, 2);

    cerr << "Graph G2\n" << *g2;

    g1->merge(*g2);
    cerr << "Graph G1 after merge with G2\n" << *g1;

    delete g1;
    delete g2;

    cerr << "Bye!\n";
}
