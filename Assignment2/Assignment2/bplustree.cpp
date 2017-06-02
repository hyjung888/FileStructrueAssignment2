#include <cstdio>
#include <cstdlib>
#include <cstring> 
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define PUTLINE printf("*===========================================*\n");
#define M 5  // 홀수 차수 생성 
#define MAX 4000	// 큐의 크기
#define TR 4000 // 스택의 크기

typedef struct Node
{
	int count;
	float Key[M - 1];
	struct Node* branch[M];
} node;

node* root;
int front = 0, rear = 0;	// 큐를 구성하기 위해서 초기화 
node* queue[MAX];	// 큐를 가르치는 노드  
int findcnt;		// 검색 경로 수 
ifstream fin;			//파일 포인터 
FILE* fp;

float* search(float k);		// Key를 탐색하는 함수 
void sequencialSearch(float k);	// 순차탐색(리프노드) 출력  함수 
void sequencialSearchF(float k);	// 순차탐색(리프노드) 파일 저장 함수
float* insertItem(float k);		// Key 삽입 함수 
node* put(node* k);		// 큐 입력 함수 
node* get();	// 큐 출력 함수 
int isEmpty();	// 큐 공백 체크 함수 
void indexNodePrint(node* t);	// 인덱스 노드 출력 함수
void printall();	// B+ Tree 전체 정보 출력 함수
void printfile();	//B+ Tree 전체 정보 화일 저장 함수
float getScore(char* c_str);

void main() {
	root = NULL;

	int i;
	char buf[128];
	fin.open("sampleData.csv");

	fin.getline(buf, 128);
	string _buf = string(buf);
	_buf = _buf.substr(0, _buf.find(','));
	float* score_idx = new float[MAX];

	for (i = 0; i < MAX; i++) {
		fin.getline(buf, 128);
		float score = getScore(buf);
		score_idx[i] = score;
	}

	for (i = 0; i < MAX; i++)
	{
		insertItem(score_idx[i]);
	}
	printfile();
	printall();
}

float getScore(char* c_str) {
	string str = string(c_str);

	int idx = 0;

	for (int i = 0; i < 2; i++) {
		idx = str.find(',', idx) + 1;
	}

	str = str.substr(idx, str.find(',', idx) - idx);

	return stof(str);
}


void printall()
{
	PUTLINE;
	printf("\n -- 현재 Tree 상태 -- \n ");
	printf("인덱스 노드 : ");
	indexNodePrint(root);
	printf("\n 리프 노드 : ");
	sequencialSearch(0);
	printf("\n\n");
	PUTLINE;
}

void printfile()
{

	fp = fopen("Students_score.idx", "wb");
	fprintf(fp, "*===========================================*\n");
	fprintf(fp, "\n - B+ Tree 상태 -- \n ");
	fprintf(fp, "\n 리프 노드 :\n");
	sequencialSearchF(0);
	fprintf(fp, "\n*===========================================*\n");
	fclose(fp);

	PUTLINE;
	printf(" 모든 B+ Tree정보를 Students_score.idx로 저장했습니다. \n");
	PUTLINE;
}

float* search(float k)
{
	node* p = root;
	int path;
	if (p == NULL)
		return NULL;
	while (1)	// p가 leaf노드일때까지 탐색
	{
		int j;
		for (j = 0; j < p->count%M; j++)	// 한 노드에서 경로를 결정
		{
			if (p->Key[j] >= k)
			{
				path = j;
				break;
			}
		}
		if (j == p->count%M)
			path = p->count%M;

		if (p->count / M != 1)
		{
			findcnt++;
		}

		if (p->count / M == 1)
			break;

		p = p->branch[j];

	}
	if (p->Key[path] == k && p->count%M != path)
	{
		return (float*)p->branch[path + 1];
	}
	else
		return NULL;
}

void sequencialSearch(float k) {
	int path, j;
	node* p = root;
	findcnt = 0;

	if (p != NULL)
	{
		while (1)	// p가 leaf노드 일때까지 탐색
		{
			for (j = 0; j < p->count%M; j++)	// 한 노드에서 경로를 결정
			{
				if (p->Key[j] >= k)
				{
					path = j;
					break;
				}
			}
			if (j == p->count%M)
				path = p->count%M;
			if (p->count / M == 1)
				break;
			p = p->branch[j];
		}
		if (p->Key[path] == k || k == 0)	// k가 0이면 minimum부터 다 출력한다. 
		{
			while (p != NULL)
			{
				for (j = 0; j < p->count%M; j++)
				{
					printf("%f\n", *(float*)p->branch[j + 1]);
					//			str = (char*)*(int*)p->branch[j+1];
				}
				p = p->branch[0];
			}
		}
	}
}

void sequencialSearchF(float k)
{
	int path, j;
	node* p = root;
	findcnt = 0;

	if (p != NULL)
	{
		while (1)	// p가 leaf노드 일때까지 탐색
		{
			int j;
			for (j = 0; j < p->count%M; j++)	// 한 노드에서 경로를 결정
			{
				if (p->Key[j] >= k)
				{
					path = j;
					break;
				}
			}
			if (j == p->count%M)
				path = p->count%M;
			if (p->count / M == 1)
				break;
			p = p->branch[j];
		}
		if (p->Key[path] == k || k == 0)	// k가 0이면 minimum부터 다 출력한다. 
		{
			while (p != NULL)
			{
				for (j = 0; j < p->count%M; j++)
				{
					fprintf(fp, "%f ", *(float*)p->branch[j + 1]);
				}
				p = p->branch[0];
			}
		}
	}
}

