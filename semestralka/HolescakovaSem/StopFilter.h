#pragma once
#include "StopData.h"
#include <vector>
#include <functional> //na function - lambda funkcie
#include <string>
#include <algorithm>
#include <cctype>

class StopFilter {
public:

    template <typename Iterator, typename Predicate> //sablona
    static std::vector<StopData> filter(Iterator begin, Iterator end, Predicate pred) { //end ukazuje na miesto za poslednym prvkom
        std::vector<StopData> result;
        for (auto it = begin; it != end; ++it) {
            if (pred(*it)) { //*it - dereferencovanie - k prvku na ktory ukazuje
                result.push_back(*it);
            }
        }
        return result;
    }

    static std::function<bool(const StopData&)> isInMunicipality(const std::string& municipalityName) {
        std::string lowerSearchMunicipality = municipalityName;
        std::transform(lowerSearchMunicipality.begin(), lowerSearchMunicipality.end(), lowerSearchMunicipality.begin(),
            ::tolower);

        return [lowerSearchMunicipality](const StopData& s) {
            std::string lowerStopMunicipality = s.municipality;
            std::transform(lowerStopMunicipality.begin(), lowerStopMunicipality.end(), lowerStopMunicipality.begin(),
                ::tolower);
            return lowerStopMunicipality == lowerSearchMunicipality;
            };
    }

    static std::function<bool(const StopData&)> isOnStreet(const std::string& streetSubstring) {
        std::string lowerSearchSubstring = streetSubstring;
        std::transform(lowerSearchSubstring.begin(), lowerSearchSubstring.end(), lowerSearchSubstring.begin(),
            ::tolower);

        // [pristupne zvonka](vstupny parameter) -> predicate(zastavka)
        return [lowerSearchSubstring](const StopData& s) {
            std::string lowerStopStreet = s.street;
            std::transform(lowerStopStreet.begin(), lowerStopStreet.end(), lowerStopStreet.begin(),
                ::tolower);
            return lowerStopStreet.find(lowerSearchSubstring) != std::string::npos;
            };
    }

    static std::function<bool(const StopData&)> isInRegion(double minLat, double maxLat, double minLon, double maxLon) {
        return [minLat, maxLat, minLon, maxLon](const StopData& s) {
            return s.latitude >= minLat && s.latitude <= maxLat &&
                std::abs(s.longitude) >= std::abs(minLon) && std::abs(s.longitude) <= std::abs(maxLon);
            };
    }
};