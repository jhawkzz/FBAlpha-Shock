#ifndef CORE_H_
#define CORE_H_

#include "shock/platform/core.h"

class Core : public CoreImpl
{
public:
   ~Core();

   int Update();
};

#endif