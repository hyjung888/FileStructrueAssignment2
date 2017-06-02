#include <cstdio>
#include <cstdlib>
#include <cstring> 
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define PUTLINE printf("*===========================================*\n");
#define M 5  // Ȧ�� ���� ���� 
#define MAX 4000	// ť�� ũ��
#define TR 4000 // ������ ũ��

typedef struct Node
{
	int count;
	float Key[M - 1];
	struct Node* branch[M];
} node;

node* root;
int front = 0, rear = 0;	// ť�� �����ϱ� ���ؼ� �ʱ�ȭ 
node* queue[MAX];	// ť�� ����ġ�� ���  
int findcnt;		// �˻� ��� �� 
ifstream fin;			//���� ������ 
FILE* fp;

float* search(float k);		// Key�� Ž���ϴ� �Լ� 
void sequencialSearch(float k);	// ����Ž��(�������) ���  �Լ� 
void sequencialSearchF(float k);	// ����Ž��(�������) ���� ���� �Լ�
float* insertItem(float k);		// Key ���� �Լ� 
node* put(node* k);		// ť �Է� �Լ� 
node* get();	// ť ��� �Լ� 
int isEmpty();	// ť ���� üũ �Լ� 
void indexNodePrint(node* t);	// �ε��� ��� ��� �Լ�
void printall();	// B+ Tree ��ü ���� ��� �Լ�
void printfile();	//B+ Tree ��ü ���� ȭ�� ���� �Լ�
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
	printf("\n -- ���� Tree ���� -- \n ");
	printf("�ε��� ��� : ");
	indexNodePrint(root);
	printf("\n ���� ��� : ");
	sequencialSearch(0);
	printf("\n\n");
	PUTLINE;
}

void printfile()
{

	fp = fopen("Students_score.idx", "wb");
	fprintf(fp, "*===========================================*\n");
	fprintf(fp, "\n - B+ Tree ���� -- \n ");
	fprintf(fp, "\n ���� ��� :\n");
	sequencialSearchF(0);
	fprintf(fp, "\n*===========================================*\n");
	fclose(fp);

	PUTLINE;
	printf(" ��� B+ Tree������ Students_score.idx�� �����߽��ϴ�. \n");
	PUTLINE;
}

float* search(float k)
{
	node* p = root;
	int path;
	if (p == NULL)
		return NULL;
	while (1)	// p�� leaf����϶����� Ž��
	{
		int j;
		for (j = 0; j < p->count%M; j++)	// �� ��忡�� ��θ� ����
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
		while (1)	// p�� leaf��� �϶����� Ž��
		{
			for (j = 0; j < p->count%M; j++)	// �� ��忡�� ��θ� ����
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
		if (p->Key[path] == k || k == 0)	// k�� 0�̸� minimum���� �� ����Ѵ�. 
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
		while (1)	// p�� leaf��� �϶����� Ž��
		{
			int j;
			for (j = 0; j < p->count%M; j++)	// �� ��忡�� ��θ� ����
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
		if (p->Key[path] == k || k == 0)	// k�� 0�̸� minimum���� �� ����Ѵ�. 
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
	node* trace[TR];	// ���Ե� ��θ� ������ ���ÿ뵵�� �迭
	int dir[TR];
	float Key;
	int i;

	node* upRight, *p;
	float* insertFileLocation = new float[sizeof(float)];
	*insertFileLocation = k;
	upRight = (node*)insertFileLocation;
	i = 0;	// trace[]�� index

	p = root;	// p�� ������ ���Ե� ��ġ�� Ž��

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

	while (1)	// p�� leaf��� �϶����� Ž��
	{
		int j;
		trace[i] = p;
		for (j = 0; j < p->count%M; j++) {	// �� ��忡�� ��θ� ����
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
	}						// �� �������� ������ p�� Key���� ���ϵ� ���. 

	if (p->Key[dir[i]] == k) {
		free(upRight);
		printf("   ### �̹� �Է��Ͻ� Key�� �ֽ��ϴ�. ###\n");
		return NULL;
	}

	// ���� �������� ������ ����. 
	Key = k;
	while (i != -1)
	{
		int path = dir[i];
		p = trace[i];
		if (p->count%M != M - 1)	// �����ص� overflow�� ������ ������
		{
			int m;
			for (m = p->count%M; m>path; m--)	// ���Ե� ĭ���� ������ ��ĭ�� �ڷ�. 
			{
				p->Key[m] = p->Key[m - 1];
				p->branch[m + 1] = p->branch[m];
			}
			p->Key[path] = Key;		// Key���� ����
			p->branch[path + 1] = upRight;	// branch�� ����. 
			p->count++;
			break;
		}

		else	// �����ϸ� overflow�� ����� ���
		{
			float nodeKey[M];
			node* nodeBranch[M + 1];
			node* newNode;
			int j, j2;
			newNode = new node[sizeof(node)];

			nodeBranch[0] = p->branch[0];
			for (j = 0, j2 = 0; j < M; j++, j2++)		// �ӽ÷� ũ�� M+1�� ��忡 ������� ����. 
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
			for (j = 0; j < M / 2; j++)	// ��� Key �������ʹ� ���λ��� ��忡 �����Ѵ�. 
			{
				newNode->Key[j] = nodeKey[M / 2 + 1 + j];
				newNode->branch[j + 1] = nodeBranch[M / 2 + 2 + j];
			}

			// ���࿡ p�� ��������̸� �ణ�� ����
			if (p->count / M == 1)
			{
				newNode->branch[0] = p->branch[0];	// sequencial pointer ����
				p->branch[0] = newNode;
				p->Key[M / 2] = nodeKey[M / 2];		// �ø� Key���� ������忡�� ����. 
				p->branch[M / 2 + 1] = nodeBranch[M / 2 + 1];
				p->count = M + M / 2 + 1;
				newNode->count = M + M / 2;
			}
			else
			{
				p->count = newNode->count = M / 2;
				p->branch[0] = nodeBranch[0];
			}

			Key = nodeKey[M / 2];	// ��� Key�� �ø��� Key�� �Ѵ�. 
			upRight = newNode;	// ���� ���� node�� �ø��� ���� ������ �ڽ����� 
		}
		i--;
	}
	if (i == -1)	// root���� overflow�� ������ ���
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



