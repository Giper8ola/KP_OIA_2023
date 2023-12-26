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

typedef struct DataBase
{
    t_inventory* array;
    t_inventory* newBaseData;
    int size;
    int intialSize;
} t_database;


int save(t_inventory* Invent, t_database*);
int loadMany(t_database*);
int loadOne(char category[100], t_database*);
int sort(t_database*);
int updateOne(int number, t_inventory newParams, t_database*);
int deleteValue(int number, t_database*);

int compare(const void* x1, const void* x2);
t_inventory* fillArrayOfInv(char* filename);
int countOfInvInFile(char* fileName);
int validateDate(int*, char);
int realoadBaseArray(t_database*);

int main()
{

    t_database baseOfInvent;
    baseOfInvent.intialSize = 0;

    
    int state;

    srand(time(NULL));
    system("chcp 1251");
    setlocale(LC_ALL, "RUS");
    char* filename = "database.bin";
    baseOfInvent.size = 1;

    do
    {
        realoadBaseArray(&baseOfInvent, filename);
        printf("\n1 - ��������� ����� � ��\n2 - �������� ������ �� ��\n3 - ����� ����� � ��\n4 - �������������� ��\n5 - �������� �����\n6 - ������� �����\n");
        scanf("%d", &state);

        switch (state)
        {
        case 1: {
            t_inventory* invent = (t_inventory*)malloc(sizeof(t_inventory));
            int saveState;
            invent[0].number = rand() % 100;

            printf("������� �������� ������������: ");
            fgets(invent[0].name, 100, stdin);
            scanf("%[^\n]", &invent[0].name);

            printf("������� ��������� ������������: ");
            fgets(invent[0].category, 100, stdin);
            scanf("%[^\n]", &invent[0].category);

            printf("������� �������� ������������: ");
            fgets(invent[0].description, 100, stdin);
            scanf("%[^\n]", &invent[0].description);

            printf("���� ��������������: \n");
            validateDate(&invent->Date.day, 'd');
            validateDate(&invent->Date.month, 'm');
            validateDate(&invent->Date.year, 'y');

            printf("������� ��������� ������������: ");
            fgets(invent[0].state, 100, stdin);
            scanf("%[^\n]", &invent[0].state);

            puts("��������� � ��?\n 1 - ��\n 2 - ���");
            scanf("%d", &saveState);
            switch (saveState)
            {
            case 1:
            {
                save(invent, &baseOfInvent);
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
            loadMany(&baseOfInvent);
            break;
        }

        case 3: {
            char category[100];
            printf("������� ��������� ������: ");
            fgets(category, 100, stdin);
            scanf("%[^\n]", &category);
            loadOne(category, &baseOfInvent);
            break;
        }

        case 4: {
            puts("�� ���� ��������������: ");
            sort(&baseOfInvent);
            break;
        }

        case 5:
        {
            int state2 = 1, number, state3;
            t_inventory newParams = { "", "", 100, "", {0, 0, 0}, "" };
            printf("������� ����� ������: ");
            scanf("%d", &number);

            do {
                printf("\n1 - �������� ��������\n2 - �������� ���������\n3 - �������� ��������\n4 - �������� ����\n5 - �������� ���������\n6 - ��������� ���������\n");
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
                    updateOne(number, newParams, &baseOfInvent);
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
            printf("������� ����� ��������, ������� ������ �������:");
            scanf("%d", &n);
            deleteValue(n, &baseOfInvent);
            break;
        }
        default:
            break;
        }

    } while (1);
}

int save(t_inventory* Invent, t_database* base)
{
    base->intialSize = 1;
    base->newBaseData = (t_inventory*)malloc(sizeof(t_inventory));
    base->newBaseData = Invent;
   
    printf("����� ������ ��������");
}

int loadMany(t_database* base)
{
    printf("������ � ���� ������: \n");
    for (int i = 0; i < base->size; i++)
    {
        if (strcmp(base->array[i].state, "-1") != 0)
            printf("\n�������� ������������: %s\n��������� ������������: %s\n����� ������������: %d\n�������� ������������: %s\n���� ��������������: %d.%d.%d\n��������� ������������: %s\n", base->array[i].name, base->array[i].category, base->array[i].number, base->array[i].description, base->array[i].Date.day, base->array[i].Date.month, base->array[i].Date.year, base->array[i].state);
    }
    printf("\n");
}


int loadOne(char category[100], t_database* base)
{
    printf("\n������ ���������: %s\n", category);
    for (int i = 0; i < base->size; i++)
    {
        if (strcmp(base->array[i].category, category) == 0)
            printf("\n�������� ������������: %s\n��������� ������������: %s\n����� ������������: %d\n�������� ������������: %s\n���� ��������������: %d.%d.%d\n��������� ������������: %s\n", base->array[i].name, base->array[i].category, base->array[i].number, base->array[i].description, base->array[i].Date.day, base->array[i].Date.month, base->array[i].Date.year, base->array[i].state);
    }
    printf("\n");
}

int sort(t_database* base)
{
    qsort(base->array, base->size, sizeof(t_inventory), compare);

    for (int i = 0; i < base->size; i++)
        printf("\n�������� ������������: %s\n��������� ������������: %s\n����� ������������: %d\n�������� ������������: %s\n���� ��������������: %d.%d.%d\n��������� ������������: %s\n", base->array[i].name, base->array[i].category, base->array[i].number, base->array[i].description, base->array[i].Date.day, base->array[i].Date.month, base->array[i].Date.year, base->array[i].state);

    printf("\n");
}

int updateOne(int number, t_inventory newParams, t_database* base)
{
    base->newBaseData = (t_inventory*)malloc(base->size * sizeof(t_inventory));
    base->intialSize = 2;

    for (int i = 0; i < base->size; i++)
    {
        base->newBaseData[i] = base->array[i];
        if (base->array[i].number == number)
        {
            strcpy(base->newBaseData[i].name, strcmp(newParams.name, "") != 0 ? newParams.name : base->array[i].name);
            strcpy(base->newBaseData[i].description, strcmp(newParams.description, "") != 0 ? newParams.description : base->array[i].description);
            strcpy(base->newBaseData[i].category, strcmp(newParams.category, "") != 0 ? newParams.category : base->array[i].category);
            strcpy(base->newBaseData[i].state, strcmp(newParams.state, "") != 0 ? newParams.state : base->array[i].state);
            if (newParams.Date.day + newParams.Date.month + newParams.Date.year != 0)
            {
                base->newBaseData[i].Date.day = newParams.Date.day;
                base->newBaseData[i].Date.month = newParams.Date.month;
                base->newBaseData[i].Date.year = newParams.Date.year;
            }
        }
        
    }
    printf("���� ������ ������� ���������");
    
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

int deleteValue(int number, t_database* base)
{
    base->newBaseData = (t_inventory*)malloc(base->size * sizeof(t_inventory));
    base->intialSize = 2;

    for (int i = 0; i < base->size; i++)
    {
        base->newBaseData[i] = base->array[i];
        if (base->array[i].number == number)
        {
            strcpy(base->newBaseData[i].state, strcmp(base->array[i].state, "-1") != 0 ? "-1" : base->array[i].state);
            break;
        }

    }
    printf("���� ������� ��������");
}

int realoadBaseArray(t_database* base, char* fileName)
{
    if (base->intialSize == 2)
    {  
        FILE* fp = fopen(fileName, "w+");
        fwrite(base->newBaseData, sizeof(t_inventory), base->size, fp);
        fclose(fp); 
    }
    if (base->intialSize == 1)
    {
        
        FILE* fp2 = fopen(fileName, "a+");
        fwrite(base->newBaseData, sizeof(t_inventory), 1, fp2);
        fclose(fp2);
    }
    base->size = countOfInvInFile(fileName);
    base->array = fillArrayOfInv(fileName);
    base->intialSize = 0;
   
}