#pragma once
#include <string>
#include <cmath>    
#include <iostream> 
#include <vector>  

struct StopData {
    int id = 0;
    double latitude = 0.0;
    double longitude = 0.0;
    std::string street;
    std::string municipality;

    bool operator==(const StopData& other) const { //kvoli double
        return id == other.id &&
            std::abs(latitude - other.latitude) < 1e-9 &&
            std::abs(longitude - other.longitude) < 1e-9 &&
            street == other.street &&
            municipality == other.municipality;
    }

    bool operator!=(const StopData& other) const { //prisposobenie zmeny ==
		return !(*this == other); //dereferencovanie, chcem aktualny objekt StopData
    }

	std::string toString() const { //vypis konkretnej stop
        return "ID: " + std::to_string(id) +
            ", LAT: " + std::to_string(latitude) +
            ", LON: " + std::to_string(longitude) +
            ", STREET: " + street +
            ", MUN: " + municipality;
    }
};