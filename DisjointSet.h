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
    void print();
    int numSubsets = 0;
    unordered_map<T, pair<list<T>*, int> > set; //the set is a map with the value as the key and a pair of a list and a unique index for the list

private:
    int index = 0;

};

template <class T>
bool DisjointSet<T>::makeSet(const T value) {
    if (set.count(value) == 1) { //checks to see if the value is a key in the map
        return false;
    }
    list<T> *newList = new list<T>;
    newList->push_back(value);
    set.insert({value, {newList, index}});
    index++; //increments index so the next list added will have a different index
    this->numSubsets++;
    return true;
}

template <class T>
int DisjointSet<T>::find(const T value) { //returns the unique index paired with each list or -1 if the value is not in the map
    for (auto currMap : set) { //range-based for loop iterating over an unordered map
        if (currMap.first == value) {
            return currMap.second.second; //returns the index which is the second element of the pair which is the second element of the map
        }
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
        list<T> *temp1 = set[value1].first;
        list<T> *temp2 = set[value2].first;
        temp1->merge(*temp2); //merges the second list onto the first list
        for (auto currNode: *temp1) { //updating each int in the list
            set[currNode].first = temp1;
            set[currNode].second = set[value1].second;
        }
        this->numSubsets--;
    }
}

template <class T>
DisjointSet<T> DisjointSet<T>::makeIntersection(unordered_map<T, T> set2) {
    DisjointSet<T> intersection;
    for (auto currNode : set) {
        if (set2.count(currNode.first) == 1) {
            if (intersection.numSubsets == 0) {
                intersection.makeSet(currNode.first);
            } else {
                intersection.makeUnion(currNode.first, intersection.set.begin()->first);
            }
        }
    }
    return intersection;
}

template <class T>
void DisjointSet<T>::print() {
    unordered_map<int, int> visitedLists;
    cout << "{";
    int i = 0;
    for (auto currMap : set) { //range-based for loop iterating over an unordered map
        list<T> *currList = currMap.second.first;
        int currListIndex = currMap.second.second;
        if (visitedLists.count(currListIndex) != 1) {
            if (i != 0) {
                cout << ", ";
            }
            cout << "[";
            int j = 0;
            for (auto currNode: *currList) { //range-based for loop iterating over a list
                cout << currNode;
                if ((j + 1) != currList->size()) {
                    cout << ", ";
                }
                visitedLists.insert({currListIndex, currListIndex});
                j++;
            }
            cout << "]";
        }
        i++;
    }
    cout << "} " << "Subsets: " << this->numSubsets << endl;
}

#endif //SMU_CS_3353_PA04_DISJOINTSET_H
