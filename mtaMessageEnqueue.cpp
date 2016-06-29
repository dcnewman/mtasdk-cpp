#include "mtasdk.hpp"
#include "mtasdk.h"
#include <stdio.h>

using namespace MTASDK_NAMESPACE;

#include "map_atypes.h"

static const char *badatype    = "Invalid address type specified";
static const char *badfrom     = "Null pointer supplied for the envelope From: "
     "address; specify an empty envelope From: address with a zero length "
     "string";
static const char *baditems    = "Unable to obtain a list of properties to set "
     "from the supplied mtaEnqueueProp object";
static const char *badnew      = "Unable to allocate memory for a string "
     "buffer";
static const char *badnflags   = "Invalid notify flag bit mask specified";
static const char *badnqadd    = "Error returned by mtaEnqueueTo()";
static const char *badnqcp     = "Error returned by mtaEnqueueCopyMessage()";
static const char *badnqfin    = "Error returned by mtaEnqueueFinish()";
#if 0
static const char *badnqfina   = "Error returned by "
     "mtaEnqueueFinish(MTA_ABORT)";
#endif
static const char *badnqinfo   = "Error returned by mtaEnqueueInfo()";
static const char *badnqstart  = "Error returned by mtaEnqueueStart()";
static const char *badnqstart2 = "Null enqueue context returned by "
     "mtaEnqueueStart()";
static const char *badnqwrt    = "Error returned by mtaEnqueueWrite()";
static const char *badnqwrtl   = "Error returned by mtaEnqueueWriteLine()";


mtaMessageEnqueue::mtaMessageEnqueue(const char *env_from, const char *env_id)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(env_from, 0, env_id, 0, 0,
						    0)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const char *env_from,
				     const mtaEnqueueProp& eprop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(env_from, 0, 0, 0, 0,
						    eprop)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const char *env_from, const char *env_id,
				     const mtaEnqueueProp& eprop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(env_from, 0, env_id, 0, 0,
						    eprop)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const char *env_from,
				     const mtaMessageDequeue& dq)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(env_from, 0, 0, 0,
						    dq.getDequeueContext(), 0)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const char *env_from,
				     const mtaMessageDequeue& dq,
				     const mtaEnqueueProp& eprop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(env_from, 0, 0, 0,
						    dq.getDequeueContext(),
						    eprop)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const std::string& env_from)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(
			 env_from.data(), env_from.size(), 0, 0, 0, 0)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const std::string& env_from,
				     const std::string& env_id)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(
			 env_from.data(), env_from.size(),
			 env_id.data(), env_id.size(), 0, 0)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const std::string& env_from,
				     const mtaEnqueueProp& eprop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(
			 env_from.data(), env_from.size(), 0, 0, 0,
			 eprop)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const std::string& env_from,
				     const std::string& env_id,
				     const mtaEnqueueProp& eprop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(
			 env_from.data(), env_from.size(),
			 env_id.data(), env_id.size(), 0, eprop)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const std::string& env_from,
				     const mtaMessageDequeue& dq)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(
			 env_from.data(), env_from.size(), 0, 0,
			 dq.getDequeueContext(), 0)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const std::string& env_from,
				     const mtaMessageDequeue& dq,
				     const mtaEnqueueProp& eprop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(
			 env_from.data(), env_from.size(), 0, 0,
			 dq.getDequeueContext(), eprop)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::mtaMessageEnqueue(const mtaMessageEnqueue& rhs)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException) :
	  pMsgNqCtx(mtaMessageEnqueue::enqueueStart(rhs)),
	  pMsgNqToString(0),
	  nMsgNqChanLen(0),
	  nMsgNqFlagsNotary(0)
{
}


mtaMessageEnqueue::~mtaMessageEnqueue()
{
     mtaMessageEnqueue::clean();
}


// Doing this is a bad idea and hence why we supply our own copy operator...
mtaMessageEnqueue&
mtaMessageEnqueue::operator=(const mtaMessageEnqueue& rhs)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException)
{
     // Ignore assignment to self
     if (this == &rhs)
	  return(*this);

     // Abort any ongoing enqueue
     mtaMessageEnqueue::clean();

     // Get a new enqueue context
     pMsgNqCtx = mtaMessageEnqueue::enqueueStart(rhs);
     pMsgNqToString = 0;

     // All done
     return(*this);
}


const char *
mtaMessageEnqueue::getChannel()
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoStr(MTA_CHANNEL, &nMsgNqChanLen));
}


size_t
mtaMessageEnqueue::getChannelLength()
     THROWS(mtaException)
{
     if (!nMsgNqChanLen)
	  mtaMessageEnqueue::getChannel();
     return(nMsgNqChanLen);
}


