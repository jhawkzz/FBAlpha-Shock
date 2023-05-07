
// See License.md for license

#include "../../includes.h"

ShockThread::~ShockThread()
{
    CloseHandle(m_handle);
}

int ShockThread::Create(ShockThreadProc proc, void* param)
{
    m_args.proc = proc;
    m_args.param = param;

    m_handle = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);

    return m_handle ? 0 : -1;
}

DWORD WINAPI ShockThreadCore::ThreadProc(void* param)
{
    ShockThread* st = (ShockThread*) param;
    st->m_args.proc(st->m_args.param);

    return 0;
}
