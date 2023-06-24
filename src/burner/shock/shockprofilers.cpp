#include "shock/shockprofilers.h"

#ifdef SHOCK_PROFILERS

HashTable<const char*, ShockProfiler, ShockProfilerCount> ShockProfilers::mProfilers;
Tree<ShockProfiler*, ShockProfilerCount> ShockProfilers::mTree;
ShockProfilers::ShockProfilerNode* ShockProfilers::mNode;

#endif