const char *
mtaMessageEnqueue::getEnvelopeFrom() const
     THROWS(mtaException)
{
     size_t len = 0;
     return(mtaMessageEnqueue::getInfoStr(MTA_ENV_FROM, &len));
}


size_t
mtaMessageEnqueue::getEnvelopeFromLength() const
     THROWS(mtaException)
{
     size_t len = 0;
     mtaMessageEnqueue::getInfoStr(MTA_ENV_FROM, &len);
     return(len);
}


const char *
mtaMessageEnqueue::getEnvelopeId() const
     THROWS(mtaException)
{
     size_t len = 0;
     return(mtaMessageEnqueue::getInfoStr(MTA_ENV_ID, &len));
}


size_t
mtaMessageEnqueue::getEnvelopeIdLength() const
     THROWS(mtaException)
{
     size_t len = 0;
     mtaMessageEnqueue::getInfoStr(MTA_ENV_ID, &len);
     return(len);
}


size_t
mtaMessageEnqueue::getNotifyFlags()
     THROWS(mtaException)
{
     if (!nMsgNqFlagsNotary)
	  nMsgNqFlagsNotary = mtaMessageEnqueue::getInfoInt(MTA_NOTIFY_FLAGS);
     return(nMsgNqFlagsNotary);
}


const char *
mtaMessageEnqueue::getNotifyStr()
     THROWS(mtaException)
{
     return(mtaMessageDequeue::getNotifyStr(
		 mtaMessageEnqueue::getNotifyFlags()));
}


size_t
mtaMessageEnqueue::getDeliveryFlags() const
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoInt(MTA_DELIVERY_FLAGS));
}


// mta_nq_t *
// mtaMessageEnqueue::getEnqueueContext() const
// {
//     return(pMsgNqCtx);
// }


bool
mtaMessageEnqueue::getExpandAliases() const
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoInt(MTA_ALIAS_EXPAND) ? true : false);
}


size_t
mtaMessageEnqueue::getExpandLimit() const
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoInt(MTA_EXPAND_LIMIT));
}


size_t
mtaMessageEnqueue::getFragmentBlockLimit() const
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoInt(MTA_FRAGMENT_BLOCKS));
}


size_t
mtaMessageEnqueue::getFragmentLineLimit() const
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoInt(MTA_FRAGMENT_LINES));
}


bool
mtaMessageEnqueue::getSortAddresses() const
     THROWS(mtaException)
{
     return(mtaMessageEnqueue::getInfoInt(MTA_ADR_SORT) ? true : false);
}


const char *
mtaMessageEnqueue::getError() const
{
     return(mtaEnqueueError(pMsgNqCtx, 0, 0, 0));
}


void
mtaMessageEnqueue::addTo(const char *to, size_t len, size_t aflags,
			 size_t nflags) const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     int ires;
     mta_item_list_t items[3];
     size_t item_index = 0;

     if (aflags)
     {
	  if (aflags < ADDR_FIRST || aflags > ADDR_LAST)
	       throw mtaExceptionBadArgs(badatype, 3);
	  items[item_index  ].item_code   = map_atypes[aflags];
	  items[item_index++].item_length = 0;
     }
     if (nflags)
     {
	  if (nflags & ~NOTIFY_RANGE_CHECK)
	       throw mtaExceptionBadArgs(badnflags, 4);
	  items[item_index  ].item_code   = MTA_NOTIFY_FLAGS;
	  items[item_index++].item_length = nflags;
     }

     if (!item_index)
	  ires = mtaEnqueueTo(pMsgNqCtx, to, len, 0);
     else
     {
	  items[item_index].item_code = MTA_END_LIST;
	  ires = mtaEnqueueTo(pMsgNqCtx, to, len, MTA_ITEM_LIST, items, 0);
     }
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqadd);
}


void
mtaMessageEnqueue::addTo(const char *to, size_t len, int item_code_1,
			 void *data_1, int item_code_2, void *data_2) const
     THROWS(mtaException)
{
     int ires;

     if (!data_1)
     {
	  if (!data_2)
	       ires = mtaEnqueueTo(pMsgNqCtx, to, len, 0);
	  else
	       ires = mtaEnqueueTo(pMsgNqCtx, to, len, item_code_2, data_2, 0);
     }
     else
     {
	  if (!data_2)
	       ires = mtaEnqueueTo(pMsgNqCtx, to, len, item_code_1, data_1, 0);
	  else
	       ires = mtaEnqueueTo(pMsgNqCtx, to, len,
				   item_code_1, data_1,
				   item_code_2, data_2, 0);
     }
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqadd);
}


