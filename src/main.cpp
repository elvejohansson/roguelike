#include "core/assert.h"
#include "core/logger.h"

int main(void) {
    Log(LogLevel::DEBUG, "Started program.");
    ASSERT(true);
}
