A C++ interface to the Oracle Unified Communications Suite Messaging
Server MTA SDK.  This code provides C++ applications with an OO MTA SDK
and was written as weekend project for a customer who needed a Java SDK
for a proof-of-concept project of their own with Oracle UCS Messaging Server.

Access to the concurrent, multithreaded message dequeuing system is
had by inheriting the abstract mtaDequeue class and providing for the
member functions `processInit()`, `processMessage()` and `processDone()`.

The code here is (1) light on comments being a shim-layer atop the
documented MTA SDK, and (2) also sprinkled with instructions for the SWIG
process which is used to generate an equivalent Java SDK.
