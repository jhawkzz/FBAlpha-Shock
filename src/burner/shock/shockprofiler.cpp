#include "shock/shockprofiler.h"

HashTable<const char*, Timer, TimerCount> TimerTree::mTimers;
Tree<Timer*, TimerCount> TimerTree::mTree;
TimerTree::TimerNode* TimerTree::mNode;
