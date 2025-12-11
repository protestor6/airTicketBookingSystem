//图相关，yys制作2025/12/3 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MinVNum 10	//最少顶点数（城市数）
#define MinANum 15	//最小边数 
#define MaxVNum 15	//最大顶点数（城市数）
#define MaxANum 210	//最大边数（实际上最多用到20条）  
#define MaxSNum 20	//地名最大长度 
#define MAX -1
typedef struct{
	char vexs[MaxVNum][MaxSNum];	//字符串，第一维表示顶点表，第二维表示字符串
	double arcs[MaxVNum][MaxVNum];	//邻接矩阵，这里矩阵里存储的是票务表中的索引，价格等信息需要从表里面找，-1表示MAX 
	int vexnum,arcnum;	//图当前的点数和边数 
	bool visited[MaxVNum],available[MaxVNum][MaxVNum];	//是否已经遍历过，用于dfs和bfs；是否可用（如果不可用则为false） 
}Graph;
void fileInit(Graph &G){	//读写文件，获取图的信息，每两点之间的权值是票务表中的id 
	FILE *fin;
	fin=fopen("info.txt","r");
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
		fout=fopen("info.txt","w");
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
	printf("邻接矩阵（MAX表示两点不通，其他数字代表有航班连通两地）：\n"); 
	for(int i=0;i<G.vexnum;i++){
		for(int j=0;j<G.vexnum;j++)
			if(G.arcs[i][j]==MAX)
				printf("%7s ","MAX");
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
	if(G.arcs[an][bn]!=MAX)	return true;
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
int main(){	//仅测试用 
	Graph G;
	fileInit(G);
	showGraph(G);
//	printf("%d",getVIndex(G,"g")); 
//	setAvailability(G,"a","b",true);
//	setArcs(G,"a","b",666);
//	showGraph(G);
	return 0;
}
