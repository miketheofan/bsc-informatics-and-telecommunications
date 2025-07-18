#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <string.h>
using namespace std;

extern int currentYear;
extern int TABLESIZE;

typedef int StudentID;
typedef string LastName; 
typedef string FirstName; 
typedef int Zip; 
typedef int Year; 
typedef double GPS;
typedef int yearofStudy; 

class HashTable;
class HashNode;
class InvertedIndex;
class StudentList;

const int EMPTY = -1;

class HashNode{

private:

	StudentID id;
	LastName last;
	FirstName first;
	Zip zip;
	Year year;
	GPS gps;
	HashNode *next;

public:

	HashNode(StudentID,LastName,FirstName,Zip,Year,GPS);

	void p();

	int getID();

	string getLast();

	string getFirst();

	int getZip();

	int getYear();

	double getGps();

	HashNode* getNext();

	void setNext(HashNode*);

	void insertNext(HashNode*);

	HashNode* search(StudentID);

	HashNode* del(StudentID);

	int count(Year);

	int getZipCount(Zip);

	void deleteList();
};

class HashTable{

private:

	HashNode ** table;

public:

	HashTable();

	~HashTable();

	bool isEmpty();

	void p();

	int HashFunction(StudentID);

	bool i(StudentID,LastName,FirstName,Zip,Year,GPS);

	HashNode* search(StudentID);

	void del(StudentID);

	int n(Year);

	StudentList* t(int,Year);

	double a(Year);

	StudentList* m(Year);

	bool checkForYear(Year);

	void c();

	void postal(int);
};

class StudentList{

private:

	HashNode* student;
	StudentList* next;

public:

	StudentList(HashNode*);

	~StudentList();

	StudentList* getNext();

	void setNext(StudentList*);

	HashNode* getStudent();

	void setStudent(HashNode*);

	void p();

	void i(HashNode*);

	StudentList* del(StudentID);

	int count();

	StudentList* t(int,Year);

	HashNode* search(StudentID);

	double a();

	StudentList* m(Year);

	void deleteList();
};


class InvertedIndex{

private:

	yearofStudy Y;
	InvertedIndex *next;
	StudentList* student;

public:

	InvertedIndex();

	~InvertedIndex();

	bool isEmpty();

	int getY();

	InvertedIndex* getNext();

	StudentList* getStudent();

	void i(HashNode*,Year);

	void p();

	void del(StudentID);

	int n(Year);

	StudentList* t(int,Year);

	HashNode* search(StudentID);

	double a(Year);

	StudentList* m(Year);

	bool checkForYear(Year);

	void c();

	void postal(int);
};