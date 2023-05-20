#ifndef THREADIMPL_H_
#define THREADIMPL_H_

#include "../../includes.h"

class ShockThreadImpl
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