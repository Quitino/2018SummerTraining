#ifndef SET_LEARN_N
#define SET_LEARN_N

#include <iostream>
#include <set>
#include <functional>

//set 集合   multiset 多重集

//set是一个集合容器，其中所包含的元素是唯一的，集合中的元素按一定的顺序排列。元素插入过程是按排序规则插入，所以不能指定插入位置。
//set采用红黑树变体的数据结构实现，红黑树属于平衡二叉树。在插入操作和删除操作上比vector快。
//set不可以直接存取元素。（不可以使用at.(pos)与[]操作符）。
//multiset与set的区别：set支持唯一键值，每个元素值只能出现一次；而multiset中同一值可以出现多次。
//不可以直接修改set或multiset容器中的元素值，因为该类容器是自动排序的。如果希望修改一个元素值，必须先删除原有的元素，再插入新的元素。


bool SetLearn(void);

#endif