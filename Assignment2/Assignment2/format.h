
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

#define HASH_LOAD_FACTOR 100
#define BLOCK_LOAD_FACTOR 127
#define DB_FILENAME "Students.DB"
#define TEST_FILENAME "Students.dat"

#ifndef FORMAT_H
#define FORMAT_H

#include "indexing.h"	


using namespace std;

struct Students {
	char name[20] = { 0, };
	int StudentID = 0;
	float score = 0;
	int advisorID = 0;

	struct Students* operator=(struct Students* std) {
		strncpy(this->name, std->name, 20);
		this->StudentID = std->StudentID;
		this->score = std->score;
		this->advisorID = std->advisorID;
		return this;
	};
};

struct Block {
	int num;
	int cnt;
	int tmp[6] = { 0,0,0,0,0,0 };
	Students stud[BLOCK_LOAD_FACTOR];

};

int BinaryToInt(char* charNum) {
	int *N = NULL;
	N = reinterpret_cast<int*>(charNum);
	return *N;
}


float BinaryToFloat(char* charNum) {
	float *f = NULL;
	f= reinterpret_cast<float*>(charNum);
	return *f;

}


struct BlockTable {
	int size;
	int numOfData;
	struct Block** block;
};

struct BlockTable* CreateBlockTable(int tableSize) {
	struct BlockTable* bTable;
	bTable = new BlockTable();
	if (!bTable) {
		cout << "Memory Allocation Fail" << endl;
		cout << "BlockTable not create" << endl;
		return NULL;
	}

	bTable->size = tableSize;
	bTable->numOfData = 0;
	bTable->block = new Block*[tableSize]();
	if (!bTable->block)
	{
		cout << "Memory Allocation Fail" << endl;
		cout << "BlockTable->block not create" << endl;
		return NULL;
	}
	for (
		int i = 0; i < tableSize; i++) {
		bTable->block[i] = new Block();
		if (!bTable->block[i]) {
			cout << "Memory Allocation Fail" << endl;
			cout << "BlockTable->block[" << i << "] not create" << endl;
			return NULL;
		}

		bTable->block[i]->num = i;
		bTable->block[i]->cnt = 0;
		memset(bTable->block[i]->stud, 0, sizeof(bTable->block[i]->stud));
	}

	cout << "BlockTable Create" << endl;

	return bTable;

}

struct BlockTable* BlockInsert(struct BlockTable* bTable, struct Students* stud) {
	struct Block* first;
	first = bTable->block[GetKey(stud->StudentID, bTable->size)];

	//1. empty
	if (first->cnt == 0) {
		first->stud[0] = stud;
	}
	//2. 이미 존재할시
	else {
		first->stud[first->cnt] = stud;
	}
	first->cnt++;
	bTable->numOfData++;

	return bTable;
}

void insertAllBlock(fstream& inputData, int N, struct BlockTable* bTable) {
	Students* tmpStudent = new Students();
	string tmpStr;
	for (int i = 0; i < N; i++) {
		getline(inputData, tmpStr);
		string tmpName = tmpStr.substr(0, tmpStr.find(',', 0));
		strncpy(tmpStudent->name, tmpName.c_str(), tmpName.size());
		tmpStr = tmpStr.substr(tmpName.size() + 1);

		string tmpID = tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStudent->StudentID = stoi(tmpID);
		tmpStr = tmpStr.substr(tmpID.size() + 1);

		string tmpScore = tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStudent->score = stof(tmpScore);
		tmpStr = tmpStr.substr(tmpScore.size() + 1);

		string tmpADid = tmpStr;
		tmpStudent->advisorID = stoi(tmpADid);

		bTable = BlockInsert(bTable, tmpStudent);
		memset(tmpStudent->name, 0, 20);
	}
}

void storeBlock(fstream& DB, struct Block* block) {
	DB.open(DB_FILENAME, ios::out | ios::binary | ios::app);
	struct Students* tmpStud = NULL;
	DB.write(reinterpret_cast<const char*>(&block->num), sizeof(int));
	DB.write(reinterpret_cast<const char*>(&block->cnt), sizeof(int));
	DB.write("                        ", 24);
	for (int i = 0; i < block->cnt; i++) {
		tmpStud = &block->stud[i];
		DB.write(tmpStud->name, sizeof tmpStud->name);
		DB.write((char*)(&tmpStud->StudentID), sizeof(int));
		DB.write((char*)(&tmpStud->score), sizeof(float));
		DB.write((char*)(&tmpStud->advisorID), sizeof(int));
	}
	for (int i = block->cnt; i <= BLOCK_LOAD_FACTOR; i++) {
		for (int i = 0; i < 32; i++)
			DB.write(" ", 1);

	}

	DB.close();
}

void saveFile(fstream& DB, struct BlockTable* bTable) {

	DB.open(DB_FILENAME, ios::out | ios::binary | ios::trunc);
	DB.write(reinterpret_cast<const char*>(&bTable->size), sizeof(int));
	DB.write(reinterpret_cast<const char*>(&bTable->numOfData), sizeof(int));
	for (int i = 0; i < 8; i++)
		DB.write(" ", 1);
	DB.close();

	for (int i = 0; i < bTable->size; i++) {
		storeBlock(DB, bTable->block[i]);
	}
}

struct BlockTable* loadBlock(fstream& DB, struct BlockTable *bTable) {
	char tmpNum[4];
	char tmpChar[32];

	DB.read(tmpChar, 32);
	strncpy(tmpNum, tmpChar, 4);
	int num = BinaryToInt(tmpNum);
	int cnt = BinaryToInt(tmpChar + 4);
	struct Students* tmpStudent = new Students();
	for (int j = 0; j < cnt; j++) {
		float score;
		DB.read(tmpChar, 32);
		char name[20];
		strncpy(tmpStudent->name, tmpChar, 20);
		strncpy(tmpNum, tmpChar + 20, 4);
		tmpStudent->StudentID = BinaryToInt(tmpNum);
		strncpy(tmpNum, tmpChar + 24, 4);
		tmpStudent->score = BinaryToFloat(tmpNum);
		tmpStudent->advisorID = BinaryToInt(tmpChar + 28);
		bTable = BlockInsert(bTable, tmpStudent);
	}
	for (int j = cnt; j <= BLOCK_LOAD_FACTOR; j++)
		DB.read(tmpChar, 32);

	delete tmpStudent;

	return bTable;
}

struct BlockTable* loadFile(fstream& DB, struct BlockTable *bTable) {
	//Block Table 생성
	DB.open(DB_FILENAME, ios::in | ios::binary);
	char tmpChar[32];
	DB.read(tmpChar, 16);
	char tmpNum[4];
	strncpy(tmpNum, tmpChar, 4);
	int num = BinaryToInt(tmpNum);
	bTable = CreateBlockTable(num);

	//각 블록당 read
	for (int i = 0; i < num; i++) {
		bTable = loadBlock(DB, bTable);
	}
	DB.close();
	return bTable;
}
#endif // !FORMAT_H
