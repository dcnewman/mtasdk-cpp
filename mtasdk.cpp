#include <pthread.h>
#include <string.h>
#include "mtasdk.hpp"
#include "mtasdk.h"

using namespace MTASDK_NAMESPACE;

extern "C" {
size_t nGblDqJbcMaxAttempts    =  5;        //  5 consecutive attempts
size_t nGblDqJbcRetryInterval  = 10;        // 10 seconds
size_t nGblDqJbcTimeout        = 30;        // 30 seconds
size_t nGblDqThreadMaxMessages = 20;        // 20 messages per thread
size_t nGblDqThreadMaxThreads  = 20;        // 20 concurrent threads
size_t nGblDqThreadStackSize   = 1024*1024; // 1 Mb
size_t nGblDqThreadWaitTimeout = 30 * 60;   // 30 minutes
};

static pthread_mutex_t mutex;
static bool            initialized = false; 

static const char *badinit = "Error returned by mtaInit()";

extern "C" void mtaDequeueDefaults(size_t *stack_size,
  size_t *messages_per_thread, size_t *threads_max, size_t *jbc_timeout,
  size_t *die_by_delay, size_t *max_patience, size_t *sleep_interval);

static void
initialize(mta_item_list_t *items)
     THROWS(mtaException)
{
     if (initialized)
	  return;

     pthread_mutex_init(&mutex, NULL);
     pthread_mutex_lock(&mutex);

     // Initialize the MTA SDK
     int ires = mtaInit(items ? MTA_ITEM_LIST : 0, items, 0);
     if (ires != MTA_OK)
     {
	  pthread_mutex_unlock(&mutex);
	  throwException(ires, badinit);
     }

     // Get actual default values from the C mtasdk
     mtaDequeueDefaults(&nGblDqThreadStackSize,
			&nGblDqThreadMaxMessages,
			&nGblDqThreadMaxThreads,
			&nGblDqJbcTimeout,
			&nGblDqThreadWaitTimeout,
			&nGblDqJbcMaxAttempts,
			&nGblDqJbcRetryInterval);

     initialized = true;
     pthread_mutex_unlock(&mutex);
}


#define PUSH_ITEM(code) \
     items[item_index++].item_code = (code)

#define PUSH_ITEM2(code,val) \
     items[item_index].item_code = (code); \
     items[item_index++].item_length = (val)

void
MTASDK_NAMESPACE::initialize(bool isChannel, size_t debug_flags)
     THROWS(mtaException)
{
     mta_item_list_t items[14];
     size_t item_index = 0;

     item_index = 0;
     PUSH_ITEM(isChannel ? MTA_CHANNEL : MTA_INTERACTIVE);

     if (!debug_flags)
	  goto skip;

     if (debug_flags & DEBUG_NODECODE)
	  PUSH_ITEM(MTA_DEBUG_NODECODE);

     if (debug_flags & DEBUG_NODEQUEUE)
	  PUSH_ITEM(MTA_DEBUG_NODEQUEUE);

     if (debug_flags & DEBUG_NOENQUEUE)
	  PUSH_ITEM(MTA_DEBUG_NOENQUEUE);

     if (debug_flags & DEBUG_NOMM)
	  PUSH_ITEM(MTA_DEBUG_NOMM);

     if (debug_flags & DEBUG_NOOS)
	  PUSH_ITEM(MTA_DEBUG_NOOS);

     if (debug_flags & DEBUG_NOSDK)
	  PUSH_ITEM(MTA_DEBUG_NOSDK);

     if (debug_flags & DEBUG_DECODE)
	  PUSH_ITEM(MTA_DEBUG_DECODE);

     if (debug_flags & DEBUG_DEQUEUE)
	  PUSH_ITEM(MTA_DEBUG_DEQUEUE);

     if (debug_flags & DEBUG_ENQUEUE)
	  PUSH_ITEM(MTA_DEBUG_ENQUEUE);

     if (debug_flags & DEBUG_MM)
	  PUSH_ITEM2(MTA_DEBUG_MM, 5);

     if (debug_flags & DEBUG_OS)
	  PUSH_ITEM(MTA_DEBUG_OS);

     if (debug_flags & DEBUG_SDK)
	  PUSH_ITEM(MTA_DEBUG_SDK);

skip:
     PUSH_ITEM(0);

     initialize(items);
}


void
MTASDK_NAMESPACE::setDebug(size_t debug_flags, size_t mm_level)
{
     if (!debug_flags)
	  return;

     mta_item_list_t items[20];
     size_t item_index = 0;

     if (debug_flags & DEBUG_NODECODE)
	  PUSH_ITEM(MTA_DEBUG_NODECODE);

     if (debug_flags & DEBUG_NODEQUEUE)
	  PUSH_ITEM(MTA_DEBUG_NODEQUEUE);

     if (debug_flags & DEBUG_NOENQUEUE)
	  PUSH_ITEM(MTA_DEBUG_NOENQUEUE);

     if (debug_flags & DEBUG_NOMM)
	  PUSH_ITEM(MTA_DEBUG_NOMM);

     if (debug_flags & DEBUG_NOOS)
	  PUSH_ITEM(MTA_DEBUG_NOOS);

     if (debug_flags & DEBUG_NOSDK)
	  PUSH_ITEM(MTA_DEBUG_NOSDK);

     if (debug_flags & DEBUG_DECODE)
	  PUSH_ITEM(MTA_DEBUG_DECODE);

     if (debug_flags & DEBUG_DEQUEUE)
	  PUSH_ITEM(MTA_DEBUG_DEQUEUE);

     if (debug_flags & DEBUG_ENQUEUE)
	  PUSH_ITEM(MTA_DEBUG_ENQUEUE);

     if (debug_flags & DEBUG_MM)
	  PUSH_ITEM2(MTA_DEBUG_MM, mm_level);

     if (debug_flags & DEBUG_OS)
	  PUSH_ITEM(MTA_DEBUG_OS);

     if (debug_flags & DEBUG_SDK)
	  PUSH_ITEM(MTA_DEBUG_SDK);

     if (!item_index)
	  return;

     PUSH_ITEM(0);

     mtaDebug(MTA_ITEM_LIST, items, 0);
}


void
MTASDK_NAMESPACE::done()
{
     if (!initialized)
	  return;

     initialized = false;
     pthread_mutex_lock(&mutex);
     mtaDone();
     pthread_mutex_unlock(&mutex);
     pthread_mutex_destroy(&mutex);
}


int
MTASDK_NAMESPACE::getErrno()
{
     return(mta_errno);
}


void
MTASDK_NAMESPACE::accountingLogClose()
{
     mtaAccountingLogClose();
}


void
MTASDK_NAMESPACE::writeLog(const char *str, size_t len)
{
     if (str)
	  mtaLog("%.*s", len ? len : strlen(str), str);
     else
	  mtaLog("%s", "");
}


void
MTASDK_NAMESPACE::writeLog(const std::string str)
{
     mtaLog("%.*s", str.size(), str.data());
}


size_t
MTASDK_NAMESPACE::blockSize()
{
     return(mtaBlockSize());
}


size_t
MTASDK_NAMESPACE::stackSize()
{
     return(mtaStackSize());
}


int
MTASDK_NAMESPACE::versionMajor()
{
     return(mtaVersionMajor());
}


int
MTASDK_NAMESPACE::versionMinor()
{
     return(mtaVersionMinor());
}


int
MTASDK_NAMESPACE::versionRevision()
{
     return(mtaVersionRevision());
}


#undef PUSH_ITEM
#undef PUSH_ITEM2
