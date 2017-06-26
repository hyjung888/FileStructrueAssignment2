#pragma once
#include "format.h"
#include "indexing.h"
#include <fstream>
#include <iostream>

bool inputStudent(HashTable* hTable, fstream& inputData, fstream& hash, fstream& DB, int N, int& tableSize);

bool inputProf(HashTable* hTable, fstream& inputData, fstream& hash, fstream& DB, int N, int& tableSize);
