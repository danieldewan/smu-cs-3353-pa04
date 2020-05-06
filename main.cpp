#include "BronKerbosch.h"
#include "DisjointSet.h"
#include <unordered_map>

using namespace std;

int main(int argc, char** argv)
{
    BronKerbosch algorithm;
    algorithm.readControlFile("control.txt");

//    DisjointSet<string> set;
//    set.makeSet("3");
//    set.makeUnion(set.set[0].front(), "4");
//    set.makeUnion(set.set[0].front(), "5");
//    set.makeUnion(set.set[0].front(), "6");
//    set.makeUnion(set.set[0].front(), "7");
//    set.makeUnion(set.set[0].front(), "8");
//    set.makeSet("9");
//    set.makeSet("11");
//    set.print();
//    set.makeUnion("9", "11");
//    set.print();
//    set.makeUnion("9", "5");
//    set.print();
//    set.remove("5");
//    set.remove("3");
//    //set.remove("7");
//    set.print();
//
//    unordered_map<string, string> map;
//    map.insert({{"5", "5"}, {"8", "8"}, {"4", "4"}, {"7", "7"}});
//
//    DisjointSet<string> intersection = set.makeIntersection(map);
//
//    intersection.print();

    return 0;
}