#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

typedef struct Date
{
    int day;
    int month;
    int year;
} t_date;

typedef struct Inventory
{
    char name[100];
    char category[100];
    int  number;
    char description[100];
    t_date Date;
    char state[100];
} t_inventory;

struct database {
    t_inventory* array;
    int size;
};

int save(char* filename, t_inventory Invent[1]);
int loadMany(char* filename);
int loadOne(char* filename, char category[100]);
int sort(char* fileName);
int updateOne(char* fileName, int number, t_inventory newParams);
int deleteValue(char* fileName, int number);

int compare(const void* x1, const void* x2);
t_inventory* fillArrayOfInv(char* filename);
int countOfInvInFile(char* fileName);
int validateDate(int*, char);

int main()
{
    int state;
    srand(time(NULL));
    system("chcp 1251");
    setlocale(LC_ALL, "RUS");
    char* filename = "database.bin";
    do
    {
        printf("\n1 - Загрузить заказ в БД\n2 - Получить заказы из БД\n3 - Найти заказ в БД\n4 - Отосортировать БД\n5 - Изменить заказ\n6 - Удалить заказ\n");
        scanf("%d", &state);

        switch (state)
        {
        case 1: {
            t_inventory invent[1];
            int saveState;
            invent[0].number = rand() % 100;

            printf("Введите название оборудования: ");
            fgets(invent[0].name, 100, stdin);
            scanf("%[^\n]", &invent[0].name);

            printf("Введите категорию оборудования: ");
            fgets(invent[0].category, 100, stdin);
            scanf("%[^\n]", &invent[0].category);

            printf("Введите описания оборудования: ");
            fgets(invent[0].description, 100, stdin);
            scanf("%[^\n]", &invent[0].description);

            printf("Дата инвертаризации: \n");
            validateDate(&invent->Date.day, 'd');
            validateDate(&invent->Date.month, 'm');
            validateDate(&invent->Date.year, 'y');

            printf("Введите состояние оборудования: ");
            fgets(invent[0].state, 100, stdin);
            scanf("%[^\n]", &invent[0].state);

            puts("Сохранить в БД?\n 1 - да\n 2 - нет");
            scanf("%d", &saveState);
            switch (saveState)
            {
            case 1:
            {
                save(filename, invent);
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
            loadMany(filename);
            break;
        }

        case 3: {
            char category[100];
            printf("Введите категорию заказа: ");
            fgets(category, 100, stdin);
            scanf("%[^\n]", &category);
            loadOne(filename, category);
            break;
        }

        case 4: {
            puts("По дате инвертаризации: ");
            sort(filename);
            break;
        }

        case 5:
        {
            int state2 = 1, number, state3;
            t_inventory newParams = { "", "", 100, "", {0, 0, 0}, "" };
            printf("Введите номер заказа: ");
            scanf("%d", &number);

            do {
                printf("\n1 - Изменить название\n2 - Изменить категорию\n3 - Изменить описание\n4 - Изменить дату\n5 - Изменить состояние\n6 - Выполнить изменения\n");
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
                    updateOne(filename, number, newParams);
                    state2 = 2;
                    break;
                }
                }
            } while (state2 == 1);
            break;
        }
        case 6:
        {
            int n;
            printf("Введите номер элемента, который хотите удалить:");
            scanf("%d", &n);
            deleteValue(filename, n);
            break;
        }
        default:
            break;
        }

    } while (1);
}

int save(char* filename, t_inventory Invent[1])
{
    size_t size = sizeof Invent[0];
    FILE* fp = fopen(filename, "a+");
    fwrite(Invent, size, 1, fp);
    printf("Заказ успено сохранен");
    fclose(fp);
}

int loadMany(char* filename)
{
    t_inventory Invent;
    FILE* fp = fopen(filename, "r");
    printf("Заказы в базе данных: \n");
    while (fread(&Invent, sizeof(Invent), 1, fp) == 1)
    {
        if (strcmp(Invent.state, "-1") != 0)
            printf("\nНазвание оборудования: %s\nКатегория оборудования: %s\nНомер оборудования: %d\nОписание оборудования: %s\nДата инвертаризации: %d.%d.%d\nСостояние оборудования: %s\n", Invent.name, Invent.category, Invent.number, Invent.description, Invent.Date.day, Invent.Date.month, Invent.Date.year, Invent.state);
    }
    printf("\n");
    fclose(fp);
}


int loadOne(char* filename, char category[100])
{
    t_inventory Invent;
    FILE* fp = fopen(filename, "r");
    printf("\nЗаказы категории: %s\n", category);
    while (fread(&Invent, sizeof(Invent), 1, fp) == 1)
    {
        if (strcmp(Invent.category, category) == 0)
            printf("\n\tНазвание оборудования: %s\n\tКатегория оборудования: %s\n\tНомер оборудования: %d\n\tОписание оборудования: %s\n\tДата инвертаризации: %d.%d.%d\n\tСостояние оборудования: %s\n", Invent.name, Invent.category, Invent.number, Invent.description, Invent.Date.day, Invent.Date.month, Invent.Date.year, Invent.state);
    }
    fclose(fp);
}

