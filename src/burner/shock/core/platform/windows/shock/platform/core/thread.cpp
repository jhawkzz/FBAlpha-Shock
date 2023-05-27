
// See License.md for license

#include "shock/core/thread.h"

Thread::~Thread()
{
    CloseHandle(mHandle);
}

int Thread::Create(ShockThreadProc proc, void* param)
{
    mArgs.proc = proc;
    mArgs.param = param;

    mHandle = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);

    return mHandle ? 0 : -1;
}

DWORD WINAPI ThreadCore::ThreadProc(void* param)
{
    Thread* st = (Thread*) param;
    st->mArgs.proc(st->mArgs.param);

    return 0;
}
