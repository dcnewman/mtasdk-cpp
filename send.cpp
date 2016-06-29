#include <stdio.h>
#include "mtasdk.hpp"

using namespace MTASDK_NAMESPACE;

main()
{
     try {
	  initialize();

	  mtaMessageEnqueue nq("dan.newman@sun.com");
	  
	  nq.addRecipient("dnewman@sun.com", mtaAddressProp().type(ADDR_CC));
	  nq.writeLine("Subject: test");
	  nq.writeLine();
	  nq.writeLine("A test message");
	  nq.enqueue();
	  done();
     }
     catch (const mtaException& e)
     {
	  fprintf(stderr, "Caught an mtaException: errno = %d\n"
		  "  %s\n  %s\n", e.getErrno(), e.getMessage());
     }
     catch (...)
     {
	  fprintf(stderr, "caught an exception\n");
     }
}
