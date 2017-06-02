#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define PUTLINE printf("*===========================================*\n");
#define M 5  // Ȧ�� ���� ���� 
#define MAX 500   // ť�� ũ��
#define TR 500 // ������ ũ��

typedef struct Node
{
	int count;      // ��忡 ����� Key�� ��, �ε��� ���� ������� ���� �÷���   
	int Key[M - 1];          // Key 
	struct Node* branch[M];  // �ּ� 
} node;

node* root;      // root�� ����ġ�� ���  
int front = 0, rear = 0;   // ť�� �����ϱ� ���ؼ� �ʱ�ȭ 
node* queue[MAX];   // ť�� ����ġ�� ���  
int findcnt;      // �˻� ��� �� 
FILE* fp;         //���� ������ 

void clrscr();
int* search(int k);      // Key�� Ž���ϴ� �Լ� 
void sequencialSearch(int k);   // ����Ž��(�������) ���  �Լ� 
void sequencialSearchF(int k);   // ����Ž��(�������) ���� ���� �Լ�
int* insertItem(int k);      // Key ���� �Լ� 
node* put(node* k);      // ť �Է� �Լ� 
node* get();   // ť ��� �Լ� 
int isEmpty();   // ť ���� üũ �Լ� 
void indexNodePrint(node* t);   // �ε��� ��� ��� �Լ�
void indexNodePrintF(node* t);  // �ε��� ��� ���� ���� �Լ�
void printEtc(node* t);      // ����Ʈ���� ��Ÿ ���� ��� �Լ�  
void printEtcF(node* t);   // ����Ʈ���� ��Ÿ ���� ���� ���� �Լ�
void insertkey();   // Key ���� �޴� �Լ�
void findkey();      // Key �˻� �޴� �Լ�
void printall();   // B+ Tree ��ü ���� ��� �Լ�
void insertfile();   //B+ Tree ���� ���� �Լ� 
void printfile();   //B+ Tree ��ü ���� ȭ�� ���� �Լ�



void main()
{
	char buff[2];
	int choice;
	root = NULL;
	printf("\
+------------------------------------+\n\
|        ���� ó���� (B+ TREE)       |\n\
|      ������(artwarez@paran.com)    |\n\
+------------------------------------+\n");
	printf("|         B+ Tree�� ���� : %d         |\n", M);
	while (1) {    // �޴�
		printf("\
+------------------------------------+\n\
|        M     E      N     U        |\n\
+------------------------------------+\n\
|      1. B+ TREE Ű�������         |\n\
|      2. B+ TREE �˻�               |\n\
|      3. B+ TREE ����               |\n\
|      4. B+ TREE ���               |\n\
|      5. B+ TREE ����(����)         |\n\
|      6. B+ TREE ���(����)         |\n\
|      7. ����                       |\n\
+------------------------------------+\n\
|   ���� �޴��� �ϳ��� �����ϼ���.   |\n\
+------------------------------------+\n\
         �޴� �Է�(1-5): ");

		choice = getchar();
		switch (choice)
		{
		case '1':
			system("cls");
			insertkey();
			clrscr();
			break;
		case '2':
			system("cls");
			findkey();
			clrscr();
			break;

		case '3':
			system("cls");
			clrscr();
			break;

		case '4':
			system("cls");
			printall();
			clrscr();
			break;

		case '5':
			system("cls");
			insertfile();
			clrscr();
			break;

		case '6':
			system("cls");
			printfile();
			clrscr();
			break;

		case '7':
			break;

		default:

			system("cls");
			break;
		}

		if (strcmp(buff, "7") == 0)
		{
			do {
				printf("   ���� �����Ͻðڽ��ϱ�?(y/n)");
			} while (!strlen(buff));
			if (strcmp(buff, "y") == 0)
				break;
			if (buff != "y")
				system("cls");
		}
	}
}

void insertkey()
{
	int Key;

	PUTLINE;
	printf("\n -- ���� Tree ���� -- \n ");
	printf("�ε��� ��� : ");
	indexNodePrint(root);
	printf("\n ���� ��� : ");
	sequencialSearch(0);
	printf("\n\n");
	PUTLINE;
	printf("   B+ Tree�� �����Ͻ� Key�� �Է��ϼ��� : ");
	scanf("%d", &Key);
	PUTLINE;
	insertItem(Key);
	printf("\n -- ���� Tree ���� -- \n ");
	printf("�ε��� ��� : ");
	indexNodePrint(root);
	printf("\n ���� ��� : ");
	sequencialSearch(0);
	printf("\n\n");
	PUTLINE;

}

