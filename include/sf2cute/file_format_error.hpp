/// @file
/// SoundFont 2 file format error class header.
///
/// @author gocha <https://github.com/gocha>

#ifndef SF2CUTE_FILE_FORMAT_ERROR_HPP_
#define SF2CUTE_FILE_FORMAT_ERROR_HPP_

#include <stdexcept>

namespace sf2cute {

/// The exception that is thrown when an input file or a data stream that is
/// supposed to conform to a certain file format specification is malformed.
class FileFormatError : public std::runtime_error {
public:
  /// Constructs a new FileFormatError.
  /// @param what_arg explanatory string
  explicit FileFormatError(const std::string & what_arg)
    : std::runtime_error(what_arg) {
  }

  /// Constructs a new FileFormatError.
  /// @param what_arg explanatory string
  explicit FileFormatError(const char * what_arg)
    : std::runtime_error(what_arg) {
  }
};

} // namespace sf2cute

#endif // SF2CUTE_FILE_FORMAT_ERROR_HPP_
