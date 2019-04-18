#ifndef MAP_LEARN_H
#define MAP_LEARN_H

#include <iostream>
#include <string>
#include <map>
#include <functional>

//map是标准的关联式容器，一个map是一个键值对序列，即(key,value)对。它提供基于key的快速检索能力
//map中key值是唯一的。集合中的元素按一定的顺序排列。元素插入过程是按排序规则插入，所以不能指定插入位置
//map的具体实现采用红黑树变体的平衡二叉树的数据结构。在插入操作和删除操作上比vector快
//map可以直接存取key所对应的value，支持[]操作符，如map[key]=value
//multimap与map的区别：map支持唯一键值，每个键只能出现一次；而multimap中相同键可以出现多次。multimap不支持[]操作符

bool MapLearn(void);

#endif