#ifndef __HUNGARIAN_H__
#define __HUNGARIAN_H__

#include <map>
#include <vector>

std::vector<std::pair<int, int>>
hungarian(const std::vector<std::vector<int>> &costs);

#endif
