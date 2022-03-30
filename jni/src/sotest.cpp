#include "sotest.h"
#include <string.h>
#include <unistd.h>

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

	void PVRStart()
	{
		//Internal control data
		SPVRScopeImplData* psData = nullptr;

		//Counter information (set at uint time)
		SPVRScopeCounterDef* psCounters = nullptr;
		unsigned int            uCounterNum = 0;

		SPVRScopeCounterReading sReading;
		memset(&sReading, 0, sizeof(sReading));

		// Step 1. initialise PVRScopeStats
		const EPVRScopeInitCode eInitCode = PVRScopeInitialise(&psData);

		if (ePVRScopeInitCodeOk == eInitCode)
		{
			LOGI("Initialised services connection.\n");
		}
		else
		{
			LOGE("Error: failed to initialise services connection.\n");
		}

		usleep(1 * 1000 * 1000);

		// Step 2. Set the active group to 0
		PVRScopeSetGroup(psData, 0);
		

		unsigned int sampleRate = 10;
		unsigned int loop = 0;

		//Step 3: get counters
		PVRScopeGetCounters(psData, &uCounterNum, &psCounters, &sReading);
		//Print each and every counter (and its group)
		LOGI("Found below the list of counters:");
		for (int i = 0; i < uCounterNum; ++i)
		{
			LOGI("Group %d %s", psCounters[i].nGroup, psCounters[i].pszName);
		}

		// Step 4. loop read & counter update
		LOGI("start loopint read ");
		while (loop < 20000)
		{
			//Poll for the counters once an interval
			usleep(100 * 1000);

			if (((loop) % sampleRate) > 0)
			{
				// Sample the counters every 10ms. Don't read or output it.
				//LOGI("%s: %d", "skip counter read ", loop);
				PVRScopeReadCounters(psData, NULL);
			}
			else
			{
				// Step 4. Read and output the counter information for group 0 to Logcat
				if (PVRScopeReadCounters(psData, &sReading))
				{
					LOGI("%d counters read out!", sReading.nValueCnt);

					// Check for all the counters in the system if the counter has a value on the given active group and ouptut it.
					for (int i = 0; i < sReading.nValueCnt; ++i)
					{
						if (i < uCounterNum) {
							if (psCounters[i].nBoolPercentage) {
								LOGI("%s : %f%%", psCounters[i].pszName, sReading.pfValueBuf[i]);
							}
							else {
								LOGI("%s : %f", psCounters[i].pszName, sReading.pfValueBuf[i]);
							}
						}
						else {
							LOGI("unamed-%d : %f", i, sReading.pfValueBuf[i]);
						}
					}

					// If we have too many results, there may be new counters available
					if (uCounterNum < sReading.nValueCnt || uCounterNum == 0)
					{
						PVRScopeGetCounters(psData, &uCounterNum, &psCounters, &sReading);

						//Print each and every counter (and its group)
						LOGI("Updated with below the list of counters:");
						for (int i = 0; i < uCounterNum; ++i)
						{
							LOGI("    Group %d %s", psCounters[i].nGroup, psCounters[i].pszName);
						}
					}
				}
				else {
					LOGI("%s: %d", "no valid counters this time ", loop);
				}
			}

			++loop;
		}

		// Step 5. Shutdown PVRScopeStats
		PVRScopeDeInitialise(&psData, &psCounters, &sReading);

		LOGI("exiting well");

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
			LOGI("scopeDate Not nptr");
        return (int)re;
    }

	int UpdateCounterList()
	{
		int re = PVRScopeGetCounters(scopeData, &counterNum, &counters, &reading);
		if (re)
		{
			LOGI("Found below the list of counters(%i): ", counterNum); // numCounter为0
			for (int i = 0; i < counterNum; ++i)
			{
				LOGI("Counter:%s in Group（%d）", counters[i].pszName, counters[i].nGroup);
			}
		}
		else
		{
			LOGI("UpdateCounterList Faild");
			counterNum = 0;
		}
		return counterNum;
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

    unsigned int GetCounters()
    {
		unsigned int counterIdx;
		
		if (scopeData)
		{
			LOGI("GetCounter1");
			
			if (PVRScopeReadCounters(scopeData, &reading))
			{
				LOGI("%d Counters read out:", reading.nValueCnt);
				for (int i = 0; i < reading.nValueCnt; ++i)
				{
					if (i < counterNum) {
						if (counters[i].nBoolPercentage) {
							LOGI("%s : %f%%", counters[i].pszName, reading.pfValueBuf[i]);
						}
						else {
							LOGI("%s : %f", counters[i].pszName, reading.pfValueBuf[i]);
						}
					}
					else {
						LOGI("unamed-%d : %f", i, reading.pfValueBuf[i]);
					}
				}

				// If we have too many results, there may be new counters available
				if (counterNum < reading.nValueCnt || counterNum == 0)
				{
					UpdateCounterList();
				}
				return reading.nValueCnt;
			}
		}
		return 0;
    }


}
