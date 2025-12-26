#pragma once
#include "../zl/zl.h"
#include "../yys/yys.h"
namespace ZyzPart{
	//相同起终点的票价排序（快速排序） 
	int partition(std::vector<Flight> &List,int low,int high);
	void QSort(std::vector<Flight> &List,int low,int high);
	void priceSort(std::vector<Flight> &List);	//从朱琳的query_by_city_priceSort()处调用 
	//根据当前时间刷新订单表状态 
	void flushStatus();
	//查看我的订单
	void checkMyOrders();
	void GUIinit();	//主界面
	void checkAndSortGUI();	//订单查找与票价排序模块（其他模块在另外两个文件里） 
}
