#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "common_types.h"

void logger_init();
void logger_close();

extern qboolean Log_debug_output_to_file;

#endif // _LOGGING_H_