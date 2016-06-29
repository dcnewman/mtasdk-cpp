#include "mtasdk.hpp"
#include "mtasdk.h"
#undef ALFA_SIZE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace MTASDK_NAMESPACE;

static const char *badaddr   = "Empty recipient address specified";
static const char *badassign = "Insufficient virtual memory to store the "
                                "supplied string; the assign() member function "
                                "for C++ std::string threw an exception";
static const char *baddisp   = "Unrecognized recipient disposition specified";
static const char *baddqadr  = "Error returned by mtaDequeueRecipientNext()";
static const char *baddqdisp = "Error returned by "
     "mtaDequeueRecipientDisposition()";
static const char *baddqid   = "Error returned by mtaDequeueThreadId()";
static const char *baddqinfo = "Error returned by mtaDequeueInfo()";
static const char *baddqline = "Error returned by mtaDequeueLineNext()";
static const char *baddqrew  = "Error returned by mtaDequeueRewind()";
static const char *badnew    = "Unable to allocate memory for a string buffer";
static const char *baddqeom  = "End of message reached";

mtaMessageDequeue::mtaMessageDequeue(mta_dq_t *dq, const char *chan,
				     const char *env_from, const char *env_id,
				     const char *msg_id, size_t nflags,
				     size_t dflags)
     THROWS(mtaExceptionNoMem) :
	  pMsgDqCtx(dq),
	  pMsgDqToString(0),
	  nMsgDqFlagsDelivery(dflags),
	  nMsgDqFlagsNotary((nflags & NOTIFY_RANGE_CHECK) ?
			    nflags & NOTIFY_RANGE_CHECK : NOTIFY_DEFAULT),
	  bMsgDqEom(false),
	  bMsgDqEor(false),
	  sMsgDqEnvFrom(),
	  sMsgDqEnvId(),
	  sMsgDqChan(),
	  sMsgDqId()
{
     try
     {
	  sMsgDqChan.assign(chan ? chan : "");
	  sMsgDqEnvFrom.assign(env_from ? env_from : "");
	  sMsgDqEnvId.assign(env_id ? env_id : "");
	  sMsgDqId.assign(msg_id ? msg_id : "");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaMessageDequeue::mtaMessageDequeue(mta_dq_t *dq,
				     const char *chan, size_t chan_len,
				     const char *env_from , size_t env_from_len,
				     const char *env_id, size_t env_id_len,
				     const char *msg_id, size_t msg_id_len,
				     size_t nflags, size_t dflags)
     THROWS(mtaExceptionNoMem) :
	  pMsgDqCtx(dq),
	  pMsgDqToString(0),
	  nMsgDqFlagsDelivery(dflags),
	  nMsgDqFlagsNotary((nflags & NOTIFY_RANGE_CHECK) ?
			    nflags & NOTIFY_RANGE_CHECK : NOTIFY_DEFAULT),
	  bMsgDqEom(false),
	  bMsgDqEor(false),
	  sMsgDqEnvFrom(),
	  sMsgDqEnvId(),
	  sMsgDqChan(),
	  sMsgDqId()
{
     try
     {
	  sMsgDqChan.assign(chan ? chan : "", chan ? chan_len : 0);
	  sMsgDqEnvFrom.assign(env_from ? env_from : "",
			       env_from ? env_from_len : 0);
	  sMsgDqEnvId.assign(env_id ? env_id : "", env_id ? env_id_len : 0);
	  sMsgDqId.assign(msg_id ? msg_id : "", msg_id ? msg_id_len : 0);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaMessageDequeue::~mtaMessageDequeue()
{
     mtaMessageDequeue::clean();
}


void
mtaMessageDequeue::clean()
{
     if (pMsgDqToString)
     {
	  std::string *pTmp = pMsgDqToString;
	  pMsgDqToString = 0;
	  delete pTmp;
     }
}


const char *
mtaMessageDequeue::getChannel()
{
     return(sMsgDqChan.c_str());
}


size_t
mtaMessageDequeue::getChannelLength()
{
     return(sMsgDqChan.size());
}


const char *
mtaMessageDequeue::getEnvelopeFrom()
{
     return(sMsgDqEnvFrom.c_str());
}


size_t
mtaMessageDequeue::getEnvelopeFromLength()
{
     return(sMsgDqEnvFrom.size());
}


const char *
mtaMessageDequeue::getEnvelopeId()
{
     return(sMsgDqEnvId.c_str());
}


size_t
mtaMessageDequeue::getEnvelopeIdLength()
{
     return(sMsgDqEnvId.size());
}


size_t
mtaMessageDequeue::getNotifyFlags()
{
     return(nMsgDqFlagsNotary);
}


const char *
mtaMessageDequeue::getNotifyStr()
{
     return(mtaMessageDequeue::getNotifyStr(nMsgDqFlagsNotary));
}


const char *
mtaMessageDequeue::getNotifyStr(size_t nflags)
{
     static char full_str[] = "FULL";
     static char hdrs_str[] = "HDRS";
     static char none_str[] = "NONE";
     static char unknown_str[] = "????";

     // Code here emulates handing in api_nq.c
     if (nflags & NOTIFY_CONTENT_FULL)
	  return(full_str);
     else if (nflags & NOTIFY_CONTENT_HEADER)
	  return(hdrs_str);
     else if (nflags & NOTIFY_CONTENT_NONE)
	  return(none_str);
     else
	  return(unknown_str);
}


size_t
mtaMessageDequeue::getDeliveryFlags()
{
     return(nMsgDqFlagsDelivery);
}


const char *
mtaMessageDequeue::getMessageId()
     THROWS3(mtaExceptionNo, mtaExceptionNoMem, mtaException)
{
     static const char *empty_str = "";

     if (!sMsgDqId.size())
     {
	  const char *ptr = 0;
	  size_t len = 0;
	  int ires = mtaDequeueInfo(pMsgDqCtx, MTA_MSG_ID, &ptr, &len);
	  if (ires == MTA_OK && ptr && len)
	  {
	       try
	       {
		    sMsgDqId.assign(ptr, len);
	       }
	       catch (...)
	       {
		    throw mtaExceptionNoMem(badassign);
	       }
	  }
	  else
	  {
	       if (ires == MTA_OK)
		    ires = MTA_NO;
	       throwException(ires, baddqinfo);
	       return(empty_str); // Never reached
	  }
     }
     return(sMsgDqId.c_str());
}


size_t
mtaMessageDequeue::getMessageIdLength()
     THROWS3(mtaExceptionNo, mtaExceptionNoMem, mtaException)
{
     size_t len = sMsgDqId.size();
     if (len)
	  return(len);
     mtaMessageDequeue::getMessageId();
     return(sMsgDqId.size());
}


bool
mtaMessageDequeue::eor() const
{
     return(bMsgDqEor);
}


bool
mtaMessageDequeue::getRecipient(mtaAddressDequeue& adr)
     THROWS(mtaException)
{
     // Return now if we've exhausted the list of envelope Recipients
     if (bMsgDqEor)
	  return(false);

     // Get the next envelope recipient
     const char *ircpt, *orcpt, *radr;
     size_t dflags, ircpt_len, nflags, orcpt_len, radr_len;
     
     radr = ircpt = orcpt = NULL;
     radr_len = dflags = ircpt_len = nflags = orcpt_len = 0;
     int ires = mtaDequeueRecipientNext(pMsgDqCtx,
					&radr, &radr_len,
					MTA_DELIVERY_FLAGS, &dflags,
					MTA_IRCPT_TO,       &ircpt, &ircpt_len,
					MTA_NOTIFY_FLAGS,   &nflags,
					MTA_ORCPT_TO,       &orcpt, &orcpt_len,
					0);
     if (ires == MTA_OK)
     {
	  adr.assign(radr, radr_len, orcpt, orcpt_len, ircpt, ircpt_len, *this,
		     nflags, dflags);
	  return(true);
     }
     else if (ires == MTA_EOF)
     {
	  bMsgDqEor = true;
	  return(false);
     }
     else
     {
	  throwException(ires, baddqadr);
	  return(false); // never reached
     }
}


void
mtaMessageDequeue::setRecipientDisposition(size_t disp, const char *adr,
					   size_t adr_len, const char *reason,
					   size_t reason_len) const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     int ires;

     // Sanity checks.  Note that if we don't do this, mtaDequeueRecipientDisp()
     // will catch the errors anyhow. And we'll still throw mtaExceptionBadArgs.
     // However, we won't throw that with the argument number set to 1.
     if (disp > DISP_LAST)
	  throw mtaExceptionBadArgs(baddisp, 1);
     else if (!adr || !adr[0])
	  throw mtaExceptionBadArgs(badaddr, 2);

     // Now set the recipient's disposition
     ires = mtaDequeueRecipientDisposition(pMsgDqCtx, adr, adr_len, disp,
					   (reason && *reason) ? MTA_REASON : 0,
					   reason, reason_len, 0);

     // And throw an exception if there was an error
     if (ires != MTA_OK)
	  throwException(ires, baddqdisp);
}


void
mtaMessageDequeue::setRecipientDisposition(size_t disp, const char *adr,
					   const char *reason) const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaMessageDequeue::setRecipientDisposition(disp, adr, 0, reason, 0);
}


void
mtaMessageDequeue::setRecipientDisposition(size_t disp, mtaAddressDequeue& adr,
					   const char *reason) const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaMessageDequeue::setRecipientDisposition(
	  disp,
	  adr.getAddress().toString(), adr.getAddress().toStringLength(),
	  reason, 0);
}

void
mtaMessageDequeue::setRecipientDisposition(size_t disp, const std::string& adr)
     const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaMessageDequeue::setRecipientDisposition(disp, adr.data(), adr.size(),
						0, 0);
}


