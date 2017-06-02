
#include <iostream>
#include <cstdlib>
#include <vector>

#ifndef INDEXING_H
#define INDEXING_H
#define HASH_FILENAME "Students.hash"

#include "format.h"


using namespace std;

struct Node {
	int data;
	Node* next;
};

struct HashNode {
	int key;
	int count;
	Node* list;
};


struct HashTable {
	int size;
	int numOfData;
	struct HashNode** table;
	//vector<struct HashNode> table;
};

int BinaryToInt(char* charNum);

struct HashTable* ReHash(struct HashTable* hTable);
struct HashTable* CreateHashTable(int tablesize) {
	//create
	struct HashTable* hTable;
	hTable = new HashTable();
	if (!hTable) {
		cout << "Memory Allocation Fail" << endl;
		cout << "HashTable not create" << endl;
		return NULL;
	}

	hTable->size = tablesize;
	hTable->numOfData = 0;
	hTable->table = new HashNode*[tablesize]();
	
	if (!hTable->table)
	{
		cout << "Memory Allocation Fail" << endl;
		cout << "HashTable->table not create" << endl;
		return NULL;
	}

	for (int i = 0; i < tablesize; i++) {
		hTable->table[i] = new HashNode();
		if (!hTable->table[i]) {
			cout << "Memory Allocation Fail" << endl;
			cout << "HashTable->table["<<i<<"] not create" << endl;
			return NULL;
		}

		hTable->table[i]->key = i;
		hTable->table[i]->count = 0;
		hTable->table[i]->list = NULL;
	}

	cout << "HashTable Create" << endl;

	return hTable;
}

int GetKey(int data, int tableSize) {
	return data%tableSize;
}

