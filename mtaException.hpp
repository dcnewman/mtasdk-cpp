#if !defined(__MTAEXCEPTION_HPP__)

#define __MTAEXCEPTION_HPP__

#include "mtaThrows.hpp"

namespace MTASDK_NAMESPACE {

class mtaException
{

public:

     mtaException(int err, const char *msg = 0, int argno = -1);
     virtual ~mtaException();

     int getErrno() const;
     int getArgno() const;
     virtual const char *getMessage() const;

     virtual const char *toString() const;

     static const char *getErrorStr(int err);

private:

     int           nErrNo;
     int           nErrArg;
     const char   *pErrMsg2;
     mutable char *pErrText;
     mutable bool  bErrFree;
     mutable char  sBuf[128];
};

#ifndef SWIG

// This routine is implemented in mtaException.cpp
// It is primarily for internal consumption

void throwException(int err, const char *msg = 0, int argno = -1)
     THROWS(mtaException);

#endif // !defined(SWIG)

// The following declarations are for finer grained exception handling.
// Probably too excessive for most purposes.

#ifndef SWIG

class mtaExceptionBadArgs: public mtaException
{

public:

     mtaExceptionBadArgs(const char *msg = 0, int argno = -1);
     virtual ~mtaExceptionBadArgs();
};

#define MTA_EXCEPT(x,y,m)						\
class x: public mtaException {					        \
public:									\
     x(const char *msg = 0) : mtaException(y, msg) { };			\
     virtual ~x() { };							\
}

MTA_EXCEPT(mtaExceptionAccess,     MTA_ACCESS,
	   "Authorization failure (MTA_ACCESS)");
MTA_EXCEPT(mtaExceptionAgain,      MTA_AGAIN,
	   "Temporary error; try again later (MTA_AGAIN)");
MTA_EXCEPT(mtaExceptionEOF,        MTA_EOF,
	   "End of file encountered (MTA_EOF)");
MTA_EXCEPT(mtaExceptionFCreate,    MTA_FCREATE,
	   "File creation error (MTA_FCREATE)");
MTA_EXCEPT(mtaExceptionFIO,        MTA_FIO,
	   "File I/O error (MTA_FIO)");
MTA_EXCEPT(mtaExceptionFOpen,      MTA_FOPEN,
	   "File open error (MTA_FOPEN)");
MTA_EXCEPT(mtaExceptionNetwork,    MTA_NETWORK,
	   "Network I/O error (MTA_NETWORK)");
MTA_EXCEPT(mtaExceptionNo,         MTA_NO,
	   "Operation failed (MTA_NO)");
MTA_EXCEPT(mtaExceptionNoMem,      MTA_NOMEM,
	   "Insufficient virtual memory (MTA_NOMEM)");
MTA_EXCEPT(mtaExceptionNoOp,       MTA_NOOP,
	   "No op (MTA_NOOP)");
MTA_EXCEPT(mtaExceptionNoSuchChan, MTA_NOSUCHCHAN,
	   "No such channel configured (MTA_NOSUCHCHAN)");
MTA_EXCEPT(mtaExceptionNoSuchHost, MTA_NOSUCHHOST,
	   "No such host configured (MTA_NOSUCHHOST)");
MTA_EXCEPT(mtaExceptionNoSuchItem, MTA_NOSUCHITEM,
	   "No such item code (MTA_NOSUCHITEM)");
MTA_EXCEPT(mtaExceptionOrder,      MTA_ORDER,
	   "MTA API subroutine called out of sequence (MTA_ORDER)");
MTA_EXCEPT(mtaExceptionSize,       MTA_SIZE,
	   "Configured size limit exceeded (MTA_SIZE)");
MTA_EXCEPT(mtaExceptionStrTru,     MTA_STRTRU,
	   "String truncated to fit buffer (MTA_STRTRU)");
MTA_EXCEPT(mtaExceptionStrTruErr,  MTA_STRTRUERR,
	   "String too long (MTA_STRTRUERR)");
MTA_EXCEPT(mtaExceptionThread,     MTA_THREAD,
	   "Invalid concurrent/simultaneous use of an MTA API context by "
	   "two or more distinct threads (MTA_THREAD)");
MTA_EXCEPT(mtaExceptionTimedOut,   MTA_TIMEDOUT,
	   "Time out error (MTA_TIMEOUT)");

#undef MTA_EXCEPT

#endif /* !defined(SWIG) */

};

#endif
