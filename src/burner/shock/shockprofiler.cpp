#include "shock/shockprofiler.h"

HashTable<const char*, ShockProfiler, ShockProfilerCount> ShockProfilerTree::mProfilers;
Tree<ShockProfiler*, ShockProfilerCount> ShockProfilerTree::mTree;
ShockProfilerTree::ShockProfilerNode* ShockProfilerTree::mNode;
