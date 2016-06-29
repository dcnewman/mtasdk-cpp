#if !defined(__MTAADDRESSPROP_HPP__)

#define __MTAADDRESSPROP_HPP__

namespace MTASDK_NAMESPACE {

class mtaAddressDequeue;

class mtaAddressProp
{

public:

     mtaAddressProp();
     mtaAddressProp(const mtaAddressProp& prop);

     virtual ~mtaAddressProp();

     mtaAddressProp& operator=(const mtaAddressProp& rhs);
     mtaAddressProp& operator=(const mtaAddressDequeue& rhs)
	  THROWS2(mtaExceptionBadArgs, mtaException);

     mtaAddressProp& dequeueContext(const mtaAddressDequeue& ctx)
	  THROWS2(mtaExceptionBadArgs, mtaException);
     mtaAddressProp& deliveryFlags(size_t dflags = 0, bool merge = true);
     mtaAddressProp& notifyFlags(size_t nflags = NOTIFY_DEFAULT)
	  THROWS(mtaExceptionBadArgs);
     mtaAddressProp& type(size_t atype = ADDR_DEFAULT)
	  THROWS(mtaExceptionBadArgs);

     void setDequeueContext(const mtaAddressDequeue& ctx)
	  THROWS2(mtaExceptionBadArgs, mtaException);

     void setDeliveryFlags(size_t dflags = 0, bool merge = true);
     size_t getDeliveryFlags() const;

     void setNotifyFlags(size_t nflags = NOTIFY_DEFAULT)
	  THROWS(mtaExceptionBadArgs);
     size_t getNotifyFlags() const;
     // Have to use (void) on next declaration to prevent confusing
     // compiler -- Forte V6.2 at least -- with subsequent (size_t)
     // declaration.
     const char *getNotifyStr(void) const;
     static const char *getNotifyStr(size_t nflags);

     void setType(size_t atype = ADDR_DEFAULT) THROWS(mtaExceptionBadArgs);
     size_t getType() const;
     const char *getTypeStr() const;

     bool equals(const mtaAddressProp& prop) const;
     const char *toString();

private:

     friend class mtaAddressEnqueue;
     friend class mtaAddressDequeue;
     friend class mtaMessageEnqueue;

     mtaAddressProp(size_t atype, size_t nflags = NOTIFY_DEFAULT,
		    size_t dflags = 0) THROWS(mtaExceptionBadArgs);

     mta_item_list_t *getItemList() const THROWS(mtaExceptionNoMem);

     void assign(size_t atype, size_t nflags, size_t dflags, mta_dq_t *dq)
	  THROWS(mtaExceptionBadArgs);
     void clean();
     void getFromDequeue(const mtaAddressDequeue& ctx, bool reset)
	  THROWS2(mtaExceptionBadArgs, mtaException);

     mta_dq_t        *pAdrDqCtx;
     mutable mta_item_list_t *pAdrItemList;
     std::string     *pAdrToString;

     size_t           nAdrFlagsDelivery;
     size_t           nAdrFlagsNotary;
     size_t           nAdrFlagsType;

     bool             bAdrFlagsDeliveryMerge;
     bool             bAdrSetFlagsDelivery;
     bool             bAdrSetFlagsNotary;
     bool             bAdrSetFlagsType;

};

};

#endif
