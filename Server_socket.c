
// The nodes are divided into two types, 

// server node and client node.

// The client node sends the connection signal and the server node receives the connection signal sent by the client node.
// The connection between a server and client node is established using the socket over the transport layer of the internet.
// After a connection has been established, the client and server nodes can share information between them using the read and write commands.
// After sharing of information is done, the nodes terminate the connection.
// We will look deeply at how the above process can be performed using socket programming in C in the following sections.

// Stages for Server
// Different stages must be performed on the server node to receive a connection sent by the client node. 
// These stages are discussed elaborately in this section.

// A. Socket Creation
// The first stage deals with the creation of a socket, which is the basic component for sending or receiving signals between nodes. The sys/socket.h header has the necessary functions to create a socket in C. In socket programming in C, a socket can be created by the socket() function with syntax,

// int socket(int domain, int type, int protocol);

// The domain represents the address family over which the communication will be performed. The domain is pre-fixed values present in the sys/socket.h header. Some domains are,

// AF_LOCAL or AF_UNIX is used for local communication or in the case where the client and server are on the same node. These sockets are called UNIX domain sockets.
// AF_INET is used to represent the IPv4 address of the client to which a connection should be made. Similarly AF_INET6 is used for IPv6 addresses. These sockets are called internet domain sockets.
// AF_BLUETOOTH is used for low-level Bluetooth connection.

// The type represents the type of communication used in the socket. Some mostly used types of communication are,

// SOCK_STREAM uses the TCP(Transmission Control Protocol) to establish a connection. This type provides a reliable byte stream of data flow and is a connection-based protocol. These sockets are called stream sockets.
// SOCK_DGRAM uses the UDP(User Datagram Protocol) which is unreliable and a connectionless protocol. These sockets are also called datagram sockets.

// The protocol represents the protocol used in the socket. This is represented by a number. When there is only one protocol in the protocol family, the protocol number will be 0, or else the specific number for the protocol has to be specified.
// The socket() function creates a socket and returns a file descriptor which represents an open file that will be utilized by the socket in reading and writing operations and the file descriptor is used to represent the socket in later stages. In case of an error in creating the socket, -1 is returned by the socket() function.

// B. Setsockopt

// The setsockopt() function in socket programming in C is used to specify some options for the socket to control the behavior of the socket. The syntax is,

// int setsockopt(int socket_descriptor, int level, int option_name, const void *value_of_option, socklen_t option_length);

// The socket is the file descriptor returned by the socket() function.
// The level parameter represents the level at which the option for the socket must be applied. The SOL_SOCKET represents the socket level and IPPROTO_TCP represents the TCP level.
// The option_name specifies the rules or options that should be modified for the socket. Some useful options are,

// SO_DEBUG is used to enable the recording of debugging information.
// SO_REUSEADDR is used to enable the reusing of local addresses in the bind() function.
// SO_SNDBUF is used to set the maximum buffer size that can be sent using the socket connection.

// The option_value is used to specify the value for the options set in the option_name parameter.
// The option_length is the length of the variable used to set the option value.
// The function returns a value of 0 of data type int on the successful application of the option and a value of -1 on failure.

// C. Bind

// The bind() function in socket programming in C is used to assign an address to a socket created using the socket() function. The syntax of bind() function is,

// int bind(int socket_descriptor , const struct sockaddr *address, socklen_t length_of_address);

// The socket_descriptor is the value of the file descriptor returned by the socket() function.
// The address is a structure of type sockaddr. We usually use a structure of type sockaddr_in to represent this information, because information such as port and address can only be stored in this structure. The sockaddr_in is cast to the sockaddr data type when calling the bind() function.
// The length_of_address represents the size of the address passed as the second parameter.
// The function returns 0 on binding the address and port successfully or returns -1 on failure.

// D. Listen

// The listen() function in socket programming is used to make the server node wait and listen for connections from the client node on the port and address specified by the bind() function. The syntax is,

// int listen(int socket_descriptor, int back_log);

// The socket_descriptor represents the value of the file descriptor returned by the socket() function.
// The back_log marks the maximum number of connection requests that can be made to the server by client nodes at a time. 
// The number of requests made after the number specified by back_log may cause an error or will be ignored by the server if the options for retransmission are set.
// The function returns 0 on listening on the address and port specified or returns -1 on failure.

// E. Accept

// The accept() function is used to establish a connection between the server and the client nodes for the transfer of data. The syntax is,

// int accept(int socket_descriptor, struct sockaddr *restrict address, socklen_t *restrict length_of_address);

// The socket_descriptor represents the value of the file descriptor returned by the socket() function.
// The address is the variable of the sockaddr_in structure in which the address of the socket returned from the function will be stored.
// The length_of_address depicts the size of the address parameter.
// The accept() function creates a new socket from the first connection request for the specified socket_descriptor and returns the file descriptor of the new socket. 
// The file descriptor of this new socket is used in the read() and write() functions to send and receive data to and from the client node.

// Stages for Client

// The client-side sends the connection requests to the server-side. To perform these several stages have to be performed on the client side,

// A. Socket Connection

// Similar to the server-side, the client-side also needs to create a socket using the socket() function and bind the socket to an address using the bind() function. 
// This will create a socket that can send the connection request to the server.

// B. Connect

// The connect() function is used to send the connection request and connect to the server node. The syntax of the function is,

// int connect(int socket_descriptor, const struct sockaddr *address, socklen_t length_of_address);

// The socket_descriptor represents the value of the file descriptor returned by the socket() function during the creation of a socket on the client-side.
// The address represents the structure with the information of the address and port number of the server node to which the connection is to be made.
// The length_of_address is the size of the address structure used in the second parameter.
// The connect() function returns a value of 0 on successfully connecting with the server and returns a value of -1 on error or the connection fails.

