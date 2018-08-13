# Project 1B - Compressed Network Communication
## INTRODUCTION:
When an application (e.g., shell) is to be used remotely, it is usually not sufficient to simply replace local device I/O (e.g., between the user's terminal and the shell) with network I/O. Remote sessions and network protocols add options and behaviors that did not exist when the application was being used locally. To handle this additional processing without making any changes to the application, it is common to create client-side and server-side agents that handle the network communication and shield the application from the complexities of the remote access protocols. Such intermediary agents can implement valuable features (e.g., encrypting traffic for enhanced security or compressing traffic to improve the performance and reduce the cost of WAN-scale communication), completely transparently to the user and application.

* In this project, you will build a multi-process telnet-like client and server. This project is a continuation of Project 1A. It can be broken up into two major steps:

* Passing input and output over a TCP socket
Compressing communication between the client and server