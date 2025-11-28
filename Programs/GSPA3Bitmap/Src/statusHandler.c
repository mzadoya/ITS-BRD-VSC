#include "eCodes.h"
#include "errorhandler.h"

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


//podvoprosom
void errorHandler(int rc) {

  const char *msg;
  switch (rc) {
  case ERR_NOT_BMP:
    msg = MSG_ERR_NOT_BMP;
    break;
  case ERR_WRONG_FORMAT:
    msg = MSG_ERR_WRONG_FORMAT;
    break;
  case ERR_READ_FAIL:
    msg = MSG_ERR_READ_FAIL;
    break;
  case ERR_OUT_OF_BOUNDS:
    msg = MSG_ERR_OUT_OF_BOUNDS;
    break;
  case ERR_SIGNATURE:
    msg = MSG_ERR_SIGNATURE;
    break;
  default:
    msg = MSG_ERR_UNKNOWN;
    break;
  }

  printError(rc, __FILE__, __LINE__, (char *)msg, false);
}