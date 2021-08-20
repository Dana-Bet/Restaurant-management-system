#ifndef _Header_hm3
#define _Header_hm3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

typedef struct Item //list of the products in the kitchen
{
	char *Name;
	int Quantity;
	int Price;
	int Orders_cnt;
	struct Item* next;
}Item;

typedef struct Orders//list of orders for every table
{
	char* Product_Name;
	int	 Price;
	int	 Quantity;
	struct Orders* next;
	struct Orders* prev;
}Orders;

typedef struct tables//struct manage for tables
{

    int Sum_of_cheak;
	struct Orders* head;
	
}M_tables ;

typedef struct List_products//struct manage for products
{
	Item* head;
	Item* tail;
}M_products;
 
void Create_Products(M_products* P, M_tables* T);//t
void Add_Item( M_products* P,int Quantity ,char* Product_name, M_tables* T);
void Order_Item(int table_num, char* product_name, int Quantity, M_products* P, M_tables* T,int Tnum);
void Remove_Item(int table_num, char* product_name, int Quantity, M_tables* T,int Tnum);
void Remove_table(int table_num, M_tables* T, int Tnum, M_products* P);

int Check_Name(M_products* P, char* name);
void Error_msg(char* str);
void DeleteTable(M_tables* T);
void DeleteList(M_products* P);

#endif









