#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MM 1			
#define M (MM * 2 + 1) 
#define MAX 50000000	
#define TR 50000000

typedef struct Node
{
	int count;			
	float Key[M - 1];
	int Id[M - 1];
	struct Node* branch[M]; 
} node;

node* root;		  
int front = 0, rear = 0;	 
node* queue[MAX];	 
int findcnt;		 
FILE* fp;			 

int* search(int k);		 
void sequencialSearch(int k);	// 순차탐색(리프노드) 출력  함수 
void sequencialSearchF(int k);	// 순차탐색(리프노드) 파일 저장 함수
int* insertItem(int k);	// Key 삽입 함수 
int deleteItem(int k);	// key 삭제 함수 
node* put(node* k);	// 큐 입력 함수 
node* get();	// 큐 출력 함수 
void indexNodePrint(node* t);	// 인덱스 노드 출력 함수
void indexNodePrintF(node* t);  // 인덱스 노드 파일 저장 함수
void insertfile();	//B+ Tree 파일 삽입 함수 
void printfile();	//B+ Tree 전체 정보 화일 저장 함수



void main(){
	insertfile();
	printfile();
}

void insertfile()
{
	char ch[16];
	char c;

	fp = fopen("sampleData.csv", "r");
	while (fscanf(fp, "%s", ch) == 1)
	{
		indexNodePrint(root);
		sequencialSearch(0);
		insertItem(atoi(ch));
		indexNodePrint(root);
		sequencialSearch(0);
		scanf("%c", &c);
	}
	fclose(fp);
}

void printfile()
{
	fp = fopen("Students_score.idx", "wb");
	indexNodePrintF(root);
	sequencialSearchF(0);
	fclose(fp);
}

