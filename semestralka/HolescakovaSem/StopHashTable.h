#pragma once
#include <libds/adt/table.h>
#include "StopData.h"

class StopHashTable : public ds::adt::HashTable<int, StopData*> { //dedim
public:
    StopHashTable();

    void populate(const std::vector<StopData>& stops);
    StopData* findStopById(int id) const;
    void insert(const int& key, StopData* data) override;

    ~StopHashTable();
};