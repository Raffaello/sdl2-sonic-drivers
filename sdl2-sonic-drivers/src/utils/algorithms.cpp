#include <utils/algorithms.hpp>
#include <stdexcept>

namespace utils
{
    int decode_VLQ(const uint8_t buf[], uint32_t& out_value)
    {
        int i = 0;
        uint8_t byte = 0;
        out_value = 0;
        do
        {
            if (i >= 4) {
                throw std::runtime_error("decode_VLQ: more than 32bits VLQ input");
            }

            byte = buf[i++];
            out_value = (out_value << 7) | (byte & 0x7F);
        } while (byte & 0x80);

        return i;
    }

    int decode_xmi_VLQ(const uint8_t buf[], uint32_t& out_value)
    {
        int i = 0;
        uint8_t byte = 0;
        out_value = 0;
        do
        {
            // 0xFFFF / 0x7F = 0x1FF = 511
            if (i >= 512) {
                throw std::runtime_error("decode_xmi_VLQ: more than 32bits VLQ input");
            }

            byte = buf[i++];
            if (byte & 0x80) {
                i--;
                break;
            }

            out_value += byte;
        } while (true);

        return i;

    }
}