bool HashSearch(struct HashTable* hTable, int data) {
	struct Node* tmp = NULL;
	tmp = hTable->table[GetKey(data, hTable->size)]->list;

	if (!tmp) {
		cout << "HashTable자체가 없다" << endl;
	}
	
	while (tmp) {
		if (tmp->data == data) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

struct HashTable* HashInsert(struct HashTable* hTable, int data) {
	struct HashNode*first;
	first = hTable->table[GetKey(data, hTable->size)];

	struct Node* tmp;
	tmp = first->list;

	struct Node* node = new Node();
	if (!node) {
		cout<<"Memory Allocation Fail" << endl;
		cout << "HashInsert -> data : " <<data<< endl;
		return NULL;
	}

	node->data = data;
	node->next = NULL;

	//1. empty
	if (!tmp) {
		first->list = node;
	}
	//2. 이미 존재할시
	else {
		struct Node* tmp2;
		tmp2 = first->list;
		first->list = node;
		node->next = tmp2;
	}

	//cout << "Data Insertion Success(Data :" << data <<" hash :"<<GetKey(data,hTable->size)<<")"<< endl;
	first->count++;
	hTable->numOfData++;
	if (first->count >= HASH_LOAD_FACTOR)
		hTable = ReHash(hTable);

	return hTable;
}

void HashDelete(struct HashTable* hTable, int data) {
	struct HashNode* first;
	first = hTable->table[GetKey(data, hTable->size)];
	struct Node* tmp;

	tmp = first->list;

	struct Node* before = tmp;
	if (!tmp)
		return;
	

	while (tmp) {
		if (tmp->data == data) {
			//처음
			if (before == tmp) {
				first->list = tmp->next;
			}
			else {
				before->next = tmp->next;
			}
			delete tmp;
			break;
		}
		before = tmp;
		tmp = tmp->next;
	}

	cout << "Data Deletion Success (Data : " << data << ")" << endl;
	first->count--;
}

void DeleteHashTable(struct HashTable* hTable) {
	for (int i = 0; i < hTable->size; i++) {
		struct Node* tmp = NULL;
		struct Node* before = NULL;
		tmp = hTable->table[i]->list;
		before = tmp;
		while (tmp) {
			before = tmp;
			tmp = tmp->next;
			delete before;
		}
	}

	for (int i = 0; i < hTable->size; i++) {
		delete hTable->table[i];
	}

	delete hTable;

	cout << "HashTable Delete" << endl;

}

void printHashTable(struct HashTable* hTable) {
	cout << "All print HashTable Data" << endl;
	for (int i = 0; i < hTable->size; i++) {
		struct HashNode* first = hTable->table[i];
		struct Node* tmp = first->list;

		cout << "Key " << i << ": ";
		while (tmp) {
			cout << tmp->data << " ";
			tmp = tmp->next;
		}
		cout << endl;
	}
}

struct HashTable* ReHash(struct HashTable* hTable) {
	//new 생성
	struct HashTable* oldTable = hTable;

	hTable = CreateHashTable(hTable->size * 2);

	if (!hTable) {
		cout << "Memory Allocation Fail" << endl;
		cout << "Fail ReHash(hashSize : " << hTable->size * 2 << ")" << endl;
		return NULL;
	}

	//cout << "ReHash시작 oldSize : " << oldTable->size << endl;
	for (int i = 0; i < oldTable->size; i++) {
		struct HashNode* first = oldTable->table[i];
		struct Node* tmp = first->list;
		struct Node* before = tmp;
		while (tmp) {
			hTable=HashInsert(hTable, tmp->data);
			tmp = tmp->next;
			//혹시 메모리 터질지 몰라서 추가함
			before = tmp;
		}
	}
	DeleteHashTable(oldTable);
	//cout << "ReHash 완료 newSize : " << hTable->size << endl;

	return hTable;
}

void saveHashNode(struct HashNode* hNode, fstream& Hash) {
	Hash.open(HASH_FILENAME, ios::out | ios::binary | ios::app);
	struct Node* tmpNode = NULL;
	Hash.write(reinterpret_cast<const char*>(&hNode->key), sizeof(int));
	Hash.write(reinterpret_cast<const char*>(&hNode->count), sizeof(int));
	for(int i=0;i<8;i++)
		Hash.write(" ", 1);

	tmpNode = hNode->list;
	for (int i = 0; i < hNode->count; i++) {
		Hash.write(reinterpret_cast<const char*>(&tmpNode->data), sizeof(int));
		tmpNode = tmpNode->next;
	}
	for (int i = hNode->count; i <= BLOCK_LOAD_FACTOR; i++) {
		Hash.write("    ", 4);
	}

	Hash.close();
}

void saveHashTable(struct HashTable* hTable, fstream& Hash) {
	Hash.open(HASH_FILENAME, ios::out | ios::binary| ios::trunc);
	Hash.write(reinterpret_cast<const char*>(&hTable->size), sizeof(int));
	Hash.write(reinterpret_cast<const char*>(&hTable->numOfData), sizeof(int));
	for (int i = 0; i < 8; i++)
		Hash.write(" ", 1);

	Hash.close();
	for (int i = 0; i < hTable->size; i++)
		saveHashNode(hTable->table[i],Hash);
}




struct HashTable* loadHashNode(fstream& Hash, struct HashTable *hTable) {
	char tmpNum[4];
	char tmpChar[16];

	Hash.read(tmpChar, 16);
	strncpy(tmpNum, tmpChar, 4);
	int num = BinaryToInt(tmpNum);
	int cnt = BinaryToInt(tmpChar + 4);
	struct HashNode* hNode = new HashNode();
	for (int j = 0; j < cnt; j++) {
		Hash.read(tmpNum, 4);
		int data = BinaryToInt(tmpNum);
		HashInsert(hTable, data);
	}
	for (int j = cnt; j <= BLOCK_LOAD_FACTOR; j++)
		Hash.read(tmpNum, 4);

	delete hNode;

	return hTable;
}

struct HashTable* loadHashFile(fstream& Hash, struct HashTable *hTable) {
	//Block Table 생성
	Hash.open(HASH_FILENAME, ios::in | ios::binary);
	char tmpChar[16];
	Hash.read(tmpChar, 16);
	char tmpNum[4];
	strncpy(tmpNum, tmpChar, 4);
	int num = BinaryToInt(tmpNum);
	hTable = CreateHashTable(num);
		//각 블록당 read
	for (int i = 0; i < num; i++) {
		hTable = loadHashNode(Hash, hTable);
	}

	Hash.close();
	return hTable;
}


#endif // !INDEXING_H
