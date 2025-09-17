
#include "bytestream/text_decoder.hpp"
#include "html/codepoint.hpp"


TextDecoder::TextDecoder(IOQueue& t_io_queue)
    : m_io_queue{ t_io_queue }
{}



char32_t TextDecoder_UTF_8::decode()
{
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
}