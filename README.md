thrift-nano
===========
A thrift compiler and library for tiny computers
The idea
========
I like thrift but it doesn't support small computers very well.  I need something like c_glib but glib is just too fat to fit on tiny computers (something like stm32 or a small arduino).  The goal of this project is to provide 2 additional generators and a library that will fit on a computer with 256k flash.  The generators it should include are:

1. A simple struct generator.  This generator will not support any standard thrift protocols and will not support optional values.
2. A more standard struct generator.  This generator will create structs that may be written to standard thrift protocols/transports.

The library should support including only those transports/protocols/services required at compile time.

Roadmap
=======
1. Create a small thrift library that will support all thrift types and the binary and compact protocols.  These protocols should be compiled out by default and only included with -DTHRIFT_NANO_PROTOCOL_&lt;BINARY|COMPACT&gt;
2. Create a generator to generate simple structs.
3. Create a generator to generate thrift compatible structs.
4. Implement more thrift features like services.  These features should still require inclusion with preprocessor macros.

Building
========
>`./build.sh`

	OR

>`mkdir build && cd build && cmake ../ && make`

Examples
========
Example arduino serial reader: https://github.com/markrileybot/thrift-nano/wiki/Example-Arduino-Serial-Reader


