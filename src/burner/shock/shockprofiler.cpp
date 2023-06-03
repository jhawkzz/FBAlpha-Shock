#include "shock/shockprofiler.h"

scHashTable<scTimer, const char*, 64> scTimerTree::m_timers;
scTree<scTimer*, 64> scTimerTree::m_tree;
scTimerTree::TimerNode* scTimerTree::m_node;
