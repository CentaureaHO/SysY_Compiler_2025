#ifndef __COMMON_ARRAY_INDEXING_H__
#define __COMMON_ARRAY_INDEXING_H__

#include <vector>

/*
    将线性索引转换为多维索引

    输入：
    - dims: 任意支持反向迭代器的容器
    - flat_idx: 线性索引
    - ret: 任意支持push_front操作的容器

    此处实际为多维索引到线性索引的逆操作
    对于
*/
#define LINEAR_TO_MULTI_INDEX(dims, flat_idx, ret)                 \
    {                                                              \
        (ret).clear();                                             \
        int idx = (flat_idx);                                      \
        for (auto it = (dims).rbegin(); it != (dims).rend(); ++it) \
        {                                                          \
            (ret).push_front(idx % *it);                           \
            idx /= *it;                                            \
        }                                                          \
    }

int FindMinStepForPosition(const std::vector<int>& dims, int relativePos, int dimsIdx, int& max_subBlock_sz);

#endif