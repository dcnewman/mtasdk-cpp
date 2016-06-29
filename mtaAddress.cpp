#include <string.h>
#include "mtasdk.hpp"
#include "mtasdk.h"

extern "C" int
mtaAddressParts(const char *adr, size_t len,
		char *phrase, size_t *plen, size_t plenmax,
		char *route, size_t *rlen, size_t rlenmax,
		char *local, size_t *llen, size_t llenmax,
		char *domain, size_t *dlen, size_t dlenmax);

using namespace MTASDK_NAMESPACE;

static const char *badassign = "Insufficient virtual memory to store the "
                                "supplied string; the assign() member function "
                                "for C++ std::string threw an exception";
static const char *badadrn   = "Null pointer supplied for the address string";
static const char *badadrp   = "Unable to parse the supplied address";
static const char *badnomem  = "Unable to allocate storage for the address "
                                 "parts";

mtaAddress::mtaAddress() :
     sAdr(),
     pAdrPhrase(0),
     pAdrRoute(0),
     pAdrLocal(0),
     pAdrDomain(0)
{
}


mtaAddress::mtaAddress(const char *adr, size_t len)
     THROWS(mtaExceptionNoMem) :
	  sAdr(),
	  pAdrPhrase(0),
	  pAdrRoute(0),
	  pAdrLocal(0),
	  pAdrDomain(0)
{
     try
     {
	  if (adr)
	  {
	       if (len)
		    sAdr.assign(adr, len);
	       else
		    sAdr.assign(adr);
	  }
	  else
	       sAdr.assign((const char *)"", (size_t)0);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaAddress::mtaAddress(const std::string& adr)
     THROWS(mtaExceptionNoMem) :
	  sAdr(),
	  pAdrPhrase(0),
	  pAdrRoute(0),
	  pAdrLocal(0),
	  pAdrDomain(0)
{
     try
     {
	  sAdr.assign(adr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaAddress::mtaAddress(const mtaAddress& adr)
     THROWS(mtaExceptionNoMem) :
	  sAdr(),
	  pAdrPhrase(0),
	  pAdrRoute(0),
	  pAdrLocal(0),
	  pAdrDomain(0)
{
     try
     {
	  sAdr.assign(adr.sAdr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


mtaAddress::~mtaAddress()
{
     mtaAddress::clean();
}


mtaAddress&
mtaAddress::operator=(const char *adr)
     THROWS(mtaExceptionNoMem)
{
     try
     {
	  sAdr.assign(adr ? adr : "");
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
     return(*this);
}


mtaAddress&
mtaAddress::operator=(const std::string& adr)
     THROWS(mtaExceptionNoMem)
{
     try
     {
	  sAdr.assign(adr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
     return(*this);
}


mtaAddress&
mtaAddress::operator=(const mtaAddress& adr)
     THROWS(mtaExceptionNoMem)
{
     // Avoid assignment to self
     if (this == &adr)
	  return(*this);

     // Wipe our past parsing results
     try
     {
	  mtaAddress::clean();
	  sAdr.assign(adr.sAdr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }

     // All done
     return(*this);
}


void
mtaAddress::clean() const
{
     if (pAdrPhrase)
     {
	  // C++ allows us to delete NULL so
	  //   we'll save ourselves any further testing
	  delete pAdrPhrase;
	  delete pAdrRoute;
	  delete pAdrLocal;
	  delete pAdrDomain;
	  pAdrPhrase = 0;
     }
     pAdrRoute  = 0;
     pAdrLocal  = 0;
     pAdrDomain = 0;
}


void
mtaAddress::assign(const char *adr, size_t len)
     THROWS(mtaExceptionNoMem)
{
     mtaAddress::clean();
     if (!adr)
     {
	  adr = "";
	  len = 0;
     }
     try
     {
	  if (len)
	       sAdr.assign(adr, len);
	  else
	       sAdr.assign(adr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


void
mtaAddress::assign(const std::string& adr)
     THROWS(mtaExceptionNoMem)
{
     mtaAddress::clean();
     try
     {
	  sAdr.assign(adr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


void
mtaAddress::assign(const mtaAddress& adr)
     THROWS(mtaExceptionNoMem)
{
     mtaAddress::clean();
     try
     {
	  sAdr.assign(adr.sAdr);
     }
     catch (...)
     {
	  throw mtaExceptionNoMem(badassign);
     }
}


const char *
mtaAddress::toString() const
{
     return(sAdr.c_str());
}


size_t
mtaAddress::toStringLength() const
{
     return(sAdr.size());
}


const char *
mtaAddress::getPhrase() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrPhrase)
	  mtaAddress::parse();
     return(pAdrPhrase->c_str());
}


size_t
mtaAddress::getPhraseLength() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrPhrase)
	  mtaAddress::parse();
     return(pAdrPhrase->size());
}


const char *
mtaAddress::getRoute() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrRoute)
	  mtaAddress::parse();
     return(pAdrRoute->c_str());
}


size_t
mtaAddress::getRouteLength() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrRoute)
	  mtaAddress::parse();
     return(pAdrRoute->size());
}


const char *
mtaAddress::getLocal() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrLocal)
	  mtaAddress::parse();
     return(pAdrLocal->c_str());
}


size_t
mtaAddress::getLocalLength() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrLocal)
	  mtaAddress::parse();
     return(pAdrLocal->size());
}


const char *
mtaAddress::getDomain() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrDomain)
	  mtaAddress::parse();
     return(pAdrDomain->c_str());
}


size_t
mtaAddress::getDomainLength() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     if (!pAdrDomain)
	  mtaAddress::parse();
     return(pAdrDomain->size());
}


bool
mtaAddress::equals(const mtaAddress& adr) const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     mtaAddress *adr2 = (mtaAddress *)&adr;

     // Compare the lengths of the local parts
     size_t l1 = mtaAddress::getLocalLength();
     size_t l2 = adr2->getLocalLength();
     if (l1 != l2)
	  return(false);

     // Compare the local parts: local parts are case sensitive
     const char *s1 = mtaAddress::getLocal();
     const char *s2 = adr2->getLocal();
     if (l1 && memcmp(s1, s2, l1))
	  return(false);

     // Compare the domain parts: domain parts are NOT case sensitive
     l1 = getDomainLength();
     l2 = adr2->getDomainLength();
     if (l1 != l2)
	  return(false);
     s1 = getDomain();
     s2 = adr2->getDomain();
     if (l1 && strncasecmp(s1, s2, l1))
	  return(false);

     // Compare the source routes: source routes are NOT case sensitive
     l1 = getRouteLength();
     l2 = adr2->getRouteLength();
     if (l1 != l2)
	  return(false);
     s1 = getRoute();
     s2 = adr2->getRoute();
     if (l1 && strncasecmp(s1, s2, l1))
	  return(false);

     // @route1:local1@domain1 == @route2:local2@domain2
     return(true);
}


bool
mtaAddress::equals(const char *adr, size_t len) const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     char local[ALFA_SIZE+1], domain[ALFA_SIZE+1], route[ALFA_SIZE+1];
     size_t dlen, llen, rlen;

     if (!adr)
	  throw mtaExceptionBadArgs(badadrn, 1);

     int ires = mtaAddressParts(adr, len,
				NULL, NULL, 0,
				route, &rlen, sizeof(route),
				local, &llen, sizeof(local),
				domain, &dlen, sizeof(domain));
     if (ires != MTA_OK)
	  throwException(ires, badadrp, 1);

     // Compare the case-sensitive local parts
     if (llen != getLocalLength())
	  return(false);
     if (llen && memcmp(getLocal(), local, llen))
	  return(false);

     // Compare the case insensitive domain names
     if (dlen != getDomainLength())
	  return(false);
     if (dlen && strncasecmp(getDomain(), domain, dlen))
	  return(false);

     // Comparse the case insensitive source routes
     if (rlen != getRouteLength())
	  return(false);
     if (rlen && strncasecmp(getRoute(), route, rlen))
	  return(false);

     // They are the same
     return(true);
}


bool
mtaAddress::equals(const std::string& adr) const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     return(mtaAddress::equals(adr.data(), adr.size()));
}


void
mtaAddress::parse() const
     THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem, mtaExceptionStrTruErr,
	     mtaException)
{
     char local[ALFA_SIZE+1], domain[ALFA_SIZE+1], phrase[ALFA_SIZE+1],
	  route[ALFA_SIZE+1];
     size_t dlen, llen, plen, rlen;

     int ires = mtaAddressParts(sAdr.data(), sAdr.size(),
				phrase, &plen, sizeof(phrase),
				route, &rlen, sizeof(route),
				local, &llen, sizeof(local),
				domain, &dlen, sizeof(domain));
     if (ires != MTA_OK)
	  throwException(ires, badadrp);

     try
     {
	  pAdrPhrase = new std::string(phrase, plen);
	  pAdrRoute  = new std::string(route,  rlen);
	  pAdrLocal  = new std::string(local,  llen);
	  pAdrDomain = new std::string(domain, dlen);
     }
     catch (...)
     {
	  // Insufficient virtual memory!
	  goto badness;
     }
     if (!pAdrPhrase || !pAdrRoute || !pAdrLocal || !pAdrDomain)
	  goto badness;

     // Success
     return;

badness:
     mtaAddress::clean();
     throw mtaExceptionNoMem(badnomem);
}
