#include <iostream>
#include <fstream>
#include <string>
#include "format.h"
#include "indexing.h"
#include "input.h"
using namespace std;


HashTable* hTable_stud;
HashTable* hTable_prof;
int main() {


	fstream inputData;

	fstream hash;
	fstream DB;
	fstream queryAns(QUERY_ANS_FILENAME, ios::out);
	queryAns.close();
	int N;

	string tmpStr;
	int tableSize_stud = 4;
	int tableSize_prof = 4;
	int input;
	cout << "1을 누르면 새로운 학생DB를 입력받습니다" << endl;
	cout << "2을 누르면 새로운 교수DB를 입력받습니다" << endl;
	cout << "0을 누르면 기존 DB를 사용합니다." << endl;
	cin >> input;
	if (input == 1) {
		inputData.open("student_data.csv", ios::in);
		inputData >> N;
		getline(inputData, tmpStr);
		//생성
		hTable_stud = CreateHashTable(tableSize_stud, hash, DB);
		//삽입
		inputStudent(hTable_stud, inputData, hash, DB, N, tableSize_stud);
		inputData.close();

	}
	else if (input == 2) {
		inputData.open("prof_data.csv", ios::in);
		inputData >> N;
		getline(inputData, tmpStr);
		//생성
		hTable_prof = CreateHashTable(tableSize_prof, hash, DB);
		//삽입
		inputProf(hTable_prof, inputData, hash, DB, N, tableSize_stud);
		inputData.close();

	}

	cout << "쿼리를 입력시작" << endl;

	inputData.open("query.dat", ios::in);
	inputData >> N;
	getline(inputData, tmpStr);
	for (int i = 0; i < N; i++) {
		//parsing student
		getline(inputData, tmpStr);
		fstream ans(QUERY_ANS_FILENAME, ios::app);
		ans << tmpStr << endl;
		string tmpType = tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStr = tmpStr.substr(tmpType.size() + 2);
		if (tmpType == "Join") {
			queryJoinProcessing();
			continue;
		}
		string tableName = tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStr = tmpStr.substr(tableName.size() + 2);

		string attributeName = tmpStr.substr(0, tmpStr.find(',', 0));
		tmpStr = tmpStr.substr(attributeName.size() + 2);
		string min, max;
		if (tmpType == "Search-Exact") {
			min = tmpStr.substr(0, tmpStr.find(',', 0));

			queryExactProcessing(tableName, attributeName,min);
		}
		else if (tmpType == "Search-Range") {
			min = tmpStr.substr(0, tmpStr.find(',', 0));
			tmpStr = tmpStr.substr(min.size() + 1);
			max = tmpStr.substr(0, tmpStr.find(',', 0));
		}



	}
	return 1;

}