// Implementation
// Let us see the implementation of socket programming using the C programming language.

// Server.c
// In the server node, a socket must be created using the socket() function, the options for the socket are set using the setsockopt() function, 
// and the socket is bound to an address and port using the bind() function, the socket is made to listen on the bound port and address using the listen() function
// and the connection request from the client is accepted using the accept() function. The main feature of server-side code is to prepare the node 
// to listen for connection requests from the client node and establish a connection with the client node. The code used to implement the server node is,


#include <sys/types.h>// provides required data types
#include <sys/socket.h>// holds address family and socket functions
#include <netinet/in.h>// has the sockaddr_in structure
#include <fcntl.h>// has functions for read and write operations [system call]
#include <stdio.h>// basic C header
#include <string.h>// header to help with strings
#include <stdlib.h>// has macros such as EXIT_FAILURE

#define CONNECTION_PORT 3500 // port through which connection is to be made

int main(){ 
    int socket_descriptor;// server socket
    int client_socket;// socket created by the bind function
    char storage_buffer[800];// buffer to store message
    int length_of_address;
    int bytes_read;
    int option_value=1;// option value for respective option_name
    struct sockaddr_in server_address;// server and client address structures
    struct sockaddr_in connection_address;
    char* message = "This is a message from the server";

    socket_descriptor = socket(AF_INET,SOCK_STREAM,0);// create an Ipv4 and TCP socket
    // Check whether the socket is created successfully
    if(socket_descriptor<0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }  
    
   
    // set options for the socket
    int status=setsockopt(socket_descriptor, SOL_SOCKET,SO_REUSEADDR , &option_value,sizeof(option_value));
    // check if options are set successfully
    if(status<0){
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;//  initializing structure elements for address
    server_address.sin_port = htons(CONNECTION_PORT);// convert port to network byte order using htons
    server_address.sin_addr.s_addr = INADDR_ANY;// any address available
    server_address.sin_zero[8]='\0';// Assigning null character to the last index of the character array
    
    // bind the socket with the values address and port from the sockaddr_in structure
    status=bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
    // check if the binding was successful
    if(status<0){
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }
    // listen on specified port with a maximum of 4 requests
    status=listen(socket_descriptor,4);
    // check if the socket is listening successfully
    if(status<0){
        perror("Couldn't listen for connections");
        exit(EXIT_FAILURE);
    }
    length_of_address = sizeof(connection_address);
    // accept connection signals from the client
    client_socket = accept(socket_descriptor, (struct sockaddr*)&connection_address, &length_of_address);
    // check if the server is accepting the signals from the client
    if(client_socket<0){
        perror("Couldn't establish connection with client");
        exit(EXIT_FAILURE);
    }

    while (1){
        // Receive data sent by the client
        memset(storage_buffer,0,sizeof(storage_buffer));
        bytes_read = read(client_socket, storage_buffer,sizeof(storage_buffer));
        storage_buffer[bytes_read] = '\0';// set the last index of the character array as a null character
        printf("Message from client: %s \n",storage_buffer);
        // send(client_socket, message, strlen(message), 0);// Send data to the client
    }

    close(socket_descriptor);// Close all the sockets created 
    close(client_socket);
    return 0;

}


// In the above example, a socket is created and made ready to listen for connection signals from the client.

// The socket uses the Ipv4 address family and is of type SOCK_STREAM which indicates the use of Transmission Control protocol.
// The option SO_REUSEADDR is specified in the arguments. This option is a boolean option and the option_value of 1 depicts that this option is turned on.
// The sockaddr_in structure has four data elements to represent the address family, port, internet address, and an additional space that is used to match the size of the sockaddr structure when performing casting operation. The structure of sockaddr_in is given below,

// struct sockaddr_in { 
//     short sin_family; // address family or domain
//     unsigned short sin_port; // port
//     struct in_addr sin_addr; // Structure for Address
//     char sin_zero[8]; //  padding for casting operations
// }; 

// The structure in_addr present inside the sockaddr_in structure is,

// struct in_addr {
//     unsigned long s_addr; // internet address value
// };

// A data element inside the structure can be accessed by the . operator and values are assigned to the elements.

// The port number is passed to the htons() function which converts the port number stored in memory to network byte order.

// The INADDR_ANY represents that any address can be used for binding. It generally indicates the address of 0.0.0.0. Other than INADDR_ANY, some useful addresses are,

// INADDR_LOOPBACK which refers to the localhost of the node. The address is 127.0.0.1.
// INADDR_BROADCAST is similar to INADDR_ANY but has the address 255.255.255.255. It is used for broadcast communication.
// The struct sockaddr_in structure is cast to type struct sockaddr to match the syntax of the function. The sizeof() function is used to find the size of struct sockaddr.

// The listen() function puts the socket in a listening state and a back_log of 4 states that a maximum of 4 connection requests can be made at a time to the server.

// The accept() function establishes a connection between server and client and returns a new file descriptor.

// The read() and write() functions are used to send and receive between client and server. The syntax of read() function is,

// ssize_t read(int file_descriptor, void *buffer, size_t size);

// The data of specified size is read and stored in the buffer. The syntax of the write function is,

// ssize_t write(int file_descriptor, const void *buffer, size_t size);

// The data of specified size is written from the buffer. On successful read or write, the read() and write() functions return the number of bytes of data that has been read or written or return the value of -1 on failure.

// The close() function is used to terminate the socket connection.

// A network byte order is an order in which the most significant bits are stored first.