void findkey()
{
	int *findadr;
	int Key;

	printf("\n");
	PUTLINE;
	printf(" �˻��Ͻ� Key�� �Է� �ϼ��� : ");
	scanf("%d", &Key);
	PUTLINE;
	findcnt = 1;
	findadr = search(Key);
	if (findadr != NULL)
	{
		printf(" -- �˻���� -- \n");
		printf(" �˻� Key :  %d \n", Key);
		printf(" ã�� ���(�ε��� ��� ����) : %d \n", findcnt);
		printf(" ����� ������ �ּ� : %d \n", findadr);
		printf(" ����� ������ ���� : %d \n", *findadr);
	}
	else
	{
		printf("   -- �˻���� -- \n");
		printf(" �˻��Ͻô� Key�� �����ϴ�. \n");
	}
	PUTLINE;
}

void delkey()
{
	int Key;

	PUTLINE;
	printf("\n -- ���� Tree ���� -- \n ");
	printf("�ε��� ��� : ");
	indexNodePrint(root);
	printf("\n ���� ��� : ");
	sequencialSearch(0);
	printf("\n\n");
	PUTLINE;
	printf("   B+ Tree���� �����Ͻ� Key�� �Է��ϼ��� : ");
	scanf("%d", &Key);
	PUTLINE;
	printf("\n -- ���� Tree ���� -- \n ");
	printf("�ε��� ��� : ");
	indexNodePrint(root);
	printf("\n ���� ��� : ");
	sequencialSearch(0);
	printf("\n\n");
	PUTLINE;
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
	printf(" -- Key�� ���� ������ �ּҿ� ���� -- \n");
	printEtc(root);
	printf("\n");
	PUTLINE;
}

void insertfile()
{
	char ch[16];
	char c;

	fp = fopen("insert.txt", "r");
	while (fscanf(fp, "%s", ch) == 1)
	{
		PUTLINE;
		printf("\n -- ���� Tree ���� -- \n ");
		printf("�ε��� ��� : ");
		indexNodePrint(root);
		printf("\n ���� ��� : ");
		sequencialSearch(0);
		printf("\n\n");
		PUTLINE;
		printf("\n B+ tree�� %d�� ����\n\n", atoi(ch));
		PUTLINE;
		insertItem(atoi(ch));
		printf("\n -- ���� Tree ���� -- \n ");
		printf("�ε��� ��� : ");
		indexNodePrint(root);
		printf("\n ���� ��� : ");
		sequencialSearch(0);
		printf("\n\n");
		PUTLINE;
		printf("\n\n <Enter> to Continue!!\n");
		scanf("%c", &c);
	}
	fclose(fp);
}

void printfile()
{

	fp = fopen("output.txt", "w");
	fprintf(fp, "*===========================================*\n");
	fprintf(fp, "\n - B+ Tree ���� -- \n ");
	fprintf(fp, "�ε��� ��� : ");
	indexNodePrintF(root);
	fprintf(fp, "\n ���� ��� : ");
	sequencialSearchF(0);
	fprintf(fp, "\n\n");
	fprintf(fp, " -- Key�� ���� ������ �ּҿ� ���� -- \n");
	printEtcF(root);
	fprintf(fp, "\n*===========================================*\n");
	fclose(fp);

	PUTLINE;
	printf(" ��� B+ Tree������ output.txt�� �����߽��ϴ�. \n");
	PUTLINE;
}

void clrscr()
{
	getchar();
	printf("\n\n ����ϽǷ��� ����Ű�� ��������.");
	getchar();
	system("cls");
}

int* search(int k)
{
	node* p = root;
	int path;
	if (p == NULL)
		return NULL;
	while (1)   // p�� leaf����϶����� Ž��
	{
		int j;
		for (j = 0; j < p->count%M; j++)   // �� ��忡�� ��θ� ����
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
		return (int*)p->branch[path + 1];
	}
	else
		return NULL;
}

