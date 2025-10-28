#include "StopHashTable.h"
#include <vector>
#include <iostream>
#include <stdexcept>

StopHashTable::StopHashTable() : ds::adt::HashTable<int, StopData*>([](const int& key) { //hashovacia funkcia
	size_t h = static_cast<size_t>(key); //pretypovanie key z int na size_t
	h = ((h >> 16) ^ h) * 0x45d9f3b; //posunie bity, porovna bity, vynasobi magickym cislom
    h = ((h >> 16) ^ h) * 0x45d9f3b;
    h = (h >> 16) ^ h;
    return h;
    }, 5003) {} //kapacita tabulky

void StopHashTable::populate(const std::vector<StopData>& stops) {
    for (const auto& stop : stops) {
        this->insert(stop.id, new StopData(stop));
    }
}

StopData* StopHashTable::findStopById(int id) const {
    StopData* stop = nullptr;
	StopData** stopPtr = &stop; //ukazovatel na ukazovatel - T je StopData* a tryFind chce T*
    if (this->tryFind(id, stopPtr)) {
        return *stopPtr;
    }
    return stop;
}

void StopHashTable::insert(const int& key, StopData* data) {
    try {
        ds::adt::HashTable<int, StopData*>::insert(key, data);
    }
    catch (const std::logic_error& e) {
        delete data;
        std::cerr << "Debug: Duplicate key " << key << " detected and ignored.\n";
    }
}

StopHashTable::~StopHashTable() {
    for (const auto& item : *this) {
        delete item.data_;
    }
}