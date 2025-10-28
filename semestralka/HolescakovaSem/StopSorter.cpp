#include "StopSorter.h"
#include "StopData.h" 
#include <vector>
#include <functional>
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h> 

StopSorter::StopSorter(ds::adt::Sort<StopData>* sorterAlgorithm) : sorterAlgorithm_(sorterAlgorithm) {}

ds::amt::ImplicitSequence<StopData> StopSorter::sort(const std::vector<StopData>& stops, StopComparator comparator) {
    ds::amt::ImplicitSequence<StopData> tempSequence(stops.size(), true);

    //naplnime sekvenciu zastavkami z vektora
    for (size_t i = 0; i < stops.size(); ++i) {
        tempSequence.access(i)->data_ = stops[i];
    }

    auto booleanCompare = [&](const StopData& a, const StopData& b) -> bool {
		return comparator(a, b) < 0; //vrati true, ak a < b
        };

    sorterAlgorithm_->sort(tempSequence, booleanCompare);

    return tempSequence;
}