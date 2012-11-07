#include "BMS_Memory.h"
#include <plib.h>
#include "BMS.h"

#define NVM_PROGRAM_PAGE 0xbd008000

void loadDataFromMemory()
{
    unsigned char changed = 0;
    
/*
    tensionMaxCell = (*(int *)(NVM_PROGRAM_PAGE + 0x000));
    gapOverMinCell = (*(int *)(NVM_PROGRAM_PAGE + 0x004));
    tensionStartFet = (*(int *)(NVM_PROGRAM_PAGE + 0x008));
    tensionMinCell = (*(int *)(NVM_PROGRAM_PAGE + 0x200));
    temperatureMaxCell = (*(int *)(NVM_PROGRAM_PAGE + 0x300));
    temperatureMaxRes = (*(int *)(NVM_PROGRAM_PAGE + 0x400));
    tensionMaxOpenContactor = (*(int *)(NVM_PROGRAM_PAGE + 0x500));

    if(tensionMaxCell > 10000 || tensionMaxCell < 0)
    {
        changed = 1;*/
        tensionMaxCell = TENSION_MAX_CELL;
    /*}
    if(tensionMinCell > 10000 || tensionMinCell < 0)
    {
        changed = 1;*/
        tensionMinCell = TENSION_MIN_CELL;
    /*}
    if(gapOverMinCell > 10000 || gapOverMinCell < 0)
    {
        changed = 1;*/
        gapOverMinCell = GAP_OVER_MIN_CELL;
    /*}
    if(tensionStartFet > 10000 || tensionStartFet < 0)
    {
        changed = 1;*/
        tensionStartFet = TENSION_START_FET;
    /*}

    if(temperatureMaxCell > 10000 || temperatureMaxCell < 0)
    {
        changed = 1;*/
        temperatureMaxCell = TEMPERATURE_MAX_CELL;
    /*}
    if(temperatureMaxRes > 10000 || temperatureMaxRes < 0)
    {
        changed = 1;*/
        temperatureMaxRes = TEMPERATURE_MAX_RES;
    /*}
    if(tensionMaxOpenContactor > 10000 || tensionMaxOpenContactor < 0)
    {
        changed = 1;*/
        tensionMaxOpenContactor = TENSION_MAX_OPEN_CONTACTOR;
    /*}
    //if(changed)
    //    saveDataToMemory();
    */
}

void saveDataToMemory()
{
    return;
    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x000), tensionMaxCell);
    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x004), gapOverMinCell);
    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x008), tensionStartFet);

    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x200), tensionMinCell);
    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x300), temperatureMaxCell);
    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x400), temperatureMaxRes);
    NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x500), tensionMaxOpenContactor);
}