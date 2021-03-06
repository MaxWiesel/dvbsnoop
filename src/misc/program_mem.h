#ifndef __PROGRAM_MEM
#define __PROGRAM_MEM

#include "dvbsnoop.h"

#if 0 // som: original code

typedef struct _TS_PROGRAM* TS_PROGRAM_P;
typedef struct _TS_STREAM* TS_STREAM_P;

typedef struct _TS_STREAM {
    u_int elementary_PID;
    u_int stream_type;
    TS_PROGRAM_P program;
    TS_STREAM_P next;
} TS_STREAM;

typedef struct _TS_PROGRAM {
    u_int PMT_PID;
    u_int program_number;
    TS_STREAM_P stream;
} TS_PROGRAM;

TS_PROGRAM* reset_ProgramMem(u_int PMT_PID, u_int program_number);
TS_PROGRAM* get_ProgramFromMem(u_int PMT_PID);
TS_STREAM* store_StreamToMem(TS_PROGRAM* program, u_int elementary_PID, u_int stream_type);
TS_STREAM* get_StreamFromMem(u_int elementary_PID);

#else

u_int get_StreamTypeFromMem(u_int elementary_PID);
void store_StreamTypeToMem(u_int elementary_PID, u_int stream_type);

u_char is_PMT_PID(u_int pid);
void set_PMT_PID(u_int pid);
void clear_PMT_PID(u_int pid);

#endif

#endif
