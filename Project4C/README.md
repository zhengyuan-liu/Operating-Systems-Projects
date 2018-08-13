# Project 4C Internet Of Things Security
## INTRODUCTION:
The Internet Of Things is populated with an ever expanding range of sensors and appliances. Initially such devices were likely to be connected to monitoring and control devices over purely local/personal networks (e.g., infra-red, Bluetooth, ZigBee), but it is increasingly common to connect such devices (directly or via a bridge) to the Internet. This enables remote monitoring and control, but it also exposes them to a variety of remote attacks.

For some targets (e.g., a national power grid or uranium separation centrifuges) their strategic importance and need for protection should be clear. It might not be immediatly obvious how one might hijack simple devices (e.g., light switches or temperature/humidity sensors) for nefarious purposes, but:

* there have been numerous instances where web-cams have been hijacked to violate peoples' privacy.
* smart devices like routers, baby-monitors, washing machines, and even lightbulbs have been conscripted into botnets to mount Distributed Denial of Service attacks and to be used for other bad purposes.
* security researchers have been able to hijack the digital controls of recent cars.
* consider the havoc that could be wrought by someone who was able to seize control of a networked pacemaker or insulin pump.
Attackers have proven innovative and resourceful in making use of compromised devices of many kinds, so even if you do not see any obvious dangers, prudence suggests that greater care be taken with the security of IOT devices. In particular, all communications and control for IOT devices should be encrypted and authenticated.
In this project we will extend your embedded temperature sensor to accept commands from, and send reports back to, a network server. You will do this over both unencrypted and encrypted channels.