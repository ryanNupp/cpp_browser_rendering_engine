#pragma once

#include <optional>
#include <deque>
#include <mutex>
#include <condition_variable>


class IOQueue {
/*
    Thread-safe input/output queue
    https://encoding.spec.whatwg.org/#concept-stream

    Empty std::optional<std::byte> represents EOF
*/
public:
    // Producer operations
    void push(std::optional<std::byte> byte);

    // Consumer operations
    std::optional<std::byte> read();
    std::optional<std::byte> peek();
    void restore(std::byte byte);

private:
    std::deque<std::byte> m_byte_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond_var;
    bool eof;
};