#include "shock/shockprofiler.h"

#ifdef SHOCK_PROFILER

HashTable<const char*, ShockProfilerNode, ShockProfilerNodeCount> ShockProfiler::mNodes;
Tree<ShockProfilerNode*, ShockProfilerNodeCount> ShockProfiler::mTree;
ShockProfiler::ShockProfilerTreeNode* ShockProfiler::mTreeNode;

#endif