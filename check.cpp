#include <stdio.h>
#include "mtasdk.hpp"

using namespace MTASDK_NAMESPACE;

main()
{
     mtaAddressProp adr;

     adr.setType(ADDR_BCC);
     adr.notifyFlags(NOTIFY_FAILURE).deliveryFlags(333);
     mtaAddressEnqueue dan("dan.newman@sun.com");
     dan = adr;
     mtaAddressEnqueue dan2("dan.newman@mtbaldy.us", adr);
     
     char buf[4096];

     printf("1: %s\n", dan.toString());
}
