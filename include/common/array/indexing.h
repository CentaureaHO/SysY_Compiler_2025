#ifndef __COMMON_ARRAY_INDEXING_H__
#define __COMMON_ARRAY_INDEXING_H__

#include <vector>

int  FindMinStepForPosition(const std::vector<int>& dims, int relativePos, int dimsIdx, int& max_subBlock_sz);
void LinearToMultiIndex(const std::vector<int>& dims, int absoluteIndex, std::vector<int>& ret);

#endif