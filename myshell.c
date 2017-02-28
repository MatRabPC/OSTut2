/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

#define BUFFER_LEN 256

int main(int argc, char *argv[], char** envp)
{
   
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };
    char *result = NULL;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    if (argv[1] != NULL){ //Reads arguement instead of input
            arg[strlen(argv[1]) - 1] = '\0';
            FILE *fp;
            fp = fopen(argv[1], "r");

            if (fp == NULL){//if batchfile doesnt exist, output not valid arguements
            printf("Batchfile %s was not found\n", arg);
            exit(1);
            }
         else{ //File is open successfully 
                printf("File open success test");
                char line [1000];
                while(fgets(line,sizeof line, fp) != NULL){
                    system(line);
                    strcpy(buffer, line);
                    continue;
                }
            }
            fclose(fp);
        return EXIT_SUCCESS;
    }

    

    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {   
        if (buffer[strlen(buffer)-1] == '\n') {
            buffer[strlen(buffer)-1] = '\0';
        }
        
        result = strtok(buffer, " ");
        strcpy(command, result);
        result = strtok(NULL, " ");

        while( result != NULL ) 
        {   
            strcat(arg, result);
            strcat(arg, " ");
            result = strtok( NULL, " " );
        }

        // cd command -- change directory
        if (strcmp(command, "cd") == 0)
        {
            chdir(arg);
        }

        // clr command -- clear terminal
        else if (strcmp(command, "clr") == 0)
        {
            system("clear");
        }

        // dir command -- display contents of current directory
        else if (strcmp(command, "dir") == 0)
        {
            system("ls");
        }

        // environ command -- list environment strings
        else if (strcmp(command, "environ") == 0)
        {
            char** env;
            for (env = envp; *env != 0; env++)
            {
                char* thisEnv = *env;
                printf("%s\n", thisEnv);    
            }
        }

        // echo command -- print comment to terminal
        else if (strcmp(command, "echo") == 0)
        {
            printf("%s\n", arg);
        }

        // help command -- S.O.S
        else if (strcmp(command, "help") == 0)
        {
            system("man more");
        }

        // pause command -- wait a minute
        else if (strcmp(command, "pause") == 0)
        {
            scanf("%c",arg);
            //getchar();
        }


        // quit command -- exit the shell
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }

        // Unsupported command
        else
        {
            printf("Command: %s || Arg: %s\n", command, arg);
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }
        
        //clear arg
        memset(arg,0,strlen(arg));

    }
    return EXIT_SUCCESS;
}
