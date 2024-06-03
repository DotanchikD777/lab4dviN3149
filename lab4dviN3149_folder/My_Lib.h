void Errors(int err)
{
    if (err == 0)
    {
        return;
    }
    else if (err == 1)
    {
        printf("Ошибка!:\n\tНеправильные использование, используйте:\n\t ./lab4dviN3149 -v\n\t./lab4dviN3149 fileName.bin\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 2)
    {
        printf("Ошибка!:\n\tНе удалось открыть файл\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 3)
    {
        printf("Ошибка!:\n\tНе удалось выделить память для нового узла списка\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 4)
    {
        printf("Ошибка!:\n\tПустой файл\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 5)
    {
        printf("Ошибка!:\n\tФайл поврежден\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 6)
    {
        printf("Ошибка!:\n\tНе удалось выделить память\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 7)
    {
        printf("Ошибка!:\n\tСлишком много строк\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 8)
    {
        printf("Ошибка!:\n\tОшибка компиляции регулярного выражения\n");
        exit(EXIT_FAILURE);
    }
    else if(err == 9)
    {
        printf("Ошибка!:\n\tСтроки не соответствуют формату\n");
        exit(EXIT_FAILURE);
    }
}
int Is_Valid_Date_Time_Format(const char *str) 
{
    // Регулярные выражения для времени и даты
    const char *time_regex = "^(0[0-9]|1[0-9]|2[0-3])\\.(0[0-9]|[1-5][0-9])\\.(0[0-9]|[1-5][0-9])$";
    const char *date_regex = "^(0[1-9]|[12][0-9]|3[01])\\.(0[1-9]|1[0-2])\\.(19[0-9]{2}|20[0-9]{2}|[0-9]{4})$";

    regex_t regex_time, regex_date;
    int reti_time, reti_date;

    reti_time = regcomp(&regex_time, time_regex, REG_EXTENDED);
    reti_date = regcomp(&regex_date, date_regex, REG_EXTENDED);
    if (reti_time || reti_date) 
    {
        Errors(8);
    }

    // Проверяем строку на соответствие регулярным выражениям
    reti_time = regexec(&regex_time, str, 0, NULL, 0);
    reti_date = regexec(&regex_date, str, 0, NULL, 0);

    regfree(&regex_time);
    regfree(&regex_date);

    if (!reti_time || !reti_date)
    {
        return 1;
    } else 
    {
        return 0;
    }
}
void remove_last_character(char *str) 
{
    int len = strlen(str);
    if (len > 0) 
    {
        str[len - 1] = '\0';
    }
}


// Структура для узла двусвязного списка
typedef struct Node 
{
    int i;
    int o;
    char *str;
    struct Node *prev;
    struct Node *next;
} Node;
// функция для создания нового узла
Node* Create_Node(int i, int o, const char *str) 
{
    Node *newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) 
    {
        Errors(3);
    }

    newNode->i = i;
    newNode->o = o;
    newNode->str = strdup(str); // пока не разобрался как strtok работает 5 лет прошло
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}
// Функция для добавления узла в конец списка
void Append_To_End(Node **head, int i, int o, const char *str) 
{

    Node *newNode = Create_Node(i, o, str);

    if (*head == NULL) 
    {
        *head = newNode;
    } 
    else 
    {
        Node *temp = *head;

        while (temp->next != NULL) 
        {
            temp = temp->next;
        }

        temp->next = newNode;
        newNode->prev = temp;
    }
}
// такая же для добавления в начало
void Append_To_Front(Node **head, int i, int o, const char *str) 
{

    Node *newNode = Create_Node(i, o, str);

    if (*head == NULL) 
    {
        *head = newNode;
    } 
    else 
    {
        newNode ->prev = NULL;
        newNode ->next = *head;
        *head = newNode;
    }
}
// Функция для считывания данных из бинарного файла и добавления их в список
void Read_And_Append_To_List(const char *filename, Node **head)
 {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) 
    {
        Errors(2);
    }

    int n, i, o;

    fread(&n, sizeof(int), 1, file); // считываем значение n
    if(n <= 0)
    {
        Errors(4);
    }
    fseek(file, (n) * (sizeof(int) + sizeof(int)), SEEK_CUR);
    fseek(file, 100, SEEK_CUR);

    for (int j = 0; j < n; j++) 
    {
        i = j;
        o = ftell(file);

        int k;
        fread(&k, sizeof(int), 1, file); // считываем размер строки

        char *str = (char*)malloc((k + 1) * sizeof(char)); // выделяем память для строки
        fread(str, sizeof(char), k, file); // считываем саму строку
        str[k] = '\0'; // добавляем конец_строки-символ

        if (Is_Valid_Date_Time_Format(str) == 0)
        {
            Errors(9);
        }

        Append_To_End(head, i, o, str); // логичнее добавлять в конец
        free(str); // освобождаем память, выделенную для строки

    }

    fclose(file);
}
// Функция для освобождения памяти, выделенной под список
void Free_List(Node *head) 
{
    Node *temp;

    while (head != NULL) 
    {
        temp = head;
        head = head->next;
        free(temp->str);
        free(temp);
    }
}
void Print_List(Node *head) 
{
    Node *current = head;
    printf("%s", "\n+----------------------------------------------------------------+\n\tВаш список:\n+----------------------------------------------------------------+\n");
    
    while (current != NULL) 
    {
        printf("\n%s\n", current->str);
        current = current->next;
    }

    printf("%s", "\n+----------------------------------------------------------------+\n");
}


int Is_Valid_File_Name(char *str)// 1 это да 0 это нет 
{
    int len = strlen(str);

    // Проверяем длину строки
    if (len <= 4) 
    {
        return 1;
    }

    // Проверяем расширение файла
    if (strcmp(str + len - 4, ".bin") != 0)
    {
        return 1;
    }

    return 0;
}
// скопировал алгоритм из программы которая делает бинарные файлы
void Print_File(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        Errors(2);
    }
    int n, k;
    fread(&n, sizeof(int), 1, file);

    fseek(file, (n) * (sizeof(int) + sizeof(int)), SEEK_CUR);
    fseek(file, 100, SEEK_CUR);
    printf("\n+----------------------------------------------------------------+\n\tСодержимое файла %s:\n+----------------------------------------------------------------+\n", filename);
    printf("\tКоличество строк = %d\n", n);

    for (int i = 0; i < n; i++)
    {
        fread(&k, sizeof(int), 1, file);

        char *str = (char*)malloc((k + 1) * sizeof(char));
        if (str == NULL)
        {
            Errors(6);
        }
        str[k] = '\0';

        fread(str, sizeof(char), k, file);
        printf("\tСтрока [%d] = %s\n", i, str);

        free(str);
    }
    printf("%s", "\n+----------------------------------------------------------------+\n");
    fclose(file);
}

void Start()
{
    printf("%s", "\n+----------------------------------------------------------------+\n\tProgram has started\n+----------------------------------------------------------------+\n");
}

void End()
{
    printf("%s", "\tProgram has ended successfully\n+----------------------------------------------------------------+\n");
    exit(EXIT_SUCCESS);
}
// веселые функции, которые занимают стэк
void Push_Front(char *str, Node **head)
{
    Append_To_Front(head, -1, -1, str);
}
void Push_Back(char *str, Node **head)
{
    Append_To_End(head, -1, -1, str);
}

void Insert_String(char *arr[], const char *str, int i) // опять же для того, чтобы strtok не поламалась
{
    int max_size = 100;
    if (i < 0 || i >= max_size) 
    {
        Errors(7);
    }

    if (arr[i] != NULL) 
    {
        free(arr[i]);  // Освободить память, если в этом месте уже есть строка
    }

    arr[i] = (char *)malloc(strlen(str) + 1);  // Выделить память для новой строки
    if (arr[i] == NULL) 
    {
        Errors(6);
    }

    strcpy(arr[i], str);  // Копировать строку в массив
}

void Remove_First(Node **head) 
{
    if (head == NULL || *head == NULL) 
    {
        return; // Список пуст или head является NULL
    }

    Node *temp = *head;
    *head = (*head)->next;
    if (*head != NULL) 
    {
        (*head)->prev = NULL;
    }

    free(temp->str);
    free(temp);
}

void Remove_Last(Node **head) 
{
    if (head == NULL || *head == NULL) 
    {
        return; // Список пуст или head является NULL
    }

    Node *current = *head;
    while (current->next != NULL) 
    {
        current = current->next;
    }

    if (current->prev != NULL) 
    {
        current->prev->next = NULL;
    } else 
    {
        *head = NULL; // Удаляемый элемент был единственным элементом списка
    }

    free(current->str);
    free(current);
}

void Print_Help_Message()
{
    printf("\n+----------------------------------------------------------------+\n\tСписок комманд и их описание\n+----------------------------------------------------------------+\n");
    printf("\tpush_front <string 1> <string 2> <string 3> ... <string 100>\n\t\tЭта команда добавляет в начало списоа сктроки которые вы ввели.\n\t\tКоличество строк 1 - 100.\n\t\tДобавляет строки по порядку.\n");
    printf("\tpush_back <string 1> <string 2> <string 3> ... <string 100>\n\t\tЭта команда добавляет в конец списоа сктроки которые вы ввели.\n\t\tКоличество строк 1 - 100.\n\t\tДобавляет строки по порядку.\n");
    printf("\tdump <filename>\n\t\tЗаписывает в указанный файл элементы из списка, если имя файла пусто, то выводит список на экран.\n\t\tМожно указать несколько файлов.\n");
    printf("\tcat <binfilename>\n\t\tВыводит на экран элементы, которые сейчас находятся в указанном файле.\n\t\tМожно указать несколько файлов.\n\t\tВыводит только файлы в формати .bin\n");
    printf("\tpop_front\n\t\tУдаляет первый элемент в списке.\n\t\tЕсли список пуст, то комманда ничего не делает.\n");
    printf("\tpop_back\n\t\tУдаляет последний элемент в списке.\n\t\tЕсли список пуст, то комманда ничего не делает.\n");
    printf("\tfilter <string>\n\t\tУдаляет все элементы в которых встречается строка.");
    printf("\n+----------------------------------------------------------------+\n");
}

void Prepare_List(Node **head)// из-за особенностей того как у меня записывается файл, я просто заного вычисляю номера и положение строк, тем более, что когда новая строка добавляется то мы не знаем ее положение, а если в начало то вообще ужас
{
    int n = 0;
    Node *last = *head;

    while (last->next != NULL) 
    {
        last = last->next;
        n += 1;
    }
    n += 1;

    int o = 1 + n * (2 * sizeof(int)) + 100;
    Node *current = *head;

    for (int i = 0; i < n; i++)
    {  
        current->i = i;
        current->o = o;
        o += 1 + strlen(current->str);
        current = current->next;
    }
}

void Write_To_File(const char* filename, Node **head) 
{
    FILE* file = fopen(filename, "wb");
    if (!file) 
    {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    int n = 0;
    Node *last = *head;

    while (last->next != NULL) 
    {
        last = last->next;
        n += 1;
    }
    n += 1;
    // Запись n
    fwrite(&n, sizeof(int), 1, file);
    Node *current = *head;
    // Запись i, o облясти
    for(int i = 0; i < n; i++)
    {
        fwrite(&current->i, sizeof(int), 1, file);
        fwrite(&current->o, sizeof(int), 1, file);

        current = current->next;
    }


    // Запись мусорной области в 100 байт
    char emptySpace[100] = {0};
    fwrite(emptySpace, sizeof(char), 100, file);

    Node *current1 = *head;
    // Запись области k, s

    for(int i = 0; i < n; i++)
    {
        int len = strlen(current1->str);
        
        fwrite(&len, sizeof(int), 1, file);
        fwrite(current1->str, sizeof(char), len, file);

        current1 = current1->next;
    }

    fclose(file);
}
void Write_To_file_txt(const char *filename, Node **head)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        Errors(2);
    }
    Node *current = *head;

    while(current != NULL )
    {
        fprintf(file, "0x%lx %p %p %s\n", (uintptr_t)current->o, (void *)current->prev, (void *)current->next, current->str);
        current = current->next;
    }

    fclose(file);
}


void Txt_Or_Bin(const char *filename, Node **head) // для корректной работы dump с разными типами файлов
{
    const char *txt = ".txt";
    const char *bin = ".bin";
    size_t name_len = strlen(filename);
    size_t format_len = strlen(txt);
    if (format_len != strlen(bin)) //не возможная ошибка, но вдруг
    {
        exit(EXIT_FAILURE);
    }
    if (name_len < format_len)
    {
        Errors(2);
    }

    if (strcmp(filename + name_len - format_len, txt) == 0)
    {
        Write_To_file_txt(filename, head);
    }
    else if (strcmp(filename + name_len - format_len, bin) == 0)
    {
        Write_To_File(filename, head);
    }
    else
    {
        Errors(2);
    }

}
void Delete_Node(const char *str, Node **head)
{
    Node *current = *head;

    while (current != NULL)
    {
        if (strcmp(current->str, str) == 0)
        {
            Node *to_delete = current;
            
            if (current->prev == NULL && current->next != NULL) // Удаляем первый элемент, есть следующий
            {
                current = current->next;
                current->prev = NULL;
                *head = current;
            }
            else if (current->next == NULL && current->prev != NULL) // Удаляем последний элемент
            {
                current = current->prev;
                current->next = NULL;
            }
            else if (current->next != NULL && current->prev != NULL) // Удаляем средний элемент
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                current = current->next;
            }
            else // Удаляем единственный элемент
            {
                *head = NULL;
                current = NULL;
            }
            
            free(to_delete->str);// освобождаем чтобы не болтались
            free(to_delete);
        }
        else
        {
            current = current->next;
        }
    }
}
