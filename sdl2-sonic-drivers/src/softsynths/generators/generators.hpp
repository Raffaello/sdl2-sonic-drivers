#pragma once
#include <cmath>
#include <limits>
#include <functional>

#ifndef M_PI
constexpr double M_PI = 3.14159265358979323846;
#endif // ! M_PI

#ifndef M_2PI
constexpr double M_2PI = 2.0 * M_PI;
#endif // ! M_2PI

namespace softsynths
{
    namespace generators
    {
        template<typename T> constexpr int32_t unsigned_max = (std::numeric_limits<T>::max() - std::numeric_limits<T>::min());
        static_assert(unsigned_max<int8_t> == std::numeric_limits<uint8_t>::max());
        static_assert(unsigned_max<int8_t> % 2 == 1);
        static_assert(unsigned_max<int16_t> == std::numeric_limits<uint16_t>::max());
        static_assert(unsigned_max<int16_t> % 2 == 1);
        static_assert(unsigned_max<uint8_t> == std::numeric_limits<uint8_t>::max());
        static_assert(unsigned_max<uint8_t> % 2 == 1);
        static_assert(unsigned_max<uint16_t> == std::numeric_limits<uint16_t>::max());
        static_assert(unsigned_max<uint16_t> % 2 == 1);

        template<typename T> constexpr int32_t mid = (std::numeric_limits<T>::max() + std::numeric_limits<T>::min()) / 2;
        static_assert(mid<int8_t> == 0);
        static_assert(mid<int8_t> == mid<int16_t>);
        static_assert(mid<uint8_t> == 0x7F);
        static_assert(mid<uint16_t> == 0x7FFF);
        static_assert(mid<int32_t> == 0);
        //static_assert(mid<float> == 0.0); // int32_t ?

        template<typename T> T generateSquare(const uint32_t x, const uint32_t oscLength)
        {
            static_assert(std::numeric_limits<T>::is_integer);
            return (x < (oscLength / 2)) ?
                std::numeric_limits<T>::max():
                std::numeric_limits<T>::min();
        }

        template<typename T> T generateSine(const uint32_t x, const uint32_t oscLength)
        {
            static_assert(std::numeric_limits<T>::is_integer);
            if (oscLength == 0) {
                return 0;
            }

            constexpr T pi_coeff = unsigned_max<T> / 2;

            return static_cast<T>(round(pi_coeff * (sin(M_2PI * x / oscLength))) + mid<T>);
        }

        template<typename T> T generateSaw(const uint32_t x, const uint32_t oscLength)
        {
            static_assert(std::numeric_limits<T>::is_integer);
            if (oscLength == 0) {
                return 0;
            }

            return ((x * unsigned_max<T>) / oscLength) + std::numeric_limits<T>::min();
        }
    
        template<typename T> T generateTriangle(const uint32_t x, const uint32_t oscLength)
        {
            static_assert(std::numeric_limits<T>::is_integer);
            if (oscLength == 0) {
                return 0;
            }

            uint32_t f2 = oscLength / 2;
                
            return (x < f2) ?
                (x * unsigned_max<T>) / f2 + std::numeric_limits<T>::min() :
                std::numeric_limits<T>::max() - ((x - f2) * unsigned_max<T> / f2);
        }

        /// <summary>
        /// Wave Form Generators
        /// </summary>
        const enum class eWaveForm
        {
            SQUARE = 0,
            SINE,
            SAW,
            TRIANGLE,
        };

        /// <summary>
        /// Generate one of the available Waves
        /// NOTE: Can be done with an array, but
        ///       In this way guarantees type safety
        ///       The switch could still be replaced with an (internal) array
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="waveForm"></param>
        /// <param name="x"></param>
        /// <param name="oscLength"></param>
        /// <returns></returns>
        template<typename T> T generateWave(const eWaveForm waveForm, const uint32_t x, const uint32_t oscLength)
        {
            static_assert(std::numeric_limits<T>::is_integer);
            switch (waveForm)
            {
            case eWaveForm::SQUARE:
                return generateSquare<T>(x, oscLength);
            case eWaveForm::SINE:
                return generateSine<T>(x, oscLength);
            case eWaveForm::SAW:
                return generateSaw<T>(x, oscLength);
            case eWaveForm::TRIANGLE:
                return generateTriangle<T>(x, oscLength);
            default:
                // error
                return 0;
            }
        }
    }
}
