#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <bitset>
#include <set>
#include <queue>

using namespace std;

#include "./items.h"
#include "./HashTable.h"
#include "./H.h"

class HyperCube{

private:

	int k;
	int w;
	int M;
	int probes;
	int dimension;
	vector<item*> items;
	vector<unordered_map<int32_t,int>> fVector;
	set<int32_t> nearVertices;
	HashTable* cube;
	vector<Hi*> hVector;

public:

	HyperCube(int,int,int,int,int);
	~HyperCube();
	void insert(item*);
	int coinFlip();
	int32_t hashFunction(item*);
	vector<pair<double,item*>> findkNN(item*,int,int);
	vector<pair<item*,double>> findRange(int,item*,int);
	set<int32_t> HammingDist(int32_t , int, int);
	vector<item*> getItems();

};

#endif