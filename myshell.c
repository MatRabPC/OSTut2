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

int sheller(char buffer[], char** envp); //the shell command function


int main(int argc, char *argv[], char** envp)
{


    if (argv[1] != NULL){
            FILE *fp;
            fp = fopen(argv[1], "r");

            if (fp == NULL){//if batchfile doesnt exist, output not valid arguements
            printf("Batchfile %s was not found\n", argv[1]);
            exit(1);
            }
            else{ //File is open successfully 
              //  printf("File open success test\n");
                char line [BUFFER_LEN];
                while(fgets(line,sizeof line, fp) != NULL){
                    
                    sheller(line, envp);} //PS, we pass envp over so that sheller may access it
                }
            
            fclose(fp);
            exit(1);
            return EXIT_SUCCESS; //fallback
        }

    char buffer[BUFFER_LEN] = { 0 };
    char cwd[1024];
    
    printf("\033[1m%s\033[0m$ ", getcwd(cwd, sizeof(cwd))); //initial line prompt directory print
   
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL) //while we still get input. Else, core dump
    {   
       
        
        sheller(buffer, envp); //call the sheller function (the shell part)
        printf("\033[1m%s\033[0m$ ", getcwd(cwd, sizeof(cwd)));
        //if we're running from the batchfile, printing the command prompt directory line looks ugly. so just print here
    }
    exit(42); //exit otherwise
}


int sheller(char buffer[], char** envp)
{
        char command[BUFFER_LEN] = { 0 };
        char arg[BUFFER_LEN] = { 0 };
        char *result = NULL;
        char cwd[1024];
        char man[BUFFER_LEN] = "man "; //for the manual. I think the pdf was refering to the man manual, forgive me if I'm wrong 
        getcwd(cwd, sizeof(cwd));
        //initialise our stuff
        if (buffer[strlen(buffer)-1] == '\n') {
                    buffer[strlen(buffer)-1] = '\0';
                } //replace trailing enter with terminator character
        result = strtok(buffer, " ");
        strcpy(command, result);
        result = strtok(NULL, " ");

        while( result != NULL ) 
        {   
            strcat(arg, result);
            strcat(arg, " ");
            result = strtok( NULL, " " );
        }
        arg[strlen(arg) - 1] = '\0';
        //tokenize, push argument to arg, include spaces (apparently, it seemed fair enough to, but unnecessary as well), and remove trailing space


        // cd command -- change directory
        if (strcmp(command, "cd") == 0)
        {
            if(strlen(arg) == 0) {
                printf( "Current Directory: %s\n", getcwd(cwd, sizeof(cwd)));
            } //if no argument

            else if( chdir( arg ) == 0 ) { //if success
                setenv("PWD", getcwd(cwd, sizeof(cwd)), 1);
            }
            else //if failure
            {
                printf( "Could not locate directory %s\n", arg);
            }
        }

        // clr command -- clear terminal
        else if (strcmp(command, "clr") == 0)
        {
            system("clear"); //POSIX function
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
                printf("%s\n", thisEnv);    //print our enviro vars. Not quite sure what formatting was wanted, though
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
            strcpy(man, "man ");
            strcat(man, arg);
            system(man); //use LINUX command 'man'
        }

        // pause command -- wait a minute
        else if (strcmp(command, "pause") == 0)
        {
            fgets(buffer, BUFFER_LEN, stdin); //waits for input. Cleanest way to accomplish without losing chars
            //scanf("%s",cwd);
            //getchar();
        }

        // quit command -- exit the shell
        else if (strcmp(command, "quit") == 0)
        {
            exit(1); 
        }

        // Unsupported command
        else
        {
            printf("Command: %s || Arg: %s\n", command, arg);
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }
        
        //clear arg
        memset(arg,0,strlen(arg));
        return 1;
}
//FIN
