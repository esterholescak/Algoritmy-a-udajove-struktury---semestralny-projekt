#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <limits>
#include "StopData.h"
#include "StopFilter.h"
#include "HierarchyNavigator.h"
#include "StopSorter.h"
#include "StopHashTable.h"

class ConsoleManager {
public:

    ConsoleManager(
        std::vector<StopData>& allStops,
        HierarchyNavigator::Tree* tree,
        StopHashTable& stopHashTable
    );

    void runApplication();

private:

    std::vector<StopData>& allStops_;
    HierarchyNavigator navigator_;
    StopHashTable& stopHashTable_;

    std::vector<StopData> currentFilteredStops_;

    void printSeparator(const std::string& title) const;
    void printStopList(const std::vector<StopData>& stops, const std::string& header) const;
    void printImplicitStopSequence(const ds::amt::ImplicitSequence<StopData>& stops, const std::string& header) const;

    char getCharCommand(const std::string& prompt) const;
    int getIntInput(const std::string& prompt) const;
    double getDoubleInput(const std::string& prompt) const;
    std::string getStringInput(const std::string& prompt) const;
    std::string getLineInput(const std::string& prompt) const;

    void handleLevel1();
    void handleLevel2();
    void handleLevel3();
    void handleLevel4(std::vector<StopData>& stopsToProcess);
};