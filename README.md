# C++ Socket Programming: Simple Server and Client

server.c - multithreaded server 

server_single.c - singlethreaded server

client.c - client


## Socket Programming

This is pre-project tutorial for socket programming

If you are familiar to socket programming, then jump to Let’s Practice Part

### 1. What is Socket?

* With socket, two different processes can communicate each other
* Socket is nothing but a ```file```
* You can just imagine that two different processes have files (socket) and the they read received data from socket and write to socket for sending data to network
* So, socket has file descriptor, which is just an integer to identify opened file

![alt text](https://github.com/kusdavletov/C-Socket-Programming-Simple-Server-and-Client/blob/master/pics/scheme.png)

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

Interaction between server and client:
![alt text](https://github.com/kusdavletov/C-Socket-Programming-Simple-Server-and-Client/blob/master/pics/scheme1.png)

