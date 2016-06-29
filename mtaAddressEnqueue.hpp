#if !defined(__MTAADDRESSENQUEUE_HPP__)

#define __MTAADDRESSENQUEUE_HPP__

#include "mtasdk.hpp"

namespace MTASDK_NAMESPACE {

class mtaAddressEnqueue
{

public:

     mtaAddressEnqueue();

     mtaAddressEnqueue(const char *adr, size_t atype = ADDR_DEFAULT)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
     mtaAddressEnqueue(const char *adr, const char *orcpt,
		       size_t atype = ADDR_DEFAULT,
		       size_t nflags = NOTIFY_DEFAULT, size_t dflags = 0)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
     mtaAddressEnqueue(const char *adr, const mtaAddressProp& prop)
	  THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue(const char *adr, const char *orcpt,
		       const mtaAddressProp& prop) THROWS(mtaExceptionNoMem);

#ifndef _NO_STD_STRING
     mtaAddressEnqueue(const std::string& adr) THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue(const std::string& adr, size_t atype)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
     mtaAddressEnqueue(const std::string& adr, const mtaAddressProp& prop)
	  THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue(const std::string& adr, const std::string& orcpt)
	  THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue(const std::string& adr, const std::string& orcpt,
		       const mtaAddressProp& prop) THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue(const std::string& adr, const std::string& orcpt,
		       size_t atype, size_t nflags, size_t dflags)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
#endif

     mtaAddressEnqueue(const mtaAddressEnqueue& adr) THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue(const mtaAddressDequeue& adr) THROWS(mtaExceptionNoMem);

     virtual ~mtaAddressEnqueue();

     mtaAddressEnqueue& operator=(const char *adr) THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue& operator=(const mtaAddressProp &prop);
     mtaAddressEnqueue& operator=(const mtaAddressEnqueue& adr)
	  THROWS(mtaExceptionNoMem);
     mtaAddressEnqueue& operator=(const mtaAddressDequeue& adr)
	  THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     mtaAddressEnqueue& operator=(const std::string& adr)
	  THROWS(mtaExceptionNoMem);
#endif

     mtaAddress& getAddress();
     mtaAddress& getORcpt();
     mtaAddressProp& getProp();

     bool equals(const mtaAddressEnqueue& adr) const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
		  mtaException);

     const char *toString();

private:

     void clean();

     std::string    *pAdrNqToString;

     mtaAddress      oAdrNq;
     mtaAddress      oAdrNqORcpt;
     mtaAddressProp  oAdrNqProp;
};

};

#endif
