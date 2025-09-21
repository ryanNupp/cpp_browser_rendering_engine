


#include "html/input_stream.hpp"
#include "html/codepoint.hpp"


void InputStream::push_back(char32_t codepoint)
{
    auto lock = std::unique_lock{ m_mutex };

    if (codepoint == Codepoint::end_of_file) {
        m_is_eof = true;
    }

    m_codept_buffer.push_back(codepoint);

    m_cond_var.notify_one();
}


char32_t InputStream::peek()
{
    auto lock = std::unique_lock{ m_mutex };

    if (m_is_eof && m_codept_buffer.empty()) {
        return Codepoint::end_of_file;
    }

    m_cond_var.wait(lock, [this]() { return !m_codept_buffer.empty(); });

    return m_codept_buffer.front();
}


std::vector<char32_t> InputStream::peek_many(size_t amount)
{
    auto lock = std::unique_lock{ m_mutex };

    m_cond_var.wait(lock, [this, amount]() { return m_is_eof || !(m_codept_buffer.size() < amount); });

    if (m_codept_buffer.size() >= amount) {
        return std::vector<char32_t>{
            m_codept_buffer.begin(),
            std::next(m_codept_buffer.begin(), amount)
        };
    }
    else {
        return {};
    }
}


char32_t InputStream::consume()
{
    auto lock = std::unique_lock{ m_mutex };

    if (m_is_eof && m_codept_buffer.empty()) {
        return Codepoint::end_of_file;
    }

    m_cond_var.wait(lock, [this]() { return !m_codept_buffer.empty(); });

    const auto res = m_codept_buffer.front();
    m_codept_buffer.pop_front();
    return res;
}


char32_t InputStream::consume_many(size_t amount)
{
    auto lock = std::unique_lock{ m_mutex };

    m_cond_var.wait(lock, [this, amount]() { return m_is_eof || !(m_codept_buffer.size() < amount); });

    if (m_codept_buffer.size() >= amount) {
        const auto res = m_codept_buffer[amount-1];
        m_codept_buffer.erase(m_codept_buffer.begin(), std::next(m_codept_buffer.begin(), amount));
        return res;
    }
    else {
        throw std::runtime_error{"Invalid consume_many(): m_is_eof && amount > m_codept_buffer.size()"};
    }
}