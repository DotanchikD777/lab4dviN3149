#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "My_Lib.h"






int main(int argc, char* argv[])
{
    char *binfilename;
    char command[256];
    Node *head = NULL;


    if (argc == 2 && strcmp(argv[1], "-v") == 0) //ну тут понятно
    {
        printf("%s", "\tДенис Ильчук Витальевич, гр. N3149\n\tВариант: 2-8-2-5\n");
        return 0;
    }
    else if (argc == 2 && (Is_Valid_File_Name(argv[1]) == 0))
    {
        Start();
        binfilename = argv[1];
    }
    else
    {
        Errors(1);
    }

    Read_And_Append_To_List(binfilename, &head); // сразу и список создается и в него из файла читается

    while (1)//цикл для того чтобы можно было сколько хочешь команд дать
    {
        if(fgets(command, sizeof(command), stdin) == NULL) // чтобы control+D работал и с EOF тоже подружить
        {
            End();
        }
        remove_last_character(command); //удаляет последний \n из комманды
        char *token = strtok(command, " "); // считываем программу

        if (strcmp(token, "cat") == 0)//сам добавил для дебага, но решил оставить
        {
            while (1) // сколько хочешь файлов можно посмотреть
            {
                token = strtok(NULL, " ");
                if(token == NULL)
                {
                    break;
                }

                Print_File(token);
            }
        }
        else if (strcmp(token, "push_front") == 0)// заполняется массив и в обратном порядке добавляется в список
        {
            char *array[100] = {NULL};
            char *arg;
            int i = 0;
            while(1)
            {
                token = strtok(NULL, " ");
                if(token == NULL)
                {
                    break;
                }


                arg = token;
                if (Is_Valid_Date_Time_Format(arg) == 0)
                {
                    Errors(9);
                }

                Insert_String(array, arg, i);
                i += 1;

            }  
            for (int j = i - 1; j >= 0; j--)
            {
                Push_Front(array[j], &head);
            }

            Prepare_List(&head);
        }
        else if(strcmp(token, "dump") == 0) // я конечно на искосок читал, но посмотреть список и записать в файл можно 
        {
            Prepare_List(&head);

            token = strtok(NULL, " ");
            if (token == NULL)
            {
                Print_List(head);
            }
            else
            {
                while(1)
                {
                    Txt_Or_Bin(token, &head);
                    token = strtok(NULL, " ");
                    if(token == NULL)
                    {
                        break;
                    }
                }
            }  
        }
        else if (strcmp(token, "push_back") == 0)// как push_front только добавляется последовательно
        {
            char *arg;
            while(1)
            {
                token = strtok(NULL, " ");
                if(token == NULL)
                {
                    break;
                }

                arg = token;
                if (Is_Valid_Date_Time_Format(arg) == 0)
                {
                    Errors(9);
                }
                Push_Back(arg, &head);
            } 

            Prepare_List(&head); 
        }
        else if (strcmp(token, "pop_front") == 0)
        {
            Remove_First(&head);

            Prepare_List(&head);
        }
        else if (strcmp(token, "pop_back") == 0)
        {
            Remove_Last(&head);

            Prepare_List(&head);
        }
        else if (strcmp(token, "help") == 0)
        {
            Print_Help_Message();
        }
        else if (strcmp(token, "filter") == 0)
        {
            token = strtok(NULL, " ");
            if(token == NULL)
            {

            }
            else
            {
                Delete_Node(token, &head);
            }
        }
        else
        {
            printf("\n+----------------------------------------------------------------+\n\tНеизвестная комманда. Список комманд доступен при вводе help\n+----------------------------------------------------------------+\n");
        }
    }

    Free_List(head);
    return 0;
}