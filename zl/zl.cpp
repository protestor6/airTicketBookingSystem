#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../yys/yys.h"
#include "zl.h"
#include "../zyz/zyz.h"
#include <vector>

// 全局线性表（存储三类核心数据）
Flight flight_list[MAX_FLIGHT];
Ticket ticket_list[MAX_TICKET];
Order order_list[MAX_ORDER];

// 全局计数器（记录当前数据量）
int flight_count = 0;
int ticket_count = 0;
int order_count = 0;

YysPart::Graph G;
std::vector<Flight> resultList;

// ====================== 排序与折半查找工具函数 ======================
// 按航班号升序排序航班表（字符串比较）
void sort_flight_list() {
    if (flight_count <= 1) return;
    for (int i = 0; i < flight_count - 1; i++) {
        for (int j = 0; j < flight_count - 1 - i; j++) {
            if (strcmp(flight_list[j].flight_num, flight_list[j+1].flight_num) > 0) {
                // 交换两个航班数据
                Flight temp = flight_list[j];
                flight_list[j] = flight_list[j+1];
                flight_list[j+1] = temp;
            }
        }
    }
}

// 按航班号升序排序票务表
void sort_ticket_list() {
    if (ticket_count <= 1) return;
    for (int i = 0; i < ticket_count - 1; i++) {
        for (int j = 0; j < ticket_count - 1 - i; j++) {
            if (strcmp(ticket_list[j].flight_num, ticket_list[j+1].flight_num) > 0) {
                // 交换两个票务数据
                Ticket temp = ticket_list[j];
                ticket_list[j] = ticket_list[j+1];
                ticket_list[j+1] = temp;
            }
        }
    }
}

// 按订单号升序排序订单表
void sort_order_list() {
    if (order_count <= 1) return;
    for (int i = 0; i < order_count - 1; i++) {
        for (int j = 0; j < order_count - 1 - i; j++) {
            if (strcmp(order_list[j].order_num, order_list[j+1].order_num) > 0) {
                // 交换两个订单数据
                Order temp = order_list[j];
                order_list[j] = order_list[j+1];
                order_list[j+1] = temp;
            }
        }
    }
}

// 折半查找航班（返回索引，-1表示未找到）
int binary_search_flight(const char* flight_num) {
    int left = 0, right = flight_count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2; // 避免溢出
        int cmp = strcmp(flight_list[mid].flight_num, flight_num);
        if (cmp == 0) {
            return mid; // 找到，返回索引
        } else if (cmp < 0) {
            left = mid + 1; // 目标在右半区
        } else {
            right = mid - 1; // 目标在左半区
        }
    }
    return -1; // 未找到
}

