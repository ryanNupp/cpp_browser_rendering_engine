#pragma once

#include <deque>
#include <vector>
#include <mutex>
#include <condition_variable>


class InputStream {
public:
    // Producer functions
    void push_back(char32_t codepoint);

    // Consumer functions
    char32_t peek();

    std::vector<char32_t> peek_many(size_t amount);

    char32_t consume();

    char32_t consume_many(size_t amount);


private:
    std::deque<char32_t> m_codept_buffer;

    std::mutex m_mutex;

    std::condition_variable m_cond_var;

    bool m_is_eof{ false };
};