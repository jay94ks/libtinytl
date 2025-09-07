#ifndef __TINYYL_QUEUE_HPP__

#if !defined(__cplusplus) || __cplusplus < 201400L
#error "`packet_buffer.hpp` requires C++14 or above."
#endif

#include "mutils.hpp"
#include <stdint.h>

namespace tinytl {

    /**
     * packet buffer.
     */
    template<uint16_t PACKET_SIZE>
    class packet_buffer {
    public:
        /* packet size. */
        static constexpr uint16_t capacity = PACKET_SIZE;

    private:
        uint8_t _buf[capacity];
        uint16_t _len;

    public:
        packet_buffer() {
            zero(_buf, capacity);
            _len = 0;
        }

        /* reset the buffer. */
        void reset() {
            _len = 0;
        }

        /* front buffer pointer. */
        inline uint8_t* front() const {
            return _buf;
        }

        /* back buffer pointer. */
        inline uint8_t* back() const {
            return _buf + _len;
        }

        /* front length. */
        inline uint16_t size() const {
            return _len;
        }

        /* back length. */
        inline uint16_t remains() const {
            return capacity - _len;
        }

        /* dequeue bytes from front buffer. */
        inline uint16_t dequeue(uint8_t* dst, uint16_t len) {
            if (len > _len) {
                len = _len;
            }

            if (len) {
                copy(dst, _buf, len);
                consume(len);
            }

            return len;
        }

        /* enqueue bytes into front buffer. */
        inline uint16_t enqueue(const uint8_t* src, uint16_t len) {
            const uint16_t abslim = remains();

            if (len > abslim) {
                len = abslim;
            }

            if (len) {
                copy(_buf + _len, src, len);
                _len += len;
            }

            return len;
        }

        /* accept back buffer to front buffer. */
        inline void accept(uint16_t n) {
            if (n) {
                _len += n;
            }
        }

        /* consume front buffer. */
        inline void consume(uint16_t n) {
            if (n) {
                uint16_t tmp = _len - n;
                if (tmp) {
                    copy(_buf, _buf + n, tmp);
                }

                _len -= n;
            }
        }

        /* find a byte. */
        inline int32_t find(uint8_t b, uint16_t offset = 0) {
            for (uint16_t i = offset; i < _len; ++i) {
                if (_buf[i] == b) {
                    return int32_t(i);
                }
            }

            return -1;
        }

    };
}

#endif