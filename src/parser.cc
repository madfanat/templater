#include "parser.h"

#include <cstddef>
#include <cstring>
#include <istream>
#include <ostream>

namespace {

constexpr std::size_t kMaxLineSize = 1024;

enum class LineType {
  kSkip,
  kPair,
  kInvalid,
};

struct ParsedLine {
  LineType type;
  char* key;
  char* value;
};

bool IsWhitespace(const int character) {
  return character == ' ' || character == '\t' || character == '\r';
}

bool IsCharacter(const int character) {
  return (character >= 'a' && character <= 'z') ||
         (character >= 'A' && character <= 'Z') ||
         (character >= '0' && character <= '9') || character == '_';
}

char* Trim(char* text) {
  while (IsWhitespace(*text)) {
    ++text;
  }

  char* end = text + std::strlen(text);

  while (end > text && IsWhitespace(*(end - 1))) {
    --end;
  }

  *end = '\0';
  return text;
}

bool IsValid(const char* word, const std::size_t max_size) {
  if (word == nullptr || *word == '\0') {
    return false;
  }

  std::size_t size = 0;

  while (*word != '\0') {
    if (!IsCharacter(*word)) {
      return false;
    }

    ++size;
    if (size > max_size) {
      return false;
    }

    ++word;
  }

  return true;
}

bool IsComment(const char* line) {
  return line[0] == '#' || (line[0] == '/' && line[1] == '/');
}

ParsedLine ParseLine(char* line) {
  char* key = Trim(line);

  if (*key == '\0' || IsComment(key)) {
    return {LineType::kSkip, nullptr, nullptr};
  }

  char* separator = std::strchr(key, '=');

  if (separator == nullptr) {
    return {LineType::kInvalid, nullptr, nullptr};
  }

  *separator = '\0';

  char* value = Trim(separator + 1);
  key = Trim(key);

  if (!IsValid(key, Map::kMaxKeySize) || !IsValid(value, Map::kMaxValueSize)) {
    return {LineType::kInvalid, nullptr, nullptr};
  }

  return {LineType::kPair, key, value};
}

void SkipWhitespace(std::istream& input) {
  while (IsWhitespace(input.peek())) {
    input.get();
  }
}

Result Replace(std::istream& input, std::ostream& output,
               const Map& variables) {
  SkipWhitespace(input);

  char key[Map::kMaxKeySize + 1];
  std::size_t key_size = 0;

  while (IsCharacter(input.peek())) {
    if (key_size >= Map::kMaxKeySize) {
      return kSyntaxError;
    }

    key[key_size] = static_cast<char>(input.get());
    ++key_size;
  }

  if (key_size == 0) {
    return kSyntaxError;
  }

  key[key_size] = '\0';
  SkipWhitespace(input);

  if (input.get() != '}' || input.get() != '}') {
    return input.bad() ? kIOError : kSyntaxError;
  }

  const char* value = variables.Get(key);

  if (value == nullptr) {
    return kMissingKey;
  }

  output << value;

  return output ? kSuccess : kIOError;
}

}  // namespace

bool ParseData(std::istream& input, Map& variables) {
  std::size_t pairs = 0;
  char line[kMaxLineSize + 2];

  while (input.getline(line, sizeof(line))) {
    if (std::strlen(line) > kMaxLineSize) {
      return false;
    }

    const ParsedLine parsed_line = ParseLine(line);

    if (parsed_line.type == LineType::kSkip) {
      continue;
    }

    if (parsed_line.type == LineType::kInvalid || pairs >= Map::kMaxPairCount ||
        !variables.Set(parsed_line.key, parsed_line.value)) {
      return false;
    }

    ++pairs;
  }

  return input.eof() && !input.bad();
}

Result ParseTemplate(std::istream& input, std::ostream& output,
                     const Map& variables) {
  int character;

  while ((character = input.get()) != std::istream::traits_type::eof()) {
    if (character == '{' && input.peek() == '{') {
      input.get();

      const Result result = Replace(input, output, variables);

      if (result != kSuccess) {
        return result;
      }
    } else {
      output.put(static_cast<char>(character));

      if (!output) {
        return kIOError;
      }
    }
  }

  return input.bad() || !output ? kIOError : kSuccess;
}
