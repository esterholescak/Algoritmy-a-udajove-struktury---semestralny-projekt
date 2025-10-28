#include "ConsoleManager.h"
#include <functional>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

ConsoleManager::ConsoleManager(
    std::vector<StopData>& allStops,
    HierarchyNavigator::Tree* tree,
    StopHashTable& stopHashTable
) :
    allStops_(allStops),
    navigator_(*tree),
    stopHashTable_(stopHashTable)
{
}

void ConsoleManager::runApplication() {
	handleLevel1();
    handleLevel2();
    handleLevel3();

    std::cout << "\nApplication finished. Goodbye :)\n";
}

void ConsoleManager::printSeparator(const std::string& title) const {
    std::cout << "\n=========== " << title << " ===========\n";
}

void ConsoleManager::printStopList(const std::vector<StopData>& stops, const std::string& header) const {
    std::cout << "\n" << header << "\n";
    if (stops.empty()) {
        std::cout << "No stops to display." << std::endl;
        return;
    }
    for (const auto& stop : stops) {
        std::cout << stop.toString() << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Total stops displayed: " << stops.size() << std::endl;
}

void ConsoleManager::printImplicitStopSequence(const ds::amt::ImplicitSequence<StopData>& stops, const std::string& header) const {
    std::cout << "\n" << header << "\n";
    if (stops.isEmpty()) {
        std::cout << "No stops to display." << std::endl;
        return;
    }
    for (size_t i = 0; i < stops.size(); ++i) {
        std::cout << stops.access(i)->data_.toString() << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Total stops displayed: " << stops.size() << std::endl;
}

char ConsoleManager::getCharCommand(const std::string& prompt) const {
    char inputChar;
    std::cout << prompt;
    std::cin >> inputChar;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //zahodia sa znaky z predchadzajuceho vstupu
    return inputChar;
}

int ConsoleManager::getIntInput(const std::string& prompt) const {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

double ConsoleManager::getDoubleInput(const std::string& prompt) const {
    double value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string ConsoleManager::getStringInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

std::string ConsoleManager::getLineInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

void ConsoleManager::handleLevel1() {
    bool continueLevel1 = true;
    while (continueLevel1) {
        printSeparator("Level 1: FILTERING");

        char filterType = getCharCommand("Filter by [m]unicipality, [s]treet, [r]egion, or [q]uit: ");
        char lf = std::tolower(filterType);

        if (lf == 'q') {
            continueLevel1 = false;
            std::cout << "Exiting Level 1...\n";
            break;
        }

        std::function<bool(const StopData&)> predicate;
        std::string filterDescription = "";
        bool validFilterInput = false;

        if (lf == 'm') {
            std::string municipality = getLineInput("Enter municipality: ");
            predicate = StopFilter::isInMunicipality(municipality);
            filterDescription = "municipality: " + municipality;
            validFilterInput = true;
        }
        else if (lf == 's') {
            std::string street = getLineInput("Enter street (substring): ");
            predicate = StopFilter::isOnStreet(street);
            filterDescription = "street containing '" + street + "'";
            validFilterInput = true;
        }
        else if (lf == 'r') {
            double minLat = getDoubleInput("Enter min latitude: ");
            double maxLat = getDoubleInput("Enter max latitude: ");
            double minLon = getDoubleInput("Enter min longitude: ");
            double maxLon = getDoubleInput("Enter max longitude: ");

            predicate = StopFilter::isInRegion(minLat, maxLat, minLon, maxLon);
            filterDescription = "region (Lat: " + std::to_string(minLat) + " - " + std::to_string(maxLat) + ", Lon: " + std::to_string(minLon) + " - " + std::to_string(maxLon) + ")";
            validFilterInput = true;
        }
        else {
            std::cout << "Unknown filter type. Skipping this filter.\n";
        }

        if (validFilterInput) {
            std::cout << "\n=========== Stops matching " << filterDescription << " ===========\n";
            std::vector<StopData> filteredStops = StopFilter::filter(allStops_.begin(), allStops_.end(), predicate);
            printStopList(filteredStops, "Filtered Stops");
            handleLevel4(filteredStops);
        }
    }
}

void ConsoleManager::handleLevel2() {
    printSeparator("Level 2: HIERARCHY");

    char command;
    while (true) {
        navigator_.printCurrent();
        command = getCharCommand("Commands: [c]hild, [p]arent, [l]ist, [f]ilter, [q]uit > ");
        char ch = std::tolower(command);

        if (ch == 'q') {
            std::cout << "Exiting Level 2...\n";
            break;
        }
        else if (ch == 'p') {
            if (!navigator_.goToParent())
                std::cout << "Already at root.\n";
        }
        else if (ch == 'l') {
            navigator_.listChildren();
        }
        else if (ch == 'c') {
            int index = getIntInput("Enter child index: ");
            if (!navigator_.goToChild(index))
                std::cout << "Invalid index.\n";
        }
        else if (ch == 'f') {
            char filterType = getCharCommand("Filter by [m]unicipality, [s]treet, [r]egion: ");
            char lf = std::tolower(filterType);

            std::function<bool(const StopData&)> predicate;
            std::string filterDescription = "";
            bool validFilterInput = false;

            if (lf == 'm') {
                std::string municipality = getLineInput("Enter municipality: ");
                predicate = StopFilter::isInMunicipality(municipality);
                filterDescription = "municipality: " + municipality;
                validFilterInput = true;
            }
            else if (lf == 's') {
                std::string street = getLineInput("Enter street (substring): ");
                predicate = StopFilter::isOnStreet(street);
                filterDescription = "street containing '" + street + "'";
                validFilterInput = true;
            }
            else if (lf == 'r') {
                double minLat = getDoubleInput("Enter min latitude: ");
                double maxLat = getDoubleInput("Enter max latitude: ");
                double minLon = getDoubleInput("Enter min longitude: ");
                double maxLon = getDoubleInput("Enter max longitude: ");
                predicate = StopFilter::isInRegion(minLat, maxLat, minLon, maxLon);
                filterDescription = "region (Lat: " + std::to_string(minLat) + " - " + std::to_string(maxLat) + ", Lon: " + std::to_string(minLon) + " - " + std::to_string(maxLon) + ")";
                validFilterInput = true;
            }
            else {
                std::cout << "Unknown filter type. Skipping this filter.\n";
            }

            if (validFilterInput) {
                std::vector<StopData> allStopsInSubtree = navigator_.getAllStopsInSubtree();
                currentFilteredStops_ = StopFilter::filter(allStopsInSubtree.begin(), allStopsInSubtree.end(), predicate);
                printStopList(currentFilteredStops_, "Filtered stops from subtree matching " + filterDescription);
                handleLevel4(currentFilteredStops_);
            }
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }
}

void ConsoleManager::handleLevel3() {
    printSeparator("Level 3: SEARCHING FOR ID IN TABLE");

    while (true) {
        std::string input = getStringInput("Enter a stop ID to search (or type 'q' to exit Level 3): ");

        if (input == "q") {
            std::cout << "Exiting Level 3...\n";
            break;
        }

        try {
            int stopId = std::stoi(input);
            StopData* stop = stopHashTable_.findStopById(stopId);
            if (stop) {
                std::cout << "Stop found: " << stop->toString() << "\n";
            }
            else {
                std::cout << "Stop with ID " << stopId << " not found.\n";
            }
        }
        catch (const std::invalid_argument&) { //vyhadzuje vynimku do outputu
            std::cout << "Invalid input. Please enter a valid stop ID or 'q'.\n";
        }
    }
}

void ConsoleManager::handleLevel4(std::vector<StopData>& stopsToProcess) {
    if (stopsToProcess.empty()) {
        std::cout << "No stops to sort." << std::endl;
        return;
    }

    std::string sortChoice = getStringInput("\nDo you want to sort these stops? ([y]es/[n]o): ");

    if (sortChoice == "y" || sortChoice == "Y") {
        printSeparator("Sorting Stops");

        //vyuzivanie QuickSort 
        ds::adt::QuickSort<StopData> quickSortAlgorithm;
        StopSorter sorter(&quickSortAlgorithm); 

        StopComparator selectedComparator;
        char sortCriteriaChoice = getCharCommand("Choose sorting criteria:\n  [m] - Sort by Municipality (Street)\n  [i] - Sort by ID\n" "Enter choice: ");
        char ch = std::tolower(sortCriteriaChoice);

        //compareStreetMunicipality
        if (ch == 'm') {
            selectedComparator = [](const StopData& s1, const StopData& s2) -> int {
                std::string lowerS1Muni = s1.municipality;
                std::string lowerS2Muni = s2.municipality;
                std::transform(lowerS1Muni.begin(), lowerS1Muni.end(), lowerS1Muni.begin(), ::tolower);
                std::transform(lowerS2Muni.begin(), lowerS2Muni.end(), lowerS2Muni.begin(), ::tolower);

                if (lowerS1Muni < lowerS2Muni) {
                    return -1;
                }
                else if (lowerS1Muni > lowerS2Muni) {
                    return 1;
                }

				//obce su rovnake
                std::string lowerS1Street = s1.street;
                std::string lowerS2Street = s2.street;
                std::transform(lowerS1Street.begin(), lowerS1Street.end(), lowerS1Street.begin(), ::tolower);
                std::transform(lowerS2Street.begin(), lowerS2Street.end(), lowerS2Street.begin(), ::tolower);

                if (lowerS1Street < lowerS2Street) {
                    return -1;
                }
                else if (lowerS1Street > lowerS2Street) {
                    return 1;
                }
          
                return 0; //obce aj ulice rovnake
            };
            std::cout << "Sorting by Municipality (street)...\n";
        }
        //compareID
        else if (ch == 'i') {
            selectedComparator = [](const StopData& s1, const StopData& s2) -> int {
                if (s1.id < s2.id) {
                    return -1;
                }
                if (s1.id > s2.id) {
                    return 1;
                }
                return 0;
                };
            std::cout << "Sorting by ID...\n";
        }
        else {
            std::cout << "Invalid sorting criteria. Please enter 'm' or 'i'. No sorting applied.\n";
            selectedComparator = nullptr;
        }

        if (selectedComparator) {
            ds::amt::ImplicitSequence<StopData> sortedSequence = sorter.sort(stopsToProcess, selectedComparator);

            printImplicitStopSequence(sortedSequence, "Sorted Stops");
        }
    }
    else {
        std::cout << "Skipping sorting.\n";
    }
}