int sort(char* fileName)
{
    t_inventory* Invent;
    Invent = fillArrayOfInv(fileName);

    int j2 = countOfInvInFile(fileName);

    qsort(Invent, j2, sizeof(t_inventory), compare);

    for (int i = 0; i < j2; i++)
        printf("\n\tНазвание оборудования: %s\n\tКатегория оборудования: %s\n\tНомер оборудования: %d\n\tОписание оборудования: %s\n\tДата инвертаризации: %d.%d.%d\n\tСостояние оборудования: %s\n", Invent[i].name, Invent[i].category, Invent[i].number, Invent[i].description, Invent[i].Date.day, Invent[i].Date.month, Invent[i].Date.year, Invent[i].state);

    printf("\n");
    free(Invent);
}

int updateOne(char* fileName, int number, t_inventory newParams)
{
    int pos, j2;
    t_inventory* Invent;
    t_inventory* newInvent;
    t_inventory inv;

    Invent = fillArrayOfInv(fileName);
    j2 = countOfInvInFile(fileName);

    newInvent = (t_inventory*)malloc(j2 * sizeof(t_inventory));


    for (int i = 0; i < j2; i++)
    {
        if (Invent[i].number == number)
        {
            pos = i;
            continue;
        }
        newInvent[i] = Invent[i];
    }

    FILE* fp = fopen(fileName, "r");
    while (fread(&inv, sizeof(inv), 1, fp) == 1)
    {
        if (inv.number == number)
        {
            strcpy(inv.name, strcmp(newParams.name, "") != 0 ? newParams.name : inv.name);
            strcpy(inv.description, strcmp(newParams.description, "") != 0 ? newParams.description : inv.description);
            strcpy(inv.category, strcmp(newParams.category, "") != 0 ? newParams.category : inv.category);
            strcpy(inv.state, strcmp(newParams.state, "") != 0 ? newParams.state : inv.state);
            if (newParams.Date.day + newParams.Date.month + newParams.Date.year != 0)
            {
                inv.Date.day = newParams.Date.day;
                inv.Date.month = newParams.Date.month;
                inv.Date.year = newParams.Date.year;
            }
            break;
        }
    }

    fclose(fp);
    newInvent[pos] = inv;

    size_t size = sizeof(newInvent[0]);
    FILE* fp2 = fopen(fileName, "w+");
    fwrite(newInvent, size, j2, fp2);
    printf("Файл успешно обновлен");
    fclose(fp2);
    free(Invent);
    free(newInvent);
}

int compare(const void* x1, const void* x2)
{
    t_inventory* A = (t_inventory*)x1;
    t_inventory* B = (t_inventory*)x2;
    if (A->Date.year == B->Date.year) {
        if (A->Date.month == B->Date.month)
            return A->Date.day - B->Date.day;
        else
            return A->Date.month - B->Date.month;
    }
    else
        return A->Date.year - B->Date.year;
}

int countOfInvInFile(char* fileName)
{
    int count = 0;
    FILE* fp = fopen(fileName, "r");
    t_inventory inv;
    while (fread(&inv, sizeof(inv), 1, fp) == 1)
    {
        count++;
    }
    fclose(fp);
    return count;
}

t_inventory* fillArrayOfInv(char* fileName)
{
    int j, j2 = 0;
    t_inventory inv;
    t_inventory* Invent;
    j = countOfInvInFile(fileName);

    FILE* fp2 = fopen(fileName, "r");
    Invent = (t_inventory*)malloc(j * sizeof(t_inventory));

    while (fread(&inv, sizeof(inv), 1, fp2) == 1)
    {
        Invent[j2] = inv;
        j2++;
    }
    fclose(fp2);
    return Invent;
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

int deleteValue(char* fileName, int number)
{
    int pos, j2;
    t_inventory* Invent;
    t_inventory* newInvent;
    t_inventory inv;

    Invent = fillArrayOfInv(fileName);
    j2 = countOfInvInFile(fileName);

    newInvent = (t_inventory*)malloc(j2 * sizeof(t_inventory));
    printf("%d\n", j2);

    for (int i = 0; i < j2; i++)
    {
        if (Invent[i].number == number)
        {
            pos = i;
            continue;
        }
        newInvent[i] = Invent[i];
    }

    FILE* fp = fopen(fileName, "r");
    while (fread(&inv, sizeof(inv), 1, fp) == 1)
    {
        if (inv.number == number)
        {

            strcpy(inv.state, strcmp(inv.state, "-1") != 0 ? "-1" : inv.state);
            break;
        }
    }

    fclose(fp);
    newInvent[pos] = inv;

    size_t size = sizeof(newInvent[0]);
    FILE* fp2 = fopen(fileName, "w+");
    fwrite(newInvent, size, j2, fp2);
    printf("Файл успешно обновлен");
    fclose(fp2);
    free(Invent);
    free(newInvent);
}