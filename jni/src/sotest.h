#ifndef __SOTest_H__
#define __SOTest_H__

#if 0
#define EXPORT_DLL __declspec(dllexport) //导出dll声明
#else
#define EXPORT_DLL 
#endif

extern "C" {

    EXPORT_DLL int Init();
}

#endif
