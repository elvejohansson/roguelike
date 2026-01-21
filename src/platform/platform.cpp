#include "platform.h"

#ifdef PLATFORM_WINDOWS
bool platformCreate_windows(Platform *out);
#elif defined(PLATFORM_MAC)
bool platformCreate_mac(Platform *out);
#elif defined(PLATFORM_LINUX)
bool platformCreate_linux(Platform *out);
#endif

bool platformInit(Platform *out) {
#ifdef PLATFORM_WINDOWS
    platformCreate_windows(out);
#elif defined(PLATFORM_MAC)
    platformCreate_mac(out);
#elif defined(PLATFORM_LINUX)
    platformCreate_linux(out);
#else
    return false;
#endif
    if (out->api.init) {
        out->api.init(out);
        return true;
    }
    return false;
}

void platformShutdown(Platform *p) {
    if (!p) {
        return;
    }

    if (p->api.shutdown) {
        p->api.shutdown(p);
    }

    p->api = {};
    p->window = nullptr;
    p->state = nullptr;
}
