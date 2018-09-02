# C Socket Programming: Simple Server and Client

`server.c` - multithreaded server 

`server_single.c` - singlethreaded server

`client.c` - client

## Socket Programming

This is pre-project tutorial for socket programming

If you are familiar to socket programming, then jump to Let’s Practice Part

### 1. What is Socket?

* With socket, two different processes can communicate each other
* Socket is nothing but a ```file```
* You can just imagine that two different processes have files (socket) and the they read received data from socket and write to socket for sending data to network
* So, socket has file descriptor, which is just an integer to identify opened file

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955905-363dae80-aef6-11e8-9719-ac759adbdfaa.png"/>
</p>

### 2. Socket Types

There are two commonly used socket types ```Stream Sockets``` and ```Datagram Sockets```. Stream sockets uses TCP for data transmission, and Datagram sockets uses UDP.

### 3. Client Process & Server Process

#### Client : Typically request to server for information.

* Create a socket with the ```socket()``` system call
* Connect socket to the address of the server using the ```connect()``` system call
* Send and receive data. There are a number of ways to do this, but the simplest way is to use the ```read()``` and ```write()``` system calls

#### Server : Takes request from the clients, perform required processing, and send it to the client

* Create a socket with the ```socket()``` system call
* Bind the socket to an address (IP + port) using the ```bind()``` system call.
* Listen for connections with the ```listen()``` system call
* Accept a connection with the ```accept()``` system call. This call typically blocks the connection until a client connects with the server
* Send and receive data using the ```read()``` and ```write()``` system calls

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955906-363dae80-aef6-11e8-9795-161a90f30b1e.png"/>
</p>

<p align="center">Interaction between server and client</p>

### Preliminary knowledge before programming

## 1. Structures

You will use socket functions, and most of the socket functions use socket address structures.

- `sockaddr` : generic socket address structure

```objectivec
struct sockaddr {
    // represents an address family, most of cases AF_INET)
    unsigned short     sa_family;
    
    // 14 bytes of protocol specific address, for the internet
    // family, port number IP address (sockaddr_in) is used
    char               sa_data[14]; 
}
```

- `sockaddr_in` : one type of sockaddr, it represents port number IP address

```objectivec
struct sockaddr_in {
    short int              sin_family;   // AF_INET
    unsigned short int     sin_port;     // 16-bit port number
    struct in_addr         sin_addr;     // 32-bit IP address
    unsigned char          sin_zero[8];  
}
```

- `in_addr` : structure used in abote sockaddr_in

```objectivec
struct in_addr {
    unsigned long s_addr;
}
```

- `hostent` : contains information related to host

```objectivec
struct hostent {
    char *h_name;       // e.g. unist.ac.kr
    char **h_aliases;   // list of host name alias
    int h_addrtype;     // AF_INET
    int h_length;       // length of ip address 
    char **h_addr_list; // points to structure in_addr
    #define h_addr h_addr_list[0]
};
```

## 2. Network Byte Orders

All computers doesn’t store bytes in same order. → Two different ways

- Little Endian : low-order byte is stored on the starting addresses
- Bit Endian : high-order byte is stored on the starting address

→ To make machines with different byte order communicate each other, Internet protocol specify a canonical byte order convention for data transmitted over the network. This is called Network Byte Order.

`sin_port` `sin_addr` of `sockaddr_in` should be set with this Network Byte Order.

```objectivec
htons() : Host to Network Short
htonl() : Host to Network Long
ntohl() : Network to Host Long
ntohs() : Network to Host Short
```

## 3. IP Address Function

These functions manipulate IP addresses between ASCII strings and network byte ordered binary values.

- int **inet_aton**(const char *strptr, struct in_addr *addrptr)

```objectivec
#include <arpa/inet.h>
int retval;
struct in_addr addrptr
memset(&addrptr, '\0', sizeof(addrptr));
retval = inet_aton("68.178.157.132", &addrptr);
```

- in_addr_t **inet_addr**(const char *strptr)

```objectivec
#include <arpa/inet.h>
struct sockaddr_in dest;
memset(&dest, '\0', sizeof(dest));
dest.sin_addr.s_addr = inet_addr("68.178.157.132");
```

char ***inet_ntoa**(struct in_addr inaddr)

```objectivec
#include <arpa/inet.h>
char *ip;
ip = inet_ntoa(dest.sin_addr);
printf("IP Address is : %s\n", ip);
```

## 4. Socket Functions

(you can use bold scripted parameter for the first use)

1. `socket`

```objectivec
#include <sys/types.h>
#include <sys/socket.h>
int socket (int family, int type, int protocol);
```

