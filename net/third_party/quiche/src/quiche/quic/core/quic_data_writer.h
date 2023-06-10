// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICHE_QUIC_CORE_QUIC_DATA_WRITER_H_
#define QUICHE_QUIC_CORE_QUIC_DATA_WRITER_H_

#include <cstddef>
#include <cstdint>

#include "absl/strings/string_view.h"
#include "quiche/quic/core/crypto/quic_random.h"
#include "quiche/quic/core/quic_types.h"
#include "quiche/quic/platform/api/quic_export.h"
#include "quiche/common/quiche_data_writer.h"
#include "quiche/common/quiche_endian.h"

namespace quic {

// This class provides facilities for packing QUIC data.
//
// The QuicDataWriter supports appending primitive values (int, string, etc)
// to a frame instance.  The internal memory buffer is exposed as the "data"
// of the QuicDataWriter.
class QUIC_EXPORT_PRIVATE QuicDataWriter : public quiche::QuicheDataWriter {
 public:
  // Creates a QuicDataWriter where |buffer| is not owned
  // using NETWORK_BYTE_ORDER endianness.
  QuicDataWriter(size_t size, char* buffer);
  // Creates a QuicDataWriter where |buffer| is not owned
  // using the specified endianness.
  QuicDataWriter(size_t size, char* buffer, quiche::Endianness endianness);
  QuicDataWriter(const QuicDataWriter&) = delete;
  QuicDataWriter& operator=(const QuicDataWriter&) = delete;

  ~QuicDataWriter();

  // Methods for adding to the payload.  These values are appended to the end
  // of the QuicDataWriter payload.

  // Write unsigned floating point corresponding to the value. Large values are
  // clamped to the maximum representable (kUFloat16MaxValue). Values that can
  // not be represented directly are rounded down.
  bool WriteUFloat16(uint64_t value);
  // Write connection ID to the payload.
  bool WriteConnectionId(QuicConnectionId connection_id);

  // Write 8-bit length followed by connection ID to the payload.
  bool WriteLengthPrefixedConnectionId(QuicConnectionId connection_id);

  // Write |length| random bytes generated by |random|.
  bool WriteRandomBytes(QuicRandom* random, size_t length);

  // Write |length| random bytes generated by |random|. This MUST NOT be used
  // for any application that requires cryptographically-secure randomness.
  bool WriteInsecureRandomBytes(QuicRandom* random, size_t length);
};

}  // namespace quic

#endif  // QUICHE_QUIC_CORE_QUIC_DATA_WRITER_H_