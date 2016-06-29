#if !defined(__MTADEQUEUEPROP_HPP__)

#define __MTADEQUEUEPROP_HPP__

namespace MTASDK_NAMESPACE {

class mtaDequeueProp
{

public:

     mtaDequeueProp() THROWS(mtaExceptionNoMem);
     mtaDequeueProp(const char *channel) THROWS(mtaExceptionNoMem);
     mtaDequeueProp(const mtaDequeueProp& prop) THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     mtaDequeueProp(const std::string& channel) THROWS(mtaExceptionNoMem);
#endif

     virtual ~mtaDequeueProp();

     mtaDequeueProp& operator=(const mtaDequeueProp& rhs)
	  THROWS(mtaExceptionNoMem);

     mtaDequeueProp& channel(const char *channel = 0) THROWS(mtaExceptionNoMem);
     mtaDequeueProp& jbcMaxAttempts(size_t max = 5);
     mtaDequeueProp& jbcRetryInterval(size_t seconds = 10);
     mtaDequeueProp& jbcTimeout(size_t seconds = 30);
     mtaDequeueProp& threadMaxMessages(size_t nmessages = 20);
     mtaDequeueProp& maxThreads(size_t nthreads = 20);
     mtaDequeueProp& threadStackSize(size_t bytes = 1024 * 1024);
     mtaDequeueProp& threadWaitTimeout(size_t seconds = 30 * 60);
#ifndef _NO_STD_STRING
     mtaDequeueProp& channel(const std::string& channel)
	  THROWS(mtaExceptionNoMem);
#endif

     void setChannel(const char *channel = 0) THROWS(mtaExceptionNoMem);
     const char *getChannel() const;
     size_t getChannelLength() const;
#ifndef _NO_STD_STRING
     void setChannel(const std::string& channel) THROWS(mtaExceptionNoMem);
#endif

     void setJbcMaxAttempts(size_t max = 5);
     size_t getJbcMaxAttempts() const;

     void setJbcRetryInterval(size_t seconds = 10);
     size_t getJbcRetryInterval() const;

     void setJbcTimeout(size_t seconds = 30);
     size_t getJbcTimeout() const;

     void setThreadMaxMessages(size_t nmessages = 20);
     size_t getThreadMaxMessages() const;

     void setMaxThreads(size_t nthreads = 20);
     size_t getMaxThreads() const;

     void setThreadStackSize(size_t bytes = 1024 * 1024);
     size_t getThreadStackSize() const;

     void setThreadWaitTimeout(size_t seconds = 30 * 60);
     size_t getThreadWaitTimeout() const;

     bool equals(const mtaDequeueProp& prop) const;
     const char *toString();

private:

     friend class mtaDequeue;

     void clean();

     size_t       nDqJbcMaxAttempts;      // MTA_JBC_MAX_ATTEMPTS
     size_t       nDqJbcRetryInterval;    // MTA_JBC_RETRY_INTERVAL
     size_t       nDqJbcTimeout;          // MTA_JBC_TIMEOUT
     size_t       nDqThreadMaxMessages;   // MTA_THREAD_MAX_MESSAGES
     size_t       nDqThreadMaxThreads;    // MTA_THREAD_MAX_THREADS
     size_t       nDqThreadStackSize;     // MTA_THREAD_STACK_SIZE
     size_t       nDqThreadWaitTimeout;   // MTA_THREAD_WAIT_TIMEOUT

     std::string  sDqChan;
     std::string *pDqToString;
};

};

#endif
