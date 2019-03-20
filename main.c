#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>


void store_string(char line[])   //storing the user input into string function
{
    fgets(line,50,stdin);    //using fgets built in function with max characters 100 and input from command
    line[strlen(line)-1]='\0';   // replacing last character from \n to \0 "c string"
}
void split_string(char* args[],char line[])  // spliting string function
{
    char copy[50];              // define copy to work with because strtoken alters the main line
    strcpy(copy,line);
    int i=0 ;
    char delim[] =" ";
    args[i] = strtok(copy,delim);   // common strtok function to split
    while(args[i]){
    i++;
    args[i]=strtok(NULL,delim);
    }
}
void special_conditions(char* args[])
{
    if(args[0]){
   if(strcmp(args[0],"exit") == 0)
    {
        exit_handler();           //exit handle function

    }else if(strcmp(args[0],"cd") == 0)
    {
        int error = chdir(args[1]);       //declaring error as retrun variable to recive error
        if (error){ fprintf(stderr, "error: %s\n", strerror(errno));}  //if error exists output error to user using errno library
    }
}
}

void exit_handler(){

    printf("exiting terminal");
    exit(0);
}

void sig_handler(int signal) {
    logfile_append();                   //signal handler executes appending file function
}
void logfile_append()
{                                           //declare file type and name
    FILE *logfile;
    logfile = fopen("logfile.log", "a");        //open file and "a" means ability to edit and append
    fprintf(logfile, "Child process was terminated \n");
    fclose(logfile);

}

int background_process(char* args[]) {          //back ground process handler
if (args[0]){
    int i;
    for(i = 0; args[i] != NULL; i++) ;          //for loop to reach end of line at i euals null
    if(args[i-1] != NULL && strcmp(args[i-1],"&")==0) {    // check for the argument before null whic is last argument
    args[i-1] = NULL;                                      // remove the & from the args before passing it to execvp
    return 0;
  } else {
    return 1;
  }
}
}
int main()
{
    char* args[10];             //main variables declaration
    char line[50];
    pid_t id;
    int flag;
    sigset(SIGCHLD,sig_handler);        //signal handler declaration

while(1)
{
    store_string(line);
    split_string(args,line);
    special_conditions(args);
    flag = background_process(args);

    id = fork();            //start the forking process
    if (id==0){             //working in child process
    execvp(args[0],args);

    }else if(id > 0){       //working in parent process
    if(flag){
    waitpid(id,0);}
    }else{
        perror("Error ");
    }
  }
    return 0 ;

}
