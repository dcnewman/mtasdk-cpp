#if !defined(__MTAADDRESSDEQUEUE_HPP__)

#define __MTAADDRESSDEQUEUE_HPP__

#include "mtasdk.hpp"

namespace MTASDK_NAMESPACE {

class mtaMessageDequeue;

class mtaAddressDequeue
{

public:

     mtaAddressDequeue();
     mtaAddressDequeue(const char *to, const char *orcpt, const char *ircpt,
		       size_t nflags, size_t dflags)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
     mtaAddressDequeue(const char *to, const char *orcpt, const char *ircpt,
		       const mtaMessageDequeue& dq, size_t nflags,
		       size_t dflags)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
#ifndef SWIG
     mtaAddressDequeue(const char *to, size_t to_len, const char *orcpt,
		       size_t orcpt_len, const char *ircpt, size_t ircpt_len,
		       const mtaMessageDequeue &dq, size_t nflags,
		       size_t dflags)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
#endif
     mtaAddressDequeue(const mtaAddressDequeue& to) THROWS(mtaExceptionNoMem);

     virtual ~mtaAddressDequeue();

     mtaAddressDequeue& operator=(const mtaAddressDequeue& rhs)
	  THROWS(mtaExceptionNoMem);

     const mtaAddress& getAddress() const;
     const mtaAddress& getORcpt() const;
     const mtaAddress& getIRcpt() const;
     const mtaAddressProp& getProp() const;

     void assign(const char *to, const char *orcpt, const char *ircpt,
		 const mtaMessageDequeue& dq, size_t nflags = NOTIFY_DEFAULT,
		 size_t dflags = 0)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
#ifndef SWIG
     void assign(const char *to, size_t to_len, const char *orcpt,
		 size_t orcpt_len, const char *ircpt, size_t ircpt_len,
		 const mtaMessageDequeue& dq, size_t nflags = NOTIFY_DEFAULT,
		 size_t dflags = 0)
	  THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem);
#endif

     bool equals(const mtaAddressDequeue& to) const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
		  mtaException);
     const char *toString();

private:

     friend class mtaAddressProp; // for access to pAdrDqCtx
     friend class mtaMessageEnqueue; // for access to pAdrDqCtx

     mta_dq_t *getDequeueContext() const;
     void clean();

     std::string    *pAdrDqToString;
     mta_dq_t       *pAdrDqCtx;

     mtaAddress      oAdrDq;
     mtaAddress      oAdrDqORcpt;
     mtaAddress      oAdrDqIRcpt;
     mtaAddressProp  oAdrDqProp;

};

};

#endif
