#ifndef __MTASDK_HPP__

#define __MTASDK_HPP__

// Do not define _NO_STD_STRING when not using SWIG
#ifdef SWIG
#  define _NO_STD_STRING
#else
#  ifdef _NO_STD_STRING
#    undef _NO_STD_STRING
#  endif // _NO_STD_STRING
#endif // SWIG

#include <string>
#include <sys/types.h>

typedef void mta_nq_t;
typedef void mta_dq_t;
typedef struct mta_item_list_s mta_item_list_t;

#define MTASDK_NAMESPACE mtasdk_v_6_0_0

namespace MTASDK_NAMESPACE {

#ifndef SWIG
#  define FINAL(t,n,v) static const t n = v
#else
#  define FINAL(t,n,v) %constant t n = v
#endif

     // Basic string size
     FINAL(size_t, ALFA_SIZE,     256); // Basic MTA string size in bytes
     FINAL(size_t, CHANLENGTH,     32); // Max length in bytes of a channel name

     // Notification flags
     FINAL(size_t, NOTIFY_CONTENT_FULL,   0x01);   // DSNs have full msg content
     FINAL(size_t, NOTIFY_CONTENT_HEADER, 0x02);   // DSNs have msg header only
     FINAL(size_t, NOTIFY_CONTENT_NONE,   0x04);   // DSNs have no msg content
     FINAL(size_t, NOTIFY_DELAY,          0x08);   // Issue delay DSNs
     FINAL(size_t, NOTIFY_FAILURE,        0x10);   // Issue failure DSNs (NDNs)
     FINAL(size_t, NOTIFY_SUCCESS,        0x20);   // Issue success DSNs
     FINAL(size_t, NOTIFY_DEFAULT, \
	  NOTIFY_DELAY | NOTIFY_FAILURE | NOTIFY_CONTENT_FULL);
     FINAL(size_t, NOTIFY_RANGE_CHECK, \
	  NOTIFY_CONTENT_FULL |	NOTIFY_CONTENT_HEADER | NOTIFY_CONTENT_NONE | \
	  NOTIFY_DELAY | NOTIFY_FAILURE | NOTIFY_SUCCESS);

     // Recipient dispostions
     FINAL(size_t, DISP_FIRST,           0); // Used internally as a range check
     FINAL(size_t, DISP_DEFERRED,        0); // Delivery failed; try again later
     FINAL(size_t, DISP_DELIVERED,       1); // Message successfully delivered
     FINAL(size_t, DISP_FAILED,          2); // Delivery failed; return to sender
     FINAL(size_t, DISP_RELAYED,         3); // Message relayed; no relay DSN
     FINAL(size_t, DISP_RELAYED_FOREIGN, 4); // Message relayed; issue relay DSN
     FINAL(size_t, DISP_RETURN,          5); // Return message as undeliverable
     FINAL(size_t, DISP_TIMEDOUT,        6); // Return message as timed out
     FINAL(size_t, DISP_LAST,            6); // Used internally as a range check

     // Address types (based upon mtasdk.h MTA_ item codes)
     FINAL(size_t, ADDR_FIRST,   1); // Used for range checks
     FINAL(size_t, ADDR_ENV_TO,  1); // Envelope recipient only (transport)
     FINAL(size_t, ADDR_TO,      2); // Envelope recipient + RFC 822 To:
     FINAL(size_t, ADDR_CC,      3); // Envelope recipient + RFC 822 Cc:
     FINAL(size_t, ADDR_BCC,     4); // Envelope recipient + RFC 822 Bcc:
     FINAL(size_t, ADDR_HDR_TO,  5); // RFC 822 To: only (header)
     FINAL(size_t, ADDR_HDR_CC,  6); // RFC 822 Cc: only (header)
     FINAL(size_t, ADDR_HDR_BCC, 7); // RFC 822 Bcc: only (header)
     FINAL(size_t, ADDR_LAST,    7); // Used for range checks
     FINAL(size_t, ADDR_DEFAULT, ADDR_TO); // Envelope recipient + RFC 822 To:

     // Error codes
     FINAL(int, MTA_OK,          0); // Successful operation
     FINAL(int, MTA_ACCESS,      1); // Authorization failure
     FINAL(int, MTA_AGAIN,       2); // Temporary failure; try again later
     FINAL(int, MTA_BADARGS,     3); // Bad or corrupt call arguments
     FINAL(int, MTA_EOF,         4); // End of file or end of data reached
     FINAL(int, MTA_FCREATE,     5); // Unable to create a message file
     FINAL(int, MTA_FIO,         6); // File read/write error
     FINAL(int, MTA_FOPEN,       7); // Unable to open the message file
     FINAL(int, MTA_NETWORK,     8); // Network I/O error
     FINAL(int, MTA_NO,          9); // Operation failed; permanent error
     FINAL(int, MTA_NOMEM,      10); // Insufficient virtual memory
     FINAL(int, MTA_NOOP,       11); // Operation completed vaccuously
     FINAL(int, MTA_NOSUCHCHAN, 12); // No such channel defined
     FINAL(int, MTA_NOSUCHHOST, 13); // No such host or domain
     FINAL(int, MTA_NOSUCHITEM, 14); // No such item code
     FINAL(int, MTA_ORDER,      15); // Subroutine called out of sequence
     FINAL(int, MTA_SIZE,       16); // Site conf size limits exceeded
     FINAL(int, MTA_STRTRU,     17); // String too long; truncated to fit
     FINAL(int, MTA_STRTRUERR,  18); // String too long; operation failed
     FINAL(int, MTA_THREAD,     19); // Threading error
     FINAL(int, MTA_TIMEDOUT,   20); // Operation timed out

     FINAL(int, ABORT,          -1);

     // Debug flags
     FINAL(size_t, DEBUG_DECODE,    0x00000001); // MIME parsing & decoding
     FINAL(size_t, DEBUG_DEQUEUE,   0x00000002); // Dequeue (DEQUEUE_DEBUG=1)
     FINAL(size_t, DEBUG_ENQUEUE,   0x00000004); // Enqueue (MM_DEBUG=5)
     FINAL(size_t, DEBUG_MM,        0x00000008); // Enqueue (MM_DEBUG=level)
     FINAL(size_t, DEBUG_OS,        0x00000010); // O/S (OS_DEBUG=1)
     FINAL(size_t, DEBUG_SDK,       0x00000020); // MTA SDK (MTA_DEBUG_SDK)

     FINAL(size_t, DEBUG_NODECODE,  0x00010000); // Disable DEBUG_DECODE
     FINAL(size_t, DEBUG_NODEQUEUE, 0x00020000); // DEQUEUE_DEBUG=0
     FINAL(size_t, DEBUG_NOENQUEUE, 0x00040000); // MM_DEBUG=0
     FINAL(size_t, DEBUG_NOMM,      0x00080000); // MM_DEBUG=0
     FINAL(size_t, DEBUG_NOOS,      0x00100000); // OS_DEBUG=0
     FINAL(size_t, DEBUG_NOSDK,     0x00200000); // MTA_DEBUG_NOSDK

#undef FINAL

};

#include "mtaException.hpp"
#include "mtaAddress.hpp"
#include "mtaAddressProp.hpp"
#include "mtaAddressDequeue.hpp"
#include "mtaAddressEnqueue.hpp"
#include "mtaDequeueProp.hpp"
#include "mtaDequeue.hpp"
#include "mtaEnqueueProp.hpp"
#include "mtaMessageDequeue.hpp"
#include "mtaMessageEnqueue.hpp"

// Declaration of non-class based functions

namespace MTASDK_NAMESPACE {

     void initialize(bool isChannel = true, size_t debug_flags = 0)
	  THROWS(mtaException);

     void done();

     void setDebug(size_t debug_flags = 0, size_t mm_level = 5);

     void writeLog(const char *str, size_t len = 0);
#ifndef _NO_STD_STRING
     void writeLog(const std::string str);
#endif

     void accountingLogClose();
     int  getErrno();

     size_t blockSize();
     size_t stackSize();
     int    versionMajor();
     int    versionMinor();
     int    versionRevision();
};

#endif