// 折半查找票务（返回索引，-1表示未找到）
int binary_search_ticket(const char* flight_num) {
    int left = 0, right = ticket_count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(ticket_list[mid].flight_num, flight_num);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// 折半查找订单（返回索引，-1表示未找到）
int binary_search_order(const char* order_num, const char* username) {
    int left = 0, right = order_count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp_order = strcmp(order_list[mid].order_num, order_num);
        if (cmp_order == 0) {
            // 订单号匹配，再校验用户名和状态
            if (strcmp(order_list[mid].username, username) == 0 && 
                order_list[mid].status == NOT_DEPARTED) {
                return mid;
            } else {
                // 订单号匹配但用户名/状态不符，继续查找（可能有重复订单号？）
                // 向左/右扩展查找
                int temp = mid - 1;
                while (temp >= left && strcmp(order_list[temp].order_num, order_num) == 0) {
                    if (strcmp(order_list[temp].username, username) == 0 && 
                        order_list[temp].status == NOT_DEPARTED) {
                        return temp;
                    }
                    temp--;
                }
                temp = mid + 1;
                while (temp <= right && strcmp(order_list[temp].order_num, order_num) == 0) {
                    if (strcmp(order_list[temp].username, username) == 0 && 
                        order_list[temp].status == NOT_DEPARTED) {
                        return temp;
                    }
                    temp++;
                }
                return -1;
            }
        } else if (cmp_order < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// ====================== 模块1：航班表、票务表、订单表基础操作 ======================
// 1.1 添加航班到航班表
void add_flight() {
    if (flight_count >= MAX_FLIGHT) {
        printf("航班表已满，无法添加！\n");
        return;
    }
    Flight f;
    printf("请输入航班号：");
    scanf("%s", f.flight_num);
    printf("请输入出发城市：");
    scanf("%s", f.start_city);
    if(YysPart::getVIndex(G,f.start_city)==-1){
        printf("不合法：地点无效，添加失败\n");
        return;
    }
    printf("请输入到达城市：");
    scanf("%s", f.end_city);
    if(YysPart::getVIndex(G,f.end_city)==-1){
        printf("不合法：地点无效，添加失败\n");
        return;
    }
    if(YysPart::isConnected(G,f.start_city,f.end_city)==false){
        printf("不合法：两地并不直接连通，添加失败\n");
        return;
    }
    printf("请输入起飞时间（格式：YYYY-MM-DD-HH:MM）：");
    scanf("%s", f.takeoff_time);
    printf("请输入到达时间（格式：YYYY-MM-DD-HH:MM）：");
    scanf("%s", f.arrive_time);
    printf("请输入基础票价：");
    scanf("%f", &f.price);
    if(f.price<0){
        printf("不合法：票价不能低于0，添加失败\n");
        return;
    }
    flight_list[flight_count++] = f;
    // 添加后排序，保证折半查找的有序性
    sort_flight_list();
    printf("航班添加成功！\n");
    //设置图G中的权值为价格
    // 折半查找获取航班索引
    int flight_idx = binary_search_flight(f.flight_num);
    if (flight_idx != -1 && f.price < G.arcs[YysPart::getVIndex(G,f.start_city)][YysPart::getVIndex(G,f.end_city)]) {
        YysPart::setArcs(G,f.start_city,f.end_city,f.price); 
    }
}

// 1.2 添加票务到票务表（需关联已存在的航班）
void add_ticket() {
    if (ticket_count >= MAX_TICKET) {
        printf("票务表已满，无法添加！\n");
        return;
    }
    Ticket t;
    printf("请输入关联航班号：");
    scanf("%s", t.flight_num);
    //折半查找检查航班是否存在
    int flight_idx = binary_search_flight(t.flight_num);
    if (flight_idx == -1) {
        printf("该航班不存在，无法添加票务！\n");
        return;
    }
    printf("请输入总座位数：");
    scanf("%d", &t.total_seats);
    t.remain_seats = t.total_seats; // 初始余票=总座位
    ticket_list[ticket_count++] = t;
    // 新增：添加后排序
    sort_ticket_list();
    printf("票务添加成功！\n");
    //确定票务有效性，提供给图G
    if(strcmp(flight_list[flight_idx].arrive_time,YysPart::currentTime())>0&&t.remain_seats>0)
        YysPart::setAvailability(G,flight_list[flight_idx].start_city,flight_list[flight_idx].end_city,true);
}

// 1.3 生成订单号（规则：USER_用户名_航班号_随机数）
void generate_order_num(char *order_num, const char *username, const char *flight_num) {
    sprintf(order_num, "USER_%s_%s_%d", username, flight_num, rand() % 10000);
}

// ====================== 模块2：航班和余票查询功能 ======================
// 2.1 根据航班号查询航班+余票
void query_by_flight_num() {
    char target[20];
    printf("请输入要查询的航班号：");
    scanf("%s", target);
    // 折半查找航班
    int flight_idx = binary_search_flight(target);
    if (flight_idx == -1) {
        printf("未找到该航班！\n");
        return;
    }
    // 折半查找对应余票
    int ticket_idx = binary_search_ticket(target);
    // 输出结果
    printf("\n================= 航班信息 =================\n");
    printf("航班号：%s\n", flight_list[flight_idx].flight_num);
    printf("出发地：%s\t\t到达地：%s\n", flight_list[flight_idx].start_city, flight_list[flight_idx].end_city);
    printf("起飞时间：%s\n", flight_list[flight_idx].takeoff_time);
    printf("到达时间：%s\n", flight_list[flight_idx].arrive_time);
    printf("基础票价：%.2f 元\n", flight_list[flight_idx].price);
    printf("--------------------------------------------\n");
    if (ticket_idx != -1) {
        printf("总座位数：%d\t\t剩余座位数：%d\n", ticket_list[ticket_idx].total_seats, ticket_list[ticket_idx].remain_seats);
    } else {
        printf("暂无该航班的票务信息！\n");
    }
    printf("============================================\n");
}

// 2.2 根据起点+终点查询所有匹配航班+余票
void query_by_city() {
    char start[MAX_CITY], end[MAX_CITY];
    printf("请输入出发城市：");
    scanf("%s", start);
    printf("请输入到达城市：");
    scanf("%s", end);
    printf("\n================= 查询结果 =================\n");
    int found = 0;
    // 注：按起止城市查询无法用折半查找
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flight_list[i].start_city, start) == 0 && strcmp(flight_list[i].end_city, end) == 0) {
            if(YysPart::getAvailability(G,flight_list[i].start_city,flight_list[i].end_city)==false)
                continue; // 用continue而非break，避免漏掉其他航班
            found = 1;
            printf("航班号：%s\t票价：%.2f 元\n", flight_list[i].flight_num, flight_list[i].price);
            // 折半查找对应余票
            int ticket_idx = binary_search_ticket(flight_list[i].flight_num);
            if (ticket_idx != -1) {
                printf("剩余座位：%d 个\n", ticket_list[ticket_idx].remain_seats);
            } else {
                printf("剩余座位：无\n");
            }
            printf("--------------------------------------------\n");
        }
    }
    if (!found) {
        printf("未找到直接飞该航线的航班！需要查看转机路线吗？\n");
        printf("  可以记下这个路线，然后重新使用这个功能查询航班号\n");
        int option;
        while(1){
            printf("0：不需要\n");
            printf("1：所有可能的转机路线\n");
            printf("2：票价最低的转机路线\n");
            printf("3：转机次数最少的路线\n");
            scanf("%d",&option);
            switch(option){
                case 0:
                    printf("============================================\n");
                    return;
                case 1:
                    YysPart::getAllRoutes(G,YysPart::getVIndex(G,start),YysPart::getVIndex(G,end));
                    printf("============================================\n");
                    return;
                case 2:
                    YysPart::Dijkstra(G,YysPart::getVIndex(G,start),YysPart::getVIndex(G,end));
                    printf("============================================\n");
                    return;
                case 3:
                    YysPart::minTransRoute(G,YysPart::getVIndex(G,start),YysPart::getVIndex(G,end));
                    printf("============================================\n");
                    return;
                default:
                    printf("非法输入\n");
            }
        }
    }
}

