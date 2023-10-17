#include "logging.h"

#include "spdlog/spdlog.h"

static bool logger_inited = false;

void logger_init() {
    if (logger_inited)
        return;
    
    spdlog::info("Welcome to spdlog!");
    logger_inited = true;
   
}

void logger_close() {

    logger_inited = false;

}

void logger_print(format_string_t<Args...> fmt, Args &&...args) {
    spdlog::info(fmt, args);
}