//图相关，yys制作2025/12/3 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
namespace YysPart{
	#include "yys.h"
	typedef struct{
		char vexs[MaxVNum][MaxSNum];	//字符串，第一维表示顶点表，第二维表示字符串
		double arcs[MaxVNum][MaxVNum];	//邻接矩阵，这里矩阵里存储的是价格等信息需要从表里面找，65536表示INF,65535表示有边 
		int vexnum,arcnum;	//图当前的点数和边数 
		bool visited[MaxVNum],available[MaxVNum][MaxVNum];	//是否已经遍历过，用于dfs和bfs；是否可用（如果不可用则为false） 
	}Graph;
	typedef struct{
		int base[MaxVNum];
		int top;
	}Stack;
	void initStack(Stack &S){
		S.top=0;
	}
	void Spush(Stack &S,int e){
		S.base[S.top++]=e;
	}
	void Spop(Stack &S,int &e){
		e=S.base[--S.top];
	}
	bool Sempty(const Stack& S){
		return S.top==0;
	}
	int Stop(const Stack& S){
		return S.base[S.top-1];
	}
	void fileInit(Graph &G){	//读写文件，获取图的信息，每两点之间的权值是票务表中的票价 
		FILE *fin;
		fin=fopen("yys/info.txt","r");
		if(fin==NULL){	//打开失败 
			printf("未找到航行图文件，将生成\n");
			printf("请输入地点数和航班数（用空格隔开）\n");
			scanf("%d%d",&G.vexnum,&G.arcnum);
			if(G.vexnum<MinVNum||G.arcnum<MinANum){
				printf("错误：地点数或航班数过小");
				exit(1);
			}
			printf("请依次输入每个地点的名称（仅限英文字母，最大长度为20，不能有空格）\n");
			for(int i=0;i<G.vexnum;i++)
				scanf("%s",G.vexs[i]);
			printf("输入邻接矩阵（-1表示无穷大，其他数字代表有航班连通两地）\n");
			for(int i=0;i<G.vexnum;i++){
				printf("第%d/%d行：",i+1,G.vexnum);
				for(int j=0;j<G.vexnum;j++)
					scanf("%lf",&G.arcs[i][j]);
			}
			//开始录入信息
			FILE *fout;
			fout=fopen("yys/info.txt","w");
			fprintf(fout,"%d %d\n",G.vexnum,G.arcnum);
			for(int i=0;i<G.vexnum;i++)
				fprintf(fout,"%s\n",G.vexs[i]);
			for(int i=0;i<G.vexnum;i++){
				for(int j=0;j<G.vexnum;j++)
					fprintf(fout,"%lf ",G.arcs[i][j]);
				fprintf(fout,"\n");
			}
			fclose(fout);
			exit(0);
		}
		fscanf(fin,"%d%d",&G.vexnum,&G.arcnum);
		for(int i=0;i<G.vexnum;i++)
			fscanf(fin,"%s",G.vexs[i]);
		for(int i=0;i<G.vexnum;i++)
			for(int j=0;j<G.vexnum;j++)
				fscanf(fin,"%lf",&G.arcs[i][j]);
		fclose(fin); 
		memset(G.visited,false,sizeof(G.visited));
		for(int i=0;i<G.vexnum;i++)
			for(int j=0;j<G.vexnum;j++)
				G.available[i][j]=false;
	}
	void showGraph(const Graph& G){	//打印图的信息 
		printf("地点数：%d 航班数：%d\n",G.vexnum,G.arcnum);
		printf("图中每个地点名称：\n");
		for(int i=0;i<G.vexnum;i++)
			printf("%s\n",G.vexs[i]);
		printf("邻接矩阵（INF表示两点不通，其他数字代表有航班连通两地）：\n"); 
		for(int i=0;i<G.vexnum;i++){
			for(int j=0;j<G.vexnum;j++)
				if(G.arcs[i][j]==INF)
					printf("%7s ","INF");
				else
					printf("%7.2lf ",G.arcs[i][j]);
			printf("\n");
		}
		printf("可用性：\n");
		for(int i=0;i<G.vexnum;i++){
			for(int j=0;j<G.vexnum;j++)
				printf("%4d ",G.available[i][j]);
			printf("\n");
		}
	}
	int getVIndex(const Graph& G,const char* str){	//返回该字符串对应点的下标，没找到则返回-1 
		int i;
		for(i=G.vexnum-1;i>=0&&strcmp(G.vexs[i],str)!=0;i--);
		return i;
	}
	bool isConnected(const Graph& G,int an,int bn){	//两地是否直接连通 
		if(an==-1||bn==-1)	return false;
		if(G.arcs[an][bn]!=INF)	return true;
		return false;
	} 
	bool isConnected(const Graph& G,const char* sour,const char* dest){	//两地是否直接连通 
		return isConnected(G,getVIndex(G,sour),getVIndex(G,dest));
	} 
	void setArcs(Graph &G,const char* sour,const char* dest,double price){	//将图中sour->dest这条边价格设为price
		int an=getVIndex(G,sour),bn=getVIndex(G,dest);
		if(an==-1||bn==-1){
			printf("错误：不存在此地区\n");
			return;
		}
		if(!isConnected(G,an,bn)){
			printf("错误：这两地不直接连通\n");
			return;
		}
		G.arcs[an][bn]=price;
	}
	void setAvailability(Graph &G,const char* sour,const char* dest,bool flag){	//将图中sour->dest这条边可用性设为flag 
		int an=getVIndex(G,sour),bn=getVIndex(G,dest);
		if(an==-1||bn==-1){
			printf("错误：不存在此地区\n");
			return;
		}
		if(!isConnected(G,an,bn)){
			printf("错误：这两地不直接连通\n");
			return;
		}
		G.available[an][bn]=flag;
	}
	bool getAvailability(Graph &G,const char* sour,const char* dest){	//获得图中sour->dest这条边可用性
		int an=getVIndex(G,sour),bn=getVIndex(G,dest);
		if(an==-1||bn==-1){
			return false;
		}
		if(!isConnected(G,an,bn)){
			return false;
		}
		return G.available[an][bn];
	}
	char* currentTime(){
		FILE *fin;
		static char cTime[17];	//防止局部变量销毁导致的野指针 
		fin=fopen("yys/currentTime.txt","r");
		if(fin==NULL){	//没有该文件
			printf("错误：未找到currentTime.txt，将生成...\n");
			printf("请输入当前时间（格式：YYYY-MM-DD-HH:MM）：");
			scanf("%s",cTime);
			FILE *fout;
			fout=fopen("yys/currentTime.txt","w");
			fprintf(fout,"%s",cTime);
			fclose(fout);
			printf("文件已生成，请重启程序！\n");
			exit(0);
		}
		fscanf(fin,"%s",cTime);
		return cTime;
	}
	void Dijkstra(Graph& G,int v0,int dest){	//2.dijkstra求票价最低转机
		int n=G.vexnum,v;	//n为顶点个数
		bool S[n];	//已确定的顶点集 
		double D[n]; 	//当前最短路径长度 
		int Path[n];	//vi直接前驱顶点序号 
		for(v=0;v<n;v++){
			S[v]=false;
			if(!G.available[v0][v])	D[v]=INF;
			else D[v]=G.arcs[v0][v];
			if(G.available[v0][v]&&D[v]<INF)	Path[v]=v0;	//如果v0和v之间有弧则前驱为v0，否则为-1
			else Path[v]=-1; 
		}
		S[v0]=true;
		D[v0]=0;
		v=-1;	//没在V-S中找到任何与该点相连的可用边就是-1 
		//初始化结束，接下来每次循环求v0到某顶点v的最短路径，把v加入S中
		for(int i=1;i<n;i++){
			int min=INF;
			for(int w=0;w<n;w++)
				if(!S[w]&&D[w]<min){
					v=w;
					min=D[w]; 
				}
			if(v==-1)	continue; //没找到就换下一个点 
			S[v]=true;
			for(int w=0;w<n;w++)	//更新从v0到集合V-S上所有顶点的最短路径长度 
				if(!S[w] && (D[v]+G.arcs[v][w]<D[w])){
					D[w]=D[v]+G.arcs[v][w];
					Path[w]=v;
				} 
		}
		//打印结果
		Stack St;
		initStack(St); 
		int t=dest;
		while(t!=v0){
			if(t==-1){	//二者之间没有可用转机路线 
				printf("错误：这两地之间目前没有可用的转机路线\n");
				return;
			}
			Spush(St,t);
			t=Path[t];
		} 
		printf("路径如下\n");
		double totDest=0;
		for(int i=1;!Sempty(St);i++){
			printf("第%d步：%d:%s->%d:%s\t价格：%.2lf\n",i,t,G.vexs[t],Stop(St),G.vexs[Stop(St)],G.arcs[t][Stop(St)]);
			totDest+=G.arcs[t][Stop(St)];
			Spop(St,t);
		}
		printf("总价格：%.2lf\n",totDest);
	}
	void resetVisited(Graph &G){	//把visited数组全部设为false，在遍历图之前必须调用 
		memset(G.visited,false,sizeof(G.visited));
	} 
	void dfs(Graph& G,int curr,int dest,std::vector<int> &path,std::vector<std::vector<int> > &allPath){	//进行深度优先搜索遍历，获得所有可能的转机路线
	//path记录前驱结点，pathlen记录path长度（最大210） 
		if(curr==dest){	//递归终止条件
		 	allPath.push_back(path);
		 	return;
		}
		for(int next=0;next<G.vexnum;next++){	//遍历所有节点
		 	if(isConnected(G,curr,next)&&G.available[curr][next]&&!G.visited[next]){
		 		G.visited[next]=true;
		 		path.push_back(next);
		 		dfs(G,next,dest,path,allPath);
		 		path.pop_back();	//回溯 
		 		G.visited[next]=false;
			}
		}
	} 
	void getAllRoutes(Graph& G,int start,int dest){	//获得图中从start到dest所有可能的转机路线 
		resetVisited(G);
		std::vector<int> path;
		std::vector<std::vector<int> > allPath;
		allPath.clear();
		G.visited[start]=true;
		dfs(G,start,dest,path,allPath);
		if(allPath.empty()){
			printf("无可用转机路线\n");
		}
		else{
			for(int i=0;i<allPath.size();i++){
				double totPrice=0;
				printf("路线%d：%d:%s",i+1,start,G.vexs[start]);
				int prev=start;	//记录上一步的点坐标，方便算价格 
				for(int j=0;j<allPath[i].size();j++){
					printf("->%d:%s %.2lf元",allPath[i][j],G.vexs[allPath[i][j]],G.arcs[prev][allPath[i][j]]);
					totPrice+=G.arcs[prev][allPath[i][j]];
					prev=allPath[i][j];
				}
				printf("\n路线总价格为：%.2lf元",totPrice);
				printf("\n");
			}
		}
	}
	void minTransRoute(Graph &G,int start,int dest){	//bfs得出转机次数最少的路线
	 	resetVisited(G); 
		std::vector<int> path;
		path.push_back(start);
		std::queue<std::vector<int> > q;	//储存当前路径的队列
		q.push(path);
		G.visited[start]=true;
		while(!q.empty()){	//只要q不空
		 	path=q.front();
		 	q.pop();
		 	int curr=path.back();	//当前结点
			if(curr==dest){
				double totPrice=0;int prev=path.front();
				printf("转机次数最少的路线为：%d：%s",path.front(),G.vexs[path.front()]);
				for(int i=1;i<path.size();i++){	//接下来的每一步 
					double price=G.arcs[prev][path[i]];
					printf(" ->%d：%s 价格：%.2lf元",path[i],G.vexs[i],price);
					totPrice+=price;
					prev=path[i];
				}
				printf("\n总价格：%.2lf元\n",totPrice);
				return;
			} 
			for(int next=0;next<G.vexnum;next++){
				if(isConnected(G,curr,next)&&G.available[curr][next]&&!G.visited[next]){
					G.visited[next]=true;
					std::vector<int> newPath=path;
					newPath.push_back(next);
					q.push(newPath);
				}
			}
		}
		//如果能执行到这里说明没有可用的转机路线
		printf("无可用转机路线\n");
	}
}
void graphGUI(YysPart::Graph& G){	//图界面设计
	int option;
	printf("\n欢迎进入航行图模块:)\n");
	printf("输入相应数字进入对应功能：\n");
	while(1){
	 	char a[MaxSNum],b[MaxSNum];
	 	printf("0：返回\n");
	 	printf("1：清屏\n");
	 	printf("2：显示该图的所有信息\n");
	 	printf("3：求票价最低转机路线\n");
	 	printf("4：求所有可能的转机路线\n");
	 	printf("5：求转机次数最少的路线\n");
	 	printf("6：将可用性全部设置为true(记得退出时使用功能7关掉)\n");
	 	printf("7：将可用性全部设置为false\n");
	 	scanf("%d",&option);
	 	switch(option){
	 		case 0:
	 			return;
	 		case 1:
	 			system("cls");
	 			break;
	 		case 2:
	 			showGraph(G);
	 			break;
	 		case 3:
	 			printf("请输入起点：\n");
	 			scanf("%s",a);
	 			if(getVIndex(G,a)==-1) {
	 				printf("地点不存在！\n");
	 				break;
				}
	 			printf("请输入终点：\n");
	 			scanf("%s",b);
	 			if(getVIndex(G,b)==-1) {
	 				printf("地点不存在！\n");
	 				break;
				}
	 			Dijkstra(G,getVIndex(G,a),getVIndex(G,b));
	 			break;
	 		case 4:
	 			printf("请输入起点：\n");
	 			scanf("%s",a);
	 			if(getVIndex(G,a)==-1) {
	 				printf("地点不存在！\n");
	 				break;
				}
	 			printf("请输入终点：\n");
	 			scanf("%s",b);
	 			if(getVIndex(G,b)==-1) {
	 				printf("地点不存在！\n");
	 				break;
				}
	 			getAllRoutes(G,getVIndex(G,a),getVIndex(G,b));
	 			break;
	 		case 5:
	 			printf("请输入起点：\n");
	 			scanf("%s",a);
	 			if(getVIndex(G,a)==-1) {
	 				printf("地点不存在！\n");
	 				break;
				}
	 			printf("请输入终点：\n");
	 			scanf("%s",b);
	 			if(getVIndex(G,b)==-1) {
	 				printf("地点不存在！\n");
	 				break;
				}
	 			minTransRoute(G,getVIndex(G,a),getVIndex(G,b));
	 			break;
	 		case 6:
	 			for(int i=0;i<G.vexnum;i++)
					for(int j=0;j<G.vexnum;j++)
						G.available[i][j]=true;
				printf("成功使所有边可用！\n");
				break;
			case 7:
	 			for(int i=0;i<G.vexnum;i++)
					for(int j=0;j<G.vexnum;j++)
						G.available[i][j]=false;
				printf("成功使所有边不可用！\n");
				break;
	 		default:
	 			printf("无效输入！\n");
		}
	}
} 
