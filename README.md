pdcs - "Packed Data Client Server" - TCP client and server project
==================================================================

pd_client
----------------
pd_client is a shared library that can be used to build a client program that allows you to connect to the pd_server.
The client can send new events to the server or download existing from the server.


pd_server
----------------
It is a server that collects data about events sent by clients.
The event consists of the following information:
<pre>
  - timestamp        Time of occurrence of the event
  - priority         lowermost, low, medium, high, highest
  - information      Text information
</pre>
The events are saved in the server database.
If a new event appears, it will be sent to all clients connected to the server.


qpd_client
----------------
This is the client program for connecting to the pd_server with the Graphical User Interface.
It is an example of using the pd_client library.
The Qt library was used to build the GUI of the program.


Data exchange protocol between client and server
------------------------------------------------
<pre>
Data sent to the server:
-------------------------------------------------------------
Data format                    Description
-------------------------------------------------------------
LOGIN|user_name|password       Login
NEW|priority|info    (*)       New event
ALL                            Download all events
LAST                           Download last event

Data sent to the client:
-------------------------------------------------------------
Data format                    Description
-------------------------------------------------------------
LOGIN|result                   The result of logging in
NEW|timestamp|priority|info    New event
ALL|timestamp|priority|info    All events
LAST|timestamp|priority|info   Last event

(*) Event timestamp is set by the server.
</pre>
Data transferred between the client and the server are encrypted with the AES 128 (ECB mode) algorithm.
The encryption key is the password of the user logging into the server.
It is a simple solution, but in this case the server does not have to store additional encryption keys.


Data frame construction
---------------------------------------------
The data frame consists of two blocks:
1 - header - size of transmitted data in text form and takes 4 bytes.
2 - body - transmitted data that is encrypted using the AES 128 algorithm - maximum size: 512 bytes.
For example:
<pre>  '  15xxxxxxxxxxxxxxx' </pre>


Building
--------
To build all projects, run:
./build

To clean all projects, run:
./clean

To install all projects, run as root:
./install

To uninstall all projects, run as root:
./uninstall

Note: 
Installation can be done only after building the project.

Comments
--------
Project was created and tested in the Arch Linux operating system.
Project uses the library: stdlib, boost, crypto++, Qt. That's why it can also work on other platforms like Windows, MacOS.

Author
------
Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
