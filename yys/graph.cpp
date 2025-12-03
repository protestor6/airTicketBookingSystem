//图相关，叶永盛制作2025/12/3 
#include <string.h>
#include <stdio.h>
#define MinVNum 15	//最少顶点数（城市数） 和边数 
#define MaxVNum 30	//最大顶点数（城市数） 和边数  
#define MaxSNum 20	//地名最大值 
typedef struct{
	char vexs[MaxVNum][MaxSNum];	//字符串，第一维表示顶点表，第二维表示字符串
	int TIndex;	//票务表中的票务信息索引，定义这个图之前要先定义票务表
	int vexnum,arcnum;	//图当前的点数和边数 
}Graph;
void fileInit(Graph &G){
	FILE *fin;
	fin=fopen("graphInfo.txt","r");
	if(fin==NULL){
		printf("未找到航行图文件，将生成\n");
		
	}
	
}
