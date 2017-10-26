#include <iostream>
using namespace std;

//存放访问的元素的下标
typedef struct index_struct {
	int x;
	int y;
}*PIndex;

typedef struct stack_struct {
	PIndex index_stack;
	int max_len;
	int len;
}*PStackIndex;


//迷宫的维数
const int n = 4;

// 0 表示不能走通 
// 1 表示能够走通
//a[0][0]      起点  恒为1
//a[n-1][n-1]  终点  恒为1
int maze[n][n] = {
	{1,0,0,0},
	{1,1,1,0},
	{1,1,1,1},
	{0,1,1,1}
};


//0 表示没有访问过
//1 表示访问过了
//初始为0 表示还没有访问过
struct visit_struct {
	int a[n][n] = { 0 };
};


//用来剪枝，寻找所有路劲中最短路径
int length = 65536;


void init_stack(PStackIndex &pstack, visit_struct vs, int len);
void push_back(PStackIndex pstack,index_struct index_s);
void pop_back(PStackIndex pstack);
void erase(PStackIndex pstack);
void find_path(PStackIndex pstack,visit_struct vs);
int test_visit(index_struct temp, visit_struct vs);
void delete_space(PStackIndex pstack);

void main()
{
	PStackIndex pstack_index = NULL;
	visit_struct visit_s;

	int max_len = 3;
	init_stack(pstack_index,visit_s,max_len);

	find_path(pstack_index,visit_s);

	//没有正确的路径
	if (length == 65536)
	{
		cout << "Can't find a right path\n" << endl;
		delete_space(pstack_index);
		getchar();

		return;
	}
	cout << "the length of shortest path is " << length << endl;
	cout << "one of the shortest path is:" << endl;
	for (int i = 0; i < length; i++)
	{
		cout << pstack_index->index_stack[i].x << " " << pstack_index->index_stack[i].y << endl;
	}
	delete_space(pstack_index);

	getchar();
}


void init_stack(PStackIndex &pstack, visit_struct vs, int len)
{
	pstack = (PStackIndex)malloc(sizeof(stack_struct));
	pstack->len = 0;
	pstack->max_len = len;
	pstack->index_stack = (PIndex)malloc(pstack->max_len * sizeof(index_struct));

	index_struct begin_index;//起点元素入栈
	begin_index.x = 0;
	begin_index.y = 0;

	vs.a[begin_index.x][begin_index.y] = 1;

	//压栈
	push_back(pstack, begin_index);
	//find_path(pstack, vs);
}

void push_back(PStackIndex pstack,index_struct index_s)
{
	if (pstack->len == pstack->max_len)
	{
		pstack->index_stack = (PIndex)realloc(pstack->index_stack,(pstack->max_len+10)*sizeof(index_struct));
		pstack->max_len += 10;
	}
	pstack->index_stack[pstack->len].x = index_s.x;
	pstack->index_stack[pstack->len].y = index_s.y;
	pstack->len++;

}

void pop_back(PStackIndex pstack)
{
	if (!pstack->len )
	{
		cout << "error, the stack is empty!" << endl;
		getchar();
		exit(0);
	}
	pstack->len--;
}

void find_path(PStackIndex pstack, visit_struct vs)
{
	int len = pstack->len;
	// 空的时候终止
	if (!len)
	{
		return;
	}
	//剪枝
	if (len > length)
	{
		pop_back(pstack);
		return;
	}
	//获取最后一个元素
	index_struct temp = pstack->index_stack[pstack->len - 1]; // 找到栈尾的元素
	index_struct next;

	//当到达终点的时候，当前路径的长度，并和上次的长度相比，寻找最小的路径
	if (temp.x == n - 1 && temp.y == n - 1)
	{
		
	/*	for (int i = 0; i < length; i++)
		{
			cout << pstack->index_stack[i].x << " " << pstack->index_stack[i].y << endl;
		}*/
		length = length > len ? len : length;
		pop_back(pstack);

		return;
	}


	//向上一步
	next.x = temp.x - 1;
	next.y = temp.y;
	if (next.x >= 0 && test_visit(next, vs) == 0 && maze[next.x][next.y] == 1)
	{
		vs.a[temp.x][temp.y] = 1;
		push_back(pstack, next);
		find_path(pstack, vs);
	}

	//向后一步
	next.x = temp.x;
	next.y = temp.y - 1;
	if (next.y >= 0 && test_visit(next, vs) == 0 && maze[next.x][next.y] == 1)
	{
		vs.a[temp.x][temp.y] = 1;
		push_back(pstack, next);
		find_path(pstack,vs);
	}

	//向前一步
	next.x = temp.x;
	next.y = temp.y + 1;
	if (next.y < n && test_visit(next, vs) == 0 && maze[next.x][next.y] == 1)
	{
		vs.a[temp.x][temp.y] = 1;
		push_back(pstack, next);
		find_path(pstack, vs);
	}

	//向下一步
	next.x = temp.x + 1;
	next.y = temp.y;
	if (next.x < n && test_visit(next, vs) == 0 && maze[next.x][next.y] == 1)
	{
		vs.a[temp.x][temp.y] = 1;
		push_back(pstack, next);
		find_path(pstack, vs);
	}

	//将该点出栈
	pop_back(pstack);
}

int test_visit(index_struct temp,visit_struct vs)
{
	return vs.a[temp.x][temp.y];
}

void delete_space(PStackIndex pstack)
{

	free(pstack->index_stack);
	free(pstack);
}


void erase(PStackIndex pstack)
{
	pstack->len = 0;
}
