#pragma once

#include <memory>
#include <queue>

#include "bytestream/text_decoder.hpp"
#include "html/input_stream.hpp"


class Preprocessor {
public:
    Preprocessor(IOQueue& t_io_queue, InputStream& t_input_stream);

    void preprocess();

private:
    std::unique_ptr<TextDecoder> m_text_decoder;

    InputStream& m_input_stream;
};