int* search(int k)
{
	node* p = root;
	int path;
	if (p == NULL)
		return NULL;
	while (1)
	{
		int j;
		for (j = 0; j<p->count%M; j++)
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
		while (1)	// p가 leaf노드 일때까지 탐색
		{
			int j;
			for (j = 0; j<p->count%M; j++)	// 한 노드에서 경로를 결정
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
				for (j = 0; j<p->count%M; j++)
				{
					printf("%d*>", *(int*)p->branch[j + 1]);
					//			str = (char*)*(int*)p->branch[j+1];
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
		while (1)	// p가 leaf노드 일때까지 탐색
		{
			int j;
			for (j = 0; j<p->count%M; j++)	// 한 노드에서 경로를 결정
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
				for (j = 0; j<p->count%M; j++)
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
	node* trace[TR];
	int dir[TR];
	int Key, i;

	node* upRight, *p;
	int* insertFileLocation = (int*)malloc(sizeof(int));
	upRight = (node*) insertFileLocation;
	i = 0;	// trace[]의 index

	*insertFileLocation = k;
	p = root;	// p를 가지고 삽입될 위치를 탐색

	*(int*)upRight = k;

	if (root == NULL)
	{
		root = (node*)malloc(sizeof(node));
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
		for (j = 0; j < p->count % M; j++)	// 한 노드에서 경로를 결정
			if (p->Key[j] >= k)
			{
				dir[i] = j;
				break;
			}
		if (j == p->count%M)
			dir[i] = p->count % M;
		if (p->count / M == 1)
			break;
		p = p->branch[j];
		i++;
	}						// 이 루프에서 나오면 p는 Key값이 삽일될 노드. 

	if (p->Key[dir[i]] == k)	// 같은 Key가 이미 트리에 존재. 
	{
		free(upRight);
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
			for (m = p->count % M; m > path; m--)	// 삽입될 칸부터 끝까지 한칸씩 뒤로. 
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
			int nodeKey[M];
			node* nodeBranch[M + 1];
			node* newNode;
			int j, j2;
			newNode = (node*)malloc(sizeof(node));

			nodeBranch[0] = p->branch[0];
			for (j = 0, j2 = 0; j<M; j++, j2++)		// 임시로 크기 M+1인 노드에 순서대로 복사. 
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
			for (j = 0; j<M / 2; j++)
			{
				p->Key[j] = nodeKey[j];
				p->branch[j + 1] = nodeBranch[j + 1];
			}
			newNode->branch[0] = nodeBranch[M / 2 + 1];
			for (j = 0; j<M / 2; j++)	// 가운데 Key 다음부터는 새로생긴 노드에 복사한다. 
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
		return 0;
	while (1)	
	{
		int j;
		trace[i] = p;
		for (j = 0; j < p->count%M; j++) {
			if (p->Key[j] >= k)
			{
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
	} 
	path = dir[i];
	if (p->Key[path] != k)
	{
		return 0;
	}
	free(p->branch[path + 1]);
	for (j = path; j<p->count%M - 1; j++)
	{
		p->Key[j] = p->Key[j + 1];
		p->branch[j + 1] = p->branch[j + 2];
	}
	p->count--;
	if (p->count%M >= M / 2)
		return 1;
	
	else
	{
		if (i == 0)	// 현재 노드가 leaf이면서 root라면
		{
			if (p->count%M == 0)	// root가 없어짐, height가 1 낮아진다. 
			{
				free(p);
				root = NULL;
			}
			return 1;
		}
		else		// 현재 노드가 root가 아니라면 
		{
			node* sibling;
			q = trace[i - 1];
			path = dir[i - 1];

			if (path != q->count && q->branch[path + 1]->count%M > M / 2)	// 오른쪽에서 빌려옴
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

			else if (path != 0 && q->branch[path - 1]->count%M > M / 2)	// 왼쪽에서 빌려옴
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

			else if (path != q->count)	// 오른쪽과 합병
			{
				sibling = q->branch[path + 1];
				for (j = 0; j<M / 2; j++)
				{
					p->Key[M / 2 - 1 + j] = sibling->Key[j];
					p->branch[M / 2 + j] = sibling->branch[j + 1];
				}
				p->branch[0] = sibling->branch[0];
				p->count = 2 * M - 2;
				free(sibling);
			}
			else	// 왼쪽과 합병
			{
				sibling = q->branch[path - 1];
				for (j = 0; j<M / 2 - 1; j++)
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

	// 이제 내부노드를 검사하면서 Key값이 모자란 부분을 다시 맞추어준다. 
	// 현재노드 trace[i]의 dir[i]번째 Key가 지워진다. 
	while (1)
	{
		node* q, *sibling;
		int path;
		int del = dir[i];
		p = trace[i];
		for (j = del; j<p->count - 1; j++)
		{
			p->Key[j] = p->Key[j + 1];
			p->branch[j + 1] = p->branch[j + 2];
		}
		p->count--;

		if (i == 0)		// root노드라면
		{
			if (p->count == 0)
			{
				root = p->branch[0];
				free(p);
			}
			return 1;
		}
		else	// root가 아니면
		{
			q = trace[i - 1];
			path = dir[i - 1];
			if (p->count >= M / 2)	// Key가 반 이상이면 종료
				return 1;
			if (path != q->count && q->branch[path + 1]->count > M / 2)	//	오른쪽에서 빌림
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

			else if (path != 0 && q->branch[path - 1]->count > M / 2)	// 왼쪽에서 빌림
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

			else if (path != q->count)	// 오른쪽과 합병
			{
				sibling = q->branch[path + 1];
				for (j = 0; j<M / 2; j++)
				{
					p->Key[M / 2 + j] = sibling->Key[j];
					p->branch[M / 2 + j + 1] = sibling->branch[j + 1];
				}
				p->Key[M / 2 - 1] = q->Key[path];
				p->branch[M / 2] = sibling->branch[0];
				p->count = M - 1;
				i--;
			}
			else	// 왼쪽과 합병
			{
				sibling = q->branch[path - 1];
				for (j = 0; j<M / 2 - 1; j++)
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
		return NULL;
	}
	i = queue[front];
	front = ++front % MAX;
	return i;
}

void indexNodePrint(node* t)
{
	if (t == NULL)
	{
		printf(" NULL입니다.");
	}
	else
	{
		put(t);
		while (!(front == rear))
		{
			int i;
			t = get();
			printf("(");
			for (i = 0; i<t->count%M - 1; i++)
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
		printf(" NULL입니다.");
	}
	else
	{
		put(t);
		while (!(front == rear))
		{
			int i;
			t = get();
			fprintf(fp, "(");
			for (i = 0; i<t->count%M - 1; i++)
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



