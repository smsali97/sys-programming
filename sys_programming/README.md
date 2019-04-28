# Client-Server Interactive Shell *by Sualeh Ali (ERP: 13005)*


## Introduction
This is a multi procesed and multiplexed interactive Client Server TCP Application that supports process management, client server messaging and maintenance of processes created.


## Choice of Architecture

![Systems Programming Diagram](https://rmlhbw.ch.files.1drv.com/y4mFQ5ezYRLosjRuJKlYvI4hlr1Kxgl1BAvMZEkVA1FocrAGxUy-4Jj8ntLJ3bDIkrP-mi_3NbG49JBjYyipJjlFNRlgrqmyClFBayepIej78ZB5SkW3cpzJkXHksSxitbzJY3iiqEn-9RDUut9iIJX6uypzu1KrjxViIlpX30EoGFMK6C1vYcpJVXXkdz9-gLWFwJ6D7kaWbpQaBrFri6EZA?width=1072&height=604&cropmode=none "Architecture Diagram")

The architecture that I have employed in this project is a multi-processed server with multiplexing I/O on both client and server-side. The reason I had to incorporate Multiplexing from both sides is because as you can see in the diagram, the server-side receives a combination of two parallel I/O input streams: one is the user interacting with the main-server (through stdin) and the other is the interaction with the client through the socket. To facilitate both interactions it is necessary for us to make sure the I/O can choose between two with which one is available.

Similarly, I have also decided to incorporate multiplexing on the client side as well. The reason being is that as you can see the client too can receve two different types of input: one which is sent by the stdin to the client which it forwards to the server and then gets the output and redirects that to the stdout. The second type of input is when the server sends a special command (for example, if the server wants to close the socket it can use the same socket to indicate a special type of input only addressed to the client and it can parse it and take the appropiate action accordingly (in this case, close the socket).

In conjuction with that, I also had to utilize some signalling mechanisms as a tool to indicate other processes of certain events. Particularly there were three cases where signals proved handy.
1. The first one being when I had to indiciate that a client process is being terminated so as part of the cleanup, close any running processes that it may have to avoid them being zombies.
2. Secondly was to use signals as a way to tell the client that if a child is terminated without using the command it can update its process table accordingly.
3. And lastly whenever the client tells the subserver to leave, it tells the main server through a signal that its leaving, and it should update its clients table as well.


The reason I decided to opt out of using multi-threading in the project was although a popular choice by many, I was as per the project spec able to use Multiplexing and some signalling mechanisms alone to achieve the project goals. While it is completely possible to do the same process with threads, the necessity for threads personally didn't substantiate much since there was no need of aspects like parallelism to be included in the project.


## Help
To view a list of commands type help on the client/server to view the list of available commands.
### Usage
```
./client <ip_address> <port_number> // Client
./server // Server
```

## Features

| Command                    | Description                                              | Type  |
|:--------------------------:|:--------------------------------------------------------:|:-----:|
| add [n1 n2 n3 n4]          | Adds list of integers                                |Client |
| sub [n1 n2 n3 n4           | Subtracts list of integers                           |Client |
| mul [n1 n2 n3 n4]          | Multiplies list of integers                          |Client |
| div [n1 n2 n3 n4]          | Divides list of integers                             |Client |
| run <process_name>         | Subtracts list of integers                           |Client |
| list [all]                 | Lists [all] the processes                            |Client |
| kill <process_name/pid/all>| Kills [all] of the processses specified by name/pid  |Client |
| exit                       | Exits the client                    |Client |
| help                       | Shows available command             | Both  |
| clist [all]                | Lists [all] the clients             | Server|
| print [ip address]         | prints message [to specified ip]    | Server|
| cexit [ip address]         | exits the client [of specified ip]  | Server|

## Limitations
Following are some of the limitations of my project
* The data structure I have employed for both the storing of the running processes and peers is a struct array, hence is fixed size in length. However, one advanage by doing that is I have fixated a maximum number of peers that will be allowed for a given server to avoid excessive load. This limitation can easily be overcome by using a dynamic data structure (for example, a linked list).
* At the moment, if the input provided to the client exceeds the buffer size, I ignore it and take a fresh new input. While this avoids buffer overfow attacks, another alternative approach would be to use a dynamic buffer size to vary depending upon the input given.
* The current project only works on IPv4 addresses. IPv6 support can be added on a later date.
* Another limitation of the project is that it assumes *sock_disconect* is a keyword that wont be used by the server and hence uses that as a communication tool to indicate a special message only intended for the client. 

## Special Stuff (For Bonus Marks!)
1. Pretty formatted the table and outputs to make it consistent and readable using ASCII style stuff
2. I have done some error handling for the arithmetic tasks (flags error for alphabetic characters in input, division by zero, etc)
3. Used advance signal management to send data *(aka sigqueues)* and polling *(Multiplexed I/O)* all of the stuff on my own ;)