void sequencialSearch(int k)
{
	int path, j;
	node* p = root;
	findcnt = 0;

	if (p != NULL)
	{
		while (1)   // p�� leaf��� �϶����� Ž��
		{
			int j;
			for (j = 0; j < p->count%M; j++)   // �� ��忡�� ��θ� ����
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
		if (p->Key[path] == k || k == 0)   // k�� 0�̸� minimum���� �� ����Ѵ�. 
		{
			while (p != NULL)
			{
				for (j = 0; j < p->count%M; j++)
				{
					printf("%d*>", *(int*)p->branch[j + 1]);
					//         str = (char*)*(int*)p->branch[j+1];
				}
				p = p->branch[0];
			}
		}
	}
}

void sequencialSearchF(int k)
{
	int path, j;
	node* p = root;
	findcnt = 0;

	if (p != NULL)
	{
		while (1)   // p�� leaf��� �϶����� Ž��
		{
			int j;
			for (j = 0; j < p->count%M; j++)   // �� ��忡�� ��θ� ����
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
		if (p->Key[path] == k || k == 0)   // k�� 0�̸� minimum���� �� ����Ѵ�. 
		{
			while (p != NULL)
			{
				for (j = 0; j < p->count%M; j++)
				{
					fprintf(fp, "%d*>", *(int*)p->branch[j + 1]);
				}
				p = p->branch[0];
			}
		}
	}
}

int* insertItem(int k)
{
	node* trace[TR];   // ���Ե� ��θ� ������ ���ÿ뵵�� �迭
	int dir[TR];
	int Key, i;

	node* upRight, *p;
	int* insertFileLocation = (int*)malloc(sizeof(int));
	*insertFileLocation = k;
	upRight = (node*)insertFileLocation;
	i = 0;   // trace[]�� index

	p = root;   // p�� ������ ���Ե� ��ġ�� Ž��

	*(int*)upRight = k;

	if (k <= 0)
	{
		printf("\n ����� Key�ν� �Է��� �� �ֽ��ϴ�. \n");
		return NULL;
	}

	if (root == NULL)
	{
		root = (node*)malloc(sizeof(node));
		root->branch[0] = NULL;
		root->Key[0] = k;
		root->branch[1] = (node*)insertFileLocation;
		root->count = M + 1;
		return insertFileLocation;
	}

	while (1)   // p�� leaf��� �϶����� Ž��
	{
		int j;
		trace[i] = p;
		for (j = 0; j < p->count%M; j++)   // �� ��忡�� ��θ� ����
			if (p->Key[j] >= k)
			{
				dir[i] = j;
				break;
			}
		if (j == p->count%M)
			dir[i] = p->count%M;
		if (p->count / M == 1)
			break;
		p = p->branch[j];
		i++;
	}                  // �� �������� ������ p�� Key���� ���ϵ� ���. 

	if (p->Key[dir[i]] == k)   // ���� Key�� �̹� Ʈ���� ����. 
	{
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
		if (p->count%M != M - 1)   // �����ص� overflow�� ������ ������
		{
			int m;
			for (m = p->count%M; m > path; m--)   // ���Ե� ĭ���� ������ ��ĭ�� �ڷ�. 
			{
				p->Key[m] = p->Key[m - 1];
				p->branch[m + 1] = p->branch[m];
			}
			p->Key[path] = Key;      // Key���� ����
			p->branch[path + 1] = upRight;   // branch�� ����. 
			p->count++;
			break;
		}

		else   // �����ϸ� overflow�� ����� ���
		{
			int nodeKey[M];
			node* nodeBranch[M + 1];
			node* newNode;
			int j, j2;
			newNode = (node*)malloc(sizeof(node));

			nodeBranch[0] = p->branch[0];
			for (j = 0, j2 = 0; j < M; j++, j2++)      // �ӽ÷� ũ�� M+1�� ��忡 ������� ����. 
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
			for (j = 0; j < M / 2; j++)   // ��� Key �������ʹ� ���λ��� ��忡 �����Ѵ�. 
			{
				newNode->Key[j] = nodeKey[M / 2 + 1 + j];
				newNode->branch[j + 1] = nodeBranch[M / 2 + 2 + j];
			}

			// ���࿡ p�� ��������̸� �ణ�� ����
			if (p->count / M == 1)
			{
				newNode->branch[0] = p->branch[0];   // sequencial pointer ����
				p->branch[0] = newNode;
				p->Key[M / 2] = nodeKey[M / 2];      // �ø� Key���� ������忡�� ����. 
				p->branch[M / 2 + 1] = nodeBranch[M / 2 + 1];
				p->count = M + M / 2 + 1;
				newNode->count = M + M / 2;
			}
			else
			{
				p->count = newNode->count = M / 2;
				p->branch[0] = nodeBranch[0];
			}

			Key = nodeKey[M / 2];   // ��� Key�� �ø��� Key�� �Ѵ�. 
			upRight = newNode;   // ���� ���� node�� �ø��� ���� ������ �ڽ����� 
		}
		i--;
	}
	if (i == -1)   // root���� overflow�� ������ ���
	{
		root = (node*)malloc(sizeof(node));
		root->count = 1;
		root->branch[0] = trace[0];
		root->branch[1] = upRight;
		root->Key[0] = Key;
	}

	return insertFileLocation;
}

int deleteItem(int k)
{
	node* trace[MAX];
	int dir[MAX];
	int i, j, path;
	node* q;
	node* p = root;
	i = 0;

	if (p == NULL)
		return 0;   // �� tree
	while (1)   // p�� leaf��� �϶����� Ž���Ѵ�
	{
		int j;
		trace[i] = p;
		for (j = 0; j < p->count%M; j++)   // �� ��忡�� ��θ� ����
			if (p->Key[j] >= k)
			{
				dir[i] = j;
				break;
			}
		if (j == p->count%M)
			dir[i] = p->count%M;
		if (p->count / M == 1)
			break;
		p = p->branch[j];
		i++;
	}   // �� �������� ������ p�� Key���� ������ ���. 
	path = dir[i];
	if (p->Key[path] != k)
	{
		printf("   ### �����Ϸ��� Key�� �����ϴ�.  ###\n");
		return 0;
	}
	// leaf���� ã�� k���� �����. 
	free(p->branch[path + 1]);
	for (j = path; j < p->count%M - 1; j++)
	{
		p->Key[j] = p->Key[j + 1];
		p->branch[j + 1] = p->branch[j + 2];
	}
	p->count--;
	if (p->count%M >= M / 2)
		return 1;

	// Key���� ���ڶ�ԵǸ� ��й�, �Ǵ� �պ��� �Ѵ�. 
	else
	{
		if (i == 0)   // ���� ��尡 leaf�̸鼭 root���
		{
			if (p->count%M == 0)   // root�� ������, height�� 1 ��������. 
			{
				free(p);
				root = NULL;
			}
			return 1;
		}
		else      // ���� ��尡 root�� �ƴ϶�� 
		{
			node* sibling;
			q = trace[i - 1];
			path = dir[i - 1];

			if (path != q->count && q->branch[path + 1]->count%M > M / 2)   // �����ʿ��� ������
			{
				sibling = q->branch[path + 1];
				p->Key[M / 2 - 1] = sibling->Key[0];
				q->Key[path] = sibling->Key[0];
				p->branch[M / 2] = sibling->branch[1];
				p->count++;
				sibling->count--;
				for (j = 0; j < sibling->count%M; j++)
				{
					sibling->Key[j] = sibling->Key[j + 1];
					sibling->branch[j + 1] = sibling->branch[j + 2];
				}
				return 1;
			}

			else if (path != 0 && q->branch[path - 1]->count%M > M / 2)   // ���ʿ��� ������
			{
				sibling = q->branch[path - 1];
				for (j = M / 2 - 1; j > 0; j--)
				{
					p->Key[j] = p->Key[j - 1];
					p->branch[j + 1] = p->branch[j];
				}
				p->Key[0] = sibling->Key[sibling->count%M - 1];
				p->branch[1] = sibling->branch[sibling->count%M];
				q->Key[path - 1] = sibling->Key[sibling->count%M - 2];
				p->count++;
				sibling->count--;
				return 1;
			}

			else if (path != q->count)   // �����ʰ� �պ�
			{
				sibling = q->branch[path + 1];
				for (j = 0; j < M / 2; j++)
				{
					p->Key[M / 2 - 1 + j] = sibling->Key[j];
					p->branch[M / 2 + j] = sibling->branch[j + 1];
				}
				p->branch[0] = sibling->branch[0];
				p->count = 2 * M - 2;
				free(sibling);
			}
			else   // ���ʰ� �պ�
			{
				sibling = q->branch[path - 1];
				for (j = 0; j < M / 2 - 1; j++)
				{
					sibling->Key[M / 2 + j] = p->Key[j];
					sibling->branch[M / 2 + 1 + j] = p->branch[j + 1];
				}
				sibling->branch[0] = p->branch[0];
				sibling->count = 2 * M - 2;
				free(p);
				p = sibling;
				dir[i - 1]--;
			}
		}
		i--;
	}

	// ���� ���γ�带 �˻��ϸ鼭 Key���� ���ڶ� �κ��� �ٽ� ���߾��ش�. 
	// ������ trace[i]�� dir[i]��° Key�� ��������. 
	while (1)
	{
		node* q, *sibling;
		int path;
		int del = dir[i];
		p = trace[i];
		for (j = del; j < p->count - 1; j++)
		{
			p->Key[j] = p->Key[j + 1];
			p->branch[j + 1] = p->branch[j + 2];
		}
		p->count--;

		if (i == 0)      // root�����
		{
			if (p->count == 0)
			{
				root = p->branch[0];
				free(p);
			}
			return 1;
		}
		else   // root�� �ƴϸ�
		{
			q = trace[i - 1];
			path = dir[i - 1];
			if (p->count >= M / 2)   // Key�� �� �̻��̸� ����
				return 1;
			if (path != q->count && q->branch[path + 1]->count > M / 2)   //   �����ʿ��� ����
			{
				sibling = q->branch[path + 1];
				p->Key[M / 2 - 1] = q->Key[path];
				q->Key[path] = sibling->Key[0];
				p->branch[M / 2] = sibling->branch[0];
				p->count++;
				sibling->count--;
				for (j = 0; j < sibling->count + 1; j++)
				{
					if (j != 0)
						sibling->Key[j - 1] = sibling->Key[j];
					sibling->branch[j] = sibling->branch[j + 1];
				}
				return 1;
			}

			else if (path != 0 && q->branch[path - 1]->count > M / 2)   // ���ʿ��� ����
			{
				sibling = q->branch[path - 1];
				for (j = M / 2 - 1; j >= 0; j--)
				{
					if (j != 0)
						p->Key[j] = p->Key[j - 1];
					p->branch[j + 1] = p->branch[j];
				}
				p->Key[0] = q->Key[path - 1];
				p->branch[0] = sibling->branch[sibling->count%M];
				q->Key[path - 1] = sibling->Key[sibling->count%M - 1];
				p->count++;
				sibling->count--;
				return 1;
			}

			else if (path != q->count)   // �����ʰ� �պ�
			{
				sibling = q->branch[path + 1];
				for (j = 0; j < M / 2; j++)
				{
					p->Key[M / 2 + j] = sibling->Key[j];
					p->branch[M / 2 + j + 1] = sibling->branch[j + 1];
				}
				p->Key[M / 2 - 1] = q->Key[path];
				p->branch[M / 2] = sibling->branch[0];
				p->count = M - 1;
				i--;
			}
			else   // ���ʰ� �պ�
			{
				sibling = q->branch[path - 1];
				for (j = 0; j < M / 2 - 1; j++)
				{
					if (j != 0)
						sibling->Key[M / 2 + j] = p->Key[j - 1];
					sibling->branch[M / 2 + 1 + j] = p->branch[j];
				}
				sibling->Key[M / 2] = q->Key[path - 1];
				sibling->count = M - 1;
				free(p);
				p = sibling;
				dir[i - 1]--;
				i--;
			}
		}
	}
	return 1;
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
	if (t == NULL)
	{
		printf(" NULL�Դϴ�.");
	}
	else
	{
		put(t);
		while (!isEmpty())
		{
			int i;
			t = get();
			printf("(");
			for (i = 0; i < t->count%M - 1; i++)
			{
				printf("%d, ", t->Key[i]);
			}
			printf("%d) ", t->Key[t->count%M - 1]);
			if (t->count / M != 1)
				for (i = 0; i <= t->count; i++)
					put(t->branch[i]);
		}
	}
}

void indexNodePrintF(node* t)
{
	if (t == NULL)
	{
		printf(" NULL�Դϴ�.");
	}
	else
	{
		put(t);
		while (!isEmpty())
		{
			int i;
			t = get();
			fprintf(fp, "(");
			for (i = 0; i < t->count%M - 1; i++)
			{
				fprintf(fp, "%d, ", t->Key[i]);
			}
			fprintf(fp, "%d) ", t->Key[t->count%M - 1]);
			if (t->count / M != 1)
				for (i = 0; i <= t->count; i++)
					put(t->branch[i]);
		}
	}
}

void printEtc(node* t)
{
	int x;
	int* y;

	for (x = 0; x < (MAX * 10); x++)
	{
		y = search(x);
		if (y != NULL)
		{
			printf(" Key�� %d�� ������ �ּҴ� %d�̰� ����� ������ %d�Դϴ�. \n", x, y, *y);

		}
	}
}

void printEtcF(node* t)
{
	int x;
	int* y;

	for (x = 0; x < (MAX * 10); x++)
	{
		y = search(x);
		if (y != NULL)
		{
			fprintf(fp, " Key�� %d�� ������ �ּҴ� %d�̰� ����� ������ %d�Դϴ�. \n", x, y, *y);

		}
	}
}

