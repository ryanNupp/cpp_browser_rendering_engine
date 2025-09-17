#include <print>
#include <thread>

#include "utility.hpp"
#include "html/html_parser.hpp"
#include "bytestream/text_decoder.hpp"
#include "html/codepoint.hpp"


HTMLParser::HTMLParser(const std::string_view t_filepath)
    : m_bytestream_source{ t_filepath, m_io_queue }
    , m_preprocessor{ m_io_queue, m_input_stream }
    //, m_tree_builder{  }
    , m_tokenizer{ m_input_stream, m_tree_builder }
{}



void HTMLParser::parse()
{
    // init seperate threads to handle bytestream source & the preprocessing of the bytestream source
    std::thread bytestream_thread( &BytestreamSource::read_bytes, &m_bytestream_source );
    std::thread preprocess_thread( &Preprocessor::preprocess, &m_preprocessor );



    while (true) {
        char32_t ch = m_input_stream.consume();

        char u8_ch[5];

        if (ch == Codepoint::end_of_file) {
            std::print("\nEND OF FILE\n");
            break;
        }
        else {
            char_utf32_to_utf8(ch, u8_ch);
            std::print("{}", u8_ch);
        }
    }



    // join threads
    preprocess_thread.join();
    bytestream_thread.join();
}