
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#pragma once

#include "indexing.h"	

using namespace std;

int BinaryToInt(char* charNum) {
	int *N = NULL;
	N = reinterpret_cast<int*>(charNum);
	return *N;
}
float BinaryToFloat(char* charNum) {
	float *f = NULL;
	f = reinterpret_cast<float*>(charNum);
	return *f;

}
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
		first->stud[0] = *stud;
	}
	//2. 이미 존재할시
	else {
		first->stud[first->cnt] = *stud;
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

void storeStudent(fstream& DB, Students* stud, int blockNum, int idx, char* fileName) {
	DB.open(fileName, ios::in | ios::out | ios::binary);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*blockNum * sizeof(Students) + 16 + idx * sizeof(Students);
	DB.seekp(pos);
	char studs[32];
	memcpy(studs, reinterpret_cast<char*>(stud), 32);
	DB.write(studs, 32);
	DB.close();
}

void storeProf(fstream& DB, Prof* prof, int blockNum, int idx, char* fileName) {
	DB.open(fileName, ios::in | ios::out | ios::binary);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*blockNum * sizeof(Prof) + 16 + idx * sizeof(Prof);
	DB.seekp(pos);
	char profs[28];
	memcpy(profs, reinterpret_cast<char*>(prof), 28);
	DB.write(profs, 28);
	DB.close();
}


void storeBlock(fstream& DB, Block* block, int blockNum, char* fileName) {
	DB.open(fileName, ios::out | ios::binary);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*blockNum + 16;
	DB.seekp(pos);

	struct Students* tmpStud = NULL;
	DB.write(reinterpret_cast<const char*>(&block->num), sizeof(int));
	DB.write(reinterpret_cast<const char*>(&block->cnt), sizeof(int));
	DB.write("                        ", 24);
	DB.close();
	for (int i = 0; i < block->cnt; i++) {
		tmpStud = &block->stud[i];
		storeStudent(DB, tmpStud, blockNum, i, fileName);
	}
	for (int i = block->cnt; i <= BLOCK_LOAD_FACTOR; i++) {
		for (int i = 0; i < 32; i++)
			DB.write(" ", 1);

	}

	DB.close();
}

void saveFile(fstream& DB, struct BlockTable* bTable, char* fileName) {

	DB.open(fileName, ios::out | ios::binary | ios::trunc);
	DB.write(reinterpret_cast<const char*>(&bTable->size), sizeof(int));
	DB.write(reinterpret_cast<const char*>(&bTable->numOfData), sizeof(int));
	for (int i = 0; i < 8; i++)
		DB.write(" ", 1);
	DB.close();

	for (int i = 0; i < bTable->size; i++) {
		storeBlock(DB, bTable->block[i], i, fileName);
	}
}

Block* loadBlockStud(fstream& DB, int blockNum, char* fileName, bool isDelete = true) {
	DB.open(fileName, ios::in | ios::out | ios::binary);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*blockNum * sizeof(Students);
	DB.seekg(pos);
	char tmpNum[4];
	char tmpChar[32];

	DB.read(tmpChar, 16);
	strncpy(tmpNum, tmpChar, 4);

	int num = BinaryToInt(tmpNum);
	int cnt = BinaryToInt(tmpChar + 4);
	Block *blk = new Block();
	blk->num = num;
	blk->cnt = cnt;
	Students* tmpStudent = new Students();
	for (int j = 0; j < cnt; j++) {
		DB.read(tmpChar, 32);
		memcpy(tmpStudent->name, tmpChar, 20);
		memcpy(tmpNum, tmpChar + 20, 4);
		tmpStudent->StudentID = BinaryToInt(tmpNum);
		memcpy(tmpNum, tmpChar + 24, 4);
		tmpStudent->score = BinaryToFloat(tmpNum);
		tmpStudent->advisorID = BinaryToInt(tmpChar + 28);
		memcpy(&blk->stud[j], tmpStudent, sizeof(Students));
	}
	for (int j = cnt; j <= BLOCK_LOAD_FACTOR; j++)
		DB.read(tmpChar, 32);

	delete tmpStudent;
	memset(tmpChar, 0, sizeof(tmpChar));
	if (isDelete) {
		DB.seekp(pos);
		for (int i = 0; i <= BLOCK_LOAD_FACTOR; i++)
			DB.write(tmpChar, 32);
	}
	DB.close();
	return blk;
}
ProfBlock* loadBlockProf(fstream& DB, int blockNum, char* fileName, bool isDelete = true) {
	DB.open(fileName, ios::in | ios::out | ios::binary);
	long pos = 16 + (BLOCK_LOAD_FACTOR + 1)*blockNum * sizeof(Prof);
	DB.seekg(pos);
	char tmpNum[4];
	char tmpChar[32];

	DB.read(tmpChar, 16);
	int num = BinaryToInt(tmpChar);
	int cnt = BinaryToInt(tmpChar + 4);
	ProfBlock *blk = new ProfBlock();
	blk->num = num;
	blk->cnt = cnt;
	Prof* tmpProf = new Prof();
	for (int j = 0; j < cnt; j++) {
		pos = DB.tellg();
		DB.read(tmpChar, sizeof(Prof));
		pos = DB.tellg();
		memcpy(tmpProf->name, tmpChar, 20);
		memcpy(tmpNum, tmpChar + 20, 4);
		tmpProf->ProfID = BinaryToInt(tmpNum);
		memcpy(tmpNum, tmpChar + 24, 4);
		tmpProf->Salary = BinaryToInt(tmpNum);
		memcpy(&blk->profs[j], tmpProf, sizeof(Prof));
	}
	for (int j = cnt; j <= BLOCK_LOAD_FACTOR; j++)
		DB.read(tmpChar, sizeof(Prof));

	delete tmpProf;
	memset(tmpChar, 0, sizeof(tmpChar));
	if (isDelete) {
		DB.seekp(pos);
		for (int i = 0; i <= BLOCK_LOAD_FACTOR; i++)
			DB.write(tmpChar, sizeof(Prof));
	}
	DB.close();
	return blk;
}




void queryExactProcessing(string tableName, string attribute, string val) {
	fstream DB;
	fstream ans(QUERY_ANS_FILENAME, ios::app);
	if (tableName == "Professors")
		DB.open(DB_PROF_FILENAME, ios::in | ios::out | ios::binary);
	else
		DB.open(DB_STUDENT_FILENAME, ios::in | ios::out | ios::binary);
	char tmpChar[32];
	DB.read(tmpChar, 16);
	int size = BinaryToInt(tmpChar);
	int key = GetKey(stoi(val), size);
	DB.close();

	if (tableName == "Professors" && attribute == "ProfID") {
		ProfBlock*tmpBlock = loadBlockProf(DB, key, DB_PROF_FILENAME);
		for (int i = 0; i < tmpBlock->cnt; i++)
		{
			Prof tmpProf = tmpBlock->profs[i];
			if (tmpProf.ProfID == stoi(val)) {
				ans << tmpProf.name << ", " << tmpProf.ProfID << ", " << tmpProf.Salary << endl;
				break;
			}

		}
	}
	else if (tableName == "Students"&& attribute == "StudentID") {
		Block*tmpBlock = loadBlockStud(DB, key, DB_STUDENT_FILENAME);
		for (int i = 0; i < tmpBlock->cnt; i++)
		{
			Students tmpStud = tmpBlock->stud[i];
			if (tmpStud.StudentID == stoi(val)) {
				ans << tmpStud.name << ", " << tmpStud.StudentID << ", " << tmpStud.score << ", " << tmpStud.advisorID << endl;
				break;
			}

		}
	}

	ans.close();
}


void queryJoinProcessing() {
	fstream profDB(DB_PROF_FILENAME, ios::in | ios::out | ios::binary);
	fstream studDB(DB_STUDENT_FILENAME, ios::in | ios::out | ios::binary);
	fstream ans(QUERY_ANS_FILENAME, ios::app);
	char tmpChar[32];
	profDB.read(tmpChar, 16);
	int profSize = BinaryToInt(tmpChar);
	studDB.read(tmpChar, 16);
	int studSize = BinaryToInt(tmpChar);
	profDB.close();
	studDB.close();
	Block*tmpBlock;
	ProfBlock*tmpProfBlock;
	for (int i = 0; i < studSize; i++) {
		tmpBlock = loadBlockStud(studDB, i, DB_STUDENT_FILENAME, false);
		for (int j = 0; j < profSize; j++) {
			tmpProfBlock = loadBlockProf(profDB, j, DB_PROF_FILENAME, false);
			for (int ii = 0; ii < tmpBlock->cnt; ii++) {
				Students tmpStud = tmpBlock->stud[ii];
				for (int jj = 0; jj < tmpProfBlock->cnt; jj++) {
					Prof tmpProf = tmpProfBlock->profs[jj];
					if (tmpStud.advisorID == tmpProf.ProfID) {
						ans << tmpStud.name << ", " << tmpStud.StudentID << ", " << tmpStud.score << ", ";
						ans << tmpProf.name << ", " << tmpProf.ProfID << ", " << tmpProf.Salary << endl;
						break;
					}
				}

			}
			delete tmpProfBlock;
		}
		delete tmpBlock;
	}
	profDB.close();
	studDB.close();
	ans.close();

}