#include "mtasdk.hpp"
#include "mtasdk.h"

#if !defined(_WIN32)
#  include <stdio.h>
#else
#  define snprintf _snprintf
#endif

using namespace MTASDK_NAMESPACE;

#include "map_atypes.h"

static const char *badatype  = "Unknown address type specified";
static const char *baddqctx  = "Invalid mtaAddressDequeue object supplied";
static const char *baddqinf  = "Error returned by mtaDequeueInfo()";
static const char *baditems  = "Unable to allocate memory for an item list";
static const char *badnew    = "Unable to allocate memory for a string buffer";
static const char *badnflags = "Unknown bits set in notify flag bit mask";

mtaAddressProp::mtaAddressProp() :
     pAdrDqCtx(0),
     pAdrItemList(0),
     pAdrToString(0),
     nAdrFlagsDelivery(0),
     nAdrFlagsNotary(NOTIFY_DEFAULT),
     nAdrFlagsType(ADDR_DEFAULT),
     bAdrFlagsDeliveryMerge(true),
     bAdrSetFlagsDelivery(false),
     bAdrSetFlagsNotary(false),
     bAdrSetFlagsType(false)
{
}

mtaAddressProp::mtaAddressProp(size_t atype, size_t nflags, size_t dflags)
     THROWS(mtaExceptionBadArgs) :
	  pAdrDqCtx(0),
	  pAdrItemList(0),
	  pAdrToString(0),
	  nAdrFlagsDelivery(dflags),
	  nAdrFlagsNotary(NOTIFY_DEFAULT),
	  nAdrFlagsType(ADDR_DEFAULT),
	  bAdrFlagsDeliveryMerge(true),
	  bAdrSetFlagsDelivery(dflags != 0 ? true : false),
	  bAdrSetFlagsNotary(nflags != NOTIFY_DEFAULT ? true : false),
	  bAdrSetFlagsType(false)
{
     if (atype < ADDR_FIRST || atype > ADDR_LAST)
     {
	  bAdrSetFlagsType = false;
	  throw mtaExceptionBadArgs(badatype, 1);
     }
     else
	  nAdrFlagsType = atype;

     if (!nflags || nflags & ~NOTIFY_RANGE_CHECK)
     {
	  bAdrSetFlagsNotary = false;
	  throw mtaExceptionBadArgs(badnflags, 2);
     }
     else
	  nAdrFlagsNotary = nflags;
}


mtaAddressProp::mtaAddressProp(const mtaAddressProp& rhs) :
     pAdrDqCtx(rhs.pAdrDqCtx),
     pAdrItemList(0),
     pAdrToString(0),
     nAdrFlagsDelivery(rhs.nAdrFlagsDelivery),
     nAdrFlagsNotary(rhs.nAdrFlagsNotary),
     nAdrFlagsType(rhs.nAdrFlagsType),
     bAdrFlagsDeliveryMerge(rhs.bAdrFlagsDeliveryMerge),
     bAdrSetFlagsDelivery(false),
     bAdrSetFlagsNotary(false),
     bAdrSetFlagsType(false)
{
}


mtaAddressProp::~mtaAddressProp()
{
     mtaAddressProp::clean();
}


void
mtaAddressProp::clean()
{
     if (pAdrToString)
     {
	  std::string *pTmp = pAdrToString;
	  pAdrToString = 0;
	  delete pTmp;
     }
     if (pAdrItemList)
     {
	  mta_item_list_t *pTmp = pAdrItemList;
	  pAdrItemList = 0;
	  mtaItemListDispose(pTmp);
     }
}


mtaAddressProp&
mtaAddressProp::operator=(const mtaAddressProp& rhs)
{
     // Ignore self assignment
     if (this == &rhs)
	  return(*this);

     // We assume that rhs isn't corrupted and further that
     // rhs.nAdrFlagsType and rhs.nAdrFlagsNotary have valid values.
     // Consequently, we don't recheck them here.

     pAdrDqCtx              = rhs.pAdrDqCtx;
     nAdrFlagsDelivery      = rhs.nAdrFlagsDelivery;
     nAdrFlagsNotary        = rhs.nAdrFlagsNotary;
     nAdrFlagsType          = rhs.nAdrFlagsType;
     bAdrFlagsDeliveryMerge = rhs.bAdrFlagsDeliveryMerge;
     bAdrSetFlagsDelivery   = rhs.bAdrSetFlagsDelivery;
     bAdrSetFlagsNotary     = rhs.bAdrSetFlagsNotary;
     bAdrSetFlagsType       = rhs.bAdrSetFlagsType;

     // Note that we need to rebuild the toString() and item_list data
     mtaAddressProp::clean();

     return(*this);
}


mtaAddressProp&
mtaAddressProp::operator=(const mtaAddressDequeue& rhs)
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaAddressProp::getFromDequeue(rhs, true);
     return(*this);
}


