pd_client - "Packed Data Client" library
========================================
pd_client is a shared library that can be used to build a client program that allows you to connect to the pd_server.
The client can send new events to the server or download existing from the server.

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


Building
--------
To build a pd_client library run:
./build_script


Installing
----------
The program can be installed only after the project has been built.

To install run as root:
make install

To uninstall run as root:
make uninstall


Author
------
Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
