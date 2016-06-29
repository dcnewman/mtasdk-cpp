#if !defined(__MTADEQUEUE_HPP__)

#define __MTADEQUEUE_HPP__

namespace MTASDK_NAMESPACE {

class mtaDequeue
{

public:

     mtaDequeue() THROWS(mtaExceptionNoMem);
     mtaDequeue(const char *channel) THROWS(mtaExceptionNoMem);
     mtaDequeue(const char *channel, const mtaDequeueProp& prop)
	   THROWS(mtaExceptionNoMem);
     mtaDequeue(const mtaDequeueProp& prop) THROWS(mtaExceptionNoMem);
     mtaDequeue(const mtaDequeue& rhs) THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     mtaDequeue(const std::string& channel) THROWS(mtaExceptionNoMem);
     mtaDequeue(const std::string& channel, const mtaDequeueProp& prop)
	   THROWS(mtaExceptionNoMem);
#endif

     virtual ~mtaDequeue();

#ifndef SWIG
     mtaDequeue& operator=(const mtaDequeueProp& rhs) THROWS(mtaExceptionNoMem);
     mtaDequeue& operator=(const mtaDequeue& rhs) THROWS(mtaExceptionNoMem);
#endif

#ifndef SWIG
#ifndef SWIG_CXX
     virtual void *processInit() = 0;
#else
     virtual jobject processInit() = 0;
#endif
#else
     virtual jobject processInit() = 0;
#endif
     virtual int processMessage(void *ctx2, mtaMessageDequeue& msg) = 0;
     virtual void processDone(void *ctx2) = 0;

     void start() THROWS2(mtaExceptionNoMem, mtaException);
     void start(const char *channel) THROWS2(mtaExceptionNoMem, mtaException);

     const char *getChannel() const;
     size_t getChannelLength() const;

     virtual const char *toString();

     // need to be accessible from C callback, but for private use
     static int processMessagePriv(void **ctx2, void *ctx1, mta_dq_t *dq,
				   const char *env_from, size_t env_from_len);
     static void processDonePriv(void *ctx2, void *ctx1);
private:

     void clean();

     mtaDequeueProp  oDqProp;
     std::string    *pDqToString;
     std::string     sDqChan;
};

};

#endif
