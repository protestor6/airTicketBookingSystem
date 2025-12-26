#pragma once
#include "../yys/yys.h"
// 最大容量宏定义
#define MAX_FLIGHT 100   // 最大航班数
#define MAX_TICKET 500   // 最大票务数
#define MAX_ORDER 300    // 最大订单数
#define MAX_CITY 20      // 城市名称最大长度

// 用户类型枚举
typedef enum {
    ADMIN,
    USER
} UserType;

typedef enum{	//订单状态 
	CANCELLED,	//已取消
	END,	//已结束 
	NOT_DEPARTED,	//买票了还没出发
	ONGOING	//在途中 
} OrderStatus;

// 1. 航班结构体（航班表）
typedef struct {
    char flight_num[20];  // 航班号
    char start_city[MAX_CITY]; // 出发城市
    char end_city[MAX_CITY];   // 到达城市
    char takeoff_time[20]; // 起飞时间
    char arrive_time[20];  // 到达时间
    float price;           // 基础票价
} Flight;

// 2. 票务结构体（票务表：关联航班+余票）
typedef struct {
    char flight_num[20];  // 关联航班号
    int total_seats;      // 总座位数
    int remain_seats;     // 剩余座位数
} Ticket;

// 3. 订单结构体（订单表：关联用户+票务）
typedef struct {
    char order_num[30];   // 订单号
    char username[20];    // 用户名
    char flight_num[20];  // 关联航班号
    int seat_count;       // 购票数量
    float total_price;    // 订单总价
    OrderStatus status;         // 订单状态
} Order;

extern Flight flight_list[MAX_FLIGHT];
extern Ticket ticket_list[MAX_TICKET];
extern Order order_list[MAX_ORDER];

// 全局计数器（记录当前数据量）
extern int flight_count;
extern int ticket_count;
extern int order_count;

extern YysPart::Graph G;
extern std::vector<Flight> resultList;
// ====================== 排序与折半查找工具函数 ======================
// 按航班号升序排序航班表（字符串比较）
void sort_flight_list() ;

// 按航班号升序排序票务表
void sort_ticket_list() ;

// 按订单号升序排序订单表
void sort_order_list() ;

// 折半查找航班（返回索引，-1表示未找到）
int binary_search_flight(const char* flight_num) ;

// 折半查找票务（返回索引，-1表示未找到）
int binary_search_ticket(const char* flight_num) ;

// 折半查找订单（返回索引，-1表示未找到）
int binary_search_order(const char* order_num, const char* username);
// ====================== 模块1：航班表、票务表、订单表基础操作 ======================
// 1.1 添加航班到航班表
void add_flight() ;

// 1.2 添加票务到票务表（需关联已存在的航班）
void add_ticket() ;

// 1.3 生成订单号（规则：USER_用户名_航班号_随机数）
void generate_order_num(char *order_num, const char *username, const char *flight_num) ;

// ====================== 模块2：航班和余票查询功能 ======================
// 2.1 根据航班号查询航班+余票
void query_by_flight_num() ;

// 2.2 根据起点+终点查询所有匹配航班+余票
void query_by_city() ;
void query_by_city_priceSort();
// ====================== 模块3：订票和退票功能 ======================
// 3.1 订票功能（用户购票，生成订单）
void book_ticket() ;

// 3.2 退票功能（用户退票，修改订单状态+恢复余票）
void refund_ticket() ;

// ====================== 系统菜单 ======================
void menu() ;
void ticketBookingGUI();	//界面设计 
