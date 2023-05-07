#ifndef SHOCKMAIN_CORE_H_
#define SHOCKMAIN_CORE_H_

class ShockMainCore
{
public:
    static void SetWindow(HWND hwnd);

protected:
    static int Update();

protected:
    static HWND m_hwnd;
};

#endif