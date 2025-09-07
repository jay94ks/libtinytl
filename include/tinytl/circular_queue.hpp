#ifndef __TINYTL_QUEUE_HPP__
#define __TINYTL_QUEUE_HPP__

#if !defined(__cplusplus) || __cplusplus < 201400L
#error "`queue.hpp` requires C++14 or above."
#endif

#include "mutils.hpp"
#include <stdint.h>

namespace tinytl {

    /* circular queue. */
    template<typename ELEM_TYPE, typename COUNT_TYPE = uint16_t, COUNT_TYPE CAPACITY = 32>
    class circular_queue {
    public:
        /* capacity. */
        static constexpr COUNT_TYPE capacity = CAPACITY;

        /* element type. */
        using elem_type = ELEM_TYPE;

        /* size type. */
        using size_type = COUNT_TYPE;
        
    private:
        elem_type _elems[CAPACITY];
        size_type _wr, _rd;

    public:
        circular_queue() {
            zero(_elems, CAPACITY);
            _wr = _rd = 0;
        }

    public:
        /* returns whether the queue is empty or not. */
        inline bool empty() const {
            return _wr == _rd;
        }

        /* returns whether the queue is full or not. */
        inline bool full() const {
            return ((_wr + 1) % capacity) == _rd;
        }

        /* reset the queue to initial state. */
        inline void reset() {
            _wr = _rd = 0;
        }
        
        /* enqueue an item to the queue. */
        inline bool enqueue(const T& el) {
            const size_type new_wr = (_wr + 1) % capacity;

            if (new_wr == _rd) {
                return false;
            }

            _elems[new_wr] = el;
            _wr = new_wr;

            return true;
        }

        /* enqueue an item to the queue. */
        inline bool dequeue(T& el) {
            if (_rd == _wr) {
                return false;
            }

            const size_type new_rd = (_rd + 1) % capacity;

            el = _elems[new_rd];
            _rd = new_rd;

            return true;
        }
    };

}

#endif