#ifndef SMU_CS_3353_PA04_BRONKERBOSCH_H
#define SMU_CS_3353_PA04_BRONKERBOSCH_H

#include <string>
#include <fstream>
#include "DisjointSet.h"
#include "Graph.h"

using namespace std;

class BronKerbosch {
public:
    struct Edge {
        string vertex1;
        string vertex2;
        int weight;
    };
    void readControlFile(char*);
    bool readInputFile(char*);
    void trivialAlgorithm(DisjointSet<string>, DisjointSet<string>, DisjointSet<string>);

private:
    char* inputFile;
    ofstream output;
    int numGraphs = 0;
    int numVertexes = 0;
    int numEdges = 0;
    vector<pair<Edge, int> > edges;
    DisjointSet<string> set;
    DisjointSet<string> graph;
    Graph<string> vertices;
};

void BronKerbosch::readControlFile(char *controlFile) { //control file just has the names of each graph file each on a separate line
    output.open("output.txt");
    ifstream inFile;
    char buffer[50];

    inFile.open(controlFile);
    if (inFile.is_open()) { //checks to see if control file opened
        while (!inFile.eof()) {
            inFile >> buffer;
            inputFile = buffer;

            if (readInputFile(inputFile)) { //if the data file was read in
                memset(buffer,0,sizeof(buffer));
                numGraphs++;
                DisjointSet<string> R;
                DisjointSet<string> X;

                trivialAlgorithm(R, graph, X);
//                trivialTime = algorithm(trivial);
//                efficientTime = algorithm(efficient);
//                printResults();
            }
        }
        output << "************************************************************************************************************************" << endl;
    }
    else {
        output << "Could not open control file." << endl;
    }
    output.close();
}

bool BronKerbosch::readInputFile(char *input) { //reads in graph from file and returns true if data file was read in or false if not
    ifstream inFile;
    inFile.open(input);
    numVertexes = 0;
    numEdges = 0;
    int weight = 0;
    string temp;
    string temp2;
    char buffer[50];

    if (inFile.is_open()) { //checks to see if data file opened
        inFile.ignore(1);
        inFile >> numVertexes;
        inFile.ignore(50, '\n');
        for (int i = 0; i < numVertexes; i++) {
            inFile >> temp;
            vertices.addOuter(temp);
            if (i == 0) {
                graph.makeSet(temp);
                temp2 = temp;
            }
            else {
                graph.makeUnion(temp2, temp);
            }
        }
        inFile.ignore(50, '\n');
        inFile.ignore(1);
        inFile >> numEdges;
        inFile.ignore(50, '\n');
        for (int i = 0; i < numEdges; i++) {
            getline(inFile, temp, ',');
            getline(inFile, temp2, ',');
            inFile >> weight;
            inFile.ignore(50, '\n');
            Edge newEdge;
            newEdge.vertex1 = temp;
            newEdge.vertex2 = temp2;
            newEdge.weight = weight;
            edges.push_back({newEdge, newEdge.weight});
            vertices.addInner(temp, temp2);
        }

        inFile.close();
        return true;
    }
    else {
        output << "Could not open file named '" << inputFile << "'." << endl;
        return false;
    }
}

void BronKerbosch::trivialAlgorithm(DisjointSet<string> R, DisjointSet<string> P , DisjointSet<string> X) {
    if ((P.numSubsets == 0) && (X.numSubsets == 0)) { //if sets P and X are both empty, set R is a maximal clique
        R.print();
    }

    list<string> *Plist = P.set.begin()->second.first;
    for (auto currNode: *Plist) {
        if (R.numSubsets == 0) {
            R.makeSet(currNode);
        }
        else {
            R.makeUnion(currNode, R.set.begin()->first);
        }

        unordered_map<string, string> neighbors = vertices.getNeighbors(currNode);

    }




}

#endif //SMU_CS_3353_PA04_BRONKERBOSCH_H
