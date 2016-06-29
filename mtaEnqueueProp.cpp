#include <stdarg.h>
#include <string.h>
#include "mtasdk.hpp"
#include "mtasdk.h"


#if !defined(_WIN32)
#  include <stdio.h>
#else
#  define vsnprintf _vsnprintf
#endif

using namespace MTASDK_NAMESPACE;

#if 0
static const char *badagain  = "Unable to create a mutex";
#endif
static const char *badassign = "Insufficient virtual memory to store the "
                                "supplied string; the assign() member function "
                                "for C++ std::string threw an exception";
static const char *badcopy   = "Unable to copy an item list";
#if 0
static const char *badmem    = "Insufficient memory to create a mutex";
#endif
static const char *badnflags = "Unknown bits set in notify flag bit mask";
static const char *badnew    = "Unable to allocate memory for a string buffer";

mtaEnqueueProp::mtaEnqueueProp()
     THROWS(mtaExceptionNoMem) :
	  pNqDqCtx(0),
	  pNqToString(0),
	  nNqExpandLimit(((size_t)-1) >> 1),
	  nNqFlagsDelivery(0),
	  nNqFlagsNotary(NOTIFY_DEFAULT),
	  nNqFragmentBlockLimit(((size_t)-1) >> 1),
	  nNqFragmentLineLimit(((size_t)-1) >> 1),
	  bNqExpandAliases(true),
	  bNqSortAddresses(true),
	  bNqSetExpandLimit(false),
	  bNqSetFlagsDelivery(false),
	  bNqSetFlagsNotary(false),
	  bNqSetFragmentBlockLimit(false),
	  bNqSetFragmentLineLimit(false),
	  bNqSetExpandAliases(false),
	  bNqSetSortAddresses(false),
	  sNqChan()
{
     try
     {
	  sNqChan.assign("");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaEnqueueProp::mtaEnqueueProp(const char *chan)
     THROWS(mtaExceptionNoMem) :
	  pNqDqCtx(0),
	  pNqToString(0),
	  nNqExpandLimit(((size_t)-1) >> 1),
	  nNqFlagsDelivery(0),
	  nNqFlagsNotary(NOTIFY_DEFAULT),
	  nNqFragmentBlockLimit(((size_t)-1) >> 1),
	  nNqFragmentLineLimit(((size_t)-1) >> 1),
	  bNqExpandAliases(true),
	  bNqSortAddresses(true),
	  bNqSetExpandLimit(false),
	  bNqSetFlagsDelivery(false),
	  bNqSetFlagsNotary(false),
	  bNqSetFragmentBlockLimit(false),
	  bNqSetFragmentLineLimit(false),
	  bNqSetExpandAliases(false),
	  bNqSetSortAddresses(false),
	  sNqChan()
{
     try
     {
	  if (chan)
	       sNqChan.assign(chan);
	  else
	       sNqChan.assign("");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaEnqueueProp::mtaEnqueueProp(const std::string& chan)
     THROWS(mtaExceptionNoMem) :
	  pNqDqCtx(0),
	  pNqToString(0),
	  nNqExpandLimit(((size_t)-1) >> 1),
	  nNqFlagsDelivery(0),
	  nNqFlagsNotary(NOTIFY_DEFAULT),
	  nNqFragmentBlockLimit(((size_t)-1) >> 1),
	  nNqFragmentLineLimit(((size_t)-1) >> 1),
	  bNqExpandAliases(true),
	  bNqSortAddresses(true),
	  bNqSetExpandLimit(false),
	  bNqSetFlagsDelivery(false),
	  bNqSetFlagsNotary(false),
	  bNqSetFragmentBlockLimit(false),
	  bNqSetFragmentLineLimit(false),
	  bNqSetExpandAliases(false),
	  bNqSetSortAddresses(false),
	  sNqChan()
{
     try
     {
	  sNqChan.assign(chan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaEnqueueProp::mtaEnqueueProp(const mtaEnqueueProp& rhs)
     THROWS(mtaExceptionNoMem) :
	  pNqDqCtx(rhs.pNqDqCtx),
	  pNqToString(0),
	  nNqExpandLimit(rhs.nNqExpandLimit),
	  nNqFlagsDelivery(rhs.nNqFlagsDelivery),
	  nNqFlagsNotary(rhs.nNqFlagsNotary),
	  nNqFragmentBlockLimit(rhs.nNqFragmentBlockLimit),
	  nNqFragmentLineLimit(rhs.nNqFragmentLineLimit),
	  bNqExpandAliases(rhs.bNqExpandAliases),
	  bNqSortAddresses(rhs.bNqSortAddresses),
	  bNqSetExpandLimit(rhs.bNqSetExpandLimit),
	  bNqSetFlagsDelivery(rhs.bNqSetFlagsDelivery),
	  bNqSetFlagsNotary(rhs.bNqSetFlagsNotary),
	  bNqSetFragmentBlockLimit(rhs.bNqSetFragmentBlockLimit),
	  bNqSetFragmentLineLimit(rhs.bNqSetFragmentLineLimit),
	  bNqSetExpandAliases(rhs.bNqSetExpandAliases),
	  bNqSetSortAddresses(rhs.bNqSetSortAddresses),
	  sNqChan()
{
     try
     {
	  sNqChan.assign(rhs.sNqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


void
mtaEnqueueProp::clean()
{
     if (!pNqToString)
	  return;

     std::string *pTmp = pNqToString;
     pNqToString = 0;
     delete pTmp;
}


mtaEnqueueProp::~mtaEnqueueProp()
{
     mtaEnqueueProp::clean();
}


mtaEnqueueProp&
mtaEnqueueProp::operator=(const mtaEnqueueProp& rhs)
     THROWS(mtaExceptionNoMem)
{
     // Ignore self assignment
     if (this == &rhs)
	  return(*this);

     mtaEnqueueProp::clean();

     try
     {
	  sNqChan.assign(rhs.sNqChan);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
	  
     pNqDqCtx                 = rhs.pNqDqCtx;
     nNqExpandLimit           = rhs.nNqExpandLimit;
     nNqFlagsDelivery         = rhs.nNqFlagsDelivery;
     nNqFlagsNotary           = rhs.nNqFlagsNotary;
     nNqFragmentBlockLimit    = rhs.nNqFragmentBlockLimit;
     nNqFragmentLineLimit     = rhs.nNqFragmentLineLimit;
     bNqExpandAliases         = rhs.bNqExpandAliases;
     bNqSortAddresses           = rhs.bNqSortAddresses;
     bNqSetExpandLimit        = rhs.bNqSetExpandLimit;
     bNqSetFlagsDelivery      = rhs.bNqSetFlagsDelivery;
     bNqSetFlagsNotary        = rhs.bNqSetFlagsNotary;
     bNqSetFragmentBlockLimit = rhs.bNqSetFragmentBlockLimit;
     bNqSetFragmentLineLimit  = rhs.bNqSetFragmentLineLimit;
     bNqSetExpandAliases      = rhs.bNqSetExpandAliases;
     bNqSetSortAddresses      = rhs.bNqSetSortAddresses;

     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::sortAddresses(bool sort)
{
     mtaEnqueueProp::setSortAddresses(sort);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::expandAliases(bool expand)
{
     mtaEnqueueProp::setExpandAliases(expand);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::deliveryFlags(size_t dflags, bool merge)
{
     mtaEnqueueProp::setDeliveryFlags(dflags, merge);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::dequeueContext(const mtaMessageDequeue& dq)
{
     mtaEnqueueProp::setDequeueContext(dq);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::expandLimit(size_t max_recipients)
{
     mtaEnqueueProp::setExpandLimit(max_recipients);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::fragmentBlockLimit(size_t max_blocks)
{
     mtaEnqueueProp::setFragmentBlockLimit(max_blocks);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::fragmentLineLimit(size_t max_lines)
{
     mtaEnqueueProp::setFragmentLineLimit(max_lines);
     return(*this);
}


mtaEnqueueProp&
mtaEnqueueProp::notifyFlags(size_t nflags)
     THROWS(mtaExceptionBadArgs)
{
     mtaEnqueueProp::setNotifyFlags(nflags);
     return(*this);
}


void
mtaEnqueueProp::setSortAddresses(bool sort)
{
     bNqSortAddresses = sort;
     bNqSetSortAddresses = true;
     mtaEnqueueProp::clean();
}


bool
mtaEnqueueProp::getSortAddresses() const
{
     return(bNqSortAddresses);
}


void
mtaEnqueueProp::setExpandAliases(bool expand)
{
     bNqExpandAliases = expand;
     bNqSetExpandAliases = true;
     mtaEnqueueProp::clean();
}


bool
mtaEnqueueProp::getExpandAliases() const
{
     return(bNqExpandAliases);
}


void
mtaEnqueueProp::setDeliveryFlags(size_t dflags, bool merge)
{
     if (merge)
	  nNqFlagsDelivery |= dflags;
     else
	  nNqFlagsDelivery = dflags;
     bNqSetFlagsDelivery = true;
     mtaEnqueueProp::clean();
}


size_t
mtaEnqueueProp::getDeliveryFlags() const
{
     return(nNqFlagsDelivery);
}


void
mtaEnqueueProp::setDequeueContext(const mtaMessageDequeue& dq)
{
     pNqDqCtx = dq.getDequeueContext();
     mtaEnqueueProp::clean();
}


void
mtaEnqueueProp::setExpandLimit(size_t max_recipients)
{
     nNqExpandLimit = max_recipients ? max_recipients : ((size_t)-1) >> 1;
     bNqSetExpandLimit = true;
     mtaEnqueueProp::clean();
}


size_t
mtaEnqueueProp::getExpandLimit() const
{
     return(nNqExpandLimit);
}


void
mtaEnqueueProp::setFragmentBlockLimit(size_t max_blocks)
{
     nNqFragmentBlockLimit = max_blocks ? max_blocks : ((size_t)-1) >> 1;
     bNqSetFragmentBlockLimit = true;
     mtaEnqueueProp::clean();
}


size_t
mtaEnqueueProp::getFragmentBlockLimit() const
{
     return(nNqFragmentBlockLimit);
}


void
mtaEnqueueProp::setFragmentLineLimit(size_t max_lines)
{
     nNqFragmentLineLimit = max_lines ? max_lines : ((size_t)-1) >> 1;
     bNqSetFragmentLineLimit = true;
     mtaEnqueueProp::clean();
}


size_t
mtaEnqueueProp::getFragmentLineLimit() const
{
     return(nNqFragmentLineLimit);
}


void
mtaEnqueueProp::setNotifyFlags(size_t nflags)
     THROWS(mtaExceptionBadArgs)
{
     if (!nflags || nflags & ~NOTIFY_RANGE_CHECK)
	  throw mtaExceptionBadArgs(badnflags, 1);
     nNqFlagsNotary = nflags;
     bNqSetFlagsNotary = true;
     mtaEnqueueProp::clean();
}


size_t
mtaEnqueueProp::getNotifyFlags() const
{
     return(nNqFlagsNotary);
}


const char *
mtaEnqueueProp::getNotifyStr() const
{
     return(mtaAddressProp::getNotifyStr(nNqFlagsNotary));
}


bool
mtaEnqueueProp::equals(const mtaEnqueueProp& rhs) const
{
     if (this == &rhs)
	  return(true);

     if (pNqDqCtx              == rhs.pNqDqCtx              &&
	 nNqExpandLimit        == rhs.nNqExpandLimit        &&
	 nNqFlagsDelivery      == rhs.nNqFlagsDelivery      &&
	 nNqFlagsNotary        == rhs.nNqFlagsNotary        &&
	 nNqFragmentBlockLimit == rhs.nNqFragmentBlockLimit &&
	 nNqFragmentLineLimit  == rhs.nNqFragmentLineLimit  &&
	 bNqExpandAliases      == rhs.bNqExpandAliases      &&
	 bNqSortAddresses      == rhs.bNqSortAddresses      &&
	 sNqChan.size()        == rhs.sNqChan.size()        &&
	 (sNqChan.size() == 0 ||
	  0 == strncasecmp(sNqChan.data(), rhs.sNqChan.data(), sNqChan.size())))
	  return(true);
     else
	  return(false);
}


mta_item_list_t *
mtaEnqueueProp::getItemList() const
     THROWS(mtaExceptionNoMem)
{
     mta_item_list_t *items = 0;

     // mtaItemList*() disposes of the item list when there's
     // an error such as insufficient memory.  As such we don't
     // need to try...catch here

     if (pNqDqCtx)
	  items = mtaEnqueueProp::itemStr(
	       items, MTA_DQ_CONTEXT, static_cast<char *>(pNqDqCtx), 0);
     if (sNqChan.size())
	  items = mtaEnqueueProp::itemStr(
	       items, MTA_CHANNEL, sNqChan.data(), sNqChan.size());
     if (bNqSetExpandLimit)
	  items = mtaEnqueueProp::itemInt(
	       items, MTA_EXPAND_LIMIT, nNqExpandLimit);
     if (bNqSetFlagsDelivery)
	  items = mtaEnqueueProp::itemInt(
	       items, MTA_DELIVERY_FLAGS, nNqFlagsDelivery);
     if (bNqSetFlagsNotary)
	  items = mtaEnqueueProp::itemInt(
	       items, MTA_NOTIFY_FLAGS, nNqFlagsNotary);
     if (bNqSetFragmentBlockLimit)
	  items = mtaEnqueueProp::itemInt(
	       items, MTA_FRAGMENT_BLOCKS, nNqFragmentBlockLimit);
     if (bNqSetFragmentLineLimit)
	  items = mtaEnqueueProp::itemInt(
	       items, MTA_FRAGMENT_LINES, nNqFragmentLineLimit);
     if (bNqSetExpandAliases)
	  items = mtaEnqueueProp::itemBool(
	       items, MTA_ALIAS_EXPAND, MTA_ALIAS_NOEXPAND, bNqExpandAliases);
     if (bNqSetSortAddresses)
	  items = mtaEnqueueProp::itemBool(
	       items, MTA_ADR_SORT, MTA_ADR_NOSORT, bNqSortAddresses);

     // We wash our hands of this item list; it's up to the caller to free it
     return(items);
}


void
mtaEnqueueProp::disposeItemList(mta_item_list_t *items)
{
     if (items)
	  mtaItemListDispose(items);
}


mta_item_list_t *
mtaEnqueueProp::itemBool(mta_item_list_t *items,
			 int item_code_true, int item_code_false, bool val)
     THROWS(mtaExceptionNoMem)
{
     mta_item_list_t *pTmp = mtaItemListInt(
	  items, val ? item_code_true : item_code_false, 0);
     if (pTmp)
	  return(pTmp);
     throw mtaExceptionNoMem();
}


mta_item_list_t *
mtaEnqueueProp::itemInt(mta_item_list_t *items, int item_code, size_t val)
     THROWS(mtaExceptionNoMem)
{
     mta_item_list_t *pTmp = mtaItemListInt(items, item_code, val);
     if (pTmp)
	  return(pTmp);
     throw mtaExceptionNoMem();
}


mta_item_list_t *
mtaEnqueueProp::itemStr(mta_item_list_t *items,
			int item_code, const char *str, size_t len)
     THROWS(mtaExceptionNoMem)
{
     mta_item_list_t *pTmp = mtaItemListStr(items, item_code, str, len);
     if (pTmp)
	  return(pTmp);
     throw mtaExceptionNoMem();
}


mta_item_list_t *
mtaEnqueueProp::itemCopy(const mta_item_list_t *items)
     THROWS(mtaExceptionNoMem)
{
     if (!items)
	  return((mta_item_list_t *)0);
     mta_item_list_t *pTmp = mtaItemListCopy(items);
     if (pTmp)
	  return(pTmp);
     throw mtaExceptionNoMem(badcopy);
}

#if __GNUC__ > 2 || defined(__llvm__)
__attribute__((__format__(__printf__, 2, 3)))
#endif
static void
appendstr(std::string *str, const char *fmt, ...)
{
     char buf[64];
     va_list ap;

     va_start(ap, fmt);
     vsnprintf(buf, sizeof(buf), fmt, ap);
     va_end(ap);
     if (str->size())
	  str->append("; ");
     str->append(buf);
}


const char *
mtaEnqueueProp::toString()
{
     if (!pNqToString)
     {
	  char buf[512];

	  std::string *pTmp = 0;
	  try
	  {
	       pTmp = new std::string(buf);
	       if (bNqSetFlagsDelivery)
		    appendstr(pTmp, "MTA_DELIVERY_FLAGS=%zu (0x%zx)",
			      nNqFlagsDelivery, nNqFlagsDelivery);
	       if (bNqSetFlagsNotary)
		    appendstr(pTmp, "MTA_NOTIFY_FLAGS=%zu (0x%zx)",
			      nNqFlagsNotary, nNqFlagsNotary);
	       if (bNqSetFragmentBlockLimit)
		    appendstr(pTmp, "MTA_FRAGMENT_BLOCKS=%zu",
			      nNqFragmentBlockLimit);
	       if (bNqSetFragmentLineLimit)
		    appendstr(pTmp, "MTA_FRAGMENT_LINES=%zu",
			      nNqFragmentLineLimit);
	       if (bNqSetExpandLimit)
		    appendstr(pTmp, "MTA_EXPAND_LIMIT=%zu", nNqExpandLimit);
	       if (bNqSetExpandAliases)
		    appendstr(pTmp, "MTA_ALIAS_%sEXPAND",
			      bNqExpandAliases ? "" : "NO");
	       if (bNqSetSortAddresses)
		    appendstr(pTmp, "MTA_ADR_%sSORT",
			      bNqSortAddresses ? "" : "NO");

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
	  pNqToString = pTmp;
     }
     return(pNqToString->c_str());
}
