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
		printf("\n1 - Загрузить новый заказ в БД\n2 - Получить заказы из БД\n3 - Вывести данные базы\n4 - Найти заказ в БД\n5 - Отосортировать БД\n6 - Изменить заказ\n7 - Удалить заказ\n8 - Сохранить базу данных\n");
		scanf("%d", &state);

		switch (state)
		{
		case 1: {
			inventory_t invent;
			int saveState;
			invent.number = rand() % 100;

			printf("Введите название оборудования: ");
			fgets(invent.name, 100, stdin);
			scanf("%[^\n]", invent.name);

			printf("Введите категорию оборудования: ");
			fgets(invent.category, 100, stdin);
			scanf("%[^\n]", invent.category);

			printf("Введите описания оборудования: ");
			fgets(invent.description, 100, stdin);
			scanf("%[^\n]", invent.description);

			printf("Дата инвертаризации: \n");
			validateDate(&invent.Date.day, 'd');
			validateDate(&invent.Date.month, 'm');
			validateDate(&invent.Date.year, 'y');

			printf("Введите состояние оборудования: ");
			fgets(invent.state, 100, stdin);
			scanf("%[^\n]", invent.state);

			puts("Сохранить в БД?\n 1 - да\n 2 - нет");
			scanf("%d", &saveState);
			switch (saveState)
			{
			case 1:
			{
				save(invent, &baseOfData);
				printf("Заказ успешно сохранен в базу данных ");
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
			printf("Введите категорию заказа: ");
			fgets(category, 100, stdin);
			scanf("%[^\n]", &category);
			findOne(category, &baseOfData);
			break;
		}

		case 5: {
			puts("По дате инвертаризации: ");
			sort(&baseOfData);
			break;
		}

		case 6:
		{
			int state2 = 1, number, state3;
			inventory_t newParams = { "", "", 100, "", {0, 0, 0}, "" };
			printf("Введите номер заказа: ");
			scanf("%d", &number);

			do {
				printf("1 - Изменить название\n2 - Изменить категорию\n3 - Изменить описание\n4 - Изменить дату\n5 - Изменить состояние\n6 - Выполнить изменения\n");
				scanf("%d", &state3);

				switch (state3) {

				case 1:
				{
					printf("Введите новое название оборудования: ");
					fgets(newParams.name, 100, stdin);
					scanf("%[^\n]", &newParams.name);
					break;
				}
				case 2:
				{
					printf("Введите новую категорию оборудования: ");
					fgets(newParams.category, 100, stdin);
					scanf("%[^\n]", &newParams.category);
					break;
				}
				case 3:
				{
					printf("Введите новое описание оборудования: ");
					fgets(newParams.description, 100, stdin);
					scanf("%[^\n]", &newParams.description);
					break;
				}
				case 4:
				{
					printf("Новая дата инвертаризации: \n");
					printf("Введите день: ");
					scanf("%d", &newParams.Date.day);
					printf("Введите месяц: ");
					scanf("%d", &newParams.Date.month);
					printf("Введите год: ");
					scanf("%d", &newParams.Date.year);
					break;
				}
				case 5:
				{
					printf("Введите новое состояние оборудования: ");
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
			printf("Введите номер элемента, который хотите удалить:");
			scanf("%d", &n);
			delete(n, &baseOfData);
			break;
		}
		case 8:
		{
			postDataBaseTables(fileName, &baseOfData);
			printf("Данные файловой базы успешно обновлены");
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
		printf("Утечка пямяти при сохранении данных");
		exit(0);
	}
}

int loadMany(database_t* base)
{
	printf("Заказы в базе данных: \n");
	printf("%5s|%20s|%20s|%20s|%20s|%20s|%20s|\n", " ", " Название оборудования", "Категория оборудования", "Номер оборудования", "Описание оборудования", "Дата инвертаризации", "Состояние оборудования");

	for (int i = 0; i < base->size; i++)
	{
		showDataLine(base, i);
	}
	printf("\n");
}


int findOne(char category[100], database_t* base)
{
	printf("\nЗаказы категории: %s\n", category);
	printf("%5s|%20s|%20s|%20s|%20s|%20s|%20s|\n", " ", " Название оборудования", "Категория оборудования", "Номер оборудования", "Описание оборудования", "Дата инвертаризации", "Состояние оборудования");

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
	printf("База данных успешно отсортирована\n");
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
	printf("База данных успешно обновлена");
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

	printf("Данные успешно скопированы из базы, колличество данных в таблице: %d\n", size);
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

	printf("Заказ под номером %d успешно удален", number);
}

int validateDate(int* date, char type)
{
	int day, month, year;
	switch (type)
	{
	case 'd':
	{
		printf("Введите день: ");
		scanf("%d", &day);
		while (day < 1 || day > 31)
		{
			printf("\33[2K\r");
			printf("Введите корректный номер дня(1 - 31): ");
			scanf("%d", &day);
		}
		*date = day;
		break;
	}
	case 'm':
	{
		printf("Введите месяц: ");
		scanf("%d", &month);
		while (month < 1 || month > 12)
		{
			printf("\33[2K");
			printf("Введите корректный номер месяца(1 - 12): ");
			scanf("%d", &month);
		}
		*date = month;
		break;
	}
	case 'y':
	{
		printf("Введите год: ");
		scanf("%d", &year);
		while (year <= 1600 || year > 2023)
		{
			printf("\33[2K");
			printf("Введите корректный номер года(1600 - 2023): ");
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