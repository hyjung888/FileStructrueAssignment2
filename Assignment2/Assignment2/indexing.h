#include <cstring>
#include <fstream>

#pragma once
#define HASH_STUDENT_FILENAME "Students.hash"
#define DB_STUDENT_FILENAME "Students.DB"
#define HASH_PROF_FILENAME "Prof.hash"
#define DB_PROF_FILENAME "Prof.DB"
#define QUERY_ANS_FILENAME "query.res"
#define HASH_LOAD_FACTOR 125

#define BLOCK_LOAD_FACTOR 127


using namespace std;
struct Node {
	int data;
	Node* next;
} ;
struct HashNode {
	int key;

	int count;
	Node* list;
} ;

struct HashTable {
	int size;
	int numOfData;
	struct HashNode** table;
	//vector<struct HashNode> table;
} ;

struct Students {
	char name[20] = { 0, };
	int StudentID = 0;
	float score = 0;
	int advisorID = 0;
};

struct Block {
	int num;
	int cnt;
	int tmp[6] = { 0,0,0,0,0,0 };
	Students stud[BLOCK_LOAD_FACTOR];
};

struct BlockTable {
	int size;
	int numOfData;
	struct Block** block;
};

struct Prof {
	char name[20] = { 0, };
	int ProfID = 0;
	int Salary = 0;
	int empty = 0;
};

struct ProfBlock {
	int num;
	int cnt;
	int tmp[6] = { 0,0,0,0,0,0 };
	Prof profs[BLOCK_LOAD_FACTOR];
};
struct ProfBlockTable {
	int size;
	int numOfData;
	struct ProfBlock** block;
};
HashTable* CreateHashTable(int tablesize, fstream& Hash, fstream& DB);
int GetKey(int data, int tableSize);
bool HashSearch(HashTable* hTable, int data);
HashTable* HashInsert(HashTable* hTable, Students& stud, fstream& Hash, fstream& DB);
HashTable* HashInsert(HashTable* hTable, Prof& prof, fstream& Hash, fstream& DB);

void HashDelete(HashTable* hTable, int data);

void DeleteHashTable(HashTable* hTable);
void printHashTable(HashTable* hTable);
HashTable* ReHashStud(HashTable* hTable, fstream& Hash,  fstream& DB,char* fileName);
HashTable* ReHashProf(HashTable* hTable, fstream& Hash,fstream& DB, char* fileName);

void saveNodeStud(Node* node, fstream& Hash, int hashNum, int idx, char* fileName);

void saveNodeProf(Node* node, fstream& Hash, int hashNum, int idx, char* fileName);
void saveHashNode(HashNode* hNode, fstream& Hash,int hashNum, char* fileName);
void saveHashTable(HashTable* hTable, fstream& Hash, char* fileName);


//HashTable* loadHashNode(fstream& Hash, HashTable *hTable, fstream& DB);
//HashTable* loadHashFile(fstream& Hash, HashTable *hTable, fstream& DB);