#include "logging.h"

static qboolean logger_inited = false;

void logger_init() {
    if (logger_inited)
        return;
    logger_inited = true;
   
}

void logger_close() {

    logger_inited = false;

}

