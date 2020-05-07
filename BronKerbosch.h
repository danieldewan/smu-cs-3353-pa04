#ifndef SMU_CS_3353_PA04_BRONKERBOSCH_H
#define SMU_CS_3353_PA04_BRONKERBOSCH_H

#include <string>
#include <fstream>
#include "DisjointSet.h"
#include "Graph.h"
#include <algorithm> //needed for sort() function
#include <ctime> //needed to use clock to time algorithm
#include <iomanip> //needed for setprecision()
#include <cstring> //needed for memset()

using namespace std;

class BronKerbosch {
public:
    void readControlFile(char*);
    bool readInputFile(char*);
    //both versions of the algorithm were implemented using the logic from https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
    //the method for choosing the pivot of the optimized algorithm uses logic from https://hal.inria.fr/inria-00070393/PDF/RR-5615.pdf
    void algorithmWithoutPivot(DisjointSet<string>, DisjointSet<string>, DisjointSet<string>);
    void algorithmWithPivot(DisjointSet<string>, DisjointSet<string>, DisjointSet<string>);
    void printClique(DisjointSet<string>);

private:
    char* inputFile;
    ofstream output;
    int numGraphs = 0;
    int numVertexes = 0;
    int numEdges = 0;

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
                DisjointSet<string> graph;
                for (int i = 0; i < vertices.outerNodes.size(); ++i) {
                    graph.insert(vertices.outerNodes[i]->data);
                }
                numGraphs++;
                DisjointSet<string> R;
                DisjointSet<string> X;
                output << "************************************************************************************************************************" << endl;
                output << "Graph " << numGraphs << ": " << "'" << inputFile << "'" << endl;
                output << "Number of Vertices in the Graph: " << numVertexes << endl;
                output << "Number of Edges in the Graph : " << numEdges << endl;
                output << "Maximal Cliques: " << endl;
                output << "Algorithm without pivot: ";
                clock_t start1 = clock();
                algorithmWithoutPivot(R, graph, X);
                double time1 = (clock() - start1) / (double) CLOCKS_PER_SEC;

                output << endl << "Algorithm with pivot: ";
                clock_t start2 = clock();
                algorithmWithPivot(R, graph, X);

                double time2 = (clock() - start2) / (double) CLOCKS_PER_SEC;

                output << endl;
                output << "Time for Algorithm without pivot: " << setprecision(3) << time1 << " seconds" << endl;
                output << "Time for Algorithm with pivot: " << setprecision(3) << time2 << " seconds" << endl;
                memset(buffer,0,sizeof(buffer)); //clears the buffer
            }
        }
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

        //custom sort function based on decreasing vertex degree using a lambda expression
        sort(vertices.outerNodes.begin(), vertices.outerNodes.end(), [](Outer<string>*  node1, Outer<string>*  node2) {
            return node1->innerNodes.size() > node2->innerNodes.size();
        });

        inFile.close();
        return true;
    }
    else {
        output << "Could not open file named '" << inputFile << "'." << endl;
        return false;
    }
}
//P is initially contains all the vertices in the graph and R and X are empty sets
void BronKerbosch::algorithmWithoutPivot(DisjointSet<string> R, DisjointSet<string> P , DisjointSet<string> X) {
        if ((P.numSubsets == 0) && (X.numSubsets == 0)) { //if sets P and X are both empty, set R is a maximal clique
            printClique(R);
        }
        if ((P.set.size() > 0) && (P.set[0].size() > 0)) { //checks to see if the disjoint set is empty and if the first subset is empty
            DisjointSet<string> tempR;
            auto temp = P.set[0]; //sets temp to the first subset in disjoint set
            for (auto currNode: temp) {
                tempR.set = R.set;
                tempR.insert(currNode); //inserts currNode into first subset in disjoint set

                unordered_map<string, string> neighbors = vertices.getNeighbors(currNode); //gets neighbors of currNode
                DisjointSet<string> tempP = P.makeIntersection(neighbors); //tempP is the intersection of P and neighbors
                DisjointSet<string> tempX = X.makeIntersection(neighbors); //tempX is the intersection of X and neighbors

                algorithmWithoutPivot(tempR, tempP, tempX); //recursively calls algorithm
                P.remove(currNode); //moves currNode from P and X to exclude currNode from consideration in future cliques
                X.insert(currNode); //inserts currNode into X
            }
        }
}

//P is initially contains all the vertices in the graph and R and X are empty sets
void BronKerbosch::algorithmWithPivot(DisjointSet<string> R, DisjointSet<string> P, DisjointSet<string> X) {

    if ((P.numSubsets == 0) && (X.numSubsets == 0)) { //if sets P and X are both empty, set R is a maximal clique
        printClique(R);
    }
    if ((P.set.size() > 0) && (P.set[0].size() > 0)) { //checks to see if the disjoint set is empty and if the first subset is empty
        auto pivotVertex = P.set[0].front(); //gets first vertex which has the highest degree
        unordered_map<string, string> pivotNeighbors = vertices.getNeighbors(pivotVertex);
        DisjointSet<string> reducedSet = P.removeMultiple(pivotNeighbors);
        DisjointSet<string> tempR;
        auto temp = reducedSet.set[0]; //sets temp to the first subset in disjoint set
        for (auto currNode: temp) {
            tempR.set = R.set;
            tempR.insert(currNode); //inserts currNode into first subset in disjoint set

            unordered_map<string, string> neighbors = vertices.getNeighbors(currNode);
            DisjointSet<string> tempP = P.makeIntersection(neighbors); //tempP is the intersection of P and neighbors
            DisjointSet<string> tempX = X.makeIntersection(neighbors); //tempX is the intersection of X and neighbors

            algorithmWithPivot(tempR, tempP, tempX); //recursively calls algorithm
            P.remove(currNode); //moves currNode from P and X to exclude currNode from consideration in future cliques
            X.insert(currNode); //inserts currNode into X
        }
    }
}

void  BronKerbosch::printClique(DisjointSet<string> clique) {
    output << "| {";
    int i =0;
    for (auto currNode : clique.set[0]) {
        output << currNode;
        if ((i + 1) != clique.set[0].size()) {
            output << ", ";
        }
        i++;
    }
    output << "}" << " ";
}

#endif //SMU_CS_3353_PA04_BRONKERBOSCH_H
