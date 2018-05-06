/// @file
/// FourCC class header
///
/// @author gocha <https://github.com/gocha>

#ifndef SF2CUTE_FOURCC_HPP_
#define SF2CUTE_FOURCC_HPP_

#include <cstring>
#include <cstdint>
#include <utility>
#include <string>
#include <ostream>

namespace sf2cute {

/// The FourCC class represents a FourCC.
///
/// FourCC is a sequence of four bytes used to uniquely identify data formats.
class FourCC {
public:
  /// Unsigned integer type for the FourCC size.
  using size_type = std::size_t;

  /// Constructs a new empty FourCC.
  FourCC() noexcept {
    data_[0] = data_[1] = data_[2] = data_[3] = 0;
  }

  /// Constructs a new FourCC using the specified characters.
  /// @param data the byte array representation of the FourCC.
  explicit FourCC(const char * data) noexcept {
    std::copy(data, data + 4, data_);
  }

  /// Indicates a FourCC object is "equal to" the other one.
  /// @param x the first object to be compared.
  /// @param y the second object to be compared.
  /// @return true if a FourCC object is "equal to" the other one.
  friend bool operator==(
      const FourCC & x,
      const FourCC & y) noexcept {
    return std::memcmp(x.data_, y.data_, 4) == 0;
  }

  /// Indicates a FourCC object is "not equal to" the other one.
  /// @param x the first object to be compared.
  /// @param y the second object to be compared.
  /// @return true if a FourCC object is "not equal to" the other one.
  friend bool operator!=(
      const FourCC & x,
      const FourCC & y) noexcept {
    return std::rel_ops::operator!=(x, y);
  }

  /// Outputs the FourCC to the specified output stream.
  /// @param out the output stream.
  /// @param fourcc the FourCC.
  friend std::ostream & operator<<(
      std::ostream & out,
      const FourCC & fourcc) {
    return out << fourcc.str();
  }

  /// Returns a byte array represents the FourCC.
  /// @return a byte array representation of the FourCC.
  const char * data() const noexcept {
    return data_;
  }

  /// Returns the number of bytes of the FourCC.
  /// @return the number of bytes of the FourCC (always 4).
  const size_type size() const noexcept {
    return 4;
  }

  /// Returns a string represents the FourCC.
  /// @return a string representation of the FourCC.
  std::string str() const noexcept {
    return std::string(data_, 4);
  }

private:
  /// Tag as four characters.
  char data_[4];
};

} // namespace sf2cute

#endif // SF2CUTE_FOURCC_HPP_
