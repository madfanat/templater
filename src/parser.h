#pragma once

#include <iosfwd>

#include "map.h"

enum Result {
  kSuccess = 0,
  kMissingKey = 1,
  kArgumentError = 2,
  kIOError = 3,
  kSyntaxError = 4,
  kOtherError = 5,
};

bool ParseData(std::istream& input, Map& variables);

Result ParseTemplate(std::istream& input, std::ostream& output,
                     const Map& variables);
