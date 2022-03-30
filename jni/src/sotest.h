#ifndef __SOTest_H__
#define __SOTest_H__

#include "../3rd/include/PVRScopeStats.h"
#include <android/log.h>

#define TAG "PVR"  
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)   
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)    
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__)    
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)   
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)   

#if 0
#define EXPORT_DLL __declspec(dllexport) //导出dll声明
#else
#define EXPORT_DLL 
#endif

extern "C" {

    SPVRScopeImplData* scopeData;

    SPVRScopeCounterDef* counters;

    SPVRScopeCounterReading reading;

    /// <summary>
    /// Counter数量
    /// </summary>
    unsigned int counterNum;
    unsigned int activeGroup;

    EXPORT_DLL void PVRStart();

    EXPORT_DLL int Init();

    EXPORT_DLL int UpdateCounterList();

    EXPORT_DLL void SetGroup(unsigned int activeGroup);

    EXPORT_DLL char* GetPVRScopeGetDescription();

    EXPORT_DLL SPVRScopeTimingPacket* GetPVRScopeReadTimingData(unsigned int* const pnCount);

    EXPORT_DLL unsigned int GetCounters();

	
}

#endif
