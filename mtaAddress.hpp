#ifndef __MTAADDRESS_HPP__

#define __MTAADDRESS_HPP__

namespace MTASDK_NAMESPACE {

class mtaAddress
{

public:

     mtaAddress();
     mtaAddress(const char *adr, size_t len = 0) THROWS(mtaExceptionNoMem);
     mtaAddress(const mtaAddress& adr) THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     mtaAddress(const std::string& adr) THROWS(mtaExceptionNoMem);
#endif

     virtual ~mtaAddress();

#ifndef SWIG // no need for these as we have the assign()'s
     mtaAddress& operator=(const char *adr) THROWS(mtaExceptionNoMem);
     mtaAddress& operator=(const mtaAddress& adr) THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     mtaAddress& operator=(const std::string& adr) THROWS(mtaExceptionNoMem);
#endif // !defined(_NO_STD_STRING)
#endif // !defined(SWIG)

     virtual void assign(const char *adr, size_t len = 0)
	  THROWS(mtaExceptionNoMem);
     virtual void assign(const mtaAddress& adr) THROWS(mtaExceptionNoMem);
#ifndef _NO_STD_STRING
     virtual void assign(const std::string& adr) THROWS(mtaExceptionNoMem);
#endif

     const char *getPhrase() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
     size_t getPhraseLength() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);

     const char *getRoute() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
     size_t getRouteLength() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);

     const char *getLocal() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
     size_t getLocalLength() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);

     const char *getDomain() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
     size_t getDomainLength() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);

     virtual bool equals(const char *adr, size_t len = 0) const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
     virtual bool equals(const mtaAddress& adr) const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
#ifndef _NO_STD_STRING
     virtual bool equals(const std::string& adr) const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);
#endif

     virtual const char *toString() const;
     virtual size_t toStringLength() const;

private:

     void clean() const;
     void parse() const
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNoMem,
		  mtaExceptionStrTruErr, mtaException);

     std::string          sAdr;
     mutable std::string *pAdrPhrase;
     mutable std::string *pAdrRoute;
     mutable std::string *pAdrLocal;
     mutable std::string *pAdrDomain;

};

};

#endif
