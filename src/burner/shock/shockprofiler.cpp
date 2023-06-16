#include "shock/shockprofiler.h"

HashTable<const char*, Timer, TimerCount> TimerTree::m_timers;
Tree<Timer*, TimerCount> TimerTree::m_tree;
TimerTree::TimerNode* TimerTree::m_node;
