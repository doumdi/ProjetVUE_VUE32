
// Todo: Make this file cleaner, use VUE32_Impl.c as template

#include "NETV32_Common.h"
#include "VUE32_Utils.h"
#include "BMS_Impl.h"


// Prototypes
void InitBMS(void);

void ImplBMS(void);

void OnMsgBMS(NETV_MESSAGE *msg);

extern HDW_MAPPING gBMS_Ress[];

extern ROUTING_TABLE gRoutingTableBMS[];

void OnEmergencyMsgBMS();


// Arrays of function pointers for each implementation
FUNC_INIT gInitFunc[1] =
{
    InitBMS,
};

FUNC_IMPL gImplFunc[1] =
{
    ImplBMS,
};

FUNC_ONMSG gOnMsgFunc[1] =
{
    OnMsgBMS,
};


HDW_MAPPING* gHardwareMap[1] =
{
    gBMS_Ress,
};

unsigned int gHardwareSize[1] =
{
    3   //BMS
};

FUNC_ONEMERGMSG gOnEmergencyMsgVUE32[1] =
{
    OnEmergencyMsgBMS,
};

ROUTING_TABLE *gRoutingTable[1] =
{
    gRoutingTableBMS,
};

unsigned char gRoutingTableSize[1] =
{
    0  //BMS
};



