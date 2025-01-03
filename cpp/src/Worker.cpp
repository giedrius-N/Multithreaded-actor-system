#include "Worker.hpp"
#include "caf/actor_ostream.hpp"

void worker_actor_state::process_cities(std::vector<City>& cities)
{
    for (const auto& city : cities)
    {
        int comfortIndex = calculate_comfort_index(city);
        self->println("City: {}, Comfort Index: {}", city.name, comfortIndex);
    }
}

int worker_actor_state::calculate_comfort_index(const City& city)
{
    int sunnyDays = city.sunnyDays;
    double averageTemp = city.averageTemp;

    // Mock calculation for mocking long running operation
    std::vector<double> dayCoef;
	for (int i = 1; i < sunnyDays + 1; i++)
	{
		double dayCoefTemp = i * averageTemp * (averageTemp / sunnyDays);
		dayCoef.push_back(dayCoefTemp);
	}
	double comfIndexTemp = 0;
	for (int i = 0; i < dayCoef.size(); i++)
	{
		comfIndexTemp += dayCoef[i] * averageTemp;
	}
	int z = 0;
	while (z < 10000000)
	{
		z++;
	}
	comfIndexTemp = comfIndexTemp / (dayCoef.size() * 100);


    return ((int) sunnyDays * (averageTemp / 100) * comfIndexTemp);
}

worker_actor::behavior_type worker_actor_state::make_behavior()
{
    return 
    {
        [this](City city)
        {
            std::vector cities = {city};
            process_cities(cities);
        },
        [this](caf::unit_t)
        {
            self->println("Worker actor triggered with unit_t");
        },
        [this](std::vector<City> cities)
        {
            process_cities(cities);
        }
    };
}
