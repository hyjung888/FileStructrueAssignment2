
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#pragma once


#include "indexing.h"	


using namespace std;



int BinaryToInt(char* charNum);
float BinaryToFloat(char* charNum);

BlockTable* CreateBlockTable(int tableSize);
BlockTable* BlockInsert(BlockTable* bTable, Students* stud);

void insertAllBlock(fstream& inputData, int N, struct BlockTable* bTable);

void storeBlock(fstream& DB, struct Block* block,char*fileName);

void saveFile(fstream& DB, struct BlockTable* bTable, char*fileName);
void storeStudent(fstream& DB, Students* stud, int blockNum, int idx,char* fileName);
void storeProf(fstream& DB, Prof* prof, int blockNum, int idx, char* fileName);


Block* loadBlockStud(fstream& DB,int blockNum, char*fileName);
ProfBlock* loadBlockProf(fstream& DB, int blockNum, char*fileName);

void queryExactProcessing(string tableName, string attribute, string val);