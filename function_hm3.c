#include "Header_hm3.h"

void Create_Products(M_products* P, M_tables* T)
{
	FILE* resturant = fopen("resturant.txt", "a");//Open the resturant file for addition
	if (resturant == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}

	Item* temp;
	char tempN[MAX];
	int illegal;
	FILE* Manot = fopen("Manot.txt", "r"); //Open the "manot" file for reading
	if (Manot == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}
	while (fscanf(Manot, "%s", tempN) !=EOF)//while is not end of file
	{
	
		if (Check_Name(P, tempN) != 0) //The function add only new products
		{
			fprintf(resturant, "\nThe Product %s is already exist!", &tempN);
			fscanf(Manot, "%d %d", &illegal, &illegal); //To move the pointer to a file
		}
		else
		{
			temp = (Item*)malloc(sizeof(Item));//allocte memory for new product at kitchen
			if (temp == NULL)
			{
				Error_msg("The allocation memory failed!");

				DeleteList(&P);
				free(&T);
				exit(1);
			}

			temp->Name = (char*)malloc((strlen(tempN)+1) * sizeof(char));
			if (temp->Name == NULL)
			{
				Error_msg("The allocation memory failed!");
				DeleteList(&P);
				free(&T);
				exit(1);
			}
			strcpy(temp->Name, tempN);
			fscanf(Manot, "%d %d", &temp->Quantity, &temp->Price);
			temp->Orders_cnt = 0;
		}

		if (temp->Quantity <= 0 || temp->Price <= 0)
		{
			fprintf(resturant,"\nIllegal input!");
			fclose(resturant);
			DeleteList(&P);
			free(&T);
			return;
		}
		else 
		{
			if (P->head == NULL) //If its the first product
			{
				P->head = temp;
				P->tail = temp;
				temp->next = NULL;
			}
			else
			{
				P->tail->next = temp;
				P->tail = temp;
				temp->next = NULL;
			}
		}
	}
	
	if (P->head != NULL)
	{
		fprintf(resturant, "\nThe input succeed!");
	}
	else
		fprintf(resturant, "\nThe input failed!");

fclose(resturant);
fclose(Manot);
}//the
/*"create_Products" made the list of the products thet exist at the kitchen every product with its quantity and price
the function get data from file text "manot" and through which it creates one direction linked list*/
void Add_Item(M_products* P, int Quantity, char* Product_name, M_tables* T)
{
	FILE* resturant = fopen("resturant.txt", "a");//Open the resturant file for addition
	if (resturant == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}

	if (Check_Name(P, Product_name)==0) //check if we have the product in the kitchen
	{
		fprintf(resturant, "\nError input!We don't have %s in the kitchen!", &Product_name);
		fclose(resturant);
		return;
	}
	if (Quantity <= 0) //If quantity is not positiv
	{
		fprintf(resturant, "\nThe input %d Unvalid!", Quantity);
		fclose(resturant);
		return;
	}
	
	if (P->head != NULL)
	{
		Item* search;
		search = P->head;
		while (search != NULL)
		{
			if ((strcmp(search->Name, Product_name)) == 0)
			{
				search->Quantity += Quantity;
				fprintf(resturant, "\n%d %s were added to the kitchen!", Quantity, Product_name);
				fclose(resturant);
				return;
			}
			search = search->next;

		}
		
	}
	fclose(resturant);
}
/*Add_item add adds quantities of existing items in the kitchen */
void Order_Item(int table_num, char* product_name, int Quantity1, M_products* P, M_tables* T, int Tnum)
{
	int price;
	FILE* resturant = fopen("resturant.txt", "a");//Open the resturant file for addition
	if (resturant == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}
	if (table_num <= 0 || table_num >=Tnum) //cheak if the t number is valid
	{
		fprintf(resturant, "\nError input!We don't have table number %d in the resturant!", table_num);
		fclose(resturant);
		return;
	}
	if (Check_Name(P, product_name) == 0)//cheak if the product is valid
	{
		fprintf(resturant, "\nSorry we don't have %s in the resturant!",product_name);
		fclose(resturant);
		return;
	}
	if (Quantity1>0)
	{
			Item* search;
			search = P->head;
			while (search != NULL)
			{
				if ((strcmp(search->Name, product_name)) == 0)
				{
					if (search->Quantity < Quantity1) //if we have enough from this dish
					{
						fprintf(resturant, "\nSorry we don't have %s now.", product_name);
						fclose(resturant);
						return;
					}
					else
					{
						search->Quantity -= Quantity1; //update the quantity at kitchen
						search->Orders_cnt+=Quantity1; //We count the orders in order to know what is the most popular dish
					    price = search->Price; 
						break;
					}

				}
				search = search->next;

			}
		
	}
	else
	{
		fprintf(resturant, "\nThe Quantity value is invalid!");
		fclose(resturant);
		return;
	}

	//create new order for this table
	    int flag = 1;
	    Orders* List_of_orders;
		List_of_orders = (Orders*)malloc(sizeof(Orders));//allocate memory for new order
		if (List_of_orders == NULL)
		{
			Error_msg("The allocation memory failed!");
			for (int i = 0; i < Tnum; i++)
			{
				if (T[i].head != NULL)
					DeleteTable(&T[i]); //Frees the allocated memory of each table.
			}
			DeleteList(P);
			exit(1);
		}
		List_of_orders->Product_Name = (char*)malloc((strlen(product_name)+1) * sizeof(char));
		if (List_of_orders->Product_Name == NULL)
		{
			Error_msg("The memory allocation failed!");
			for (int i = 0; i <Tnum; i++)
			{
				if (T[i].head != NULL)
					DeleteTable(&T[i]); //Frees the allocated memory of each table.
			}
			Error_msg("The allocation memory failed!");
			DeleteList(P);
			exit(1);
		}
		strcpy(List_of_orders->Product_Name, product_name);
		List_of_orders->Quantity = Quantity1;
		List_of_orders->Price = price;
		List_of_orders->next = NULL;
		List_of_orders->prev = NULL;

		if ((T[table_num - 1].head )== NULL)//if the table is empty and its the first order
		{
			T[table_num - 1].head = List_of_orders;
		}

		else   //Add to active table
		{
			Orders* search , *search2;
			
			search = T[table_num - 1].head;
			search2= T[table_num - 1].head;

			while (search->next != NULL)
			{
				if ((strcmp(search->Product_Name, product_name)) == 0)//If the product is already exist in the table
				{
					search->Quantity += Quantity1;
					flag =0;
				}

				search = search->next;

			}
			if (flag) //if we want to add new product to open table 
			{
				search2 = search;
				List_of_orders->prev = search2;
				search->next = List_of_orders;
			}


		}
	T[table_num - 1].Sum_of_cheak += price * Quantity1;
	
	fprintf(resturant, "\n%d %s were added to table number %d!", Quantity1, product_name,table_num);

	fclose(resturant);

}
/*The function updates the table information according to the dish that it ordered.
"Order_item makes sure to update the total sum of the table and its items.*/
void Remove_Item(int table_num, char* product_name, int Quantity2, M_tables* T, int Tnum)
{
	int flag = 1;
	FILE* resturant = fopen("resturant.txt", "a");//Open the resturant file for addition
	if (resturant == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}
	if (table_num <= 0 || table_num >= Tnum) //cheak if the table number is valid
	{
		fprintf(resturant, "\nError input!We don't have table number %d in the resturant!", table_num);
		fclose(resturant);
		return;
	}
	if (Quantity2 < 0)//
	{
		fprintf(resturant, "\nThe input is unvalid!");//cheak if the quantity number is positiv
		fclose(resturant);
		return;
	}

	if (T[table_num - 1].head == NULL)
	{

		fprintf(resturant, "\nThe table is not ordered yet!");//check if the table is empty
		fclose(resturant);
		return;
	}
	Orders* search;
	search = T[table_num - 1].head;
	while (search != NULL)
		{
		if ((strcmp(search->Product_Name, product_name)) == 0)//I want to check if the table ordered this  dish
			{
					
			if (search->Quantity >= Quantity2)//If the quantity is less then the exist value
				{
							search->Quantity -= Quantity2;
							int price = search->Price;
							T[table_num - 1].Sum_of_cheak -= Quantity2 * price;
							if (search->Quantity == 0)//If now the quantity is 0
							{
								if ((search->prev == NULL) && (search->next == NULL)) //In case there is only 1 product in the table.
									T[table_num- 1].head = NULL;
								else if (search->next == NULL) //In case the product that returned is the last in the linked list.
									search->prev->next = NULL;
								else if (search->prev == NULL) { //In case the product that returned is the first in the linked list.
									T[table_num - 1 - 1].head = search->next;
									search->next->prev = NULL;
								}
								else {
									search->prev->next = search->next;
									search->next->prev = search->prev;
								}
								free(search->Product_Name);
								free(search);
								return;
							}
						}
						else
						{
							fprintf(resturant, "\nError input!The table ordered less then %d %s!", &Quantity2, &product_name);
							fclose(resturant);
							return;
						}
					
				}
				search = search->next;
				flag = 0;
			}

	if (flag)
	{
		fprintf(resturant, "\nError input!The table didnt order %s!", &product_name);
		fclose(resturant);
		return;

	}
}
/*When a table returns a dish, the function takes care to lower the amount of the products at the table according
to the price and quantities of products on the table*/
void Remove_table(int table_num, M_tables* T, int Tnum, M_products* P)
{
	FILE* resturant = fopen("resturant.txt", "a");//Open the resturant file for addition
	if (resturant == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}
	if (table_num <= 0 || table_num >= Tnum) //cheak if the table number is valid
	{
		fprintf(resturant, "\nError input!We don't have table number %d in the resturant!", table_num);
		fclose(resturant);
		return;
	}
	if (T[table_num - 1].Sum_of_cheak !=0)
	{
		fprintf(resturant, "\nThe orders of table number %d are:", table_num);
		Orders* check1= T[table_num - 1].head;
		while (check1 != NULL)
		{
			fprintf(resturant, "\n %d %s ,", check1->Quantity , check1->Product_Name);
			check1 = check1->next;
			
		}

		fprintf(resturant, "%d nis,Please!", T[table_num - 1].Sum_of_cheak);
		T[table_num - 1].Sum_of_cheak= 0;
		DeleteTable(&T[table_num - 1]); //Frees the allocated memory of each product in the table.
	}
	else // If the total sum is 0.
	{
		fprintf(resturant, "\nError input!The table number %d is not ordered yet!",table_num);
		fclose(resturant);
		return;
	}
	M_tables* close = T;
	int i,flag = 1;
	for ( i = 0; i < Tnum; i++)  //check if is the last table at resturant
	{
		if (close[i].Sum_of_cheak != 0)
		{
			flag = 0;
		}
	}

	if (flag)
	{
		Item* FMax, * Smax;
		FMax = P->head;
		int max = 0;
		char max_product[MAX];
		while (FMax != NULL)
		{
			if (FMax->Orders_cnt > max)
			{
				max = FMax->Orders_cnt;
				Smax = FMax;
			}
			FMax = FMax->next;
		}
		
		fprintf(resturant, "\nThe most popular dish today is %s!was ordered %d times!", Smax->Name, max);
		DeleteList(P);//If is there are no open tables at the resturant.
		fclose(resturant);
	}
	else
		fclose(resturant);
		return;

}
/*When a table requests an account, the function displays the products it ordered and the total account amount.
The function is responsible for releasing the product allocation for each table and the table itself in the array.*/


 int Check_Name(M_products *P, char* name)
{
		Item* search = P->head;
		while (search != NULL)
		{
			if (strcmp(search->Name, name)== 0)
				return(1);
			
			search = search->next;
			
		}
		
	return 0;
}
 //The function return 1 or 0 . 1 if the name exist in the list and zero if not.
 void Error_msg(char* str)
 {
	 printf("\n%s", str);

 }
 void DeleteTable(M_tables* T)
 {
	 while (T->head != NULL)
	 {
		Orders* temp = T->head;
		 T->head = T->head->next;
		 free(temp->Product_Name);
		 free(temp);
	 }
 }
 // Function that free the allocated memory of a specific table.

 void DeleteList(M_products* P)
 {
	 Item* temp;
	 while (P->head)
	 {
		 temp = P->head;
		 P->head = P->head->next;
		 if (temp->Name)
			 free(temp->Name);
		 free(temp);
	 }
 }
 //Function that free the allocated memory of all the products in the kitchen.

