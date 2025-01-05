/*
* CityTypes.hpp
* Definition of the City struct
*/
#ifndef CITY_TYPES_HPP
#define CITY_TYPES_HPP

#include "caf/all.hpp"
#include <vector>
#include <string>

struct City 
{
    std::string id;
    std::string name;
    int sunnyDays;
    double averageTemp;
};

template <class Inspector>
bool inspect(Inspector& f, City& x) 
{
    return f.object(x).fields
    (
        f.field("id", x.id),
        f.field("name", x.name),
        f.field("sunnyDays", x.sunnyDays),
        f.field("averageTemp", x.averageTemp)
    );
}

#endif // CITY_TYPES_HPP
