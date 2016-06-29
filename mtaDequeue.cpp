#include <stdio.h>
#include <stdlib.h>
#include "mtasdk.hpp"
#include "mtasdk.h"
#undef CHANLENGTH
#undef ALFA_SIZE

using namespace MTASDK_NAMESPACE;

typedef struct {
     void *ctx2;
} mta_per_thread_data_t;

static const char *badassign = "Insufficient virtual memory to store the "
                                "supplied string; the assign() member function "
                                "for C++ std::string threw an exception";
#ifdef __DOOF_DO_PROPS__
static const char *badchanlen = "Channel names may not exceed 32 bytes";
#endif
static const char *baddqstart = "Failure returned by mtaDequeueStart()";
static const char *badnew     = "Unable to allocate memory for a string buffer";

mtaDequeue::mtaDequeue()
     THROWS(mtaExceptionNoMem) :
	  oDqProp(),
	  pDqToString(0),
	  sDqChan()
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


mtaDequeue::mtaDequeue(const char *chan)
     THROWS(mtaExceptionNoMem) :
	  oDqProp(),
	  pDqToString(0),
	  sDqChan()
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


mtaDequeue::mtaDequeue(const char *chan, const mtaDequeueProp& prop)
     THROWS(mtaExceptionNoMem) :
	  oDqProp(prop),
	  pDqToString(0),
	  sDqChan()
{
     // mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     //  may toss an exception.  Just let it do so
     oDqProp = prop;

     try
     {
	  if (chan)
	       sDqChan.assign(chan);
	  else
	       sDqChan.assign(oDqProp.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeue::mtaDequeue(const std::string& chan)
     THROWS(mtaExceptionNoMem) :
	  oDqProp(),
	  pDqToString(0),
	  sDqChan()
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


mtaDequeue::mtaDequeue(const std::string& chan, const mtaDequeueProp& prop)
     THROWS(mtaExceptionNoMem) :
	  oDqProp(),
	  pDqToString(0),
	  sDqChan()
{
     // mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     //  may toss an exception.  Just let it do so
     oDqProp = prop;

     try
     {
	  sDqChan.assign(chan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeue::mtaDequeue(const mtaDequeueProp& prop)
     THROWS(mtaExceptionNoMem) :
	  oDqProp(),
	  pDqToString(0),
	  sDqChan()
{
     // mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     //  may toss an exception.  Just let it do so
     oDqProp = prop;

     try
     {
	  sDqChan.assign(oDqProp.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeue::mtaDequeue(const mtaDequeue& rhs)
     THROWS(mtaExceptionNoMem) :
	  oDqProp(),
	  pDqToString(0),
	  sDqChan()
{
     // mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     //  may toss an exception.  Just let it do so.
     oDqProp = rhs.oDqProp;

     try
     {
	  sDqChan.assign(oDqProp.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaDequeue::~mtaDequeue()
{
     mtaDequeue::clean();
} 


mtaDequeue&
mtaDequeue::operator=(const mtaDequeue& rhs)
     THROWS(mtaExceptionNoMem)
{
     // Ignore assignment to self
     if (this == &rhs)
	  return(*this);

     mtaDequeue::clean();
     pDqToString = 0;

     // mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     //  may toss an exception.  Just let it do so.
     oDqProp = rhs.oDqProp;

     try
     {
	  sDqChan.assign(rhs.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }

     return(*this);
}


mtaDequeue&
mtaDequeue::operator=(const mtaDequeueProp& rhs)
     THROWS(mtaExceptionNoMem)
{
     mtaDequeue::clean();
     pDqToString = 0;

     // mtaDequeueProp::operator=(const mtaDequeueProp& rhs)
     //  may toss an exception.  Just let it do so.
     oDqProp = rhs;

     try
     {
	  sDqChan.assign(rhs.sDqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }

     return(*this);
}

extern "C" mta_dq_process_message_t processMessagePrivWrap;
extern "C" int processMessagePrivWrap(
    void **ctx2, void *ctx1, mta_dq_t *dq,
    const char *env_from, size_t env_from_len) {
    mtaDequeue *obj = static_cast<mtaDequeue *>(ctx1);

    return obj->processMessagePriv(ctx2, ctx1, dq, env_from, env_from_len);
}

extern "C" mta_dq_process_done_t processDonePrivWrap;
extern "C" void processDonePrivWrap(void *ctx2, void *ctx1) {
    mtaDequeue *obj = static_cast<mtaDequeue *>(ctx1);

    obj->processDonePriv(ctx2, ctx1);
}

void
mtaDequeue::start(const char *channel)
     THROWS2(mtaExceptionNoMem, mtaException)
{
     // Note that the call to mtaDequeueStart() will block.  While blocked,
     // threads will be spun off.  These threads will process queued message
     // files via calls to processMessagePriv().  processMessagePriv() will
     // call the processMessage() member function of the derived class.

     size_t clen;
     if (channel)
     {
	  try
	  {
	       sDqChan.assign(channel);
	  }
	  catch (...)
	  {
	       throw mtaExceptionNoMem(badassign);
	  }
	  clen = sDqChan.size();
     }
     else
     {
	  try
	  {
	       sDqChan.assign("");
	  }
	  catch (...)
	  {
	       throw mtaExceptionNoMem(badassign);
	  }
	  clen = 0;
     }
     int ires = mtaDequeueStart(
	  const_cast<mtaDequeue *>(this),
	  processMessagePrivWrap,
	  processDonePrivWrap,
	  MTA_JBC_MAX_ATTEMPTS,    oDqProp.getJbcMaxAttempts(),
	  MTA_JBC_RETRY_INTERVAL,  oDqProp.getJbcRetryInterval(),
	  MTA_JBC_TIMEOUT,         oDqProp.getJbcTimeout(),
	  MTA_THREAD_MAX_MESSAGES, oDqProp.getThreadMaxMessages(),
	  MTA_THREAD_MAX_THREADS,  oDqProp.getMaxThreads(),
	  MTA_THREAD_STACK_SIZE,   oDqProp.getThreadStackSize(),
	  MTA_THREAD_WAIT_TIMEOUT, oDqProp.getThreadWaitTimeout(),
	  clen ? MTA_CHANNEL : 0,  sDqChan.data(), clen,
	  0);
     if (ires != MTA_OK)
	  throwException(ires, baddqstart);
}


void
mtaDequeue::start()
     THROWS2(mtaExceptionNoMem, mtaException)
{
     mtaDequeue::start(0);
}


const char *
mtaDequeue::getChannel() const
{
     return(sDqChan.c_str());
}


size_t
mtaDequeue::getChannelLength() const
{
     return(sDqChan.size());
}


int
mtaDequeue::processMessage(void *ctx2, mtaMessageDequeue& msg)
{
    (void)ctx2; (void)msg;
     return(MTA_NO);
}


void
mtaDequeue::processDone(void *ctx2)
{
    (void)ctx2;
     return;
}


void *
mtaDequeue::processInit()
{
     return((void *)0);
}


extern "C" int
mtaDequeue::processMessagePriv(void **ctx2, void *ctx1, mta_dq_t *dq,
			       const char *env_from, size_t env_from_len)
{
     // Sanity check
     if (!ctx1 || !ctx2)
	  return(MTA_ABORT);

     // ctx1 is the address of "this"
     mtaDequeue *obj = (mtaDequeue *)ctx1;
     mta_per_thread_data_t *pCtx2 = 0;

     // If this is the first call, then alloc an object using the
     // processInit() routine of our derived class
     if (!(*ctx2))
     {
	  pCtx2 = (mta_per_thread_data_t *)
	       malloc(sizeof(mta_per_thread_data_t));
	  if (!pCtx2)
	       return(MTA_ABORT);

	  // Initialize
	  pCtx2->ctx2 = obj->processInit();

	  // And save for later use
	  *ctx2 = (void *)pCtx2;
     }
     else
	  // Get the per-thread context info
	  pCtx2 = (mta_per_thread_data_t *)*ctx2;

     // Collect the message-level information
     // For the sake of efficiency, we don't bother to actually get the
     // message id.  Instead, we have code in mtaMessageDequeue::getMessageId()
     // to pull that information if and when it is requested.

     const char *chan       = 0;
     size_t      chan_len   = 0;
     const char *env_id     = 0;
     size_t      env_id_len = 0;
     const char *msg_id     = 0;
     size_t      msg_id_len = 0;
     size_t      nflags     = NOTIFY_DEFAULT;
     size_t      dflags     = 0;

     mtaDequeueInfo(dq, MTA_DELIVERY_FLAGS, &dflags,
		        MTA_CHANNEL,        &chan,   &chan_len,
		        MTA_ENV_ID,         &env_id, &env_id_len,
//		        MTA_MSG_ID,         &msg_id, &msg_id_len,
		        MTA_NOTIFY_FLAGS,   &nflags,
		        0);

     // Now call the processMessage() routine of the derived class
     int ires;
     try
     {
	  mtaMessageDequeue msgdq(dq, chan, chan_len, env_from, env_from_len,
				  env_id, env_id_len, msg_id, msg_id_len,
				  nflags, dflags);
	  ires = (ABORT == obj->processMessage(pCtx2->ctx2, msgdq)) ?
	       MTA_ABORT : MTA_OK;
     }
     catch (...)
     {
	  // Abort on *any* exception
	  ires = MTA_ABORT;
     }

     // All done
     return(ires);
}


extern "C" void
mtaDequeue::processDonePriv(void *ctx2, void *ctx1)
{
     if (!ctx1)
	  // Insufficient context to call the processDone() of the derived class
	  return;

     try
     {

	  // Obtain the ctx2 data, if any, generated by the derived
	  // class' processInit() routine.  Then dispose of the ctx2
	  // created by our private processMessagePriv() routine.

	  void *pCtx2;
	  mta_per_thread_data_t *pCtx = (mta_per_thread_data_t *)ctx2;
	  if (pCtx)
	  { 
	       pCtx2 = pCtx->ctx2;
	       if (pCtx)
		    delete pCtx;
	  }
	  else
	       pCtx2 = 0;

	  // Now call the derived class' processDone() member function
	  mtaDequeue *pObj = (mtaDequeue *)ctx1;
	  pObj->processDone(pCtx2);

     }
     catch (...)
     {
	  // Ignore any exceptions and get on with life
     }
}


const char *
mtaDequeue::toString()
{
     if (!pDqToString)
     {
	  char buf[512];

	  snprintf(buf, sizeof(buf),
		   "channel=\"%.*s\"; %s",
		   (int) sDqChan.size(), sDqChan.data(),
		   oDqProp.toString());
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


void
mtaDequeue::clean()
{
     if (!pDqToString)
	  return;
     std::string *pTmp = pDqToString;
     pDqToString = 0;
     delete pTmp;
}

#ifdef __DOOF_DO_PROPS__

static const char *badprop    = "Unrecognized property code specified";
static const char *badprop2   = "Specified property is numeric valued";

// Dequeue property codes (based upon mtasdk.h MTA_ item codes)
FINAL int CHANNEL             =  15;
FINAL int JBC_MAX_ATTEMPTS    =  88;
FINAL int JBC_RETRY_INTERVAL  =  89;
FINAL int JBC_TIMEOUT         =  90;
FINAL int THREAD_MAX_MESSAGES = 112;
FINAL int THREAD_MAX_THREADS  = 113;
FINAL int THREAD_STACK_SIZE   = 114;
FINAL int THREAD_WAIT_TIMEOUT = 115;

size_t
mtaDequeue::getProperty(int property) const
     THROWS(mtaExceptionBadArgs)
{
     switch(property)
     {
     case JBC_MAX_ATTEMPTS :
	  return(nDqJbcMaxAttempts);

     case JBC_RETRY_INTERVAL :
	  return(nDqJbcRetryInterval);

     case JBC_TIMEOUT :
	  return(nDqJbcTimeout);

     case THREAD_MAX_THREADS :
	  return(nDqThreadMaxThreads);

     case THREAD_MAX_MESSAGES :
	  return(nDqThreadMaxMessages);

     case THREAD_STACK_SIZE :
	  return(nDqThreadStackSize);

     case THREAD_WAIT_TIMEOUT :
	  return(nDqThreadWaitTimeout);

     default :
	  throw mtaExceptionBadArgs(badprop, 1);
     }
}


void
mtaDequeue::setProperty(int property, size_t value)
     THROWS(mtaExceptionBadArgs)
{
     switch(property)
     {
     case JBC_MAX_ATTEMPTS :
	  nDqJbcMaxAttempts = value;
	  break;

     case JBC_RETRY_INTERVAL :
	  nDqJbcRetryInterval = value;
	  break;

     case JBC_TIMEOUT :
	  nDqJbcTimeout = value;
	  break;

     case THREAD_MAX_THREADS :
	  nDqThreadMaxThreads = value;
	  break;

     case THREAD_MAX_MESSAGES :
	  nDqThreadMaxMessages = value;
	  break;

     case THREAD_STACK_SIZE :
	  nDqThreadStackSize = value;
	  break;

     case THREAD_WAIT_TIMEOUT :
	  nDqThreadWaitTimeout = value;
	  break;

     case CHANNEL :
     default :
	  throw mtaExceptionBadArgs(badprop, 1);
     }

     // Need to rebuild toString() data
     mtaDequeue::clean();
}


void
mtaDequeue::setProperty(int property, const char *value, size_t len)
     THROWS2(mtaExceptionBadArgs, mtaStrTruErr)
{
     if (property != CHANNEL)
	  throw mtaExceptionBadArgs(badprop2, 1);

     if (!len)
	  len = value ? strlen(value) : 0;
     if (len > CHANLENGTH)
	  throw mtaExceptionStrTruErr(badchanlen);

     // We'll need to rebuild the toString() data
     mtaDequeue::clean();

     // Do the assignment
     sDqChan.assign(value ? value : "", len);
}

#endif
