#pragma once
#include "StopData.h"
#include <vector>
#include <functional>
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h>

using StopComparator = std::function<int(const StopData&, const StopData&)>;

class StopSorter {
public:
    StopSorter(ds::adt::Sort<StopData>* sorterAlgorithm);
    ds::amt::ImplicitSequence<StopData> sort(const std::vector<StopData>& stops, StopComparator comparator);

private:
    ds::adt::Sort<StopData>* sorterAlgorithm_;
};