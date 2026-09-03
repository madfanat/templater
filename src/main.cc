#include <cstring>
#include <fstream>
#include <iostream>

#include "map.h"
#include "parser.h"

namespace {

struct Arguments {
  const char* template_path = nullptr;
  const char* data_path = nullptr;
  const char* output_path = nullptr;
};

bool HasValue(const char* value) { return value != nullptr && *value != '\0'; }

bool ParseArguments(const int argc, char* argv[], Arguments& arguments) {
  for (int index = 1; index < argc; ++index) {
    const char* argument = argv[index];

    if (std::strcmp(argument, "-t") == 0) {
      if (++index >= argc) {
        return false;
      }
      arguments.template_path = argv[index];
    } else if (std::strcmp(argument, "-d") == 0) {
      if (++index >= argc) {
        return false;
      }
      arguments.data_path = argv[index];
    } else if (std::strcmp(argument, "-o") == 0) {
      if (++index >= argc) {
        return false;
      }
      arguments.output_path = argv[index];
    } else if (std::strncmp(argument,
                            "--template=", sizeof("--template=") - 1) == 0) {
      arguments.template_path = argument + sizeof("--template=") - 1;
    } else if (std::strncmp(argument, "--data=", sizeof("--data=") - 1) == 0) {
      arguments.data_path = argument + sizeof("--data=") - 1;
    } else if (std::strncmp(argument, "--output=", sizeof("--output=") - 1) ==
               0) {
      arguments.output_path = argument + sizeof("--output=") - 1;
    } else {
      return false;
    }
  }

  return HasValue(arguments.template_path) && HasValue(arguments.data_path) &&
         (arguments.output_path == nullptr || HasValue(arguments.output_path));
}

}  // namespace

int main(int argc, char* argv[]) {
  Arguments arguments;

  if (!ParseArguments(argc, argv, arguments)) {
    return kArgumentError;
  }

  std::ifstream data(arguments.data_path);

  if (!data) {
    return kIOError;
  }

  Map variables;

  if (!ParseData(data, variables)) {
    return data.bad() ? kIOError : kOtherError;
  }

  std::ifstream source(arguments.template_path);

  if (!source) {
    return kIOError;
  }

  if (arguments.output_path != nullptr) {
    std::ofstream output(arguments.output_path);

    if (!output) {
      return kIOError;
    }

    return ParseTemplate(source, output, variables);
  }

  return ParseTemplate(source, std::cout, variables);
}
