#include <common/array/indexing.h>
#include <algorithm>
#include <iostream>
using namespace std;

int FindMinStepForPosition(const vector<int>& dims, int linear_index, int dimsIdx, int& max_subBlock_sz)
{
    int min_dim_step = 1;
    max_subBlock_sz  = 1;
    for (size_t i = dimsIdx + 1; i < dims.size(); ++i) max_subBlock_sz *= dims[i];
    while (linear_index % max_subBlock_sz != 0)
    {
        ++min_dim_step;
        max_subBlock_sz /= dims[dimsIdx + min_dim_step - 1];
    }
    cerr << "min_dim_step: " << min_dim_step << ", max_subBlock_sz: " << max_subBlock_sz << endl;
    return min_dim_step;
}