/* SDL based application */
#include "IronBarons.h"
#include <SDL.h>

#include "core\logging.h"

int main(int argc, char *argv[]) {
    int result = -1;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
	// Assert(argc > 0);
    logger_init();
}