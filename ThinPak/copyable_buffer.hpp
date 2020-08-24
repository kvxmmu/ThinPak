//
// Created by kvxmmu on 8/24/20.
//

#ifndef THINPAK_COPYABLE_BUFFER_HPP
#define THINPAK_COPYABLE_BUFFER_HPP

#include <cstring>


template <typename T, size_t size>
class CopyableBuffer {
public:
    T buffer[size];

    CopyableBuffer() = default;

    CopyableBuffer(const CopyableBuffer &buffer) {
        memcpy(this->buffer, buffer.buffer, size);
    }

    T *get() {
        return this->buffer;
    }
};

template <size_t size>
using CopyableCharBuffer = CopyableBuffer<char, size>;

template <size_t size>
using CopyableUint8Buffer = CopyableBuffer<uint8_t, size>;

template <size_t size>
using CopyableSizeBuffer = CopyableBuffer<size_t, size>;

template <size_t size>
using CopyableIntBuffer = CopyableBuffer<int, size>;

#endif //THINPAK_COPYABLE_BUFFER_HPP
