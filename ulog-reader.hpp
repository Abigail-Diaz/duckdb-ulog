#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace ulog {

// File header: 7-byte magic, 1-byte version, uint64 timestamp (microseconds).
inline constexpr std::array<std::uint8_t, 7> kMagic = {0x55, 0x4c, 0x6f, 0x67, 0x01, 0x12, 0x35};
inline constexpr std::size_t kFileHeaderSize = 16;

// Every message starts with: uint16 msg_size (payload only) + uint8 msg_type.
// Copy bytes into this struct with memcpy. Don't cast a pointer into the
// mapped file: the data may not be aligned, and a misaligned read is undefined behavior.
#pragma pack(push, 1)
struct MessageHeader {
    std::uint16_t msg_size;
    std::uint8_t msg_type;  // 'B', 'F', 'A', 'D', ... see the spec
};
#pragma pack(pop)

}