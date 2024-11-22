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

/*
    @ref: https://github.com/yuhuifishash/SysY

    该函数用于在多维数组中确定当前线性索引所在的位置，
    计算需要深入的维度数（返回值），以及当前可以处理的最大子块大小（max_subBlock_sz）。

    参数：
    - dims：表示数组的各个维度大小的整数向量
    - linear_index：当前处理的元素在数组中的线性索引，即展开为一维数组时的位置
    - dimsIdx：当前处理的维度索引
    - max_subBlock_sz：用于返回在当前维度下可以处理的最大子块大小
                        例如，对于int a[5][4][3]
                        第一维度可以处理的最大子块大小即为4*3=12

    返回值：
    - min_dim_step：需要深入的维度数，即从当前维度开始，需要再深入多少个维度才能正确匹配初始化列表的嵌套层次
*/
int FindMinStepForPosition(const std::vector<int>& dims, int linear_index, int dimsIdx, int& max_subBlock_sz);

#endif