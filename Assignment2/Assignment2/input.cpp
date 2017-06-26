#include "format.h"
#include "indexing.h"
#include <fstream>
#include <iostream>

using namespace std;
bool inputStudent(HashTable* hTable,fstream& inputData, fstream& hash, fstream& DB, int N, int& tableSize) {

	Students *tmpStudent = new Students();
	string tmpStr;
	
	for (int i = 0; i<N; i++) {
		//parsing student
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
		//end parsing

		hTable = HashInsert(hTable, *tmpStudent, hash, DB);
		memset(tmpStudent->name, 0, 20);
		tableSize = hTable->size;
	}
	return true;
}

bool inputProf(HashTable* hTable, fstream& inputData, fstream& hash, fstream& DB, int N, int& tableSize) {

	Prof *tmpProf = new Prof();
	string tmpStr;

	for (int i = 0; i<N; i++) {
		//parsing student
		getline(inputData, tmpStr);
		string tmpName = tmpStr.substr(0, tmpStr.find(',', 0));
		strncpy(tmpProf->name, tmpName.c_str(), tmpName.size());
		tmpStr = tmpStr.substr(tmpName.size() + 1);

		string tmpID = tmpStr.substr(0, tmpStr.find(',', 0));
		tmpProf->ProfID = stoi(tmpID);
		tmpStr = tmpStr.substr(tmpID.size() + 1);

		string tmpSalary = tmpStr;
		tmpProf->Salary = stoi(tmpSalary);
		//end parsing

		hTable = HashInsert(hTable, *tmpProf, hash, DB);
		memset(tmpProf->name, 0, 20);
		tableSize = hTable->size;
	}
	return true;
}