void
mtaMessageEnqueue::addRecipient(const char *to, size_t atype, size_t nflags)
     const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaMessageEnqueue::addTo(to, 0, atype, nflags);
}


void
mtaMessageEnqueue::addRecipient(const char *to, const mtaAddressProp& aprop)
     const
     THROWS(mtaException)
{
     mtaMessageEnqueue::addTo(to, 0, MTA_ITEM_LIST, aprop.getItemList());
}


void
mtaMessageEnqueue::addRecipient(const std::string& adr, size_t atype,
				size_t nflags) const
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaMessageEnqueue::addTo(adr.data(), adr.size(), atype, nflags);
}


void
mtaMessageEnqueue::addRecipient(const std::string& to,
 				const mtaAddressProp& aprop) const
     THROWS(mtaException)
{
     mtaMessageEnqueue::addTo(to.data(), to.size(),
			      MTA_ITEM_LIST, aprop.getItemList());
}


void
mtaMessageEnqueue::addRecipient(const mtaAddressDequeue& to) const
     THROWS(mtaException)
{
     mtaMessageEnqueue::addTo(to.getAddress().toString(),
			      to.getAddress().toStringLength(),
			      MTA_DQ_CONTEXT, to.getDequeueContext());
}


void
mtaMessageEnqueue::addRecipient(const mtaAddressDequeue& to,
				const mtaAddressProp& aprop) const
     THROWS(mtaException)
{
     mtaMessageEnqueue::addTo(to.getAddress().toString(),
			      to.getAddress().toStringLength(),
			      MTA_ITEM_LIST, aprop.getItemList(),
			      MTA_DQ_CONTEXT, to.getDequeueContext());
}


void
mtaMessageEnqueue::write(const char *data, size_t len) const
     THROWS(mtaException)
{
     int ires = mtaEnqueueWrite(pMsgNqCtx, data, len, 0);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqwrt);
}


void
mtaMessageEnqueue::writeLine(const char *data, size_t len) const
     THROWS(mtaException)
{
     int ires = mtaEnqueueWriteLine(pMsgNqCtx, data, len, 0);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqwrtl);
}


void
mtaMessageEnqueue::write(const std::string& str) const
     THROWS(mtaException)
{
     int ires = mtaEnqueueWrite(pMsgNqCtx, str.data(), str.size(), 0);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqwrt);
}


void
mtaMessageEnqueue::writeLine(const std::string& str) const
     THROWS(mtaException)
{
     int ires = mtaEnqueueWriteLine(pMsgNqCtx, str.data(), str.size(), 0);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqwrtl);
}


void
mtaMessageEnqueue::enqueue()
     THROWS(mtaException)
{
     int ires = mtaEnqueueFinish(pMsgNqCtx, 0);
     pMsgNqCtx = 0;
     try
     {
	  mtaMessageEnqueue::clean();
     }
     catch (...)
     {
	  // do nothing: the error that is imperative that we report
	  // is that, if any, from mtaEnqueueFinish()
     }
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqfin);
}


void
mtaMessageEnqueue::abort()
{
     if (!pMsgNqCtx)
	  return;

     mtaMessageEnqueue::clean();
}


void
mtaMessageEnqueue::copyMessage(const mtaMessageDequeue& rhs, bool rewind)
     const
     THROWS(mtaException)
{
     int ires = mtaEnqueueCopyMessage(pMsgNqCtx, rhs.getDequeueContext(),
				      rewind ? 1 : 0);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqcp);
}


size_t
mtaMessageEnqueue::getInfoInt(int item_code) const
     THROWS(mtaException)
{
     size_t val = 0;
     int ires = mtaEnqueueInfo(pMsgNqCtx, item_code, &val, 0);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqinfo);
     return(val);
}


const char *
mtaMessageEnqueue::getInfoStr(int item_code, size_t *len) const
     THROWS(mtaException)
{
     const char *val = 0;
     if (len)
	  len = 0;
     int ires = mtaEnqueueInfo(pMsgNqCtx, item_code, &val, len);
     if (ires != MTA_OK)
	  mtaMessageEnqueue::throwException(ires, badnqinfo);
     return(val);
}


void
mtaMessageEnqueue::clean()
{
     // Abort any ongoing enqueue operation
     if (pMsgNqCtx)
     {
	  mta_nq_t *pTmp = pMsgNqCtx;
	  pMsgNqCtx = 0;
	  mtaEnqueueFinish(pTmp, MTA_ABORT, 0);
     }
     if (pMsgNqToString)
     {
	  std::string *pTmp = pMsgNqToString;
	  pMsgNqToString = 0;
	  delete pTmp;
     }
     nMsgNqChanLen     = 0;
     nMsgNqFlagsNotary = 0;
}


