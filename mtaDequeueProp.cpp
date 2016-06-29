#include "mtasdk.hpp"
#include <string.h>
#include <stdio.h>

// Imported from mtasdk.cpp
extern "C" {
extern size_t nGblDqJbcMaxAttempts;
extern size_t nGblDqJbcRetryInterval;
extern size_t nGblDqJbcTimeout;
extern size_t nGblDqThreadMaxMessages;
extern size_t nGblDqThreadMaxThreads;
extern size_t nGblDqThreadStackSize;
extern size_t nGblDqThreadWaitTimeout;
};

using namespace MTASDK_NAMESPACE;

static const char *badassign = "Insufficient virtual memory to store the "
                                "supplied string; the assign() member function "
                                "for C++ std::string threw an exception";
static const char *badnew    = "Unabe to allocate memory for a string buffer";

mtaDequeueProp::mtaDequeueProp()
     THROWS(mtaExceptionNoMem) :
	  nDqJbcMaxAttempts(nGblDqJbcMaxAttempts),
	  nDqJbcRetryInterval(nGblDqJbcRetryInterval),
	  nDqJbcTimeout(nGblDqJbcTimeout),
	  nDqThreadMaxMessages(nGblDqThreadMaxMessages),
	  nDqThreadMaxThreads(nGblDqThreadMaxThreads),
	  nDqThreadStackSize(nGblDqThreadStackSize),
	  nDqThreadWaitTimeout(nGblDqThreadWaitTimeout),
	  sDqChan(),
	  pDqToString(0)
{
     try
     {
	  sDqChan.assign("");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeueProp::mtaDequeueProp(const char *chan)
     THROWS(mtaExceptionNoMem) :
	  nDqJbcMaxAttempts(nGblDqJbcMaxAttempts),
	  nDqJbcRetryInterval(nGblDqJbcRetryInterval),
	  nDqJbcTimeout(nGblDqJbcTimeout),
	  nDqThreadMaxMessages(nGblDqThreadMaxMessages),
	  nDqThreadMaxThreads(nGblDqThreadMaxThreads),
	  nDqThreadStackSize(nGblDqThreadStackSize),
	  nDqThreadWaitTimeout(nGblDqThreadWaitTimeout),
	  sDqChan(),
	  pDqToString(0)
{
     try
     {
	  if (chan)
	       sDqChan.assign(chan);
	  else
	       sDqChan.assign("");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeueProp::mtaDequeueProp(const std::string& chan)
     THROWS(mtaExceptionNoMem) :
	  nDqJbcMaxAttempts(nGblDqJbcMaxAttempts),
	  nDqJbcRetryInterval(nGblDqJbcRetryInterval),
	  nDqJbcTimeout(nGblDqJbcTimeout),
	  nDqThreadMaxMessages(nGblDqThreadMaxMessages),
	  nDqThreadMaxThreads(nGblDqThreadMaxThreads),
	  nDqThreadStackSize(nGblDqThreadStackSize),
	  nDqThreadWaitTimeout(nGblDqThreadWaitTimeout),
	  sDqChan(),
	  pDqToString(0)
{
     try
     {
	  sDqChan.assign(chan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeueProp::mtaDequeueProp(const mtaDequeueProp& prop)
     THROWS(mtaExceptionNoMem) :
	  nDqJbcMaxAttempts(prop.nDqJbcMaxAttempts),
	  nDqJbcRetryInterval(prop.nDqJbcRetryInterval),
	  nDqJbcTimeout(prop.nDqJbcTimeout),
	  nDqThreadMaxMessages(prop.nDqThreadMaxMessages),
	  nDqThreadMaxThreads(prop.nDqThreadMaxThreads),
	  nDqThreadStackSize(prop.nDqThreadStackSize),
	  nDqThreadWaitTimeout(prop.nDqThreadWaitTimeout),
	  sDqChan(),
	  pDqToString(0)
{
     try
     {
	  sDqChan.assign(prop.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeueProp::~mtaDequeueProp()
{
     mtaDequeueProp::clean();
}


mtaDequeueProp&
mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     THROWS(mtaExceptionNoMem)
{
     if (this == &rhs)
	  return(*this);

     mtaDequeueProp::clean();

     try
     {
	  sDqChan.assign(rhs.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }

     nDqJbcMaxAttempts    = rhs.nDqJbcMaxAttempts;
     nDqJbcRetryInterval  = rhs.nDqJbcRetryInterval;
     nDqJbcTimeout        = rhs.nDqJbcTimeout;
     nDqThreadMaxMessages = rhs.nDqThreadMaxMessages;
     nDqThreadMaxThreads  = rhs.nDqThreadMaxThreads;
     nDqThreadStackSize   = rhs.nDqThreadStackSize;
     nDqThreadWaitTimeout = rhs.nDqThreadWaitTimeout;
     pDqToString          = 0;

     return(*this);
}


void
mtaDequeueProp::clean()
{
     if (!pDqToString)
	  return;
     std::string *pTmp = pDqToString;
     pDqToString = 0;
     delete pTmp;
}


mtaDequeueProp&
mtaDequeueProp::channel(const char *chan)
     THROWS(mtaExceptionNoMem)
{
     try
     {
	  sDqChan.assign(chan ? chan : "");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::channel(const std::string& chan)
     THROWS(mtaExceptionNoMem)
{
     try
     {
	  sDqChan.assign(chan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::jbcMaxAttempts(size_t max)
{
     nDqJbcMaxAttempts = max;
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::jbcRetryInterval(size_t seconds)
{
     nDqJbcRetryInterval = (seconds > 2) ? seconds : 2;
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::jbcTimeout(size_t seconds)
{
     nDqJbcTimeout = (seconds > 2) ? seconds : 2;
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::threadMaxMessages(size_t nmessages)
{
     nDqThreadMaxMessages = (nmessages > 1) ? nmessages : 1;
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::maxThreads(size_t nthreads)
{
     nDqThreadMaxThreads = (nthreads > 1) ? nthreads : 1;
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::threadStackSize(size_t bytes)
{
     nDqThreadStackSize = bytes;
     return(*this);
}


mtaDequeueProp&
mtaDequeueProp::threadWaitTimeout(size_t seconds)
{
     nDqThreadWaitTimeout = (seconds > 2) ? seconds : 2;
     return(*this);
}


void
mtaDequeueProp::setJbcMaxAttempts(size_t max)
{
     mtaDequeueProp::clean();
     nDqJbcMaxAttempts = max;
}


size_t
mtaDequeueProp::getJbcMaxAttempts() const
{
     return(nDqJbcMaxAttempts);
}


void
mtaDequeueProp::setJbcRetryInterval(size_t seconds)
{
     mtaDequeueProp::clean();
     nDqJbcRetryInterval = (seconds > 2) ? seconds : 2;
}


size_t
mtaDequeueProp::getJbcRetryInterval() const
{
     return(nDqJbcRetryInterval);
}


void
mtaDequeueProp::setJbcTimeout(size_t seconds)
{
     mtaDequeueProp::clean();
     nDqJbcTimeout = (seconds > 2) ? seconds : 2;
}


size_t
mtaDequeueProp::getJbcTimeout() const
{
     return(nDqJbcTimeout);
}


void
mtaDequeueProp::setThreadMaxMessages(size_t nmessages)
{
     mtaDequeueProp::clean();
     nDqThreadMaxMessages = (nmessages > 1) ? nmessages : 1;
}


size_t
mtaDequeueProp::getThreadMaxMessages() const
{
     return(nDqThreadMaxMessages);
}


void
mtaDequeueProp::setMaxThreads(size_t nthreads)
{
     mtaDequeueProp::clean();
     nDqThreadMaxThreads = (nthreads > 1) ? nthreads : 1;
}


size_t
mtaDequeueProp::getMaxThreads() const
{
     return(nDqThreadMaxThreads);
}


void
mtaDequeueProp::setThreadStackSize(size_t bytes)
{
     mtaDequeueProp::clean();
     nDqThreadStackSize = bytes;
}


size_t
mtaDequeueProp::getThreadStackSize() const
{
     return(nDqThreadStackSize);
}


void
mtaDequeueProp::setThreadWaitTimeout(size_t seconds)
{
     mtaDequeueProp::clean();
     nDqThreadWaitTimeout = (seconds > 2) ? seconds : 2;
}


size_t
mtaDequeueProp::getThreadWaitTimeout() const
{
     return(nDqThreadWaitTimeout);
}


void
mtaDequeueProp::setChannel(const char *chan)
     THROWS(mtaExceptionNoMem)
{
     try
     {
	  sDqChan.assign(chan ? chan : "");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


void
mtaDequeueProp::setChannel(const std::string& chan)
     THROWS(mtaExceptionNoMem)
{
     try
     {
	  sDqChan.assign(chan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


const char *
mtaDequeueProp::getChannel() const
{
     return(sDqChan.c_str());
}


size_t
mtaDequeueProp::getChannelLength() const
{
     return(sDqChan.size());
}


bool
mtaDequeueProp::equals(const mtaDequeueProp& prop) const
{
     if (nDqJbcMaxAttempts    == prop.nDqJbcMaxAttempts    &&
	 nDqJbcRetryInterval  == prop.nDqJbcRetryInterval  &&
	 nDqJbcTimeout        == prop.nDqJbcTimeout        &&
	 nDqThreadMaxMessages == prop.nDqThreadMaxMessages &&
	 nDqThreadMaxThreads  == prop.nDqThreadMaxThreads  &&
	 nDqThreadStackSize   == prop.nDqThreadStackSize   &&
	 nDqThreadWaitTimeout == prop.nDqThreadWaitTimeout &&
	 sDqChan.size()       == prop.sDqChan.size()       &&
	 (sDqChan.size() == 0 ||
	  !strncasecmp(sDqChan.data(), prop.sDqChan.data(), sDqChan.size())))
	  return(true);
     else
	  return(false);
}

const char *
mtaDequeueProp::toString()
{
     if (!pDqToString)
     {
	  char buf[512];

	  snprintf(buf, sizeof(buf),
		   "MTA_CHANNEL=\"%.*s\"; "
		   "MTA_JBC_MAX_ATTEMPTS=%zu; MTA_JBC_RETRY_INTERVAL=%zu; "
		   "MTA_JBC_TIMEOUT=%zu; MTA_THREAD_MAX_MESSAGES=%zu; "
		   "MTA_JBC_THREAD_MAX_THREADS=%zu; MTA_THREAD_STACK_SIZE=%zu; "
		   "MTA_THREAD_WAIT_TIMEOUT=%zu",
		   static_cast<int>(sDqChan.size()), sDqChan.data(),
		   nDqJbcMaxAttempts, nDqJbcRetryInterval, nDqJbcTimeout,
		   nDqThreadMaxMessages, nDqThreadMaxThreads,
		   nDqThreadStackSize, nDqThreadWaitTimeout);

	  std::string *pTmp = 0;
	  try
	  {
	       pTmp = new std::string(buf);
	  }
	  catch (...)
	  {
	       if (pTmp)
		    delete pTmp;
	       // Would prefer to throw an exception except for the fact that
	       // Java's Object::toString() doesn't.  That makes SWIG'ing this
	       // problematic if we choose to toss an exception.
	       return(badnew);
	  }
	  if (!pTmp)
	       // Would prefer to throw an exception except for the fact that
	       // Java's Object::toString() doesn't.  That makes SWIG'ing this
	       // problematic if we choose to toss an exception.
	       return(badnew);
	  pDqToString = pTmp;
     }
     return(pDqToString->c_str());
}
