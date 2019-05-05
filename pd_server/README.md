pd_server - "Packet Data" Server
================================
pd_server is a server for storing data in a database (in this case sqlite), and for sending them to clients.
As a database, sqlite was used as a simple, single-file and good database for gathering information. In addition, sqlite does not require a database server.


Configuration
-------------
The default server TCP port is 11111, but can be changed by setting it in the command line argument.
The database consists of only one 'events' table.
<pre>
This table has three fields:
    timestamp - INTEGER PRIMARY KEY
    priority - INTEGER
    info - TEXT
</pre>
The database is created during the first query in the 'evens.db' file.
The file storing user data - 'secret.pdb', if it is missing, is created at the server startup with the default user 'admin' and the password 'admin'.

<pre>
secret.pdb file format:
-----------------------
user1|password1
user2|password2
user3|password3
userX|passwordX
</pre>
There must be no spaces or tabs in each line.

The program is installed in the /opt/pd_server directory, to which only the root user has access, and only root can boot the server.
It is a security requirement that only root has access to the database and file that stores user data.
After installing in the /opt/pd_server directory, the program must be run with root user privileges so that it has the ability to save files in this directory.
If pd_server is to be run by another user, pay attention to the permissions to write files in the directory.
Of course, you can put it in any directory if you need it.


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


Command Line Arguments
----------------------
<pre>
  -d, --debug        -  Enables debug info (stdout)
  -h, --help         -  Print help
  -p, --port [num]   -  TCP Port (default: 11111)
</pre>


Building
--------
To build a pd_server run:   ./build_script


Installing
----------
The program can be installed only after the project has been built.
<pre>
To install run as root:
make install

To uninstall run as root:
make uninstall
<pre>

Author
------
Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
