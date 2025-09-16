#pragma once

#include "bytestream/io_queue.hpp"
#include "html/codepoint.hpp"


class TextDecoder {
public:
    TextDecoder(IOQueue& t_io_queue)
        : m_io_queue{ t_io_queue }
    {}


    virtual ~TextDecoder() = default;
    virtual char32_t decode() = 0;

protected:
    IOQueue& m_io_queue;
};




class TextDecoder_UTF_8 : public TextDecoder {
public:
    using TextDecoder::TextDecoder;
    char32_t decode() override {
        std::optional<std::byte> curr_byte = m_io_queue.read();
        if (!curr_byte.has_value()) {
            return Codepoint::end_of_file;
        }

        int bytes_needed{ get_bytes_needed(*curr_byte) };
        uint32_t code_point{ static_cast<uint32_t>(*curr_byte & info_bits[bytes_needed]) };

        // process all the continuation bytes after initial byte
        while (bytes_needed > 0) {
            code_point <<= 6;

            curr_byte = m_io_queue.read();
            if (!curr_byte.has_value()) {
                throw std::runtime_error("EOF before receiving final byte for a UTF-8 codepoint");
            }

            // ensure this is a valid continuation byte
            if (is_in_bounds(*curr_byte, cont_byte_low_bound, cont_byte_high_bound)) {
                code_point |= static_cast<char32_t>(*curr_byte & cont_byte_info_bits);
            }
            else {
                m_io_queue.restore(*curr_byte);
                throw std::runtime_error("Invalid continuation byte for a UTF-8 codepoint");
            }

            --bytes_needed;
        }


        return code_point;
    };

private:
    static constexpr std::byte low_bounds[4] {
        std::byte{ 0b0000'0000 },
        std::byte{ 0b1100'0010 },
        std::byte{ 0b1110'0000 },
        std::byte{ 0b1111'0000 }
    };
    static constexpr std::byte high_bounds[4] {
        std::byte{ 0b0111'1111 },
        std::byte{ 0b1101'1111 },
        std::byte{ 0b1110'1111 },
        std::byte{ 0b1111'0100 }
    };
    static constexpr std::byte info_bits[4] {
        std::byte{ 0b0111'1111 },
        std::byte{ 0b0001'1111 },
        std::byte{ 0b0000'1111 },
        std::byte{ 0b0000'0111 }
    };
    static constexpr auto cont_byte_low_bound  = std::byte{ 0b1000'0000 };
    static constexpr auto cont_byte_high_bound = std::byte{ 0b1011'1111 };
    static constexpr auto cont_byte_info_bits  = std::byte{ 0b0011'1111 };

    static bool is_in_bounds(const std::byte value, const std::byte low, const std::byte high)
    {
        return value >= low && value <= high;
    }

    static int get_bytes_needed(const std::byte value)
    {
        for (int i{ 0 }; i < 4; ++i) {
            if (is_in_bounds(value, low_bounds[i], high_bounds[i])) {
                return i;
            }
        }
        // invalid initial byte
        throw std::runtime_error("Invalid first byte for a UTF-8 codepoint");
    }
};