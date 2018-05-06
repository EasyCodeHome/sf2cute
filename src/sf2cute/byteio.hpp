/// @file
/// Byte I/O templates.
///
/// @author gocha <https://github.com/gocha>

#ifndef SF2CUTE_BYTEIO_HPP_
#define SF2CUTE_BYTEIO_HPP_

#include <stdint.h>
#include <iostream>

namespace sf2cute {

/// Writes an 8-bit integer.
/// @param out the output iterator.
/// @param value the number to be written.
/// @return the output iterator that points to the next element of the written data.
/// @tparam OutputIterator an Iterator that can write to the pointed-to element.
template <typename OutputIterator>
OutputIterator WriteInt8(OutputIterator out, uint8_t value) {
  static_assert(sizeof(*out) == 1, "Element size of OutputIterator must be 1.");

  *out = static_cast<char>(value);
  out = std::next(out, 1);

  return out;
}

/// Writes a 16-bit integer in little-endian order.
/// @param out the output iterator.
/// @param value the number to be written.
/// @return the output iterator that points to the next element of the written data.
/// @tparam OutputIterator an Iterator that can write to the pointed-to element.
template <typename OutputIterator>
OutputIterator WriteInt16L(OutputIterator out, uint16_t value) {
  static_assert(sizeof(*out) == 1, "Element size of OutputIterator must be 1.");

  *out = value & 0xff;
  out = std::next(out, 1);
  *out = static_cast<char>((value >> 8) & 0xff);
  out = std::next(out, 1);

  return out;
}

/// Writes a 32-bit integer in little-endian order.
/// @param out the output iterator.
/// @param value the number to be written.
/// @return the output iterator that points to the next element of the written data.
/// @tparam OutputIterator an Iterator that can write to the pointed-to element.
template <typename OutputIterator>
OutputIterator WriteInt32L(OutputIterator out, uint32_t value) {
  static_assert(sizeof(*out) == 1, "Element size of OutputIterator must be 1.");

  *out = static_cast<char>(value & 0xff);
  out = std::next(out, 1);
  *out = static_cast<char>((value >> 8) & 0xff);
  out = std::next(out, 1);
  *out = static_cast<char>((value >> 16) & 0xff);
  out = std::next(out, 1);
  *out = static_cast<char>((value >> 24) & 0xff);
  out = std::next(out, 1);

  return out;
}

/// Writes an 8-bit integer to a stream.
/// @param out the output stream.
/// @param value the number to be written.
/// @return the output stream.
template<typename CharT, typename Traits>
auto WriteInt8ToStream(std::basic_ostream<CharT, Traits>& out, uint8_t value) -> decltype(out) {
  char data[1] = { static_cast<char>(value) };
  out.write(data, 1);
  return out;
}

/// Writes an 16-bit integer to a stream in little-endian order.
/// @param out the output stream.
/// @param value the number to be written.
/// @return the output stream.
template<typename CharT, typename Traits>
auto WriteInt16LToStream(std::basic_ostream<CharT, Traits>& out, uint16_t value) -> decltype(out) {
  char data[2];
  WriteInt16L(data, value);
  out.write(data, 2);
  return out;
}

/// Writes an 32-bit integer to a stream in little-endian order.
/// @param out the output stream.
/// @param value the number to be written.
/// @return the output stream.
template<typename CharT, typename Traits>
auto WriteInt32LToStream(std::basic_ostream<CharT, Traits>& out, uint32_t value) -> decltype(out) {
  char data[4];
  WriteInt32L(data, value);
  out.write(data, 4);
  return out;
}

/// Reads an 8-bit integer.
/// @param in the input iterator.
/// @param value the output pointer to which the 8-bit value read will be written.
/// @return the input iterator that points to the next element of the read data.
/// @tparam InputIterator an Iterator that can read from the pointed-to element.
template <typename InputIterator>
InputIterator ReadInt8(InputIterator in, uint8_t * value) {
  static_assert(sizeof(*in) == 1, "Element size of InputIterator must be 1.");

  *value = static_cast<uint8_t>(*in);
  in = std::next(in, 1);

  return in;
}

/// Reads a 16-bit integer in little-endian order.
/// @param in the input iterator.
/// @param value the output pointer to which the 16-bit value read will be written.
/// @return the input iterator that points to the next element of the read data.
/// @tparam InputIterator an Iterator that can read from the pointed-to element.
template <typename InputIterator>
InputIterator ReadInt16L(InputIterator in, uint16_t * value) {
  static_assert(sizeof(*in) == 1, "Element size of InputIterator must be 1.");

  uint16_t value_ = static_cast<uint8_t>(*in);
  in = std::next(in, 1);
  value_ |= static_cast<uint8_t>(*in) << 8;
  in = std::next(in, 1);
  *value = value_;

  return in;
}

/// Reads a 32-bit integer in little-endian order.
/// @param in the input iterator.
/// @param value the output pointer to which the 32-bit value read will be written.
/// @return the input iterator that points to the next element of the read data.
/// @tparam InputIterator an Iterator that can read from the pointed-to element.
template <typename InputIterator>
InputIterator ReadInt32L(InputIterator in, uint32_t * value) {
  static_assert(sizeof(*in) == 1, "Element size of InputIterator must be 1.");

  uint32_t value_ = static_cast<uint8_t>(*in);
  in = std::next(in, 1);
  value_ |= static_cast<uint8_t>(*in) << 8;
  in = std::next(in, 1);
  value_ |= static_cast<uint8_t>(*in) << 16;
  in = std::next(in, 1);
  value_ |= static_cast<uint8_t>(*in) << 24;
  in = std::next(in, 1);
  *value = value_;

  return in;
}

/// Reads an 8-bit integer to a stream.
/// @param in the input stream.
/// @param value the output pointer to which the 8-bit value read will be written.
/// @return the input stream.
template<typename CharT, typename Traits>
auto ReadInt8FromStream(std::basic_istream<CharT, Traits>& in, uint8_t * value) -> decltype(in) {
  in.read(value, 1);
  return in;
}

/// Reads an 16-bit integer to a stream in little-endian order.
/// @param in the input stream.
/// @param value the output pointer to which the 16-bit value read will be written.
/// @return the input stream.
template<typename CharT, typename Traits>
auto ReadInt16LFromStream(std::basic_istream<CharT, Traits>& in, uint16_t * value) -> decltype(in) {
  char data[2];
  in.read(data, 2);
  ReadInt16L(data, value);
  return in;
}

/// Reads an 32-bit integer to a stream in little-endian order.
/// @param in the input stream.
/// @param value the output pointer to which the 32-bit value read will be written.
/// @return the input stream.
template<typename CharT, typename Traits>
auto ReadInt32LFromStream(std::basic_istream<CharT, Traits>& in, uint32_t * value) -> decltype(in) {
  char data[4];
  in.read(data, 4);
  ReadInt32L(data, value);
  return in;
}

} // namespace sf2cute

#endif // SF2CUTE_BYTEIO_HPP_
