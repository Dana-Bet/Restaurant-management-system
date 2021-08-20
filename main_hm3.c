#include "Header_hm3.h"

void main ()
{
	int i;
	M_tables* arr;
	int Tnum, Num_function,Quan ,table_num;
	char* item[MAX];
	FILE* resturant = fopen("resturant.txt", "w");
	if (resturant == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}
	fprintf(resturant,"\nHere are the program actions:");

	FILE* instruction = fopen("Instructions.txt", "r");
	if (instruction == NULL)
	{
		Error_msg("Unable to open the file!");
		exit(1);
	}

	//create the array of tables
	fscanf(instruction, "%d", &Tnum);
	if (Tnum <= 0)
	{
		fprintf(resturant, "\nThe number of the tables is invalid!");
		exit(1);
	}
	arr = (M_tables*)malloc(Tnum * sizeof(M_tables));
	if (arr == NULL)
	{
		Error_msg("The allocation memory failed!");
		exit(1);
	}
	for (i = 0; i < Tnum; i++)
	{
		arr[i].head = NULL;
		arr[i].Sum_of_cheak = 0;
	}
	//The array of tabels is ready
	

	M_products	P; // manage the products list in the kitchen
	P.head = NULL;
	P.tail = NULL;

	while (feof(instruction)==0)
	{

	
		fscanf(instruction, "%d", &Num_function);
		if (Num_function <= 0 || Num_function > 5)
		{
			fprintf(resturant, "\nThe number of the function is unvalid!");
			fclose(resturant);
			exit(1);
		}
		fclose(resturant);
		switch (Num_function)
		{
		case 1:
			Create_Products(&P,arr);
			break;
		case 2:
			fscanf(instruction, "%s %d", &item, &Quan);
			Add_Item(&P, Quan, item,&arr);
			break;
		case 3:
			fscanf(instruction, "%d %s %d", &table_num, &item, &Quan);
			Order_Item(table_num, item, Quan, &P, arr, Tnum);
			break;
		case 4:
			fscanf(instruction, "%d %s %d", &table_num, &item, &Quan);
			Remove_Item(table_num, item, Quan, arr, Tnum);
			break;
		case 5:
			fscanf(instruction, "%d", &table_num);
			Remove_table(table_num, arr, Tnum,&P);
			break;
		}


	}














	fclose(instruction);

}