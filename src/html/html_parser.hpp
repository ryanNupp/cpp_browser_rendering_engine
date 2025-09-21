#pragma once

#include <stack>
#include <list>

#include "bytestream/bytestream.hpp"
#include "bytestream/io_queue.hpp"
#include "html/input_stream_preprocessor.hpp"
#include "html/html_element.hpp"
#include "html/html_tokenizer.hpp"
#include "html/html_tree_builder.hpp"


class HTMLParser {
public:
    explicit HTMLParser(const std::string_view t_filepath);

    void parse();

private:
    struct State {
        enum class InsertionMode : uint8_t {
            before_html,
            before_head,
            in_head,
            in_head_noscript,
            after_head,
            in_body,
            text,
            in_table,
            in_table_text,
            in_caption,
            in_column_group,
            in_table_body,
            in_row,
            in_cell,
            in_template,
            after_body,
            in_frameset,
            after_frameset,
            after_after_body,
            after_after_frameset
        };

        std::stack<HTMLElement> open_elements;
        std::list<HTMLElement> active_formatting_elements;

        // Element pointers
        HTMLElement* head;
        HTMLElement* form;

        // Scripting level
        int scripting_level = 0;

        // Parsing state flags
        bool scripting_flag;
        bool frameset_ok_flag = true;
        bool parser_pause_flag = false;

        InsertionMode insertion_mode;
    };

    IOQueue m_io_queue;
    InputStream m_input_stream;

    BytestreamSource m_bytestream_source;
    Preprocessor m_preprocessor;
    //HTMLToken m_token;
    HTMLTreeBuilder m_tree_builder;
    HTMLTokenizer m_tokenizer;

    State m_parse_state;
};