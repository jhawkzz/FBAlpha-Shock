#ifndef THREAD_CORE_H_
#define THREAD_CORE_H_

#include "shock/includes.h"

class ThreadCore
{
protected:
    struct ThreadArgs
    {
        ShockThreadProc proc;
        void* param;
    };

    HANDLE mHandle;
    ThreadArgs mArgs;

    static DWORD WINAPI ThreadProc(void* param);
};

#endif