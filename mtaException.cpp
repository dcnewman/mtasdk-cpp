#include "mtasdk.hpp"
#include "mtasdk.h"
#include <stdlib.h>
#include <string.h>

using namespace MTASDK_NAMESPACE;

mtaException::mtaException(int err, const char *msg, int argno) :
     nErrNo(err),
     nErrArg(argno),
     pErrMsg2(msg),
     pErrText(0),
     bErrFree(false)
{
}


mtaException::~mtaException()
{
     if (bErrFree && pErrText)
	  free(pErrText);
}


int
mtaException::getArgno() const
{
     return(nErrArg);
}


void
MTASDK_NAMESPACE::throwException(int ires, const char *msg, int argno)
     THROWS(mtaException)
{
     switch(ires)
     {
     case MTA_ACCESS :
	  throw mtaExceptionAccess(msg);

     case MTA_AGAIN :
	  throw mtaExceptionAgain(msg);

     case MTA_BADARGS :
	  throw mtaExceptionBadArgs(msg, argno);

     case MTA_EOF :
	  throw mtaExceptionEOF(msg);

     case MTA_FCREATE :
	  throw mtaExceptionFCreate(msg);

     case MTA_FIO :
	  throw mtaExceptionFIO(msg);

     case MTA_FOPEN :
	  throw mtaExceptionFOpen(msg);

     case MTA_NETWORK :
	  throw mtaExceptionNetwork(msg);

     case MTA_NO :
	  throw mtaExceptionNo(msg);

     case MTA_NOMEM :
	  throw mtaExceptionNoMem(msg);

     case MTA_NOOP :
	  throw mtaExceptionNoOp(msg);

     case MTA_NOSUCHCHAN :
	  throw mtaExceptionNoSuchChan(msg);

     case MTA_NOSUCHHOST :
	  throw mtaExceptionNoSuchHost(msg);

     case MTA_NOSUCHITEM :
	  throw mtaExceptionNoSuchItem(msg);

     case MTA_ORDER :
	  throw mtaExceptionOrder(msg);

     case MTA_SIZE :
	  throw mtaExceptionSize(msg);

     case MTA_STRTRU :
	  throw mtaExceptionStrTru(msg);

     case MTA_STRTRUERR :
	  throw mtaExceptionStrTruErr(msg);

     case MTA_THREAD :
	  throw mtaExceptionThread(msg);

     case MTA_TIMEDOUT :
	  throw mtaExceptionTimedOut(msg);

     default :
	  throw mtaException(ires, msg);
     }
}


int
mtaException::getErrno() const
{
     return(nErrNo);
}


const char *
mtaException::getMessage() const
{
     // If we have already built the string, then return it now
     if (pErrText)
	  return(pErrText);

     // If we don't have supplemental error text, then we can
     // simply return the result of mtaStrError(nErrNo)
     if (!pErrMsg2)
     {
	  pErrText = (char *)mtaStrError(nErrNo, 0);
	  bErrFree = false;
	  return(pErrText);
     }

     // Build the string: <mtaStrError(nErrNo)> + "; " + <pErrMsg2>

     // Determine the length of the composite string
     const char *str1 = mtaStrError(nErrNo, 0);
     size_t l1 = str1 ? strlen(str1) : 0;
     size_t l2 = pErrMsg2 ? strlen(pErrMsg2) : 0;
     size_t l3 = l1 + l2 + 1;
     if (l1 && l2)
	  l3 += 2;

     // We checked this already, but just in case
     if (bErrFree && pErrText)
     {
	  free(pErrText);
	  bErrFree = false;
	  pErrText = 0;
     }

     // Determine where to store the data
     char *str3;
     bool freeit;
     if (l3 > sizeof(sBuf))
     {
	  // Static buffer is too small
	  str3 = (char *)malloc(l3);
	  if (!str3)
	       // Insufficient memory
	       return(str1 ? str1 : pErrMsg2);
	  freeit = true;
     }
     else
     {
	  // We can use our static buffer
	  str3 = sBuf;
	  freeit = false;
     }

     // Now build the string
     char *tmp = str3;
     if (l1 && l2)
     {
	  memcpy(tmp, str1, l1);
	  tmp += l1;
	  *tmp++ = ';';
	  *tmp++ = ' ';
     }
     if (l2)
	  memcpy(tmp, pErrMsg2, l2);
     tmp += l2;
     *tmp = '\0';

     // Save it
     pErrText = str3;
     bErrFree = freeit;

     // And return
     return(pErrText);
}


const char *
mtaException::toString() const
{
     return(getMessage());
}


const char *
mtaException::getErrorStr(int err)
{
     return(mtaStrError(err, 0));
}


mtaExceptionBadArgs::mtaExceptionBadArgs(const char *msg, int argno) :
     mtaException(MTA_BADARGS, msg, argno)
{
}


mtaExceptionBadArgs::~mtaExceptionBadArgs()
{
}
