// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable_constraint.h"

CableConstraint::CableConstraint() {
  limit = -999999;
}

CableConstraint::~CableConstraint() {}

bool CableConstraint::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validates case-weather
  if (case_weather.Validate(is_included_warnings, messages_error) == false) {
    is_valid = false;
  }

  // validates limit
  if (limit < 0
      || ((limit < 100) && (is_included_warnings == true))
      || ((100000 < limit) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE CONSTRAINT - Invalid limit");
    }
  }

  return is_valid;
}