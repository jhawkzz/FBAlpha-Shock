
// See License.md for license

#include "../../includes.h"

ShockThread::~ShockThread()
{
    CloseHandle(mHandle);
}

int ShockThread::Create(ShockThreadProc proc, void* param)
{
    mArgs.proc = proc;
    mArgs.param = param;

    mHandle = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);

    return mHandle ? 0 : -1;
}

DWORD WINAPI ShockThreadCore::ThreadProc(void* param)
{
    ShockThread* st = (ShockThread*) param;
    st->mArgs.proc(st->mArgs.param);

    return 0;
}
