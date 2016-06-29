#ifndef __MAP_ATYPES_H__

#define __MAP_ATYPES_H__

#include "mtasdk.h"

static int map_atypes[ADDR_LAST+1] = {
     -1, MTA_ENV_TO, MTA_TO, MTA_CC, MTA_BCC,
         MTA_HDR_TO, MTA_HDR_CC, MTA_HDR_BCC};

#endif
