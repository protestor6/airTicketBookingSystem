#pragma once
#include <vector>
namespace YysPart{
	//宏定义
	#define MinVNum 10	//最少顶点数（城市数）
	#define MinANum 15	//最小边数 
	#define MaxVNum 15	//最大顶点数（城市数）
	#define MaxANum 210	//最大边数（实际上最多用到20条）  
	#define MaxSNum 20	//地名最大长度 
	#define INF 65536	//邻接矩阵中的无穷大 
	//结构体类型 
		//图类型
		typedef struct{
			char vexs[MaxVNum][MaxSNum];	//字符串，第一维表示顶点表，第二维表示字符串
			double arcs[MaxVNum][MaxVNum];	//邻接矩阵，这里矩阵里存储的是票务表中的索引，价格等信息需要从表里面找，-1表示MAX 
			int vexnum,arcnum;	//图当前的点数和边数 
			bool visited[MaxVNum],available[MaxVNum][MaxVNum];	//是否已经遍历过，用于dfs和bfs；是否可用（如果不可用则为false） 
		}Graph;
		//栈类型（用于dijkstra打印结果） 
		typedef struct{
			int base[MaxVNum];
			int top;
		}Stack;
	//（图）核心功能
		void fileInit(Graph &G);	//1.用文件读写实现飞行网络存储（用这种方式建图方便修改） 
		void Dijkstra(Graph& G,int v0,int dest);	//2.dijkstra求票价最低转机路线
		//3.以下两个函数实现第三个功能：求转机次数最少的路线（bfs）
			void dfs(Graph& G,int curr,int dest,std::vector<int> &path
			,std::vector<std::vector<int> > &allPath);	//进行深度优先搜索遍历，用path记录途径的路线，找到dest就把路线存进allPath 
			void getAllRoutes(Graph& G,int start,int dest);	//调用dfs函数，获得图中从start到dest所有可能的转机路线，并打印
		void minTransRoute(Graph &G,int start,int dest);	//4.bfs得出转机次数最少的路线
	//实现图核心功能需要用到的其他函数
		//栈相关
			void initStack(Stack &S);	//初始化栈 
			void Spush(Stack &S,int e);	//压入栈 
			void Spop(Stack &S,int &e);	//从栈弹出存进e中 
			bool Sempty(const Stack& S);	//判断栈是否为空 
			int Stop(const Stack& S);	//返回栈顶元素 
		//图相关
			int getVIndex(const Graph& G,const char* str);	//返回该字符串对应点的下标，没找到则返回-1 
			bool isConnected(const Graph& G,int an,int bn);	//两点是否直接连通，anbn为点的下标 
			bool isConnected(const Graph& G,const char* sour,const char* dest);	//两点是否直接连通 
			void resetVisited(Graph &G);	//把visited数组全部设为false，在遍历图之前必须调用 
	//测试用函数
		void showGraph(const Graph& G);	//打印图的全部信息 
	//对外提供的函数
		//给朱琳
			void setArcs(Graph &G,const char* sour,const char* dest,double price);	//将图中sour->dest这条边价格设为price
			void setAvailability(Graph &G,const char* sour,const char* dest,bool flag);	//将图中sour->dest这条边可用性设为flag  
}
