#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_from_user();
char **updated_list(FILE * file, char *removed_duty);

int main()
{
    char c;
    printf("What do you want to do(a - add, r - remove) : ");
    scanf(" %c", &c); //Choose the act
    getchar();//delete znak riadka
    if (c == 'a')// add something
    {
        FILE *file = fopen("ToDo.txt", "a");
        if (!file)
        {
            return 1;
        }
        char *duty = NULL;
        while(1)
        {
            duty = read_from_user();
            if(strcmp(duty, "stop") == 0)
            {
                free(duty);
                break;
            }
            fprintf(file,"%s\n", duty);
            free(duty);
        }
        fclose(file);
        return 0;
    }
    
    else if (c == 'r')// remove something
    {
        char *removed_duty = NULL;
        while(1)
        {
            FILE *file = fopen("ToDo.txt", "r");
            if(!file)
            {
                return 1;
            }
            removed_duty = read_from_user();
            if(removed_duty == NULL)
            {
                fclose(file);
                return 1;
            }
            if(strcmp(removed_duty, "stop") == 0)
            {
                fclose(file);
                free(removed_duty);
                break;
            }
            char **new_list = updated_list(file, removed_duty);
            fclose(file);
            if (new_list == NULL)
            {
                free(removed_duty);
                return 1;
            }
            
            FILE *new_file = fopen("ToDo.txt", "w");
            int i = 0;
            while(new_list[i] != NULL)//Write in new file and free memory
            {
                fprintf(new_file,"%s\n", new_list[i]);
                free(new_list[i++]);
            }
            free(new_list);
            fclose(new_file);
        }
    }
    
}

char* read_from_user()
{
    int c;
    char *str = NULL;
    int len = 0;
    while(((c = getchar()) != '\n') && (c != EOF))//read string char by char
    {
        char *tmp = realloc(str, len + 2);
        if(!tmp)
        {
            free(str);
            return NULL;
        }
        str = tmp;
        str[len++] = (char)c;
        str[len] = '\0';
    }
    return str;
}

char **updated_list(FILE *file, char *removed_duty)
{
    char **updated_list = NULL;
    char *str = NULL;
    int count = 0;
    while(1)
    {
        int c = fgetc(file);
        if(c == EOF) break;
        str = NULL;
        int len = 0;
        do
        {
            char *tmp = realloc(str, len + 2);
            if(!tmp)
            {
                free(str);
                return NULL;
            }
            str = tmp;
            str[len++] = (char)c;
            str[len] = '\0';
        } while(((c = fgetc(file)) !='\n') && (c != EOF));//read string char by char
        if(strcmp(str, removed_duty) != 0)//write sting in new list
        {
            updated_list = realloc(updated_list, sizeof(char*) * (count + 2));
            updated_list[count] = str;
            updated_list[++count] = NULL; 
        }
        else
        {
            free(str);
        }
    }
    return updated_list;
}