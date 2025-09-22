#include <thread>

#include "html/html_parser.hpp"


HTMLParser::HTMLParser(const std::string_view t_filepath)
    : m_bytestream_source{ t_filepath, m_io_queue }
    , m_preprocessor{ m_io_queue, m_input_stream }
    , m_tree_builder{ m_token }
    , m_tokenizer{ m_input_stream, m_token, m_tree_builder }
{}



void HTMLParser::parse()
{
    // init seperate threads to handle bytestream source & the preprocessing of the bytestream source
    std::thread bytestream_thread( &BytestreamSource::read_bytes, &m_bytestream_source );
    std::thread preprocess_thread( &Preprocessor::preprocess, &m_preprocessor );

    while (m_tokenizer.get_eof_status() == false) {
        m_tokenizer.get_next_token(); // emitting tokens to treebuilder
    }

    // join threads
    preprocess_thread.join();
    bytestream_thread.join();
}