#include "core/assert.h"
#include "core/logger.h"
#include "core/math.h"

int main(void) {
    Log(LogLevel::DEBUG, "Started program.");
    ASSERT(true);
}
