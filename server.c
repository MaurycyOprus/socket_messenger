#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <math.h>
#include "if_exists.h"
#include "remove_line.h"
#include "get_second.h"
#include "remove_sock.h"


char client_message[2000];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg)
{
  int if_logged = 0;
  int newSocket = *((int *)arg);
  printf("New Connection\n");
  while(1)
  {
    recv(newSocket , client_message , 2000 , 0);
    char *option = malloc(sizeof(client_message));
    strcpy(option, client_message);
    printf("%s\n", option);
    memset(client_message, 0, sizeof client_message);
    sleep(1);
    // option
    if(strcmp(option, "registration") == 0){
        printf("Option: %s\n", option);
        // username
        recv(newSocket , client_message , 2000 , 0);
        printf("%s\n",client_message);
        char *username = malloc(sizeof(client_message));
        strcpy(username,client_message);
        memset(client_message, 0, sizeof client_message);
        sleep(1);

        // password
        recv(newSocket , client_message , 2000 , 0);
        printf("%s\n",client_message);
        char *passwd = malloc(sizeof(client_message));
        strcpy(passwd,client_message);
        sleep(1);
        char return_message[100];


        // save file with user data
        char filename[50];
        sprintf(filename, "%s.txt", username);
        printf("%s\n", filename);
        if(access(filename, F_OK) == 0){
          strcpy(return_message,"Username already taken.");
          send(newSocket,return_message,sizeof(return_message),0);
          printf("Username already taken.\n");
        }
        else{
          strcpy(return_message,"Registration Successful.");
          send(newSocket,return_message,sizeof(return_message),0);
          FILE * fp;
          fp = fopen(filename, "a");
          fputs(passwd, fp);
          printf("Registration sucessful.\n");
          fclose(fp);
          
        } 
        memset(&client_message, 0, sizeof (client_message));
        memset(&return_message, 0, sizeof (return_message));
    }

    
    // login and chat
    else if(strcmp(option, "login") == 0 && if_logged == 0)
    {
      printf("Login\n");
      // login

      // username
      recv(newSocket , client_message , 2000 , 0);
      printf("%s\n",client_message);
      char *username = malloc(sizeof(client_message));
      strcpy(username,client_message);

      // username verification
      char filename[50];
      int user_ver = 0;
      sprintf(filename, "%s.txt", username);
      if(access(filename, F_OK) == 0){
        printf("User exists.\n");
        user_ver = 1;
      }
      else{
        printf("User doesn't exist.\n");
      }
      memset(client_message, 0, sizeof client_message);
      sleep(1);

      // password
      recv(newSocket , client_message , 2000 , 0);
      printf("%s\n",client_message);
      char *passwd = malloc(sizeof(client_message));
      strcpy(passwd,client_message);
      memset(client_message, 0, sizeof client_message);

      // password verification
      int passwd_ver = 0;
      char pwd[50];
      if(user_ver == 1){
        FILE * fp;
        fp = fopen(filename, "r");
        fgets(pwd, strlen(passwd) + 1, fp);
        if(strcmp(pwd, passwd) == 0){
          printf("Correct password.\n");
          passwd_ver = 1;
        }
        else{
          printf("Wrong password.\n");
        }
      }
      sleep(1);

      char return_message[100];
      if(user_ver == 1 && passwd_ver == 1){
        
        if(search(username) != 1){
          if_logged = 1;
          strcpy(return_message,"Login successful.");
          send(newSocket,return_message,sizeof(return_message),0);
          printf("Login successful.\n");
          FILE * fp;
          fp = fopen("users_online.txt", "a");
          fputs(username, fp);
          fputc('\n', fp);
          fclose(fp);
          sleep(1);
          break;
        }
        else{
          strcpy(return_message,"User already logged in on other device.");
          send(newSocket,return_message,sizeof(return_message),0);
          printf("Multiple login tries.\n");

        }
      }
      else{
        strcpy(return_message,"Wrong login or password.");
        send(newSocket,return_message,sizeof(return_message),0);
        printf("Login failed.\n");
        memset(&return_message, 0, sizeof (return_message));
      }
      memset(&return_message, 0, sizeof (return_message));

    }
    else if(strcmp(option, "messages") == 0 && if_logged == 0)
    {
      printf("I'm in messages\n");
      char filename[50];
      sleep(1);
      char *username = malloc(sizeof(client_message));
      recv(newSocket , client_message , 2000 , 0);
      strcpy(username, client_message);
      sprintf(filename, "%s.txt", username);
      printf("%s\n", filename);
      FILE * fpp;
      fpp = fopen(filename, "a");
      fprintf(fpp, "\n%d", newSocket);
      fclose(fpp);
      break;
    }
    else{
      printf("User left app without logging in.\n");
      pthread_exit(NULL);
    }
  }
    // add friends
    // load friends list (online friends only)
    // start conversation (in real time only)
  if(if_logged == 1)
  {
    printf("User logged\n");
    for(;;){
      int n;
      n=recv(newSocket , client_message , 2000 , 0);
      printf("%s\n",client_message);
          if(n<1){
              break;
          }
      if(strcmp(client_message, "online") == 0)
      {
        int count_users = 0;
        FILE * fp;
        fp = fopen("users_online.txt", "r");
        while(!feof(fp)){
          char ch = fgetc(fp);
          if(ch == '\n'){
            count_users++;
          }
        }
        char *message = malloc(sizeof(client_message));
        printf("Users online: %d\n", count_users);
        sprintf(message, "%d", count_users);
        sleep(1);
        send(newSocket, message ,sizeof(message),0);
        memset(&client_message, 0, sizeof (client_message));

        // send online users
        char user[50];
        rewind(fp);
        for(int i = 0; i<count_users; i++){
          char *message_user = malloc(sizeof(client_message));
          fgets(user, 50, fp);
          strcpy(message_user, user);
          printf("Sending user: %s", user);
          sleep(1);
          send(newSocket,message_user,sizeof(message_user),0);
          memset(&message_user, 0, sizeof(message_user));
        }
        fclose(fp);
      }
      else if(strcmp(client_message, "send") == 0){
        printf("Message sending\n");
        char filename[50];
        int sock;
        int len1;
        int len2;
        char *message = malloc(sizeof(client_message));
        memset(&client_message, 0, sizeof (client_message));
        recv(newSocket , client_message , 2000 , 0);
        sleep(1);
        char *sender = malloc(sizeof(client_message));    //sender nick
        strcpy(sender, client_message);
        memset(&client_message, 0, sizeof (client_message));
        recv(newSocket , client_message , 2000 , 0);
        sleep(1);
        char *receiver = malloc(sizeof(client_message));  //receiver nick
        strcpy(receiver,client_message);
        sprintf(filename, "%s.txt", receiver);            //receiver's file
        memset(&client_message, 0, sizeof (client_message));
        recv(newSocket , client_message , 2000 , 0);
        sleep(1);
        char *text = malloc(sizeof(client_message));      //text
        strcpy(text,client_message);
        memset(&client_message, 0, sizeof (client_message));
        sock = second_line(filename);                 //receiver's socket
        printf("Receiver: %s, sock: %d, text: %s\n", receiver, sock, text);
        sleep(1);
        send(sock,sender,strlen(sender),0);           //sender's nick
        sleep(1);
        send(sock,text,sizeof(client_message),0);               //text

    }
      

      else if(strcmp(client_message, "exit") == 0)
      {
        char filename[50];
        memset(&client_message, 0, sizeof (client_message));
        recv(newSocket , client_message , 2000 , 0);
        char *username = malloc(sizeof(client_message));
        strcpy(username,client_message);
        removing("users_online.txt", username);
        printf("User logged out\n");
        sprintf(filename, "%s.txt", username);
        delete_line(filename);
        memset(&client_message, 0, sizeof (client_message));
        pthread_exit(NULL);
      }
      memset(&client_message, 0, sizeof (client_message));

  }
}
}
    

int main(){
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Create the socket. 
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);

  // Configure settings of the server address struct
  // Address family = Internet 
  serverAddr.sin_family = AF_INET;

  //Set port number, using htons function to use proper byte order 
  serverAddr.sin_port = htons(1100);

  //Set IP address to localhost 
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);


  //Set all bits of the padding field to 0 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Bind the address struct to the socket 
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  //Listen on the socket
  if(listen(serverSocket,50)==0)
      printf("Listening\n");
  else
    printf("Error\n");
    pthread_t thread_id;

    while(1)
    {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        if( pthread_create(&thread_id, NULL, socketThread, &newSocket) != 0 )
           printf("Failed to create thread\n");

        pthread_detach(thread_id);
        //pthread_join(thread_id,NULL);
    }
  return 0;
}