qpd_client - Qt "Packet Data" Client
====================================
qpd_client is the client program for connecting to the pd_server with the Graphical User Interface.
It is an example of using the pd_client library.
The Qt library was used to build the GUI of the program.

Information about the connection is stored in the .qpd_client file in the home directory.


Command Line Arguments
----------------------
<pre>
  -d, --debug        -  Enables debug info (stdout)
  -h, --help         -  Print help
</pre>


Building
--------
To build a qpd_client run:   ./build_script


Installing
----------
The program can be installed only after the project has been built.

To install run as root:   make install

To uninstall run as root:   make uninstall


Author
------
Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
