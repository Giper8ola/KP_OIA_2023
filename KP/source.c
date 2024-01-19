#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Date
{
	int day;
	int month;
	int year;
} date_t;

typedef struct Inventory
{
	char name[100];
	char category[100];
	int  number;
	char description[100];
	date_t Date;
	char state[100];
} inventory_t;

typedef struct DataBase
{
	inventory_t* array;
	int size;
} database_t;

int save(inventory_t, database_t*);
int loadMany(database_t*);
int findOne(char category[100], database_t*);
int sort(database_t*);
int updateOne(int number, inventory_t newParams, database_t*);
int delete(int number, database_t*);

int postDataBaseTables(char* fileName, database_t* base);
int getDataBaseTables(char* fileName, database_t* base);

void showDataLine(database_t*, int);
int compare(const void* x1, const void* x2);
int validateDate(int*, char);

int main() {
	system("chcp 1251");
	setlocale(LC_ALL, "RUS");

	database_t baseOfData;
	baseOfData.size = 0;
	baseOfData.array = (inventory_t*)malloc(baseOfData.size * sizeof(inventory_t));

	char* fileName = "database.bin";

	int state;
	do
	{
		printf("\n1 - ��������� ����� ����� � ��\n2 - �������� ������ �� ��\n3 - ������� ������ ����\n4 - ����� ����� � ��\n5 - �������������� ��\n6 - �������� �����\n7 - ������� �����\n8 - ��������� ���� ������\n");
		scanf("%d", &state);

		switch (state)
		{
		case 1: {
			inventory_t invent;
			int saveState;
			invent.number = rand() % 100;

			printf("������� �������� ������������: ");
			fgets(invent.name, 100, stdin);
			scanf("%[^\n]", invent.name);

			printf("������� ��������� ������������: ");
			fgets(invent.category, 100, stdin);
			scanf("%[^\n]", invent.category);

			printf("������� �������� ������������: ");
			fgets(invent.description, 100, stdin);
			scanf("%[^\n]", invent.description);

			printf("���� ��������������: \n");
			validateDate(&invent.Date.day, 'd');
			validateDate(&invent.Date.month, 'm');
			validateDate(&invent.Date.year, 'y');

			printf("������� ��������� ������������: ");
			fgets(invent.state, 100, stdin);
			scanf("%[^\n]", invent.state);

			puts("��������� � ��?\n 1 - ��\n 2 - ���");
			scanf("%d", &saveState);
			switch (saveState)
			{
			case 1:
			{
				save(invent, &baseOfData);
				printf("����� ������� �������� � ���� ������ ");
				break;
			}
			case 2:
			{
				break;
			}
			default:
				break;
			}
			break;
		}
		case 2: {
			getDataBaseTables(fileName, &baseOfData);
			break;
		}
		case 3: {
			loadMany(&baseOfData);
			break;
		}

		case 4: {
			char category[100];
			printf("������� ��������� ������: ");
			fgets(category, 100, stdin);
			scanf("%[^\n]", &category);
			findOne(category, &baseOfData);
			break;
		}

		case 5: {
			puts("�� ���� ��������������: ");
			sort(&baseOfData);
			break;
		}

		case 6:
		{
			int state2 = 1, number, state3;
			inventory_t newParams = { "", "", 100, "", {0, 0, 0}, "" };
			printf("������� ����� ������: ");
			scanf("%d", &number);

			do {
				printf("1 - �������� ��������\n2 - �������� ���������\n3 - �������� ��������\n4 - �������� ����\n5 - �������� ���������\n6 - ��������� ���������\n");
				scanf("%d", &state3);

				switch (state3) {

				case 1:
				{
					printf("������� ����� �������� ������������: ");
					fgets(newParams.name, 100, stdin);
					scanf("%[^\n]", &newParams.name);
					break;
				}
				case 2:
				{
					printf("������� ����� ��������� ������������: ");
					fgets(newParams.category, 100, stdin);
					scanf("%[^\n]", &newParams.category);
					break;
				}
				case 3:
				{
					printf("������� ����� �������� ������������: ");
					fgets(newParams.description, 100, stdin);
					scanf("%[^\n]", &newParams.description);
					break;
				}
				case 4:
				{
					printf("����� ���� ��������������: \n");
					printf("������� ����: ");
					scanf("%d", &newParams.Date.day);
					printf("������� �����: ");
					scanf("%d", &newParams.Date.month);
					printf("������� ���: ");
					scanf("%d", &newParams.Date.year);
					break;
				}
				case 5:
				{
					printf("������� ����� ��������� ������������: ");
					fgets(newParams.state, 100, stdin);
					scanf("%[^\n]", &newParams.state);
					break;
				}
				case 6:
				{
					updateOne(number, newParams, &baseOfData);
					state2 = 2;
					break;
				}
				}
			} while (state2 == 1);
			break;
		}
		case 7:
		{
			int n;
			printf("������� ����� ��������, ������� ������ �������:");
			scanf("%d", &n);
			delete(n, &baseOfData);
			break;
		}
		case 8:
		{
			postDataBaseTables(fileName, &baseOfData);
			printf("������ �������� ���� ������� ���������");
		}
		default:
			break;
		}

	} while (1);
}



int save(inventory_t Invent, database_t* base)
{
	base->size += 1;
	inventory_t* tempTable = (inventory_t*)realloc(base->array, base->size * sizeof(inventory_t));

	if (tempTable != NULL) {
		tempTable[base->size - 1] = Invent;
		base->array = tempTable;
	}
	else {
		printf("������ ������ ��� ���������� ������");
		exit(0);
	}
}

