#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include "format.h"
#include "indexing.h"
using namespace std;



int main() {


	fstream inputData("sampleData.csv",ios::in);
	
	if (!inputData.is_open()) {
		cout << "read sampleData.csv error" << endl;
	}
	string tmpStr;
	int N;
	inputData >> N;
	getline(inputData, tmpStr);

	
	int tableSize = 4;
	//생성
	struct HashTable* hTable = CreateHashTable(tableSize);
	//삽입

	struct Students *tmpStudent = new Students();

	for(int i=0;i<N;i++) {
		getline(inputData, tmpStr);
		string tmpName=tmpStr.substr(0, tmpStr.find(',', 0));
		strncpy(tmpStudent->name, tmpName.c_str(), tmpName.size());
		tmpStr = tmpStr.substr(tmpName.size()+1);

		string tmpID= tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStudent->StudentID = stoi(tmpID);
		tmpStr = tmpStr.substr(tmpID.size() + 1);

		string tmpScore= tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStudent->score = stof(tmpScore);
		tmpStr = tmpStr.substr(tmpScore.size() + 1);

		string tmpADid = tmpStr;
		tmpStudent->advisorID = stoi(tmpADid);


		hTable=HashInsert(hTable, tmpStudent->StudentID);
		memset(tmpStudent->name, 0, 20);
		tableSize = hTable->size;
	}
	inputData.close();
	cout << "파일에 쓰기 시작" << endl;
	inputData.open("sampleData.csv", ios::in);
	inputData >> N;
	getline(inputData, tmpStr);

	struct BlockTable *bTable = CreateBlockTable(tableSize);

	insertAllBlock(inputData, N, bTable);
	
	fstream DB;
	saveFile(DB, bTable);

	fstream hash;
	saveHashTable(hTable, hash);

	HashTable* hT = NULL;

	hT=loadHashFile(hash, hT);

	printHashTable(hT);
	return 1;
}