void
mtaMessageEnqueue::throwException(int ires, const char *msg)  const
     THROWS(mtaException)
{
     // Don't throw an exception on MTA_OK
     if (ires == MTA_OK)
	  return;

     // Now toss our cookies
     MTASDK_NAMESPACE::throwException(ires, msg);
}


mta_nq_t *
mtaMessageEnqueue::enqueueStart(const char *env_from, size_t env_from_len,
				const char *env_id, size_t env_id_len,
				mta_dq_t *dq, mta_item_list_t *items)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException)
{
     int ires;
     mta_nq_t *nq = 0;

     if (!env_from)
	  throw mtaExceptionBadArgs(badfrom, 1);

     if (dq)
     {
	  // When a dequeue context is supplied, we ignore any envelope ID
	  // and use the envelope ID from the dequeue context
	  ires = mtaEnqueueStart(&nq, env_from, env_from_len,
				 MTA_DQ_CONTEXT, dq,
				 items ? MTA_ITEM_LIST : 0, items, 0);
     }
     else
     {
	  if (env_id)
	       ires = mtaEnqueueStart(&nq, env_from, env_from_len,
				      MTA_ENV_ID, env_id, env_id_len,
				      items ? MTA_ITEM_LIST : 0, items, 0);
	  else
	       ires = mtaEnqueueStart(&nq, env_from, env_from_len,
				      items ? MTA_ITEM_LIST : 0, items, 0);
     }

     // Check the outcome
     if (ires != MTA_OK)
	  MTASDK_NAMESPACE::throwException(ires, badnqstart);
     else if (!nq)
	  // Should never happen: nq == 0 should imply ires != MTA_OK
	  throw mtaExceptionNo(badnqstart2);

     // Success!
     return(nq);
}


mta_nq_t *
mtaMessageEnqueue::enqueueStart(const char *env_from, size_t env_from_len,
				const char *env_id, size_t env_id_len,
				mta_dq_t *dq, const mtaEnqueueProp& prop)
     THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
	     mtaException)
{
     mta_item_list_t *items;
     mta_nq_t *nq = 0;

     try
     {
	  items = prop.getItemList();
     }
     catch (mtaException& e)
     {
	  throw mtaExceptionNoMem(baditems);
     }
     catch (...)
     {
	  throw;
     }

     try {
	  nq = mtaMessageEnqueue::enqueueStart(env_from, env_from_len,
					       env_id, env_id_len, dq, items);
     }
     catch (...)
     {
	  if (items)
	       prop.disposeItemList(items);
	  throw;
     }
     return(nq);
}


mta_nq_t *
mtaMessageEnqueue::enqueueStart(const mtaMessageEnqueue& rhs)
     THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException)
{
     return(mtaMessageEnqueue::enqueueStart(rhs.getEnvelopeFrom(),
					    rhs.getEnvelopeFromLength(),
					    0, 0, 0, 0));
}


const char *
mtaMessageEnqueue::toString()
{
     if (!pMsgNqToString)
     {
	  char buf[512];

	  const char *from = mtaMessageEnqueue::getEnvelopeFrom();
	  size_t flen = mtaMessageEnqueue::getEnvelopeFromLength();
	  const char *notify= mtaMessageEnqueue::getNotifyStr();
	  const char *envid = mtaMessageEnqueue::getEnvelopeId();
	  size_t elen = mtaMessageEnqueue::getEnvelopeIdLength();

	  snprintf(buf, sizeof(buf),
		   "MAIL FROM:<%.*s> NOTIFY=%s ENVID=%.*s (nq=%zx; "
		   "dflags=%zd; expand-aliases=%s; sort-address=%s; "
		   "fragment block limit=%zu; fragment-line-limit=%zu)",
		   static_cast<int>(from ? flen : 3), from ? from : "?@?",
		   notify ? notify : "???",
		   static_cast<int>(envid ? elen : 3), envid ? envid : "???",
		   mtaMessageEnqueue::getDeliveryFlags(),
		   mtaMessageEnqueue::getDeliveryFlags(),
		   mtaMessageEnqueue::getExpandAliases() ? "true" : "false",
		   mtaMessageEnqueue::getSortAddresses() ? "true" : "false",
		   mtaMessageEnqueue::getFragmentBlockLimit(),
		   mtaMessageEnqueue::getFragmentLineLimit());
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
	  pMsgNqToString = pTmp;
     }
     return(pMsgNqToString->c_str());
}
