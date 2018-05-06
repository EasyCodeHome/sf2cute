/// @file
/// RIFF (Resource Interchange File Format) library implementation
///
/// The RIFF library defines small classes for writing a RIFF file.
///
/// @author gocha <https://github.com/gocha>

#include "riff.hpp"

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sf2cute/file_format_error.hpp>

#include "byteio.hpp"

namespace sf2cute {

/// Constructs a RIFFChunkHeader from a stream.
RIFFChunkHeader RIFFChunkHeader::FromStream(std::istream & in) {
  // Save exception bits of input stream.
  const std::ios_base::iostate old_exception_bits = in.exceptions();
  // Set exception bits to get input error as an exception.
  in.exceptions(std::ios::badbit);

  try {
    // Read the chunk header.
    char fourcc_[4];
    in.read(fourcc_, 4);
    if (!in.good()) {
      throw FileFormatError("Not a valid RIFF chunk. Unable to read the FourCC.");
    }

    // Read the chunk size.
    uint32_t size_;
    ReadInt32LFromStream(in, &size_);
    if (!in.good()) {
      throw FileFormatError("Not a valid RIFF chunk. Unable to read the chunk size.");
    }

    // Recover exception bits of input stream.
    in.exceptions(old_exception_bits);

    return RIFFChunkHeader(FourCC(fourcc_), size_);
  }
  catch (const std::exception &) {
    // Recover exception bits of input stream.
    in.clear();
    in.exceptions(old_exception_bits);

    // Rethrow the exception.
    throw;
  }
}

/// Writes this chunk to the specified output stream.
void RIFFChunk::Write(std::ostream & out) const {
  // Save exception bits of output stream.
  const std::ios_base::iostate old_exception_bits = out.exceptions();
  // Set exception bits to get output error as an exception.
  out.exceptions(std::ios::badbit | std::ios::failbit);

  try {
    // Write the chunk header.
    WriteHeader(out, fourcc(), data_.size());

    // Write the chunk data.
    out.write(reinterpret_cast<const char *>(data_.data()), data_.size());

    // Write a padding byte if necessary.
    if (data_.size() % 2 != 0) {
      WriteInt8ToStream(out, 0);
    }

    // Recover exception bits of output stream.
    out.exceptions(old_exception_bits);
  }
  catch (const std::exception &) {
    // Recover exception bits of output stream.
    out.clear();
    out.exceptions(old_exception_bits);

    // Rethrow the exception.
    throw;
  }
}

/// Writes a chunk header to the specified output stream.
void RIFFChunk::WriteHeader(std::ostream & out,
    FourCC fourcc,
    size_type size) {
  // Throw exception if the chunk size exceeds the maximum.
  if (size > UINT32_MAX) {
    std::ostringstream message_builder;
    message_builder << "RIFF chunk \"" << fourcc << "\" size too large.";
    throw std::length_error(message_builder.str());
  }

  // Save exception bits of output stream.
  const std::ios_base::iostate old_exception_bits = out.exceptions();
  // Set exception bits to get output error as an exception.
  out.exceptions(std::ios::badbit | std::ios::failbit);

  try {
    // Write the FourCC.
    out.write(fourcc.data(), fourcc.size());

    // Write the chunk size.
    WriteInt32LToStream(out, static_cast<uint32_t>(size));

    // Recover exception bits of output stream.
    out.exceptions(old_exception_bits);
  }
  catch (const std::exception &) {
    // Recover exception bits of output stream.
    out.clear();
    out.exceptions(old_exception_bits);

    // Rethrow the exception.
    throw;
  }
}

/// Appends the specified RIFFChunkInterface to this chunk.
void RIFFListChunk::AddSubchunk(std::unique_ptr<RIFFChunkInterface> && subchunk) {
  subchunks_.push_back(std::move(subchunk));
}

/// Removes all subchunks from this chunk.
void RIFFListChunk::ClearSubchunks() {
  subchunks_.clear();
}

/// Writes this chunk to the specified output stream.
void RIFFListChunk::Write(std::ostream & out) const {
  // Write the chunk header.
  WriteHeader(out, fourcc(), size() - 8);

  // Write each subchunks.
  for (const auto & subchunk : subchunks_) {
    subchunk->Write(out);
  }
}

/// Writes a "LIST" chunk header to the specified output stream.
void RIFFListChunk::WriteHeader(std::ostream & out,
    FourCC fourcc,
    size_type size) {
  // Throw exception if the chunk size exceeds the maximum.
  if (size > UINT32_MAX) {
    std::ostringstream message_builder;
    message_builder << "RIFF chunk \"" << fourcc << "\" size too large.";
    throw std::length_error(message_builder.str());
  }

  // Save exception bits of output stream.
  const std::ios_base::iostate old_exception_bits = out.exceptions();
  // Set exception bits to get output error as an exception.
  out.exceptions(std::ios::badbit | std::ios::failbit);

  try {
    // Write the chunk ID "LIST".
    out.write("LIST", 4);

    // Write the chunk size.
    WriteInt32LToStream(out, static_cast<uint32_t>(size));

    // Write the list type.
    out.write(fourcc.data(), fourcc.size());

    // Recover exception bits of output stream.
    out.exceptions(old_exception_bits);
  }
  catch (const std::exception &) {
    // Recover exception bits of output stream.
    out.clear();
    out.exceptions(old_exception_bits);

    // Rethrow the exception.
    throw;
  }
}

/// Writes this RIFF to the specified output stream.
void RIFF::Write(std::ostream & out) const {
  // Save exception bits of output stream.
  const std::ios_base::iostate old_exception_bits = out.exceptions();
  // Set exception bits to get output error as an exception.
  out.exceptions(std::ios::badbit | std::ios::failbit);

  try {
    // Write the RIFF header.
    WriteHeader(out, fourcc(), size() - 8);

    // Write each chunks.
    for (const auto & chunk : chunks_) {
      chunk->Write(out);
    }

    // Recover exception bits of output stream.
    out.exceptions(old_exception_bits);
  }
  catch (const std::exception &) {
    // Recover exception bits of output stream.
    out.clear();
    out.exceptions(old_exception_bits);

    // Rethrow the exception.
    throw;
  }
}

/// Writes a "RIFF" chunk header to the specified output stream.
void RIFF::WriteHeader(std::ostream & out,
    FourCC fourcc,
    size_type size) {
  // Throw exception if the RIFF file size exceeds the maximum.
  if (size > UINT32_MAX) {
    throw std::length_error("RIFF file size too large.");
  }

  // Save exception bits of output stream.
  const std::ios_base::iostate old_exception_bits = out.exceptions();
  // Set exception bits to get output error as an exception.
  out.exceptions(std::ios::badbit | std::ios::failbit);

  try {
    // Write the ID "RIFF".
    out.write("RIFF", 4);

    // Write the file size.
    WriteInt32LToStream(out, static_cast<uint32_t>(size));

    // Write the form type.
    out.write(fourcc.data(), fourcc.size());

    // Recover exception bits of output stream.
    out.exceptions(old_exception_bits);
  }
  catch (const std::exception &) {
    // Recover exception bits of output stream.
    out.clear();
    out.exceptions(old_exception_bits);

    // Rethrow the exception.
    throw;
  }
}

} // namespace sf2cute
