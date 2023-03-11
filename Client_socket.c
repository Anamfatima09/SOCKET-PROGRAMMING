// On the client-side, a socket is created by the socket() function and bound to a port and address 
// using the bind() function. The client can send a connection request to the server node 
// using the connect() function. The client-side code features sending a connection request 
// to the server node. The code used to implement the client node is,



#include <sys/types.h>// provides required data types
#include <sys/socket.h>// holds address family and socket functions
#include <netinet/in.h>// has the sockaddr_in structure
#include <fcntl.h>// has functions for read and write operations
#include <stdio.h>// basic C header
#include <string.h>// header to help with strings
#include<stdlib.h>// has macros such as EXIT_FAILURE

#define CONNECTION_PORT 3500 // port through which connection is to be made

int main(){

    struct sockaddr_in server_address;// structure to represent the address
    char message[100];// Message to be sent to the server
    char recieve_buffer[100];// storage buffer to receive messages
    int socket_descriptor = socket(AF_INET,SOCK_STREAM,0);// create an Ipv4 and TCP socket
    // Check whether the socket is created successfully
    if(socket_descriptor<0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }  
    int status=0;
    // initialize address structure for binding
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
    // set address to any address available
    server_address.sin_addr.s_addr =INADDR_ANY;
    server_address.sin_zero[8]='\0';
    // connect to the server
    status=connect(socket_descriptor,(struct sockaddr*)&server_address, sizeof(server_address));
    if(status<0){
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
   }    
    while(1){
        memset(message,0,sizeof(message));
        printf("Please Write What you want to Share\n");
        scanf("%s", message);
        printf("What we are sending from the Client %s\n",message);
        write(socket_descriptor, message, strlen(message));// send message to the server
        // read(socket_descriptor, recieve_buffer, 100);//receive a message from the server
        // printf("Message from server: %s\n", recieve_buffer);
    }

    close(socket_descriptor);// terminate the socket connection
    return 0;
}

// The creation of the socket is similar to the server-side and the server and client node 
// are connected using the connect() function. Then the message is sent to the client using 
// the write() function and messages can be received using the read() function.