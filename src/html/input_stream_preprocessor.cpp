#include <memory>
#include "codepoint.hpp"
#include "html/html_parse_error.hpp"

#include "html/input_stream_preprocessor.hpp"


Preprocessor::Preprocessor(IOQueue& t_io_queue, InputStream& t_input_stream)
    : m_text_decoder{ std::make_unique<TextDecoder_UTF_8>(t_io_queue) }
    , m_input_stream{ t_input_stream }
{}


void Preprocessor::preprocess()
{
    char32_t curr_codepoint;
    char32_t next_codepoint;
    bool next_codepoint_set{ false };

    while (curr_codepoint != Codepoint::end_of_file) {
        if (next_codepoint_set) {
            next_codepoint_set = false;
            curr_codepoint = next_codepoint;
        }
        else {
            curr_codepoint = m_text_decoder->decode();
        }


        /*  Process errors  */

        if (Codepoint::is_surrogate(curr_codepoint)) {
            handle_parse_error(ParseError::surrogate_in_input_stream);
            continue;
        }
        else if (Codepoint::is_noncharacter(curr_codepoint)) {
            handle_parse_error(ParseError::noncharacter_in_input_stream);
            continue;
        }
        else if ( Codepoint::is_control(curr_codepoint)
               && curr_codepoint != '\0'
               && !Codepoint::is_ascii_whitespace(curr_codepoint)
        ) {
            handle_parse_error(ParseError::control_character_in_input_stream);
            continue;
        }


        /*  Normalize newlines  */

        // Any occurance of either:
        // (a) pair of:  U+000D Carriage Return  +  U+000A Line Feed
        // (b) single:   U+000D Carriage Return
        // must be replaced with just a singular:  U+000A Line Feed
        if (curr_codepoint == Codepoint::carriage_return) {
            next_codepoint = m_text_decoder->decode();

            // if next codepoint is not a line feed, it must be reconsumed
            // next loop cycle (set next_codepoint_set flag to true, to mark
            // that it must be reconsumed as the curr_codepoint next cycle)
            if (next_codepoint != Codepoint::line_feed) {
                next_codepoint_set = true;
            }

            m_input_stream.push_back(Codepoint::line_feed);
        }
        else {
            m_input_stream.push_back(curr_codepoint);
        }
    }

    m_input_stream.push_back(Codepoint::end_of_file);
}