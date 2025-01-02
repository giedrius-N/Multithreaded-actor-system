#ifndef CITY_TYPES_HPP
#define CITY_TYPES_HPP

#include "caf/all.hpp"
#include <vector>
#include <string>

struct City 
{
    int id;
    std::string name;
    int sunnyDays;
    double averageTemp;
};

template <class Inspector>
bool inspect(Inspector& f, City& x) 
{
    return f.object(x).fields(
        f.field("id", x.id),
        f.field("name", x.name),
        f.field("sunnyDays", x.sunnyDays),
        f.field("averageTemp", x.averageTemp)
    );
}

CAF_BEGIN_TYPE_ID_BLOCK(custom_types, caf::first_custom_type_id)

CAF_ADD_TYPE_ID(custom_types, (City))                  
CAF_ADD_TYPE_ID(custom_types, (std::vector<City>))     

CAF_END_TYPE_ID_BLOCK(custom_types)

#endif // CITY_TYPES_HPP
