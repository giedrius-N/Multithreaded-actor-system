/*
* AtomConfig.hpp
* Configuration file for CAF atoms and city type
*/
#ifndef ATOM_CONFIG_HPP
#define ATOM_CONFIG_HPP

#include "caf/all.hpp"
#include "CityTypes.hpp"

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(system, first_custom_type_id)

CAF_ADD_TYPE_ID(system, (City))
CAF_ADD_TYPE_ID(system, (std::vector<City>))  
CAF_ADD_ATOM(system, send_city)
CAF_ADD_ATOM(system, send_cities)
CAF_ADD_ATOM(system, python_done)
CAF_ADD_ATOM(system, get_cities_from_python)
CAF_ADD_ATOM(system, send_printer)
CAF_ADD_ATOM(system, done_processing)
CAF_ADD_ATOM(system, send_json_path)
CAF_ADD_ATOM(system, start_processing)

CAF_END_TYPE_ID_BLOCK(system)

#endif