#ifndef THREAD_CORE_H_
#define THREAD_CORE_H_

#include "../../includes.h"

class ShockThreadCore
{
protected:
    struct ThreadArgs
    {
        ShockThreadProc proc;
        void* param;
    };

    HANDLE m_handle;
    ThreadArgs m_args;

    static DWORD WINAPI ThreadProc(void* param);
};

#endif