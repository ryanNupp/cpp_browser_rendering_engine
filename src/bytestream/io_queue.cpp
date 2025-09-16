

#include "bytestream/io_queue.hpp"


std::optional<std::byte> IOQueue::read()
{
    auto lock = std::unique_lock{ m_mutex };

    if (eof && m_byte_queue.empty()) {
        return {};
    }
    
    m_cond_var.wait(lock, [this]() { return !m_byte_queue.empty(); });

    const auto res = m_byte_queue.front();
    m_byte_queue.pop_front();
    return res;
}


std::optional<std::byte> IOQueue::peek()
{
    auto lock = std::unique_lock{ m_mutex };

    if (eof && m_byte_queue.empty()) {
        return {};
    }

    m_cond_var.wait(lock, [this]() { return !m_byte_queue.empty(); });

    const auto res = m_byte_queue.front();
    return res;
}


void IOQueue::push(std::optional<std::byte> byte)
{
    auto lock = std::unique_lock{ m_mutex };

    if (!byte.has_value()) {
        eof = true;
    }
    else {
        m_byte_queue.push_back(*byte);
    }

    m_cond_var.notify_one();
}


void IOQueue::restore(std::byte byte)
{
    auto lock = std::unique_lock{ m_mutex };

    m_byte_queue.push_front(byte);

    m_cond_var.notify_one();
}