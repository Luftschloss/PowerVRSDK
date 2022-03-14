#include "sotest.h"
#include "../3rd/include/PVRScopeStats.h"

extern "C" {

    int Init()
    {
        SPVRScopeImplData* data;
        int re = PVRScopeInitialise(&data);
        return re;
    }
}
