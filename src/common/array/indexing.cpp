#include <common/array/indexing.h>
#include <algorithm>
using namespace std;

int FindMinStepForPosition(const vector<int>& dims, int relativePos, int dimsIdx, int& max_subBlock_sz)
{
    int min_dim_step = 1;
    int blockSz      = 1;
    for (size_t i = dimsIdx + 1; i < dims.size(); ++i) blockSz *= dims[i];
    while (relativePos % blockSz != 0)
    {
        ++min_dim_step;
        blockSz /= dims[dimsIdx + min_dim_step - 1];
    }
    max_subBlock_sz = blockSz;
    return min_dim_step;
}

void LinearToMultiIndex(const vector<int>& dims, int absoluteIndex, vector<int>& ret)
{
    ret.clear();
    ret.reserve(dims.size());

    for (auto it = dims.rbegin(); it != dims.rend(); ++it)
    {
        ret.push_back(absoluteIndex % *it);
        absoluteIndex /= *it;
    }

    reverse(ret.begin(), ret.end());
}