float* insertItem(float k)
{
	node* trace[TR];	// 삽입될 경로를 저장할 스택용도의 배열
	int dir[TR];
	float Key;
	int i;

	node* upRight, *p;
	float* insertFileLocation = new float[sizeof(float)];
	*insertFileLocation = k;
	upRight = (node*)insertFileLocation;
	i = 0;	// trace[]의 index

	p = root;	// p를 가지고 삽입될 위치를 탐색

	*(float*)upRight = k;

	if (root == NULL)
	{
		root = new node[sizeof(node)];
		root->branch[0] = NULL;
		root->Key[0] = k;
		root->branch[1] = (node*)insertFileLocation;
		root->count = M + 1;
		return insertFileLocation;
	}

	while (1)	// p가 leaf노드 일때까지 탐색
	{
		int j;
		trace[i] = p;
		for (j = 0; j < p->count%M; j++) {	// 한 노드에서 경로를 결정
			if (p->Key[j] >= k) {
				dir[i] = j;
				break;
			}
		}
		if (j == p->count%M)
			dir[i] = p->count%M;
		if (p->count / M == 1)
			break;
		p = p->branch[j];
		i++;
	}						// 이 루프에서 나오면 p는 Key값이 삽일될 노드. 

	if (p->Key[dir[i]] == k) {
		free(upRight);
		printf("   ### 이미 입력하신 Key가 있습니다. ###\n");
		return NULL;
	}

	// 이제 본격적인 삽입을 시작. 
	Key = k;
	while (i != -1)
	{
		int path = dir[i];
		p = trace[i];
		if (p->count%M != M - 1)	// 삽입해도 overflow가 생기지 않으면
		{
			int m;
			for (m = p->count%M; m>path; m--)	// 삽입될 칸부터 끝까지 한칸씩 뒤로. 
			{
				p->Key[m] = p->Key[m - 1];
				p->branch[m + 1] = p->branch[m];
			}
			p->Key[path] = Key;		// Key값을 삽입
			p->branch[path + 1] = upRight;	// branch를 관리. 
			p->count++;
			break;
		}

		else	// 삽입하면 overflow가 생기는 경우
		{
			float nodeKey[M];
			node* nodeBranch[M + 1];
			node* newNode;
			int j, j2;
			newNode = new node[sizeof(node)];

			nodeBranch[0] = p->branch[0];
			for (j = 0, j2 = 0; j < M; j++, j2++)		// 임시로 크기 M+1인 노드에 순서대로 복사. 
			{
				if (j == path)
				{
					nodeKey[j] = Key;
					nodeBranch[j + 1] = upRight;
					j++;
					if (j >= M)
						break;
				}
				nodeKey[j] = p->Key[j2];
				nodeBranch[j + 1] = p->branch[j2 + 1];
			}
			for (j = 0; j < M / 2; j++)
			{
				p->Key[j] = nodeKey[j];
				p->branch[j + 1] = nodeBranch[j + 1];
			}
			newNode->branch[0] = nodeBranch[M / 2 + 1];
			for (j = 0; j < M / 2; j++)	// 가운데 Key 다음부터는 새로생긴 노드에 복사한다. 
			{
				newNode->Key[j] = nodeKey[M / 2 + 1 + j];
				newNode->branch[j + 1] = nodeBranch[M / 2 + 2 + j];
			}

			// 만약에 p가 리프노드이면 약간의 수정
			if (p->count / M == 1)
			{
				newNode->branch[0] = p->branch[0];	// sequencial pointer 관리
				p->branch[0] = newNode;
				p->Key[M / 2] = nodeKey[M / 2];		// 올릴 Key값을 리프노드에도 남김. 
				p->branch[M / 2 + 1] = nodeBranch[M / 2 + 1];
				p->count = M + M / 2 + 1;
				newNode->count = M + M / 2;
			}
			else
			{
				p->count = newNode->count = M / 2;
				p->branch[0] = nodeBranch[0];
			}

			Key = nodeKey[M / 2];	// 가운데 Key를 올리는 Key로 한다. 
			upRight = newNode;	// 새로 만든 node를 올리는 값의 오른쪽 자식으로 
		}
		i--;
	}
	if (i == -1)	// root에서 overflow가 생겼을 경우
	{
		root = new node[sizeof(node)];
		root->count = 1;
		root->branch[0] = trace[0];
		root->branch[1] = upRight;
		root->Key[0] = Key;
	}

	return insertFileLocation;
}

node* put(node* k)
{
	queue[rear] = k;
	rear = ++rear % MAX;
	return k;
}

node* get()
{
	node* i;
	if (front == rear)
	{
		printf("\n Queue underflow.");
		return NULL;
	}
	i = queue[front];
	front = ++front % MAX;
	return i;
}

int isEmpty()
{
	return (front == rear);
}

void indexNodePrint(node* t)
{
	put(t);
	while (!isEmpty())
	{
		int i;
		t = get();
		printf("(");
		for (i = 0; i < t->count%M - 1; i++)
		{
			printf("%f, ", t->Key[i]);
		}
		printf("%f) ", t->Key[t->count%M - 1]);
		if (t->count / M != 1)
			for (i = 0; i <= t->count; i++)
				put(t->branch[i]);
	}

}