mtaAddressProp&
mtaAddressProp::dequeueContext(const mtaAddressDequeue& ctx)
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaAddressProp::getFromDequeue(ctx, true);
     return(*this);
}


mtaAddressProp&
mtaAddressProp::deliveryFlags(size_t dflags, bool merge)
{
     bAdrSetFlagsDelivery = true;
     bAdrFlagsDeliveryMerge = merge;
     if (merge)
	  nAdrFlagsDelivery |= dflags;
     else
	  nAdrFlagsDelivery = dflags;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();

     return(*this);
}


mtaAddressProp&
mtaAddressProp::notifyFlags(size_t nflags)
     THROWS(mtaExceptionBadArgs)
{
     if (!nflags || nflags & ~NOTIFY_RANGE_CHECK)
	  throw mtaExceptionBadArgs(badnflags, 1);
     bAdrSetFlagsNotary = true;
     nAdrFlagsNotary = nflags & NOTIFY_RANGE_CHECK;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();

     return(*this);
}


mtaAddressProp&
mtaAddressProp::type(size_t atype)
     THROWS(mtaExceptionBadArgs)
{
     if (atype < ADDR_FIRST || atype > ADDR_LAST)
	  throw mtaExceptionBadArgs(badatype, 1);
     bAdrSetFlagsType = true;
     nAdrFlagsType = atype;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();

     return(*this);
}


void
mtaAddressProp::setDequeueContext(const mtaAddressDequeue& ctx)
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     mtaAddressProp::getFromDequeue(ctx, true);
}


void
mtaAddressProp::setDeliveryFlags(size_t dflags, bool merge)
{
     bAdrSetFlagsDelivery = true;
     bAdrFlagsDeliveryMerge = merge;
     if (merge)
	  nAdrFlagsDelivery |= dflags;
     else
	  nAdrFlagsDelivery = dflags;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();
}


size_t
mtaAddressProp::getDeliveryFlags() const
{
     return(nAdrFlagsDelivery);
}


void
mtaAddressProp::setNotifyFlags(size_t nflags)
     THROWS(mtaExceptionBadArgs)
{
     if (!nflags || nflags & ~NOTIFY_RANGE_CHECK)
	  throw mtaExceptionBadArgs(badnflags, 1);
     bAdrSetFlagsNotary = true;
     nAdrFlagsNotary = nflags & NOTIFY_RANGE_CHECK;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();
}


size_t
mtaAddressProp::getNotifyFlags() const
{
     return(nAdrFlagsNotary);
}


const char *
mtaAddressProp::getNotifyStr() const
{
     return(mtaAddressProp::getNotifyStr(nAdrFlagsNotary));
}


const char *
mtaAddressProp::getNotifyStr(size_t nflags)
{
     size_t mask;
     static const char s0f0d0[] = "NEVER";
     static const char s0f0d1[] = "DELAY";
     static const char s0f1d0[] = "FAILURE";
     static const char s0f1d1[] = "FAILURE,DELAY";
     static const char s1f0d0[] = "SUCCESS";
     static const char s1f0d1[] = "SUCCESS,DELAY";
     static const char s1f1d0[] = "SUCCESS,FAILURE";
     static const char s1f1d1[] = "SUCCESS,FAILURE,DELAY";
     static const char *combos[8] = {s0f0d0, s0f0d1, s0f1d0, s0f1d1,
				     s1f0d0, s1f0d1, s1f1d0, s1f1d1};

     mask = (nflags & NOTIFY_SUCCESS) ? 4 : 0;
     if (nflags & NOTIFY_FAILURE)
	  mask |= 2;
     if (nflags & NOTIFY_DELAY)
	  mask |= 1;
     return(combos[mask]);
}


void
mtaAddressProp::setType(size_t atype)
     THROWS(mtaExceptionBadArgs)
{
     if (atype < ADDR_FIRST || atype > ADDR_LAST)
	  throw mtaExceptionBadArgs(badatype, 1);
     bAdrSetFlagsType = true;
     nAdrFlagsType = atype;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();
}


size_t
mtaAddressProp::getType() const
{
     return(nAdrFlagsType);
}


const char *
mtaAddressProp::getTypeStr() const
{
     static const char str1[] = "Env To";
     static const char str2[] = "Env To, Hdr To";
     static const char str3[] = "Env To, Hdr Cc";
     static const char str4[] = "Env To, Hdr Bcc";
     static const char str5[] = "Hdr To";
     static const char str6[] = "Hdr Cc";
     static const char str7[] = "Hdr Bcc";
     static const char unknown[] = "?Bad Value?";
     static const char *strings[ADDR_LAST + 2] = {
	  unknown, str1, str2, str3, str4, str5, str6, str7, unknown};

     if (ADDR_FIRST <= nAdrFlagsType && nAdrFlagsType <= ADDR_LAST)
	  return(strings[nAdrFlagsType]);
     else
	  return(strings[ADDR_LAST + 1]);
}