//与赵英姿的按价格升序排序结合 
void query_by_city_priceSort() {
    char start[MAX_CITY], end[MAX_CITY];
    printf("请输入出发城市：");
    scanf("%s", start);
    printf("请输入到达城市：");
    scanf("%s", end);
    printf("\n================= 查询结果 =================\n");
    int found = 0;
    resultList.clear();
    // 注：按起止城市查询无法用折半查找
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flight_list[i].start_city, start) == 0 && strcmp(flight_list[i].end_city, end) == 0) {
        	if(YysPart::getAvailability(G,flight_list[i].start_city,flight_list[i].end_city)==false)
                continue; // 用continue而非break，避免漏掉其他航班
            found = 1;
            resultList.push_back(flight_list[i]);			
        }
    }
    ZyzPart::priceSort(resultList);
    for (int i = 0; i < resultList.size(); i++) {
        printf("航班号：%s\t票价：%.2f 元\n", resultList.at(i).flight_num, resultList.at(i).price);
        // 替换：折半查找对应余票
        int ticket_idx = binary_search_ticket(resultList.at(i).flight_num);
        if (ticket_idx != -1) {
            printf("剩余座位：%d 个\n", ticket_list[ticket_idx].remain_seats);
        } else {
            printf("剩余座位：无\n");
        }
        printf("--------------------------------------------\n");
    }
    if (!found) {
        printf("未找到直接飞该航线的航班！需要查看转机路线吗？\n");
        printf("  可以记下这个路线，然后重新使用这个功能查询航班号\n");
        int option;
        while(1){
            printf("0：不需要\n");
            printf("1：所有可能的转机路线\n");
            printf("2：票价最低的转机路线\n");
            printf("3：转机次数最少的路线\n");
            scanf("%d",&option);
            switch(option){
                case 0:
                    printf("============================================\n");
                    return;
                case 1:
                    YysPart::getAllRoutes(G,YysPart::getVIndex(G,start),YysPart::getVIndex(G,end));
                    printf("============================================\n");
                    return;
                case 2:
                    YysPart::Dijkstra(G,YysPart::getVIndex(G,start),YysPart::getVIndex(G,end));
                    printf("============================================\n");
                    return;
                case 3:
                    YysPart::minTransRoute(G,YysPart::getVIndex(G,start),YysPart::getVIndex(G,end));
                    printf("============================================\n");
                    return;
                default:
                    printf("非法输入\n");
            }
        }
    }
}

