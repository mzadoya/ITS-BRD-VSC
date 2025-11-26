#include "eCodes.h"

int inputRecognizer(char* info) { //TODO rename 

    if (info[0] != 0x00) {
        return STATUS_RLE;
    }

    switch (info[1])
    {
        case 0x00:
            return STATUS_EOL;
            break;
        case 0x01:
            return STATUS_END_OF_FILE;
            break;
        case 0x02:
            return STATUS_DELTA;
            break;
        default:
        return STATUS_ABSOLUTE;
            break;
     }   

    return OK;
}