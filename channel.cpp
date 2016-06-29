#include <stdio.h>
#include "mtasdk.hpp"

using namespace MTASDK_NAMESPACE;

class dequeue : public mtaDequeue
{

public:

     dequeue(const mtaDequeueProp& p) : mtaDequeue(p), bFirst(true) { };
     ~dequeue() { };

     int processMessage(void *ctx2, mtaMessageDequeue& msg);
     void processDone(void *ctx2);

private:

     bool bFirst;
};


int
dequeue::processMessage(void *ctx2, mtaMessageDequeue &msg)
{
     printf("processMessage(this=%x; msg=%x)\n", this, &msg);

     if (bFirst)
     {
	  printf("HELO\n");
	  bFirst = false;
     }
     else
	  printf("RSET\n");

     printf("MAIL FROM:<%s> RET=%s ENVID=%s\n", msg.getEnvelopeFrom(),
	    msg.getNotifyStr(), msg.getEnvelopeId());

     mtaAddressDequeue adr;
     while(msg.getRecipient(adr))
     {
	  printf("RCPT TO:<%s> NOTIFY=%s ORCPT=%s\n",
		 adr.getAddress(), adr.getProp().getNotifyStr(), adr.getORcpt());
	  msg.setRecipientDisposition(DISP_DEFERRED, adr, "Blow me");
     }

     printf("DATA\n");

     const char *line;
     size_t len;
     while(msg.getLine(&line, &len))
	  printf("%.*s\n", len, line);

     printf(".\n");

     return(0);
}

void
dequeue::processDone(void *ctx2)
{
     printf("processDone(this=%x)\n", this);
     printf("QUIT\n");
}

main()
{
     initialize();

     dequeue dqObj = mtaDequeueProp("x_doof").maxThreads(3);
     dqObj.start();

     done();
}
