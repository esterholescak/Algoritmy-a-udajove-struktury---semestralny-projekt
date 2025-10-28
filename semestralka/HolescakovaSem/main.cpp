#include "StopData.h"
#include "FileReader.h"
#include "StopFilter.h"
#include "HierarchyBuilder.h"
#include "HierarchyNavigator.h"
#include "StopHashTable.h"
#include "StopSorter.h"
#include "ConsoleManager.h"
#include <libds/adt/sorts.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <limits>
#include <libds/heap_monitor.h>

int main() {
    initHeapMonitor();

    std::cout << "Loading stops data...\n";
    std::vector<StopData> allStops;
    try {
        allStops = FileReader::loadFromFile("GRT_Stops.csv");
        std::cout << "\nSuccessfully loaded " << allStops.size() << " stops.\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    if (allStops.empty()) {
        std::cerr << "No stops loaded. Exiting." << std::endl;
        return 1;
    }

    HierarchyNavigator::Tree* tree = HierarchyBuilder::build(allStops);

    StopHashTable stopHashTable;
    stopHashTable.populate(allStops);

    ConsoleManager consoleManager(allStops, tree, stopHashTable);
    consoleManager.runApplication();

    delete tree;

    return 0;
}