void
mtaMessageDequeue::setRecipientDisposition(size_t disp, const std::string& adr,
					   const std::string& reason) const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaMessageDequeue::setRecipientDisposition(disp, adr.data(), adr.size(),
						reason.data(), reason.size());
}


bool
mtaMessageDequeue::eom() const
{
     return(bMsgDqEom);
}


bool
mtaMessageDequeue::getLine(const char **line, size_t *len)
     THROWS(mtaException)
{
     // Return now if we've exhausted the message body
     if (bMsgDqEom)
     {
	  if (line)
	       *line = NULL;
	  if (len)
	       *len = 0;
	  return(false);
     }

     // Get the next line
     int ires = mtaDequeueLineNext(pMsgDqCtx, line, len);
     if (ires == MTA_OK)
	  return(true);
     else if (ires == MTA_EOF)
     {
	  bMsgDqEom = true;
	  if (line)
	       *line = NULL;
	  if (len)
	       *len = 0;
	  return(false);
     }
     else
     {
	  throwException(ires, baddqline);
	  return(false);  // Never reached
     }
}


const char *
mtaMessageDequeue::getLine()
     THROWS(mtaException)
{
     static const char *empty = "";

     // Return now if we've exhausted the message body
     if (bMsgDqEom)
     {
	  throwException(MTA_EOF, baddqeom);
	  return(NULL);  // Never reached
     }

     // Get the next line
     size_t len;
     const char *line = NULL;
     int ires = mtaDequeueLineNext(pMsgDqCtx, &line, &len);
     if (ires == MTA_OK)
     {
	  if (!line)
	  {
	       len  = 0;
	       line = empty;
	  }
	  char *tmp = (char *)malloc(len + 1);
	  if (!tmp)
	  {
	       throwException(MTA_NOMEM, badnew);
	       return(NULL);
	  }
	  memcpy(tmp, line, len);
	  tmp[len] = '\0';
	  return((const char *)tmp);
     }
     throwException(ires, (ires == MTA_EOF) ? baddqeom : baddqline);
     return(NULL);  // Never reached
}


void
mtaMessageDequeue::rewind()
     THROWS(mtaException)
{
     int ires = mtaDequeueRewind(pMsgDqCtx);
     if (ires == MTA_OK)
	  bMsgDqEom = false;
     else
	  throwException(ires, baddqrew);
}


int
mtaMessageDequeue::threadId() const
     THROWS(mtaException)
{
     int ires = mtaDequeueThreadId(pMsgDqCtx);
     if (ires < 0)
	  throwException(mtaErrno(), baddqid);
     return(ires);
}


mta_dq_t *
mtaMessageDequeue::getDequeueContext() const
{
     return(pMsgDqCtx);
}


const char *
mtaMessageDequeue::toString()
{
     if (!pMsgDqToString)
     {
	  std::string *pTmp = 0;
	  char buf[512];

	  snprintf(buf, sizeof(buf),
		   "MAIL FROM:<%.*s> RET=%s ENVID=%.*s",
		   static_cast<int>(getEnvelopeFromLength()),
                   getEnvelopeFrom(),
		   getNotifyStr(), static_cast<int>(getEnvelopeIdLength()),
		   getEnvelopeId());
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
	  pMsgDqToString = pTmp;

     }
     return(pMsgDqToString->c_str());
}
