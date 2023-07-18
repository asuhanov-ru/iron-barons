#include <stdarg.h>

#include "logging.h"
#include "core/vmallocator.h"
#include "core/core.h"
#include "core/cmdline.h"
#include "core/globals.h"



static const char *FILTERS_ENABLED_BY_DEFAULT[] =
{
	"error",
	"warning",
	"general",
	"scripting"
};

struct logger_filter_struct {
	char name[NAME_LENGTH];
	bool enabled;
};

static SCP_vector<logger_filter_struct>& filter_vector()
{
	static SCP_vector<logger_filter_struct> vec;
	return vec;
}

bool Log_debug_output_to_file = true;

void logger_print(const char* id = nullptr, const char* temp = nullptr);

static bool logger_inited      = false;

static FILE* Log_fp            = nullptr;
static bool Log_close_fp       = false;
static const char* Logfilename = nullptr;
static int logger_filter_loaded = 0;

void load_filter_info()
{
    logger_filter_struct* filter_ptr;

	for (const char *name : FILTERS_ENABLED_BY_DEFAULT) {
		filter_vector().emplace_back();
		filter_ptr = &filter_vector().back();

		strcpy_s(filter_ptr->name, name);
		filter_ptr->enabled = true;
	}

	logger_filter_loaded = 1;
}

void logger_printf(const char *id, const char *format, ...) {
    SCP_string temp;
    va_list args;

    if ((id == nullptr) || (format == nullptr))
        return;
    
    va_start(args, format);
	// vsprintf(temp, format, args);
	va_end(args);

	logger_print(id, temp.c_str());

}

void logger_print(const char *id, const char *tmp) {
    if ((id == nullptr) || (tmp == nullptr) || !logger_inited)
		return;

    if (Log_debug_output_to_file) {
		if (Log_fp != nullptr) {
			fputs(tmp, Log_fp);
			fflush(Log_fp);
		}
	}
}

void logger_init() {
    if (logger_inited)
        return;

    if (Log_fp == nullptr) {
        SCP_string logpath;
        if (!Cmdline_log_to_stdout) {

            Logfilename = "game.log";

            logpath = Logfilename;

			Log_fp       = fopen(logpath.c_str(), "wb");
			Log_close_fp = true;


        } else {
            Log_fp = stdout;
            Log_close_fp = false;
            logpath = "<stdout>";
        }
    }
    
}

void logger_close() {
    if(Log_fp != nullptr) {
        time_t timedate = time(nullptr);
        char datestr[50];

        memset(datestr, 0, sizeof(datestr));
        strftime(datestr, sizeof(datestr)-1, "%a %b %d %H:%M:%S %Y", localtime(&timedate));

        logger_printf("General", "... Log closed, %s\n", datestr);

        if (Log_close_fp) {
			fclose(Log_fp);
		}
		Log_fp       = nullptr;
		Log_close_fp = false;
    }

    logger_inited = false;

}