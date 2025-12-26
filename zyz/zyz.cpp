#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../yys/yys.h"
#include "../zl/zl.h"
#include "zyz.h"
namespace ZyzPart{
	//相同起终点的票价排序（快速排序）（不对转机生效，启动函数是zl.h中的query_by_city_priceSort();） 
	int partition(std::vector<Flight> &List,int low,int high){
		Flight t=List.at(low);
		float pivotkey=List.at(low).price;
		while(low<high){
			while(low<high&&List.at(high).price>=pivotkey) --high;
			List.at(low)=List.at(high);
			while(low<high&&List.at(low).price<=pivotkey) ++low;
			List.at(high)=List.at(low);
		}
		List.at(low)=t;
		return low;
	}
	void QSort(std::vector<Flight> &List,int low,int high){
		if(low<high){
			int pivotloc=partition(List,low,high);
			QSort(List,low,pivotloc-1);
			QSort(List,pivotloc+1,high);
		}
	}
	void priceSort(std::vector<Flight> &List){	//从朱琳的query_by_city_priceSort()处调用 
		int low=0,high=resultList.size()-1;
		QSort(List,low,high);
	}
	void flushStatus(){	//订单表状态修改，每次进入菜单都会刷新一次，根据当前时间和订单时间确定状态 
		for(int j=0;j<order_count;j++){
			if(order_list[j].status!=END&&order_list[j].status!=CANCELLED){
				char* flight_num=order_list[j].flight_num;
				int i;
			    for (i = 0; i < flight_count; i++) {
			        if (strcmp(flight_list[i].flight_num,flight_num) == 0) {
			            break;
			        }
			    }
				if(strcmp(YysPart::currentTime(),flight_list[i].arrive_time)>0)
					order_list[j].status=END;
				else if(strcmp(YysPart::currentTime(),flight_list[i].takeoff_time)>0) 
					order_list[j].status=ONGOING;
			}
		}
	}
	//查看我的订单
	void checkMyOrders(){
		char name[20];
		printf("输入用户名：\n");
		scanf("%s",name);
		int tot=0;
		const char *STAT[]={"已取消","已结束","待出发","进行中"}; 
		for(int i=0;i<order_count;i++){
			if(strcmp(order_list[i].username,name)==0){
				printf("订单号：%s 用户名：%s 关联航班号：%s 购票数量：%d 订单总价：%.2lf 订单状态：%s\n"
				,order_list[i].order_num,order_list[i].username,order_list[i].flight_num,order_list[i].seat_count,order_list[i].total_price
				,STAT[order_list[i].status]);
				++tot;
			}
		}
		if(tot>0) printf("一共%d条关于%s的订单信息！\n",tot,name);
		else printf("无关于%s的订单信息！\n",name);
	}
	void GUIinit(){	//主页面
		YysPart::fileInit(G);
		int option;
		printf("欢迎使用飞机订票系统！\n");
		while(1){
			printf("输入数字进入相应模块：\n");
			printf("1.航行图与路线规划模块\n");
			printf("2.订票与退票模块\n");
			printf("3.订单查找与票价排序模块\n");
			printf("4.清屏\n");
			printf("0.退出\n");
			scanf("%d",&option);
			switch(option){
				case 0:
					return;
				case 1:
					graphGUI(G);	//graph.cpp
					break;
				case 2:
					ticketBookingGUI();	//zl.cpp
					break;
				case 3:
					checkAndSortGUI();
					break;
				case 4:
					system("cls");
					break;
				default:
					printf("非法输入！\n");
			}
		}
	}
	void checkAndSortGUI(){	//订单查找与票价排序模块
		int option;
		printf("欢迎使用订单查找与票价排序模块！\n");
		while(1){
			printf("输入数字进入相应功能：\n");
			printf("1. 按起止城市查询航班+余票，并按票价进行升序排序\n");
			printf("2.查看我的订单\n");
			printf("3.清屏\n");
			printf("0.退出\n");
			scanf("%d",&option);
			switch(option){
				case 0:
					return;
				case 1:
					query_by_city_priceSort();	//zl.cpp
					break;
				case 2:
					checkMyOrders();
					break;
				case 3:
					system("cls");
					break;
				default:
					printf("非法输入！\n");
			}
		}
	} 
}
