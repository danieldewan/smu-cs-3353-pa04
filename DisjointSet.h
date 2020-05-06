#ifndef SMU_CS_3353_PA04_DISJOINTSET_H
#define SMU_CS_3353_PA04_DISJOINTSET_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

template <class T>
class DisjointSet {
public:
    bool makeSet(const T);
    int find(const T);
    void makeUnion(const T, const T);
    DisjointSet<T> makeIntersection(unordered_map<T, T>);
    void insert(const T);
    bool remove(const T);
    DisjointSet<T> removeMultiple(unordered_map<T, T>);
    void clear();
    void print();
    int numSubsets = 0;
    vector<list<T >> set;
};

template <class T>
bool DisjointSet<T>::makeSet(const T value) {
    for (auto currSet : set) { //range-based for loop iterating over a vector
        for (auto currNode: currSet) { //range-based for loop iterating over a list
            if (currNode == value) {
                return false;
            }
        }
    }
    list<T> newList;
    newList.push_back(value);
    set.push_back(newList);
    this->numSubsets++;

    return true;
}

template <class T>
int DisjointSet<T>::find(const T value) { //returns the unique index paired with each list or -1 if the value is not in the map
    int index = 0;
    for (auto currSet : set) { //range-based for loop iterating over a vector
        for (auto currNode: currSet) { //range-based for loop iterating over a list
            if (currNode == value) {
                return index;
            }
        }
        index++;
    }
    return -1;
}

template <class T>
void DisjointSet<T>::makeUnion(const T value1, const T value2) {
    makeSet(value1); //checks to see if value1 is in a set and if not it makes a new set containing it
    makeSet(value2); //checks to see if value2 is in a set and if not it makes a new set containing it
    if (find(value1) != find(value2)) { //checks to see if values are already in the same set
        int index1 = find(value1);
        int index2 = find(value2);
        set[index1].merge(set[index2]); //merges the second list onto the first list
        set.erase(set.begin() + index2); //removes the second list from the vector
        this->numSubsets--;
    }
}

template <class T>
DisjointSet<T> DisjointSet<T>::makeIntersection(unordered_map<T, T> set2) {
    DisjointSet<T> intersection;
    if (set.size() > 0) {
        for (auto currNode : set[0]) {
            if (set2.count(currNode) == 1) {
                intersection.insert(currNode);
            }
        }
    }
    return intersection;
}

template <class T>
void DisjointSet<T>::insert(const T value) {
    if (set.size() == 0) {
        makeSet(value);
    } else {
        set[0].push_back(value);
    }
}

template <class T>
bool DisjointSet<T>::remove(const T value) {
    for (auto currNode : set[0]) {
        if (currNode == value) {
            set[0].remove(value);
            return true;
        }
    }
    return false;
}

template <class T>
DisjointSet<T> DisjointSet<T>::removeMultiple(unordered_map<T, T> set2) {
    DisjointSet<T> reducedSet;
    if (set.size() > 0) {
        for (auto currNode : set[0]) {
            if (set2.count(currNode) == 0) {
                reducedSet.insert(currNode);
            }
        }
    }
    return reducedSet;
}

template <class T>
void DisjointSet<T>::clear() {

}

template <class T>
void DisjointSet<T>::print() {
    cout << "{";
    int i = 0;
    for (auto currSet : set) { //range-based for loop iterating over a vector
        cout << "[";
        int j = 0;
        for (auto currNode: currSet) { //range-based for loop iterating over a list
            cout << currNode;
            if ((j + 1) != currSet.size()) {
                cout << ", ";
            }
            j++;
        }
        cout << "]";
        if ((i + 1) != set.size()) {
            cout << ", ";
        }
        i++;
    }
    cout << "} " << "Subsets: " << this->numSubsets << endl;
}

#endif //SMU_CS_3353_PA04_DISJOINTSET_H
