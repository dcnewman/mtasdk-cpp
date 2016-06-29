#include "mtasdk.hpp"

#if !defined(_WIN32)
#  include <stdio.h>
#else
#  define snprintf _snprintf
#endif

using namespace MTASDK_NAMESPACE;

static const char *badnew = "Unable to allocate memory for a string buffer";


mtaAddressEnqueue::mtaAddressEnqueue() :
     pAdrNqToString(0),
     oAdrNq(""),
     oAdrNqORcpt(""),
     oAdrNqProp()
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const char *adr, size_t atype)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(adr),
	  oAdrNqProp(atype)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const char *adr,
				     const mtaAddressProp& prop)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(adr),
	  oAdrNqProp(prop)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const char *adr, const char *orcpt,
				     size_t atype, size_t nflags,
				     size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(orcpt),
	  oAdrNqProp(atype, nflags, dflags)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const std::string& adr)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(adr),
	  oAdrNqProp()
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const std::string& adr, size_t atype)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(adr),
	  oAdrNqProp(atype)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const std::string& adr,
				     const mtaAddressProp& prop)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(adr),
	  oAdrNqProp(prop)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const std::string& adr,
				     const std::string& orcpt)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(orcpt),
	  oAdrNqProp()
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const std::string& adr,
				     const std::string& orcpt,
				     const mtaAddressProp& prop)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(orcpt),
	  oAdrNqProp(prop)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const std::string& adr,
				     const std::string& orcpt,
				     size_t atype, size_t nflags,
				     size_t dflags)
     THROWS2(mtaExceptionBadArgs, mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr),
	  oAdrNqORcpt(orcpt),
	  oAdrNqProp(atype, nflags, dflags)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const mtaAddressEnqueue& adr)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr.oAdrNq),
	  oAdrNqORcpt(adr.oAdrNqORcpt),
	  oAdrNqProp(adr.oAdrNqProp)
{
}


mtaAddressEnqueue::mtaAddressEnqueue(const mtaAddressDequeue& adr)
     THROWS(mtaExceptionNoMem) :
	  pAdrNqToString(0),
	  oAdrNq(adr.getAddress()),
	  oAdrNqORcpt(adr.getORcpt()),
	  oAdrNqProp(adr.getProp())
{
}


mtaAddressEnqueue::~mtaAddressEnqueue()
{
     mtaAddressEnqueue::clean();
}


void
mtaAddressEnqueue::clean()
{
     if (!pAdrNqToString)
	  return;

     std::string *pTmp = pAdrNqToString;
     pAdrNqToString = 0;
     delete pTmp;
}


mtaAddressEnqueue&
mtaAddressEnqueue::operator=(const char *rhs)
     THROWS(mtaExceptionNoMem)
{
     mtaAddressEnqueue::clean();
     oAdrNq.assign(rhs);
     return(*this);
}


mtaAddressEnqueue&
mtaAddressEnqueue::operator=(const std::string& rhs)
     THROWS(mtaExceptionNoMem)
{
     mtaAddressEnqueue::clean();
     oAdrNq.assign(rhs);
     return(*this);
}


mtaAddressEnqueue&
mtaAddressEnqueue::operator=(const mtaAddressEnqueue& rhs)
     THROWS(mtaExceptionNoMem)
{
     // Ignore assignment to self
     if (this == &rhs)
	  return(*this);

     mtaAddressEnqueue::clean();

     oAdrNq      = rhs.oAdrNq;
     oAdrNqORcpt = rhs.oAdrNqORcpt;
     oAdrNqProp  = rhs.oAdrNqProp;

     return(*this);
}


mtaAddressEnqueue&
mtaAddressEnqueue::operator=(const mtaAddressDequeue& rhs)
     THROWS(mtaExceptionNoMem)
{
     mtaAddressEnqueue::clean();

     oAdrNq      = rhs.getAddress();
     oAdrNqORcpt = rhs.getORcpt();
     oAdrNqProp  = rhs.getProp();

     return(*this);
}


mtaAddressEnqueue&
mtaAddressEnqueue::operator=(const mtaAddressProp& rhs)
{
     mtaAddressEnqueue::clean();

     oAdrNqProp  = rhs;

     return(*this);
}


mtaAddress&
mtaAddressEnqueue::getAddress()
{
     return(oAdrNq);
}


mtaAddress&
mtaAddressEnqueue::getORcpt()
{
     return(oAdrNqORcpt);
}


mtaAddressProp&
mtaAddressEnqueue::getProp()
{
     return(oAdrNqProp);
}


const char *
mtaAddressEnqueue::toString()
{
     // We rebuild the string from scratch each time: it's not
     // pratical to try to track when changes are made to our
     // data members

     char buf[512];

     // Note that snprintf() returns the length THAT WOULD HAVE
     // printed were the buffer size unlimited.  It is NOT the
     // length which did print....
     snprintf(buf, sizeof(buf),
	      "RCPT TO:<%.*s> NOTIFY=%s ORCPT=rfc822;%.*s",
	      (int)oAdrNq.toStringLength(), oAdrNq.toString(),
	      oAdrNqProp.getNotifyStr(),
	      (int)oAdrNqORcpt.toStringLength(), oAdrNqORcpt.toString());

     // Now store the result in our std::string
     if (!pAdrNqToString)
     {
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
	  pAdrNqToString = pTmp;
     }
     else
	  pAdrNqToString->assign(buf);

     return(pAdrNqToString->c_str());
}


bool
mtaAddressEnqueue::equals(const mtaAddressEnqueue& adr) const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (oAdrNq.equals(adr.oAdrNq) &&
	 oAdrNqORcpt.equals(adr.oAdrNqORcpt) &&
	 oAdrNqProp.equals(adr.oAdrNqProp))
	  return(true);
     else
	  return(false);
}
