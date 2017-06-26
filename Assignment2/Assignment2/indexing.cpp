#include <iostream>
#include <cstdlib>
#include <vector>
#include "indexing.h"
#include "format.h"

using namespace std;


HashTable* CreateHashTable(int tablesize, fstream& Hash,fstream& DB) {
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
			cout << "HashTable->table[" << i << "] not create" << endl;
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
bool HashSearch(HashTable* hTable, int data) {
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

HashTable* HashInsert(HashTable* hTable, Students& stud, fstream& Hash, fstream& DB) {
	//struct HashTable* HashInsert(struct HashTable* hTable, int data, fstream& Hash, fstream& DB) {
	struct HashNode* first;
	int data = stud.StudentID;
	int key = GetKey(data, hTable->size);
	first = hTable->table[key];

	struct Node* tmp;
	tmp = first->list;

	struct Node* node = new Node();
	if (!node) {
		cout << "Memory Allocation Fail" << endl;
		cout << "HashInsert -> data : " << data << endl;
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

	//3. 파일에 쓰기

	char chrKey[4];
	char chrcnt[4];
	memcpy(chrKey, reinterpret_cast<const char*>(&key), 4);
	memcpy(chrcnt, reinterpret_cast<const char*>(&first->count), 4);
	//hash 파일
	Hash.open(HASH_STUDENT_FILENAME, ios::out | ios::binary | ios::in);
	Hash.write(reinterpret_cast<const char*>(&hTable->size), sizeof(int));
	Hash.write(reinterpret_cast<const char*>(&hTable->numOfData), sizeof(int));
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1) *key * sizeof(int);
	Hash.seekp(pos);
	if (!Hash.is_open())
		cout << "1" << endl;
	Hash.write(chrKey, sizeof(int));
	Hash.write(chrcnt, sizeof(int));
	Hash.close();
	saveNodeStud(node, Hash, key, first->count-1, HASH_STUDENT_FILENAME);

	//DB 파일
	DB.open(DB_STUDENT_FILENAME, ios::out | ios::binary | ios::in);
	DB.write(reinterpret_cast<const char*>(&hTable->size), sizeof(int));
	DB.write(reinterpret_cast<const char*>(&hTable->numOfData), sizeof(int));
	pos = 16 + (BLOCK_LOAD_FACTOR + 1)*key*sizeof(Students);
	DB.seekp(pos);
	DB.write(chrKey, sizeof(int));
	DB.write(chrcnt, sizeof(int));
	DB.close();
	storeStudent(DB, &stud, key, first->count-1, DB_STUDENT_FILENAME);


	if (first->count >= HASH_LOAD_FACTOR)
		hTable = ReHashStud(hTable, Hash, DB, HASH_STUDENT_FILENAME);

	Hash.clear();
	return hTable;
}

HashTable* HashInsert(HashTable* hTable, Prof& prof, fstream& Hash, fstream& DB) {
	//struct HashTable* HashInsert(struct HashTable* hTable, int data, fstream& Hash, fstream& DB) {
	struct HashNode* first;
	int data = prof.ProfID;
	int key = GetKey(data, hTable->size);
	first = hTable->table[key];

	struct Node* tmp;
	tmp = first->list;

	struct Node* node = new Node();
	if (!node) {
		cout << "Memory Allocation Fail" << endl;
		cout << "HashInsert -> data : " << data << endl;
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

	//3. 파일에 쓰기

	char chrKey[4];
	char chrcnt[4];
	memcpy(chrKey, reinterpret_cast<const char*>(&key), 4);
	memcpy(chrcnt, reinterpret_cast<const char*>(&first->count), 4);
	//hash 파일
	Hash.open(HASH_PROF_FILENAME, ios::out | ios::binary | ios::in);
	Hash.write(reinterpret_cast<const char*>(&hTable->size), sizeof(int));
	Hash.write(reinterpret_cast<const char*>(&hTable->numOfData), sizeof(int));
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1) *key * sizeof(int);
	Hash.seekp(pos);
	if (!Hash.is_open())
		cout << "1" << endl;
	Hash.write(chrKey, sizeof(int));
	Hash.write(chrcnt, sizeof(int));
	Hash.close();
	saveNodeProf(node, Hash, key, first->count - 1, HASH_PROF_FILENAME);

	//DB 파일
	DB.open(DB_PROF_FILENAME, ios::out | ios::binary | ios::in);
	DB.write(reinterpret_cast<const char*>(&hTable->size), sizeof(int));
	DB.write(reinterpret_cast<const char*>(&hTable->numOfData), sizeof(int));
	pos = 16 + (BLOCK_LOAD_FACTOR + 1)*key*sizeof(Prof);
	DB.seekp(pos);
	DB.write(chrKey, sizeof(int));
	DB.write(chrcnt, sizeof(int));
	DB.close();
	storeProf(DB, &prof, key, first->count - 1, DB_PROF_FILENAME);


	if (first->count >= HASH_LOAD_FACTOR)
		hTable = ReHashProf(hTable, Hash,  DB, HASH_PROF_FILENAME);

	Hash.clear();
	return hTable;
}

void HashDelete(HashTable* hTable, int data) {
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

//		cout << "Key " << i << ": ";
		while (tmp) {
			cout << tmp->data << " ";
			tmp = tmp->next;
		}
		cout << endl;
	}
}

HashTable* ReHashStud(HashTable* hTable, fstream& Hash,fstream& DB,char* fileName) {
	//new 생성
	struct HashTable* oldTable = hTable;

	hTable = CreateHashTable(hTable->size * 2,Hash,DB);

	if (!hTable) {
		cout << "Memory Allocation Fail" << endl;
		cout << "Fail ReHash(hashSize : " << hTable->size * 2 << ")" << endl;
		return NULL;
	}

	cout << "ReHash시작 oldSize : " << oldTable->size << endl;
	for (int i = 0; i < oldTable->size; i++) {
		Block* tmpBlock = loadBlockStud(DB, i, HASH_PROF_FILENAME);
		Hash.open(fileName, ios::out | ios::binary | ios::in);
		long pos = 16 + (BLOCK_LOAD_FACTOR + 1) *i*sizeof(int);
		Hash.seekp(pos);
		for (int i = 0; i <= BLOCK_LOAD_FACTOR;i++)
			Hash.write("    ", 4);
		Hash.close();
		Students tmpStud;
		for (int i = 0; i < tmpBlock->cnt; i++) {
			tmpStud = tmpBlock->stud[i];
			if (tmpStud.advisorID==0 && tmpStud.StudentID == 0)
				break;
			hTable = HashInsert(hTable, tmpBlock->stud[i], Hash, DB);
		}
		delete tmpBlock;
	}
	DeleteHashTable(oldTable);
	cout << "ReHash 완료 newSize : " << hTable->size << endl;

	return hTable;
}

HashTable* ReHashProf(HashTable* hTable, fstream& Hash, fstream& DB, char* fileName) {
	//new 생성
	struct HashTable* oldTable = hTable;

	hTable = CreateHashTable(hTable->size * 2, Hash, DB);

	if (!hTable) {
		cout << "Memory Allocation Fail" << endl;
		cout << "Fail ReHash(hashSize : " << hTable->size * 2 << ")" << endl;
		return NULL;
	}

	cout << "ReHash시작 oldSize : " << oldTable->size << endl;
	for (int i = 0; i < oldTable->size; i++) {
		ProfBlock* tmpBlock = loadBlockProf(DB, i, DB_PROF_FILENAME);
		Hash.open(fileName, ios::out | ios::binary | ios::in);
		long pos = 16 + (BLOCK_LOAD_FACTOR + 1) *i*sizeof(int);
		Hash.seekp(pos);
		for (int i = 0; i <= BLOCK_LOAD_FACTOR; i++)
			Hash.write("    ", 4);
		Hash.close();
		Prof tmpProf;
		for (int i = 0; i < tmpBlock->cnt; i++) {
			tmpProf = tmpBlock->profs[i];
			if (tmpProf.Salary == 0 && tmpProf.ProfID == 0)
				break;
			hTable = HashInsert(hTable, tmpProf, Hash, DB);
		}
		delete tmpBlock;
	}
	DeleteHashTable(oldTable);
	cout << "ReHash 완료 newSize : " << hTable->size << endl;

	return hTable;
}
void saveNodeStud(Node* node, fstream& Hash,int hashNum,int idx,char* fileName) {
	Hash.close();
	Hash.open(fileName, ios::in | ios::out | ios::binary);
	
//	Hash.open(HASH_FILENAME, ios::out | ios::binary | ios::app);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*hashNum*sizeof(int) + 8 + idx * 4;
	Hash.seekp(pos);

	if (!Hash.is_open()) {
		cout << "BAD" << endl;
	}
//	cout << "pos : "<< pos <<" init : "<<init<<" tellp : "<<Hash.tellp()<< endl;
	//cout << node.data << endl;
	char num[4];
	memcpy(num,reinterpret_cast<const char*>(&node->data),4);
 	Hash.write(num, sizeof(num));
	Hash.close();
}
void saveNodeProf(Node* node, fstream& Hash, int hashNum, int idx, char* fileName) {
	Hash.close();
	Hash.open(fileName, ios::in | ios::out | ios::binary);

	//	Hash.open(HASH_FILENAME, ios::out | ios::binary | ios::app);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*hashNum*sizeof(int) + 8 + idx * 4;
	Hash.seekp(pos);

	if (!Hash.is_open()) {
		cout << "BAD" << endl;
	}
	//	cout << "pos : "<< pos <<" init : "<<init<<" tellp : "<<Hash.tellp()<< endl;
	//cout << node.data << endl;
	char num[4];
	memcpy(num, reinterpret_cast<const char*>(&node->data), 4);
	Hash.write(num, sizeof(num));
	Hash.close();
}
void saveHashNode(HashNode* hNode, fstream& Hash,int hashNum, char* fileName) {
/*
//	Hash.open(HASH_FILENAME, ios::out | ios::binary | ios::app);
	Hash.open(fileName, ios::in | ios::out | ios::binary);
	long pos = 16 + (BLOCK_STUDENT_LOAD_FACTOR +1) *hashNum*sizeof(int);
	Hash.seekp(pos);
	cout << "pos : " << pos << endl;
	struct Node* tmpNode = NULL;
	Hash.write(reinterpret_cast<const char*>(&hNode->key), sizeof(int));
	Hash.write(reinterpret_cast<const char*>(&hNode->count), sizeof(int));
	cout << hNode->key << " " << hNode->count << endl;
	tmpNode = hNode->list;
	for (int i = 0; i < hNode->count; i++) {
		saveNodeStud(tmpNode, Hash, hashNum,i,fileName);
		tmpNode = tmpNode->next;
	}
	for (int i = hNode->count; i < BLOCK_STUDENT_LOAD_FACTOR; i++) {
		Hash.write("    ", 4);
	}

	Hash.close();
	*/
}
void saveHashTable(struct HashTable* hTable, fstream& Hash, char* fileName) {
	Hash.open(fileName, ios::out | ios::binary | ios::trunc);
	Hash.write(reinterpret_cast<const char*>(&hTable->size), sizeof(int));
	Hash.write(reinterpret_cast<const char*>(&hTable->numOfData), sizeof(int));
	for (int i = 0; i < 8; i++)
		Hash.write(" ", 1);

	Hash.close();
	for (int i = 0; i < hTable->size; i++)
		saveHashNode(hTable->table[i], Hash,i,fileName);

	Hash.close();
}

HashTable* loadHashNode(fstream& Hash, struct HashTable *hTable, fstream& DB) {
	/*
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
		HashInsert(hTable, data, Hash, DB);
	}
	for (int j = cnt; j <= HASH_STUDENT_LOAD_FACTOR; j++)
		Hash.read(tmpNum, 4);

	delete hNode;
	*/
	return hTable;
	
}
HashTable* loadHashFile(fstream& Hash, struct HashTable *hTable, fstream& DB) {
	/*
	//Block Table 생성
	Hash.open(HASH_FILENAME, ios::in | ios::binary);
	char tmpChar[16];
	Hash.read(tmpChar, 16);
	char tmpNum[4];
	strncpy(tmpNum, tmpChar, 4);
	int num = BinaryToInt(tmpNum);
	hTable = CreateHashTable(num,Hash,DB);
	//각 블록당 read
	for (int i = 0; i < num; i++) {
		hTable = loadHashNode(Hash, hTable, DB);
	}

	Hash.close();
	*/
	return hTable;
}