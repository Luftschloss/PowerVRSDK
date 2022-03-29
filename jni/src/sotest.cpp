#include "sotest.h"
#include <string.h>

extern "C" {

	inline const char* ToString(EPVRScopeInitCode v)
	{
		switch (v)
		{
			case ePVRScopeInitCodeOk:   return "ePVRScopeInitCodeOk";
			case ePVRScopeInitCodeOutOfMem:   return "ePVRScopeInitCodeOutOfMem";
			case ePVRScopeInitCodeDriverSupportNotFound: return "ePVRScopeInitCodeDriverSupportNotFound";
			case ePVRScopeInitCodeDriverSupportInsufficient: return "ePVRScopeInitCodeDriverSupportInsufficient";
			case ePVRScopeInitCodeDriverSupportInitFailed: return "ePVRScopeInitCodeDriverSupportInitFailed";
			case ePVRScopeInitCodeDriverSupportQueryInfoFailed: return "ePVRScopeInitCodeDriverSupportQueryInfoFailed";
		}
	}

    int Init()
    {
		reading.pfValueBuf = nullptr;
		reading.nValueCnt = 0;
		reading.nReadingActiveGroup = 99;
		EPVRScopeInitCode re = PVRScopeInitialise(&scopeData);
		LOGI("InitResult: %s", ToString(re));
		if (scopeData == 0)
			LOGI("Null scopeData");
		else
			LOGI("scopeData：%d", sizeof(*scopeData));
        return (int)re;
    }

	int UpdateCounterList()
	{
		int re = PVRScopeGetCounters(scopeData, &numCounter, &counters, &reading);
		if (re)
		{
			LOGI("UpdateCounterList %i ", numCounter); // numCounter为0
		}
		else
		{
			LOGI("UpdateCounterList Faild");
			numCounter = 0;
		}
		return numCounter;
	}

    char* GetPVRScopeGetDescription()
    {
        return (char*)PVRScopeGetDescription();
    }

	void SetGroup(unsigned int group)
	{
		PVRScopeSetGroup(scopeData, group);
		activeGroup = group;
	}

    SPVRScopeTimingPacket* GetPVRScopeReadTimingData(unsigned int* const pnCount)
    {
        const struct SPVRScopeTimingPacket* timePacket;
        timePacket = PVRScopeReadTimingData(scopeData, pnCount);
        return (SPVRScopeTimingPacket*)timePacket;
    }

    unsigned int GetCounter(EPVRScopeStandardCounter standardCounter)
    {
        unsigned int counterNum;
		unsigned int counterIdx;
		unsigned int readCounter = 0;
		
		if (scopeData)
		{
			SPVRScopeCounterReading* psReading = nullptr;
			LOGD("GetCounter1");
			psReading = &reading;
			
			if (PVRScopeReadCounters(scopeData, psReading) && psReading)
			{
				LOGD("GetCounter2");
				readCounter = reading.nValueCnt;
				// When the active group is changed, retrieve new indices
				counterIdx = PVRScopeFindStandardCounter(counterNum, counters, activeGroup, standardCounter);
				counterNum = reading.pfValueBuf[counterIdx];
				numCounter = 25;
				for (int i = 0; i < numCounter; ++i)
				{
					LOGI("Idx:%i-->%s", i, counters[i].pszName);
					if (i < reading.nValueCnt)
					{
						////Print the 3D Load
						//if (strcmp(counters[i].pszName, "GPU task load: 3D core") == 0)
						//{
						//	LOGD("%s : %f\%", psCounters[i].pszName, sReading.pfValueBuf[i]);
						//}
						////Print the TA Load
						//else if (strcmp(counters[i].pszName, "GPU task load: TA core") == 0)
						//{
						//	
						//	LOGD("%s : %f\%", psCounters[i].pszName, sReading.pfValueBuf[i]);
						//}
					}
				}
			}
		}
        return readCounter;
    }


}
