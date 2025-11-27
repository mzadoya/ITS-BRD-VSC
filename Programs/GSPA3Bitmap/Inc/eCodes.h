#ifndef ECODES_H
#define ECODES_H


#define STATUS_EOL 5
#define STATUS_ABSOLUTE 4
#define STATUS_DELTA 3
#define STATUS_RLE 2
#define STATUS_END_OF_FILE 1

#define OK 0
#define ERR_NOT_BMP -1
#define ERR_WRONG_FORMAT -2
#define ERR_READ_FAIL -3
#define ERR_OUT_OF_BOUNDS -4
#define ERR_SIGNATURE -5


#define MSG_ERR_NOT_BMP "Invalid Header Signature (Expected 'BM')"
#define MSG_ERR_WRONG_FORMAT "Unsupported Format (Compression or BitDepth mismatch)"
#define MSG_ERR_READ_FAIL "Unexpected End of File or Read Fail"
#define MSG_ERR_OUT_OF_BOUNDS "Image dimensions exceed Display Resolution"
#define MSG_ERR_SIGNATURE "Corrupted Data Stream"
#define MSG_ERR_UNKNOWN "Unknown Sys Error"
#define MSG_ERR_HANDSHAKE "Handshake Failed"

#endif