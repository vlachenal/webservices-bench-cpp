# webservices-bench-cpp

Project to test AnCH Framework integration.

## Synopsis

I have been asked my wishes for the technical base components by the company in which I am working. The Java projects can be found [here](https://github.com/vlachenal/webservices-bench) and [here](https://github.com/vlachenal/webservices-bench-client).

I decide to make C++/Thrift projects to test AnCH Framework in a real project.

## Differences between C++ and Java implamentations

Java server implementation runs on a web server (I use Tomcat). The requests are send through HTTP.

The C++ server implementation is a Thrift standalone server.
