#ifndef __TINYTL_QUEUE_HPP__
#define __TINYTL_QUEUE_HPP__

#if !defined(__cplusplus) || __cplusplus < 201400L
#error "`mutils.hpp` requires C++14 or above."
#endif

#ifndef __GNUC__
#include <type_traits>
#endif

#include <string.h>
#include <stddef.h>

namespace tinytl {

#ifdef __GNUC__
    /* returns whether the `T` is trivially copyable or not. */
    template<typename T>
    constexpr bool is_trivially_copyable = __is_trivially_copyable(T);
#else
    template<typename T>
    constexpr bool is_trivially_copyable = std::is_trivially_copyable<T>::value
#endif

    namespace internals {
        template <typename T, bool trivial = is_trivially_copyable<T>>
        struct mutils {
            /* set `n` elements of `dst` as `default` using `for` loop. */
            static inline void zero(T* dst, size_t n) {
                for (size_t i = 0; i < n; ++i) {
                    dst[i] = T();
                }
            }

            /* copy `n` elements from `dst` to `src` using `for` loop. */
            static inline void copy(T* dst, const T* src, size_t n) {
                for (size_t i = 0; i < n; ++i) {
                    dst[i] = static_cast<const T&>(*(&src[i]));
                }
            }

            /* move `n` elements from `dst` to `src` using `for` loop. */
            static inline void move(T* dst, const T* src, size_t n) {
                for (size_t i = 0; i < n; ++i) {
                    dst[i] = static_cast<T&&>(src[i]);
                }
            }
        };
        
        template <typename T>
        struct mutils<T, true> {
            /* set `n` elements of `dst` as zero using `memset`. */
            static inline void zero(T* dst, size_t n) {
                memset(dst, 0, sizeof(T) * n);
            }
            
            /* copy `n` elements from `dst` to `src` using `memcpy`. */
            static inline void copy(T* dst, const T* src, size_t n) {
                memcpy(dst, src, sizeof(T) * n);
            }

            /* move `n` elements from `dst` to `src` using `for` loop. */
            static inline void move(T* dst, const T* src, size_t n) {
                memmove(dst, src, sizeof(T) * n);
            }
        };
    }

    /* fill zero `n` elements from `dst` to `src`. */
    template<typename T>
    inline void zero(T* dst, size_t n) {
        internals::mutils<T>::zero(dst, n);
    }

    /* copy `n` elements from `dst` to `src`. */
    template<typename T>
    inline void copy(T* dst, const T* src, size_t n) {
        internals::mutils<T>::copy(dst, src, n);
    }

    /* cast `T` reference to LHS `T` reference. */
    template<typename T>
    inline T&& move(T& src) {
        return static_cast<T&&>(src);
    }

    /* move `n` elements from `dst` to `src`. */
    template<typename T>
    inline void move(T* dst, const T* src, size_t n) {
        internals::mutils<T>::move(dst, src, n);
    }
}

#endif