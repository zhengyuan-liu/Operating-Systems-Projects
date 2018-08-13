# Project 4C - Internet Of Things Security
## INTRODUCTION:
The Internet Of Things is populated with an ever expanding range of sensors and appliances. Initially such devices were likely to be connected to monitoring and control devices over purely local/personal networks (e.g., infra-red, Bluetooth, ZigBee), but it is increasingly common to connect such devices (directly or via a bridge) to the Internet. This enables remote monitoring and control, but it also exposes them to a variety of remote attacks.

For some targets (e.g., a national power grid or uranium separation centrifuges) their strategic importance and need for protection should be clear. It might not be immediatly obvious how one might hijack simple devices (e.g., light switches or temperature/humidity sensors) for nefarious purposes, but:

* there have been numerous instances where web-cams have been hijacked to violate peoples' privacy.
* smart devices like routers, baby-monitors, washing machines, and even lightbulbs have been conscripted into botnets to mount Distributed Denial of Service attacks and to be used for other bad purposes.
* security researchers have been able to hijack the digital controls of recent cars.
* consider the havoc that could be wrought by someone who was able to seize control of a networked pacemaker or insulin pump.

Attackers have proven innovative and resourceful in making use of compromised devices of many kinds, so even if you do not see any obvious dangers, prudence suggests that greater care be taken with the security of IOT devices. In particular, all communications and control for IOT devices should be encrypted and authenticated.

In this project we will extend your embedded temperature sensor to accept commands from, and send reports back to, a network server. You will do this over both unencrypted and encrypted channels.

## PROJECT DESCRIPTION:
### Part 1: Communication with a Logging Server
Write a program (called lab4c_tcp) that:

* builds and runs on your Beaglebone.
* is based on the temperature sensor application you built previously (including the --period=, --scale= and --log= options).
* accepts the following (mandatory) new parameters:
  * --id=9-digit-number
  * --host=name or address
  * --log=filename
  * port number
    Note that there is no `--port=` in front of the port number. This is a non-switch parameter.
* accepts the same commands and generates the same reports as the previous Beaglebone project, but now the input and output are from/to a network connection to a server.
 1. open a TCP connection to the server at the specified address and port
 2. immediately send (and log) an ID terminated with a newline:
ID=ID-number 
This new report enables the server to keep track of which devices it has received reports from.
 3. as before, send (and log) newline terminated temperature reports over the connection
 4. as before, process (and log) newline-terminated commands received over the connection
If your temperature reports are mis-formatted, the server will return a LOG command with a description of the error.
Having logged these commands will help you find and fix any problems with your reports.
 5. as before, the last command sent by the server will be an OFF.
Unlike the previous project, the button will not be used as an alternative manual shutdown mechanism.
Do not accept commands from standard input, or send received commands or generated reports to standard output.

As before, assume that the temperature sensor has been connected to Analog input 0.

### Part 2: Authenticated TLS Session Encryption
Write a program (called lab4c_tls) that:

builds and runs on your Beaglebone
is based on the remote logging appliance build in part 1
operates by:
opening a TLS connection to the server at the specified address and port
sending (and logging) your student ID followed by a newline
sending (and logging) temperature reports over the connection
processing (and logging) commands received over the connection
