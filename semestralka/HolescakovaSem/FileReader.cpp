#include "FileReader.h" 
#include "StopData.h"
#include <fstream>      
#include <sstream>
#include <iostream>     
#include <stdexcept>   

std::vector<StopData> FileReader::loadFromFile(const std::string& filename) {
    std::vector<StopData> stops;
    std::ifstream file(filename); //automaticky otvori aj zatvori subor

    if (!file.is_open()) { //kontrola ci sa subor otvoril
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {}; //vrati prazdny vector - error
    }

    std::string line;
    if (!std::getline(file, line)) { //kontrola ci subor neni prazdny, zahodenie hlavicky
        std::cerr << "CSV file is empty: " << filename << std::endl;
        return stops; //vrati prazdny vector - warning
    }

    int lineNumber = 1;
    while (std::getline(file, line)) {
        lineNumber++;
        std::istringstream ss(line); //prechadzanie konkretneho riadku
        std::string item;
        StopData stop;

        try {
            std::getline(ss, item, ';'); //x
            std::getline(ss, item, ';'); //y

			std::getline(ss, item, ';'); //objectid

            std::getline(ss, item, ';'); //stopid
            if (item.empty()) {
                throw std::invalid_argument("Missing StopId");
            }
            stop.id = std::stoi(item); //konvertuje string na int

            std::getline(ss, item, ';'); //easygo

            std::getline(ss, item, ';'); //street
            if (item.empty()) {
                throw std::invalid_argument("Missing Street");
            }
            stop.street = item;

            std::getline(ss, item, ';'); //crossstreet
            std::getline(ss, item, ';'); //easting
            std::getline(ss, item, ';'); //northing

            std::getline(ss, item, ';'); //longitude
            if (item.empty()) {
                throw std::invalid_argument("Missing Longitude");
            }
            stop.longitude = std::stod(item); //konvertuje string na double

            std::getline(ss, item, ';'); //latitude
            if (item.empty()) {
                throw std::invalid_argument("Missing Latitude");
            }
            stop.latitude = std::stod(item);

            std::getline(ss, item, ';'); //municipality
            if (item.empty()) {
                throw std::invalid_argument("Missing Municipality");
            }
            stop.municipality = item;

            stops.push_back(stop); //pridanie stop do vectoru

        }
        catch (const std::exception& e) { //vyhadzuje vynimku do outputu
            std::cerr << "Error processing line " << lineNumber << ": " << line << " - " << e.what() << std::endl;
        }
    }
    return stops;
}