// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/weather_load_case.h"

#include <cmath>

WeatherLoadCase::WeatherLoadCase() {
  density_ice = -999999;
  description = "";
  pressure_wind = -999999;
  temperature_cable = -999999;
  thickness_ice = -999999;
}

WeatherLoadCase::~WeatherLoadCase() {
}

bool WeatherLoadCase::Validate(const bool& is_included_warnings,
                               std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates density-ice
  if (density_ice < 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("WEATHER LOAD CASE - Invalid ice density");
    }
  }

  // validates pressure-wind
  if (pressure_wind < 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("WEATHER LOAD CASE - Invalid wind pressure");
    }
  }

  // validates temperature-cable
  if (temperature_cable < -50) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("WEATHER LOAD CASE - Invalid cable "
                                "temperature");
    }
  }

  // validates thickness-ice
  if (thickness_ice < 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("WEATHER LOAD CASE - Invalid ice thickness");
    }
  }

  return is_valid;
}
