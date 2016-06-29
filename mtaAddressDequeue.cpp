#include "mtasdk.hpp"

#if !defined(_WIN32)
#  include <stdio.h>
#else
#  define snprintf _snprintf
#endif

using namespace MTASDK_NAMESPACE;

static const char *badnew = "Unable to allocate memory for a string buffer";

mtaAddressDequeue::mtaAddressDequeue() :
     pAdrDqToString(0),
     pAdrDqCtx(0),
     oAdrDq(""),
     oAdrDqORcpt(""),
     oAdrDqIRcpt(""),
     oAdrDqProp(ADDR_ENV_TO)
{
}


mtaAddressDequeue::mtaAddressDequeue(const char *to, const char *orcpt,
				     const char *ircpt, size_t nflags,
				     size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrDqToString(0),
	  pAdrDqCtx(0),
	  oAdrDq(to),
	  oAdrDqORcpt(orcpt ? orcpt : (to ? to : "")),
	  oAdrDqIRcpt(ircpt ? ircpt : (to ? to : "")),
	  oAdrDqProp(ADDR_ENV_TO, nflags, dflags)
{
}


mtaAddressDequeue::mtaAddressDequeue(const char *to, size_t to_len,
				     const char *orcpt, size_t orcpt_len,
				     const char *ircpt, size_t ircpt_len,
				     const mtaMessageDequeue& dq,
				     size_t nflags, size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrDqToString(0),
	  pAdrDqCtx(dq.getDequeueContext()),
	  oAdrDq(to),
	  oAdrDqORcpt(orcpt ? orcpt : (to ? to : ""),
		      orcpt ? orcpt_len : (to ? to_len : 0)),
	  oAdrDqIRcpt(ircpt ? ircpt : (to ? to : ""),
		      ircpt ? ircpt_len : (to ? to_len : 0)),
	  oAdrDqProp(ADDR_ENV_TO, nflags, dflags)
{
}


mtaAddressDequeue::mtaAddressDequeue(const char *to, const char *orcpt,
				     const char *ircpt,
				     const mtaMessageDequeue& dq,
				     size_t nflags, size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrDqToString(0),
	  pAdrDqCtx(dq.getDequeueContext()),
	  oAdrDq(to),
	  oAdrDqORcpt(orcpt ? orcpt : (to ? to : "")),
	  oAdrDqIRcpt(ircpt ? ircpt : (to ? to : "")),
	  oAdrDqProp(ADDR_ENV_TO, nflags, dflags)
{
}


mtaAddressDequeue::mtaAddressDequeue(const mtaAddressDequeue& to)
     THROWS(mtaExceptionNoMem) :
	  pAdrDqToString(0),
	  pAdrDqCtx(to.pAdrDqCtx),
	  oAdrDq(to.oAdrDq),
	  oAdrDqORcpt(to.oAdrDqIRcpt),
	  oAdrDqIRcpt(to.oAdrDqIRcpt),
	  oAdrDqProp(to.oAdrDqProp)
{
}


mtaAddressDequeue::~mtaAddressDequeue()
{
     mtaAddressDequeue::clean();
}


void
mtaAddressDequeue::clean()
{
     if (!pAdrDqToString)
	  return;

     std::string *pTmp = pAdrDqToString;
     pAdrDqToString = 0;
     delete pTmp;
}


mtaAddressDequeue&
mtaAddressDequeue::operator=(const mtaAddressDequeue& rhs)
     THROWS(mtaExceptionNoMem)
{
     // Ignore assignment to self
     if (this == &rhs)
	  return(*this);

     mtaAddressDequeue::clean();

     pAdrDqCtx   = rhs.pAdrDqCtx;
     oAdrDq      = rhs.oAdrDq;
     oAdrDqORcpt = rhs.oAdrDqORcpt;
     oAdrDqIRcpt = rhs.oAdrDqIRcpt;
     oAdrDqProp  = rhs.oAdrDqProp;

     return(*this);
}


const mtaAddress&
mtaAddressDequeue::getAddress() const
{
     return(oAdrDq);
}


const mtaAddress&
mtaAddressDequeue::getORcpt() const
{
     return(oAdrDqORcpt);
}


const mtaAddress&
mtaAddressDequeue::getIRcpt() const
{
     return(oAdrDqIRcpt);
}


const mtaAddressProp&
mtaAddressDequeue::getProp() const
{
     return(oAdrDqProp);
}


const char *
mtaAddressDequeue::toString()
{
     // We rebuild the string from scratch each time: it's not
     // pratical to try to track when changes are made to our
     // data members

     char buf[512];

     // Note that snprintf() returns the length THAT WOULD HAVE
     // printed were the buffer size unlimited.  It is NOT the
     // length which did print....
     snprintf(buf, sizeof(buf),
	      "RCPT TO:<%.*s> NOTIFY=%s ORCPT=rfc822;%.*s "
	      "IRCPT=rfc822;%.*s",
	      (int)oAdrDq.toStringLength(), oAdrDq.toString(),
	      oAdrDqProp.getNotifyStr(),
	      (int)oAdrDqORcpt.toStringLength(), oAdrDqORcpt.toString(),
	      (int)oAdrDqIRcpt.toStringLength(), oAdrDqIRcpt.toString());

     if (!pAdrDqToString)
     {
	  std::string *pTmp = 0;
	  try
	  {
	       pTmp = new std::string(buf);
	  }
	  catch (...)
	  {
	       if (pTmp)
		    // Would prefer to throw an exception but for the fact
		    // that Java's toString() doesn't
		    delete pTmp;
	       return(badnew);
	  }
	  if (!pTmp)
	       // Would prefer to throw an exception but for the fact
	       // that Java's toString() doesn't
	       return(badnew);
	  pAdrDqToString = pTmp;
     }
     else
	  pAdrDqToString->assign(buf);

     return(pAdrDqToString->c_str());
}


void
mtaAddressDequeue::assign(const char *to, const char *orcpt, const char *ircpt,
			  const mtaMessageDequeue& dq, size_t nflags,
			  size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem)
{
     pAdrDqCtx = dq.getDequeueContext();
     oAdrDq.assign(to ? to : "");
     oAdrDqORcpt.assign(orcpt ? orcpt : (to ? to : ""));
     oAdrDqIRcpt.assign(ircpt ? ircpt : (to ? to : ""));
     oAdrDqProp.assign(ADDR_ENV_TO, nflags, dflags, pAdrDqCtx);
}


void
mtaAddressDequeue::assign(const char *to, size_t to_len, const char *orcpt,
			  size_t orcpt_len, const char *ircpt, size_t ircpt_len,
			  const mtaMessageDequeue& dq, size_t nflags,
			  size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem)
{
     pAdrDqCtx = dq.getDequeueContext();
     oAdrDq.assign(to, to_len);
     oAdrDqORcpt.assign(orcpt, orcpt_len);
     oAdrDqIRcpt.assign(ircpt, ircpt_len);
     oAdrDqProp.assign(ADDR_ENV_TO, nflags, dflags, pAdrDqCtx);
}


bool
mtaAddressDequeue::equals(const mtaAddressDequeue& to) const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (pAdrDqCtx == to.pAdrDqCtx &&
	 oAdrDq.equals(to.oAdrDq) &&
	 oAdrDqORcpt.equals(to.oAdrDqORcpt) &&
	 oAdrDqIRcpt.equals(to.oAdrDqIRcpt) &&
	 oAdrDqProp.equals(to.oAdrDqProp))
	  return(true);
     else
	  return(false);
}


mta_dq_t *
mtaAddressDequeue::getDequeueContext() const
{
     return(pAdrDqCtx);
}
