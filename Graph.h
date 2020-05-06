#ifndef SMU_CS_3353_PA04_GRAPH_H
#define SMU_CS_3353_PA04_GRAPH_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <fstream>
#include <unordered_map>

using namespace std;

template <class T> class Outer; //outer nodes in adjacency list
template <class T> class Graph; //implemented as an adjacency list

template <class T>
class Inner { //inner nodes in adjacency list
public:
    friend class Graph<T>;
    friend class Outer<T>;
    Outer<T> *node;
    bool visited;
    Inner(Outer<T> *newNode):node(newNode), visited(false){} //inner constructor
};

template <class T>
class Outer {
public:
    friend class Graph<T>;
    list<Inner<T> > innerNodes; //each outher node has a list of inner nodes
    const T data;
    int layer; //how far the current node is from the source node
    double pathsTo; //number of shortest paths from source node to this node
    double pathsThrough; //number of shortest paths from source node passing through this node (always at least one)
    Outer<T> *prevNode;
    bool visited;
    Outer():data(nullptr), visited(false), prevNode(nullptr), layer(-1), pathsTo(0.0), pathsThrough(1.0){}
    Outer(const T outerData):data(outerData), visited(false), prevNode(nullptr), layer(-1), pathsTo(0.0), pathsThrough(1.0){}
    const T getData() const {return data;}
    void addInner(Outer<T> *node) { //adds a node to the inner list of an outer node
        Inner<T> newInner(node);
        innerNodes.push_back(newInner);
    }
    bool removeInner(const T removeNode) { //removes an inner node from the list of an outer node
        auto curr = innerNodes.begin(); //auto is used to iterate through lists
        while (curr != innerNodes.end()) {
            if (curr->node->data == removeNode) {
                innerNodes.erase(curr);
                return true;
            }
            curr++;
        }
        return false;
    }
};


template <class T>
class Graph {
public:
    vector<Outer<T>* > outerNodes; //the outer nodes are stored in a vector, so the entire graph is a vector of lists
    unordered_map<T, double> edges; //stores the edges between nodes and their betweenness scores
    bool addOuter(const T);
    bool addInner(const T , const T);
    void removeInner(const T, const T);
    Outer<T>* goToNode(const T);
    bool contains(const T);
    unordered_map<T, T> getNeighbors(const T);
    void print();
};

template <class T>
bool Graph<T>::addOuter(const T data) { //adds an outer node to vector of outer nodes in graph
    typename vector<Outer<T>*>::iterator curr = outerNodes.begin(); //typename is used to iterate through vectors

    while(curr != outerNodes.end()) {
        if ((*curr)->data == data) {
            return false; //if outer is already in graph
        }
        curr++;
    }
    Outer<T> *newOuter = new Outer<T>(data);
    outerNodes.push_back(newOuter);
    return true; //if the node has been added to the outer nodes
}

template <class T>
bool Graph<T>::addInner(const T outerData, const T innerData) { //adds an inner node to the list of a specific outer node in the vector
    typename vector<Outer<T>*>::iterator curr = outerNodes.begin();
    Outer<T> *newNode;

    while ((*curr)->data != outerData) {
        if (curr == outerNodes.end()) {
            return false; //if the outer node is not in the vector
        }
        curr++;
    }
    if ((*curr)->data == outerData) {
        newNode = new Outer<T>(innerData); //creates a new outer node
        (*curr)->addInner(newNode); //adds new node to inner
    }
    else {
        return false;
    }
    //repeats the same process again except the first node is now the outer node and the second is the inner node
    curr = outerNodes.begin();
    while ((*curr)->data != innerData) {
        if (curr == outerNodes.end()) {
            return false; //if the outer node is not in the vector
        }
        curr++;
    }
    newNode = new Outer<T>(outerData); //creates a new outer node
    (*curr)->addInner(newNode); //adds new node to inner

    return true; //if the nodes were added to the graph
}

template <class T>
void Graph<T>::removeInner(const T edge1, const T edge2) { //removes an inner node to the list of a specific outer node in the vector
    Outer<T>* temp = goToNode(edge1); //goes to specific outer node
    if (temp->removeInner(edge2)) { //returns true if it was possible to remove that edge
        temp = goToNode(edge2);
        temp->removeInner(edge1); //removes the first node from the second node as the outer node

        if(edges.find(edge1 + "-" + edge2) != edges.end()) { //removes the edge between the two nodes
            edges.erase(edge1 + "-" + edge2);
        }
        else if (edges.find(edge2 + "-" + edge1) != edges.end()){
            edges.erase(edge2 + "-" + edge1);
        }
    }
}

template <class T>
Outer<T>* Graph<T>::goToNode(const T startNode) { //goes to a specific outer node based specific data
    typename vector<Outer<T>*>::iterator curr = outerNodes.begin();
    if ((*curr)->data != startNode) {
        while ((*curr)->data != startNode) {
            curr++;
        }
    }
    return *curr;
}

template <class T>
bool Graph<T>::contains(const T startNode) {
    typename vector<Outer<T>*>::iterator curr = outerNodes.begin();
    while (curr != outerNodes.end()) {
        if ((*curr)->data == startNode) {
            return true;
        }
        curr++;
    }
    return false;
}

template <class T>
unordered_map<T, T> Graph<T>::getNeighbors(const T node) {
    unordered_map<T, T> neighbors;
    Outer<T>* outerNode = goToNode(node);
    auto innerCurr = outerNode->innerNodes.begin();
    while (innerCurr != outerNode->innerNodes.end()) {
        neighbors.insert({innerCurr->node->getData(), innerCurr->node->getData()});
        innerCurr++;
    }
    return neighbors;
}

template <class T>
void Graph<T>::print() { //prints all outer nodes and their inner nodes in the graph
    typename vector<Outer<T>*>::const_iterator outerCurr = outerNodes.begin();

    while (outerCurr != outerNodes.end()) {
        cout << (*outerCurr)->getData() << ": ";
        auto innerCurr = ((*outerCurr)->innerNodes).begin();

        while (innerCurr != ((*outerCurr)->innerNodes).end()) {
            cout << innerCurr->node->getData() << " ";
            innerCurr++;
        }
        cout << endl;
        outerCurr++;
    }
    cout << endl;
}

#endif //SMU_CS_3353_PA04_GRAPH_H
