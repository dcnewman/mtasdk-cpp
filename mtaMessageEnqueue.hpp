#ifndef __MTAMESSAGEENQUEUE_HPP__

#define __MTAMESSAGEENQUEUE_HPP__

namespace MTASDK_NAMESPACE {

class mtaMessageEnqueue
{

public:

     mtaMessageEnqueue(const char *env_from, const char *env_id = 0)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);
     mtaMessageEnqueue(const char *env_from, const mtaEnqueueProp& rhs)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);
     mtaMessageEnqueue(const char *env_from, const char *env_id,
		       const mtaEnqueueProp& rhs)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);
     mtaMessageEnqueue(const char *env_from, const mtaMessageDequeue& dq)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);
     mtaMessageEnqueue(const char *env_from, const mtaMessageDequeue& dq,
		       const mtaEnqueueProp& rhs)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);

#ifndef _NO_STD_STRING
     mtaMessageEnqueue(const std::string& env_from)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);
     mtaMessageEnqueue(const std::string& env_from, const mtaEnqueueProp& rhs)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);
     mtaMessageEnqueue(const std::string& env_from, const std::string& env_id)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);
     mtaMessageEnqueue(const std::string& env_from, const std::string& env_id,
		       const mtaEnqueueProp& rhs)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);
     mtaMessageEnqueue(const std::string& env_from,
		       const mtaMessageDequeue& dq)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);
     mtaMessageEnqueue(const std::string& env_from, const mtaMessageDequeue& dq,
		       const mtaEnqueueProp& rhs)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);
#endif

     mtaMessageEnqueue(const mtaMessageEnqueue& rhs)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);

     virtual ~mtaMessageEnqueue();

     mtaMessageEnqueue& operator=(const mtaMessageEnqueue& rhs)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);

     const char *toString();

     // These can only be queried: they are set when the object is created
     // and hence there's no ability to set them through the object

     const char *getChannel() THROWS(mtaException);
     size_t getChannelLength() THROWS(mtaException);

     const char *getEnvelopeFrom() const THROWS(mtaException);
     size_t getEnvelopeFromLength() const THROWS(mtaException);

     const char *getEnvelopeId() const THROWS(mtaException);
     size_t getEnvelopeIdLength() const THROWS(mtaException);

     size_t getNotifyFlags() THROWS(mtaException);
     const char *getNotifyStr() THROWS(mtaException);

     size_t getDeliveryFlags() const THROWS(mtaException);
     bool   getExpandAliases() const THROWS(mtaException);
     size_t getExpandLimit() const THROWS(mtaException);
     size_t getFragmentBlockLimit() const THROWS(mtaException);
     size_t getFragmentLineLimit() const THROWS(mtaException);
     bool   getSortAddresses() const THROWS(mtaException);

     // Operations which may be performed on a message being enqueued
     const char *getError() const;

     void addRecipient(const char *to, size_t atype = 0, size_t nflags = 0)
	  const THROWS2(mtaExceptionBadArgs, mtaException);
     void addRecipient(const char *to, const mtaAddressProp& aprop) const
	  THROWS(mtaException);
     void addRecipient(const mtaAddressDequeue& to) const
	  THROWS(mtaException);
     void addRecipient(const mtaAddressDequeue& to,
		       const mtaAddressProp& aprop) const
	  THROWS(mtaException);
#ifndef _NO_STD_STRING
     void addRecipient(const std::string& to, size_t atype = 0,
		       size_t nflags = 0) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
     void addRecipient(const std::string& to, const mtaAddressProp& aprop)
	  const THROWS(mtaException);
#endif

     void write(const char *data, size_t len) const THROWS(mtaException);
     void writeLine(const char *line = 0, size_t len = 0) const
	  THROWS(mtaException);

#ifndef _NO_STD_STRING
     void write(const std::string& data) const THROWS(mtaException);
     void writeLine(const std::string& str) const THROWS(mtaException);
#endif

     void copyMessage(const mtaMessageDequeue& rhs, bool rewind = true) const
	  THROWS(mtaException);

     void enqueue() THROWS(mtaException);
     void abort();

private:

     static mta_nq_t *enqueueStart(const char *env_from, size_t env_from_len,
				   const char *env_id, size_t env_id_len,
				   mta_dq_t *dq, mta_item_list_t *items)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);
     static mta_nq_t *enqueueStart(const char *env_from, size_t env_from_len,
				   const char *env_id, size_t env_id_len,
				   mta_dq_t *dq, const mtaEnqueueProp& prop)
	  THROWS4(mtaExceptionBadArgs, mtaExceptionNo, mtaExceptionNoMem,
		  mtaException);
     static mta_nq_t *enqueueStart(const mtaMessageEnqueue& rhs)
	  THROWS3(mtaExceptionBadArgs, mtaExceptionNo, mtaException);

     void addTo(const char *to, size_t len, size_t atype, size_t nflags) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
     void addTo(const char *to, size_t len = 0, int item_code_1 = 0,
		void *data_1 = 0, int item_code_2 = 0, void *data_2 = 0) const
	  THROWS(mtaException);

     const char *getInfoStr(int item_code, size_t *len) const
	  THROWS(mtaException);
     size_t getInfoInt(int item_code) const THROWS(mtaException);

     void throwException(int ires, const char *msg = 0) const
	  THROWS(mtaException);

     void clean();

     mta_nq_t    *pMsgNqCtx;
     std::string *pMsgNqToString;

     size_t       nMsgNqChanLen;
     size_t       nMsgNqFlagsNotary;

};

};

#endif