int loadMany(database_t* base)
{
	printf("������ � ���� ������: \n");
	printf("%5s|%20s|%20s|%20s|%20s|%20s|%20s|\n", " ", " �������� ������������", "��������� ������������", "����� ������������", "�������� ������������", "���� ��������������", "��������� ������������");

	for (int i = 0; i < base->size; i++)
	{
		showDataLine(base, i);
	}
	printf("\n");
}


int findOne(char category[100], database_t* base)
{
	printf("\n������ ���������: %s\n", category);
	printf("%5s|%20s|%20s|%20s|%20s|%20s|%20s|\n", " ", " �������� ������������", "��������� ������������", "����� ������������", "�������� ������������", "���� ��������������", "��������� ������������");

	for (int i = 0; i < base->size; i++)
	{
		if (strcmp(base->array[i].category, category) == 0)
			showDataLine(base, i);
	}
	printf("\n");
}

int sort(database_t* base)
{
	qsort(base->array, base->size, sizeof(inventory_t), compare);
	printf("���� ������ ������� �������������\n");
}

int updateOne(int number, inventory_t newParams, database_t* base)
{
	inventory_t* tempArray = (inventory_t*)malloc(base->size * sizeof(inventory_t));

	for (int i = 0; i < base->size; i++)
	{
		tempArray[i] = base->array[i];
		if (base->array[i].number == number)
		{
			strcpy(tempArray[i].name, strcmp(newParams.name, "") != 0 ? newParams.name : base->array[i].name);
			strcpy(tempArray[i].description, strcmp(newParams.description, "") != 0 ? newParams.description : base->array[i].description);
			strcpy(tempArray[i].category, strcmp(newParams.category, "") != 0 ? newParams.category : base->array[i].category);
			strcpy(tempArray[i].state, strcmp(newParams.state, "") != 0 ? newParams.state : base->array[i].state);
			if (newParams.Date.day + newParams.Date.month + newParams.Date.year != 0)
			{
				tempArray[i].Date.day = newParams.Date.day;
				tempArray[i].Date.month = newParams.Date.month;
				tempArray[i].Date.year = newParams.Date.year;
			}
		}

	}
	base->array = tempArray;
	printf("���� ������ ������� ���������");
}


int getDataBaseTables(char* fileName, database_t* base)
{
	base->size = 0;
	base->array = (inventory_t*)malloc(base->size * sizeof(inventory_t));

	inventory_t* tempTable = (inventory_t*)malloc(base->size * sizeof(inventory_t));
	inventory_t table;

	FILE* fp = fopen(fileName, "r");

	int size = 0;

	while (fread(&table, sizeof(inventory_t), 1, fp) == 1)
	{
		size += 1;

		tempTable = (inventory_t*)realloc(base->array, size * sizeof(inventory_t));
		if (tempTable != NULL)
		{
			base->array = tempTable;
			base->array[size - 1] = table;
		}
	}
	base->size = size == 0 ? 0 : size;


	fclose(fp);

	printf("������ ������� ����������� �� ����, ����������� ������ � �������: %d\n", size);
}

int postDataBaseTables(char* fileName, database_t* base)
{
	FILE* fp = fopen(fileName, "w");
	fwrite(base->array, sizeof(inventory_t), base->size, fp);
	fclose(fp);

	free(base->array);
	base->size = 0;
}

int compare(const void* x1, const void* x2)
{
	inventory_t* A = (inventory_t*)x1;
	inventory_t* B = (inventory_t*)x2;
	if (A->Date.year == B->Date.year) {
		if (A->Date.month == B->Date.month)
			return A->Date.day - B->Date.day;
		else
			return A->Date.month - B->Date.month;
	}
	else
		return A->Date.year - B->Date.year;
}

int delete(int number, database_t* base)
{
	int size = 0;
	inventory_t* tempTable = (inventory_t*)malloc(size * sizeof(inventory_t));

	for (int i = 0; i < base->size; i++)
	{
		if (base->array[i].number != number)
		{
			size += 1;
			tempTable = (inventory_t*)realloc(tempTable, size * sizeof(inventory_t));
			tempTable[size - 1] = base->array[i];
		}
	}
	base->size = size;
	base->array = tempTable;

	printf("����� ��� ������� %d ������� ������", number);
}

int validateDate(int* date, char type)
{
	int day, month, year;
	switch (type)
	{
	case 'd':
	{
		printf("������� ����: ");
		scanf("%d", &day);
		while (day < 1 || day > 31)
		{
			printf("\33[2K\r");
			printf("������� ���������� ����� ���(1 - 31): ");
			scanf("%d", &day);
		}
		*date = day;
		break;
	}
	case 'm':
	{
		printf("������� �����: ");
		scanf("%d", &month);
		while (month < 1 || month > 12)
		{
			printf("\33[2K");
			printf("������� ���������� ����� ������(1 - 12): ");
			scanf("%d", &month);
		}
		*date = month;
		break;
	}
	case 'y':
	{
		printf("������� ���: ");
		scanf("%d", &year);
		while (year <= 1600 || year > 2023)
		{
			printf("\33[2K");
			printf("������� ���������� ����� ����(1600 - 2023): ");
			scanf("%d", &year);
		}
		*date = year;
		break;
	}
	default:
		break;
	}
}


void showDataLine(database_t* base, int ind)
{
	printf("%5s|%22s|%22s|%20d|%21s|%*s%d.%s%d.%d|%22s|\n",
		" ", base->array[ind].name, base->array[ind].category, base->array[ind].number, base->array[ind].description, 10, base->array[ind].Date.day < 10 ? "0" : "", base->array[ind].Date.day, base->array[ind].Date.month < 10 ? "0" : "", base->array[ind].Date.month, base->array[ind].Date.year, base->array[ind].state
	);
}