// ====================== 模块3：订票和退票功能 ======================
// 3.1 订票功能（用户购票，生成订单）
void book_ticket() {
    char username[20], flight_num[20];
    int seat_num;
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入要预订的航班号：");
    scanf("%s", flight_num);
    // 1. 折半查找航班 + 获取票价
    int flight_idx = binary_search_flight(flight_num);
    if (flight_idx == -1) {
        printf("航班不存在！\n");
        return;
    }
    float price = flight_list[flight_idx].price;

    // 2. 折半查找检查余票
    int ticket_idx = binary_search_ticket(flight_num);
    if (ticket_idx == -1) {
        printf("该航班无票务信息！\n");
        return;
    }
    printf("请输入购票数量：");
    scanf("%d", &seat_num);
    if (seat_num <= 0 || seat_num > ticket_list[ticket_idx].remain_seats) {
        printf("购票数量无效或余票不足！当前余票：%d\n", ticket_list[ticket_idx].remain_seats);
        return;
    }
    // 3. 生成订单
    if (order_count >= MAX_ORDER) {
        printf("订单表已满，无法订票！\n");
        return;
    }
    Order o;
    strcpy(o.username, username);
    strcpy(o.flight_num, flight_num);
    o.seat_count = seat_num;
    o.total_price = price * seat_num;
    o.status = NOT_DEPARTED;
    generate_order_num(o.order_num, username, flight_num);
    order_list[order_count++] = o;
    // 添加订单后排序
    sort_order_list();
    // 4. 更新余票
    ticket_list[ticket_idx].remain_seats -= seat_num;
    if(ticket_list[ticket_idx].remain_seats==0){ //如果没有空座位则有效性变为false 
        YysPart::setAvailability(G,flight_list[flight_idx].start_city,flight_list[flight_idx].end_city,false);
    }
    printf("\n================= 订票成功 =================\n");
    printf("订单号：%s\n", o.order_num);
    printf("用户名：%s\n", o.username);
    printf("航班号：%s\n", o.flight_num);
    printf("购票数量：%d\n", o.seat_count);
    printf("订单总价：%.2f 元\n", o.total_price);
    printf("============================================\n");
}

// 3.2 退票功能（用户退票，修改订单状态+恢复余票）
void refund_ticket() {
    char username[20], order_num[30];
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入要退票的订单号：");
    scanf("%s", order_num);
    // 1. 折半查找订单
    int order_idx = binary_search_order(order_num, username);
    if (order_idx == -1) {
        printf("订单不存在或已退票！\n");
        return;
    }
    // 2. 恢复余票
    char *flight_num = order_list[order_idx].flight_num;
    int seat_num = order_list[order_idx].seat_count;
    // 折半查找票务
    int ticket_idx = binary_search_ticket(flight_num);
    if (ticket_idx != -1) {
        ticket_list[ticket_idx].remain_seats += seat_num;
    }
    // 3. 标记订单为取消 
    order_list[order_idx].status = CANCELLED;
    printf("\n退票成功！已为航班 %s 恢复 %d 个座位\n", flight_num, seat_num);
}

// ====================== 系统菜单 ======================
void menu() {
    printf("\n\n========== 功能 ==========\n");
    printf("1. 添加航班\n");
    printf("2. 添加票务\n");
    printf("3. 按航班号查询航班+余票\n");
    printf("4. 按起止城市查询航班+余票\n");
    printf("5. 订票\n");
    printf("6. 退票\n");
    printf("7. 清屏\n");
    printf("0. 返回\n");
    printf("==================================\n");
    printf("请输入操作序号：");
}

// ====================== 界面设计 ======================
void ticketBookingGUI() {
    int choice;
    srand((unsigned int)time(NULL)); // 初始化随机数种子（用于生成订单号）
    printf("========== 欢迎使用订票与退票模块 ==========\n");
    while (1) {
        ZyzPart::flushStatus();
        menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: add_flight(); break;
            case 2: add_ticket(); break;
            case 3: query_by_flight_num(); break;
            case 4: query_by_city(); break;
            case 5: book_ticket(); break;
            case 6: refund_ticket(); break;
            case 7: system("cls"); break;
            case 0: printf("退出模块！感谢使用！\n"); return;
            default: printf("输入无效，请重新选择！\n");
        }
    }
}
