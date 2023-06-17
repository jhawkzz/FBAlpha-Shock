#include "shock/shockprofilers.h"

HashTable<const char*, ShockProfiler, ShockProfilerCount> ShockProfilers::mProfilers;
Tree<ShockProfiler*, ShockProfilerCount> ShockProfilers::mTree;
ShockProfilers::ShockProfilerNode* ShockProfilers::mNode;
