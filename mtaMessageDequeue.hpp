#ifndef __MTAMESSAGEDEQUEUE_HPP__

#define __MTAMESSAGEDEQUEUE_HPP__

namespace MTASDK_NAMESPACE {

class mtaMessageDequeue
{

public:

     virtual ~mtaMessageDequeue();

     // Realization of the abstract member functions from the mtaMessage class
     const char *getChannel();
     size_t getChannelLength();

     const char *getEnvelopeFrom();
     size_t getEnvelopeFromLength();

     const char *getEnvelopeId();
     size_t getEnvelopeIdLength();

     size_t getNotifyFlags();
     const char *getNotifyStr();
     static const char *getNotifyStr(size_t nflags);

     size_t getDeliveryFlags();

     const char *toString();

     // Additional properties of a queued message
     const char *getMessageId()
	  THROWS3(mtaExceptionNo, mtaExceptionNoMem, mtaException);
     size_t getMessageIdLength()
	  THROWS3(mtaExceptionNo, mtaExceptionNoMem, mtaException);

     int threadId() const THROWS(mtaException);

     // Opertions which may be performed on a queued message
     bool eor() const;
     bool getRecipient(mtaAddressDequeue& adr) THROWS(mtaException);

     void setRecipientDisposition(size_t disp, const char *adr,
				  const char *reason = 0) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
     void setRecipientDisposition(size_t disp, mtaAddressDequeue &adr,
				  const char *reason = 0) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
#ifndef SWIG
     void setRecipientDisposition(size_t disp, const char *adr, size_t adr_len,
				  const char *reason, size_t reason_len) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
#endif

#ifndef _NO_STD_STRING
     void setRecipientDisposition(size_t disp, const std::string& adr) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
     void setRecipientDisposition(size_t disp, const std::string& adr,
				  const std::string& reason) const
	  THROWS2(mtaExceptionBadArgs, mtaException);
#endif

     bool eom() const;

#ifndef SWIG
     bool getLine(const char **line, size_t *len) THROWS(mtaException);
#endif
     const char *getLine() THROWS(mtaException);
     const char *getHandle(size_t *len) THROWS(mtaException);

     void rewind() THROWS(mtaException);

private:

     mtaMessageDequeue(mta_dq_t *dq, const char *chan, const char *env_from,
		       const char *env_id, const char *msg_id, size_t nflags,
		       size_t dflags) THROWS(mtaExceptionNoMem);
#ifndef SWIG
     mtaMessageDequeue(mta_dq_t *dq, const char *chan, size_t chan_len,
		       const char *env_from, size_t env_from_len,
		       const char *env_id, size_t env_id_len,
		       const char *msg_id, size_t msg_id_len,
		       size_t nflags, size_t dflags)
	  THROWS(mtaExceptionNoMem);
#endif

     friend class mtaMessageEnqueue;
     friend class mtaEnqueueProp;
     friend class mtaAddressDequeue;
     friend class mtaDequeue;

     mta_dq_t *getDequeueContext() const;
     void clean();

     mta_dq_t    *pMsgDqCtx;
     std::string *pMsgDqToString;

     size_t       nMsgDqFlagsDelivery;
     size_t       nMsgDqFlagsNotary;

     bool         bMsgDqEom;
     bool         bMsgDqEor;

     std::string  sMsgDqEnvFrom;
     std::string  sMsgDqEnvId;
     std::string  sMsgDqChan;
     std::string  sMsgDqId;
};

};

#endif