void
mtaAddressProp::getFromDequeue(const mtaAddressDequeue& ctx, bool reset)
     THROWS2(mtaExceptionBadArgs, mtaException)
{
     if (reset)
	  nAdrFlagsType = ADDR_ENV_TO;

     // Note that we need to rebuild the toString() and item list data
     mtaAddressProp::clean();

     // Make sure that the mtaAddressDequeue reference is still good
     mta_dq_t *pDqCtx = ctx.pAdrDqCtx;
     if (!pDqCtx)
	  throw mtaExceptionBadArgs(baddqctx, 1);

     // Now pull the information from the dequeue context
     int ires = mtaDequeueInfo(pDqCtx,
			       MTA_DELIVERY_FLAGS, &nAdrFlagsDelivery,
			       MTA_NOTIFY_FLAGS, &nAdrFlagsNotary,
			       0);
     // Return on success
     if (ires == MTA_OK)
     {
	  bAdrSetFlagsDelivery = false;
	  bAdrSetFlagsNotary   = false;
	  bAdrSetFlagsType     = false;
 	  return;
     }

     // Otherwise toss an exception
     throwException(ires, baddqinf);
}


const char *
mtaAddressProp::toString()
{
     if (!pAdrToString)
     {
 	  char buf[256];

 	  // Note that snprintf() returns the length THAT WOULD HAVE
 	  // printed were the buffer size unlimited.  It is NOT the
 	  // length which did print....
 	  snprintf(buf, sizeof(buf), "type=%s (%zu); "
                   "notify=%s (0x%zx); dflags=%zu (0x%zx); dq=0x%p",
 		   mtaAddressProp::getTypeStr(), nAdrFlagsType,
 		   mtaAddressProp::getNotifyStr(), nAdrFlagsNotary,
 		   nAdrFlagsDelivery, nAdrFlagsDelivery, pAdrDqCtx);

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
	  pAdrToString = pTmp;
     }
     return(pAdrToString->c_str());
}


bool
mtaAddressProp::equals(const mtaAddressProp& prop) const
{
     if (nAdrFlagsDelivery != prop.nAdrFlagsNotary ||
	 nAdrFlagsNotary   != prop.nAdrFlagsNotary ||
	 nAdrFlagsType     != prop.nAdrFlagsType   ||
	 pAdrDqCtx         != prop.pAdrDqCtx)
	  return(false);
     else
	  return(true);
}


void
mtaAddressProp::assign(size_t atype, size_t nflags, size_t dflags, mta_dq_t *dq)
     THROWS(mtaExceptionBadArgs)
{
     // Sanity checks
     if (atype < ADDR_FIRST || atype > ADDR_LAST)
	  throw mtaExceptionBadArgs(badatype, 1);
     if (!nflags || nflags & ~NOTIFY_RANGE_CHECK)
	  throw mtaExceptionBadArgs(badnflags, 2);

     // Wipe any toString() and item list data
     mtaAddressProp::clean();

     // And set the values
     pAdrDqCtx              = dq;
     nAdrFlagsType          = atype;
     nAdrFlagsDelivery      = dflags;
     nAdrFlagsNotary        = nflags;
     bAdrFlagsDeliveryMerge = true;
}


mta_item_list_t *
mtaAddressProp::getItemList() const
     THROWS(mtaExceptionNoMem)
{
     // Return the item list if it already exists
     if (pAdrItemList)
	  return(pAdrItemList);

     // We need to build the item list
     mta_item_list_t *items = 0;

     if (pAdrDqCtx)
     {
	  items = mtaItemListStr(items, MTA_DQ_CONTEXT,
				 static_cast<const char *>(pAdrDqCtx), 0);
	  if (!items)
	       goto bad;
     }

     if (bAdrSetFlagsDelivery)
     {
	  items = mtaItemListInt(items,
				 bAdrFlagsDeliveryMerge ?
				 MTA_DELIVERY_FLAGS : MTA_DELIVERY_FLAGS_ABS,
				 nAdrFlagsDelivery); 
	  if (!items)
	       goto bad;
     }

     if (bAdrSetFlagsNotary)
     {
	  items = mtaItemListInt(items, MTA_NOTIFY_FLAGS, nAdrFlagsNotary);
	  if (!items)
	       goto bad;
     }

     if (bAdrSetFlagsType)
     {
	  items = mtaItemListInt(items, map_atypes[nAdrFlagsType], 0);
	  if (!items)
	       goto bad;
     }
     
     pAdrItemList = items;
     return(pAdrItemList);

bad:
     throw mtaExceptionNoMem(baditems);
     return((mta_item_list_t *)0);
}
