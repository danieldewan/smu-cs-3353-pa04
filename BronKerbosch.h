#ifndef SMU_CS_3353_PA04_BRONKERBOSCH_H
#define SMU_CS_3353_PA04_BRONKERBOSCH_H

#include <string>
#include <fstream>
#include "DisjointSet.h"
#include "Graph.h"
#include<algorithm>

using namespace std;

class BronKerbosch {
public:
    struct Vertex {
        string name;
        int degree; //number of edges connecting to the vertex
    };

    void readControlFile(char*);
    bool readInputFile(char*);
    void algorithmWithoutPivot(DisjointSet<string>, DisjointSet<string>, DisjointSet<string>);
    void algorithmWithPivot(DisjointSet<string>, DisjointSet<string>, DisjointSet<string>);

private:
    char* inputFile;
    ofstream output;
    int numGraphs = 0;
    int numVertexes = 0;
    int numEdges = 0;

    DisjointSet<string> graph;
    Graph<string> vertices;
    unordered_map<int, pair<string, int>> x;
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
                algorithmWithoutPivot(R, graph, X);

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

            vertices.addInner(temp, temp2);
        }

        //custom sort function based on decreasing vertex degree using lambda expression
        sort(vertices.outerNodes.begin(), vertices.outerNodes.end(), [](Outer<string>*  node1, Outer<string>*  node2) {
            return node1->innerNodes.size() > node2->innerNodes.size();
        });
        for (int i = 0; i < vertices.outerNodes.size(); ++i) {
            graph.insert(vertices.outerNodes[i]->data);
        }

        inFile.close();
        return true;
    }
    else {
        output << "Could not open file named '" << inputFile << "'." << endl;
        return false;
    }
}

void BronKerbosch::algorithmWithoutPivot(DisjointSet<string> R, DisjointSet<string> P , DisjointSet<string> X) {
    if ((P.numSubsets == 0) && (X.numSubsets == 0)) { //if sets P and X are both empty, set R is a maximal clique
        R.print();
    }
    if ((P.set.size() > 0) && (P.set[0].size() > 0)) {
        DisjointSet<string> tempR;
        auto temp = P.set[0];
        for (auto currNode: temp) {
            tempR.set = R.set;
            tempR.insert(currNode);

            unordered_map<string, string> neighbors = vertices.getNeighbors(currNode);
            DisjointSet<string> tempP = P.makeIntersection(neighbors);
            DisjointSet<string> tempX = X.makeIntersection(neighbors);

            algorithmWithoutPivot(tempR, tempP, tempX);
            P.remove(currNode);
            X.insert(currNode);
            if (P.set[0].size() == 0) {
                break;
            }
        }
    }
}

void BronKerbosch::algorithmWithPivot(DisjointSet<string> R, DisjointSet<string> P, DisjointSet<string> X) {
    if ((P.numSubsets == 0) && (X.numSubsets == 0)) { //if sets P and X are both empty, set R is a maximal clique
        R.print();
    }
    if ((P.set.size() > 0) && (P.set[0].size() > 0)) {
        DisjointSet<string> tempR;
        auto temp = P.set[0];
        for (auto currNode: temp) {
            tempR.set = R.set;
            if (tempR.set.size() == 0) {
                tempR.makeSet(currNode);
            } else {
                tempR.makeUnion(currNode, tempR.set[0].front());
            }

            unordered_map<string, string> neighbors = vertices.getNeighbors(currNode);
            DisjointSet<string> tempP = P.makeIntersection(neighbors);
            DisjointSet<string> tempX = X.makeIntersection(neighbors);

            algorithmWithoutPivot(tempR, tempP, tempX);
            P.remove(currNode);
            if (X.numSubsets == 0) {
                X.makeSet(currNode);
            } else {
                X.makeUnion(currNode, X.set[0].front());
            }
            if (P.set[0].size() == 0) {
                break;
            }
        }
    }
}

#endif //SMU_CS_3353_PA04_BRONKERBOSCH_H
