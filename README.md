# Long REST
## Summary
Long REST is a combination of a TCP/IP and RESTful applications library, with the goal of making a simple abstraction
of both. Long REST is intended for simple, personal, and non-commercial applications.

## Why?
Long REST is a personal project. One day I was looking at the Raspberry Pi and WebAssembly, which prompted the thought
that I wanted to make a simple application web application for me and my friends to work with while playing Dungeons
and Dragons, as well as many other tabletop roleplaying games. In order to do this, I would need both a HTTP server
and a server side application to run the program with.

The thought continued that I should try my hand at making a RESTful application. So I decided that I would now need
a HTTP server, RESTful server-side application, and a RESTful WebAssembly client-side application. I then thought
that I should write my own RESTful library to run my application through and continued to that if I can write my
own RESTful library that I could use it to write my own HTTP server and combine my entire client-side application
into a single, multifunction program.

With a new goal in mind and a way to get to it, I started Long REST and realized the scope, and how I would
like to share my work with the world in the hopes that I am not the only one who could use the code that I
write. So, here we are with me giving my work to the world.

## Planned Versions
While Long REST is a personal project to experement and expand my understanding of RESTful applications and 
the HTTP protocol as well as keeping practiced with C++, I still plan to have some specific versions that
have defined features.

### V1.0
* TCP classes that allow for simplification of the default C++ sockets
* HTTP classes that allow for easy code interaction with HTTP headers
* A HTTP server that serves as an example program
* WARNING: The build script provided with this version, and all following versions until mentioned otherwise, 
will build the default example project

### V1.1
* A TCP Event Manager class that will allow for single threaded event handling, which creative programmers may
be able to use to create a multithreaded program.
* TCP, and possibly HTTP, exceptions

### V1.2
* Add SSL/TLS compatability
* Upgrade build script to allow for installation of RESTful

### V1.3
* Add more examples with build scripts
* Move the default example in with the rest of the examples
* Upgrade build script to build the library, then build all examples
