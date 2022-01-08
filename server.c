#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg)
{
  int newSocket = *((int *)arg);
  printf("new thread \n");
  recv(newSocket , client_message , 2000 , 0);
  char *registration = malloc(sizeof(client_message));
  strcpy(registration, client_message);
  printf("%s\n", registration);
  memset(client_message, 0, sizeof client_message);
  sleep(1);
  // registration
  if(strcmp(registration, "yes") == 0){
      printf("Registration: %s\n", registration);
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
        fp = fopen(filename, "w+");
        fputs(passwd, fp);
        printf("Registration sucessful.\n");
        fclose(fp);
        
      } 
      memset(&client_message, 0, sizeof (client_message));
      memset(&return_message, 0, sizeof (return_message));
      printf("Exit Registration \n");
      pthread_exit(NULL);
  }

  // login and chat
  else
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
      fgets(pwd, 50, fp);
      if(strcmp(pwd, passwd) == 0){
        passwd_ver = 1;
      }
    }
    sleep(1);

    char return_message[100];
    int if_logged = 0;
    if(user_ver == 1 && passwd_ver == 1){
      if_logged = 1;
      strcpy(return_message,"Login successful.");
      send(newSocket,return_message,sizeof(return_message),0);
      printf("Login successful.\n");
      FILE * fp;
      fp = fopen("users_online.txt", "a");
      fputs(username, fp);
      fputc('\n', fp);
      fclose(fp);
    }
    else{
      strcpy(return_message,"Wrong login or password.");
      send(newSocket,return_message,sizeof(return_message),0);
      printf("Login failed.\n");
      memset(&return_message, 0, sizeof (return_message));
      pthread_exit(NULL);
    }
    memset(&return_message, 0, sizeof (return_message));


    // add friends
    // load friends list (online friends only)
    // start conversation (in real time only)
    if(if_logged == 1){

    }
    int n;
    for(;;){
      n=recv(newSocket , client_message , 2000 , 0);
      printf("%s\n",client_message);
          if(n<1){
              break;
          }

      char *message = malloc(sizeof(client_message));
      strcpy(message,client_message);

      sleep(1);
      send(newSocket,message,sizeof(message),0);
      memset(&client_message, 0, sizeof (client_message));

      }
      printf("Exit socketThread \n");

      pthread_exit(NULL);
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