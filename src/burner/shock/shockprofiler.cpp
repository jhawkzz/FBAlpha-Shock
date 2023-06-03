#include "shock/shockprofiler.h"

scTree<scTimer*, 64> scTimerTree::m_tree;
scTimerTree::TimerNode* scTimerTree::m_node;
