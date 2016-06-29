#ifndef __MTAENQUEUEPROP_HPP__

#define __MTAENQUEUEPROP_HPP__

namespace MTASDK_NAMESPACE {

class mtaEnqueueProp
{

public:

     mtaEnqueueProp() THROWS(mtaExceptionNoMem);
     mtaEnqueueProp(const char *channel) THROWS(mtaExceptionNoMem);
     mtaEnqueueProp(const mtaEnqueueProp& rhs) THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     mtaEnqueueProp(const std::string& channel) THROWS(mtaExceptionNoMem);
#endif

     virtual ~mtaEnqueueProp();

     mtaEnqueueProp& operator=(const mtaEnqueueProp& rhs)
	  THROWS(mtaExceptionNoMem);

     mtaEnqueueProp& deliveryFlags(size_t dflags = 0, bool merge = true);
     mtaEnqueueProp& dequeueContext(const mtaMessageDequeue& dq);
     mtaEnqueueProp& expandAliases(bool expand = true);
     mtaEnqueueProp& expandLimit(size_t max_recipients = ((size_t)-1) >> 1);
     mtaEnqueueProp& fragmentBlockLimit(size_t max_blocks = ((size_t)-1) >> 1);
     mtaEnqueueProp& fragmentLineLimit(size_t max_lines = ((size_t)-1) >> 1);
     mtaEnqueueProp& notifyFlags(size_t nflags = NOTIFY_DEFAULT)
	  THROWS(mtaExceptionBadArgs);
     mtaEnqueueProp& sortAddresses(bool sort = true);

     void setSortAddresses(bool sort = true);
     bool getSortAddresses() const;

     void setDeliveryFlags(size_t dflags = 0, bool merge = true);
     size_t getDeliveryFlags() const;

     void setDequeueContext(const mtaMessageDequeue& dq);

     void setExpandAliases(bool expand = true);
     bool getExpandAliases() const;

     void setExpandLimit(size_t max_recipients = ((size_t)-1) >> 1);
     size_t getExpandLimit() const;

     void setFragmentBlockLimit(size_t max_blocks = ((size_t)-1) >> 1);
     size_t getFragmentBlockLimit() const;

     void setFragmentLineLimit(size_t max_lines = ((size_t)-1) >> 1);
     size_t getFragmentLineLimit() const;

     void setNotifyFlags(size_t nflags = NOTIFY_DEFAULT)
	  THROWS(mtaExceptionBadArgs);
     size_t getNotifyFlags() const;
     const char *getNotifyStr() const;

     bool equals(const mtaEnqueueProp& rhs) const;
     const char *toString();

private:

     friend class mtaMessageEnqueue;

     void clean();

     mta_item_list_t *getItemList() const THROWS(mtaExceptionNoMem);
     static void disposeItemList(mta_item_list_t *list);

     static mta_item_list_t *itemCopy(const mta_item_list_t *items)
	  THROWS(mtaExceptionNoMem);
     static mta_item_list_t *itemInt(mta_item_list_t *items, int item_code,
				     size_t val)
	  THROWS(mtaExceptionNoMem);
     static mta_item_list_t *itemStr(mta_item_list_t *items, int item_code,
				     const char *str, size_t len = 0)
	  THROWS(mtaExceptionNoMem);
     static mta_item_list_t *itemBool(mta_item_list_t *items,
				      int item_code_true, int item_code_false,
				      bool val)
	  THROWS(mtaExceptionNoMem);

     mta_dq_t    *pNqDqCtx;
     std::string *pNqToString;

     size_t       nNqExpandLimit;
     size_t       nNqFlagsDelivery;
     size_t       nNqFlagsNotary;
     size_t       nNqFragmentBlockLimit;
     size_t       nNqFragmentLineLimit;

     bool         bNqExpandAliases;
     bool         bNqSortAddresses;

     bool         bNqSetExpandLimit;
     bool         bNqSetFlagsDelivery;
     bool         bNqSetFlagsNotary;
     bool         bNqSetFragmentBlockLimit;
     bool         bNqSetFragmentLineLimit;
     bool         bNqSetExpandAliases;
     bool         bNqSetSortAddresses;

     std::string  sNqChan;

};

};

#endif