- family : AF_INET, AF_INET6, AF_LOCAL, AF_ROUTE, AF_KEY
- type : SOCK_STREAM (TCP), SOCK_DGRAM (UDP), SOCK_SEQPACKET, SOCK_RAW
- protocol : IPPROTO_TCP, IPPROTO_UDP, IPPROTO_SCTP, (0 : system default)

→ This function returns socket descriptor, so you can use it for another functions

2. `connect`

```objectivec
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
```

- sockfd : socket descriptor returned by the socket function
- serv_addr : sockaddr that contains destination IP address and port
- addrlen : set it to sizeof(struct sockaddr)

3. `bind`

```objectivec
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *my_addr,int addrlen);
```

- my_addr : sockaddr that contains local IP address and port

4. `listen`

```objectivec
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd,int backlog);
```

- converts unconnected socket to passive socket (kernel should accept incoming connection request directed to this socket)
- **backlog** : maximum number of connections the kernel should queue for this socket

5. `accept`

```objectivec
#include <sys/types.h>
#include <sys/socket.h>

int accept (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
```

- returns the next completed connection from the front of the completed connection queue
- cliaddr : sockaddr struct that contains client IP address and port
- **addrlen : set it to sizeof (struct sockaddr)**

5. `send`

```objectivec
int send(int sockfd, const void *msg, int len, int flags);
```

6. `recv`

```objectivec
int recv(int sockfd, void *buf, int len, unsigned int flags);
```
- buf : buffer to read the information into
- len : It is the maximum length of the buffer
- flags : set it to 0

7. for UDP connection

```objectivec
int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);
int recvfrom(int sockfd, void *buf, int len, unsigned int flags struct sockaddr *from, int *fromlen);
```
- `sendto` and `recvfrom` functions are used instead of `send` `recv`

8. `close`

```objectivec
int close( int sockfd );
```

- close communication

### 5. Additional Functions

1. `fork`

- creates new process which is exact copy of current process
- current process is parent, and copied process is child

```objectivec
#include <sys/types.h>
#include <unistd.h>

int fork(void);
```

- fork returns 0 when it is child process, and returns child process id when it is parent process. If it is failed, returns -1

2. `bzero`

- place *n*bytes of null byte to string s

```objectivec
void bzero(void *s, int nbyte);
```

3. `bcmp`

- compare nbytes of byte string s1 and byte string s2

```objectivec
int bcmp(const void *s1, const void *s2, int nbyte);
```

returns 0 if identical, 1 otherwise.

4. `bcopy`

- copy nbytes of byte string s1 to byte string s2

```objectivec
void bcopy(const void *s1, void *s2, int nbyte);
```

5. `memset`

- allocate memory and return pointer which points to the newly allocated memory

```objectivec
void *memset(void *s, int c, int nbyte);
```

- s : source to be set
- c : character to set on nbyte places
- nbyte : number of bytes

## 6. Let’s Practice : echo server and client

- You have to implement connecting server and client,
and your server and client should quit when the client sends “quit”
you can compile code with command line in the directory of project :

```
> make
```

- run server and client on server at the same time
- with two different terminal windows

```
> ./client
> ./server
```

- Sample Output

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955907-36d64500-aef6-11e8-886e-1fcf77b377c4.png"/>
</p>

<p align="center">client</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955908-36d64500-aef6-11e8-9888-ab63856ad2d4.png"/>
</p>

<p align="center">server</p>

## 7. Practice more! : multi user server

- There can be multiple clients which tries to connect to server simultaneously
- Current echo server doesn’t accept new connection if it is already accepted
- You have to change echo server. Using fork() function, each connection should be run concurrently.
- Hint (pseudo code of multi user server) :

```
listen()
while (1) {
    newsockfd = accept();
    pid = fork();
    if (pid == 0) { // client process
        close(sockfd);
        // do some process - read and write
        exit(0);
    } else { // parent process
        close(newsockfd);
    }
}
```

- Test : you can just open three terminal windows, run one server and two clients
- test order : 

```
> run one server and two clients
> client1 sends “hello1”
> client2 sends “hello2”
> client2 sends “world2”
> client1 sends “world1”
> client1 sends “quit” and client2 sends “quit”
```

Output :

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955909-36d64500-aef6-11e8-9345-1033eb29599c.png"/>
</p>

<p align="center">client1</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955910-36d64500-aef6-11e8-8f04-3a7a2deb1b6f.png"/>
</p>

<p align="center">client2</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/19291492/44955911-376edb80-aef6-11e8-929e-56667b372253.png"/>
</p>

<p align="center">multi user server</p>
