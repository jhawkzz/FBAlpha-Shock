#include "shock/shockprofiler.h"

scHashTable<scTimer, const char*, TimerCount> scTimerTree::m_timers;
scTree<scTimer*, TimerCount> scTimerTree::m_tree;
scTimerTree::TimerNode* scTimerTree::m_node;
