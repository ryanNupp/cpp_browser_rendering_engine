#include <filesystem>

#include "bytestream/bytestream.hpp"
#include "bytestream/io_queue.hpp"


BytestreamSource::BytestreamSource(std::string_view t_filepath, IOQueue& t_io_queue)
    : m_ifstream{ std::filesystem::path{ t_filepath }, std::ios::binary }
    , m_io_queue{ t_io_queue }
{}


// Meant to run on it's own thread
// put bytes into the ioQueue (meant to be network bytestream, for now from a file)
//
void BytestreamSource::read_bytes()
{
    while (m_ifstream.peek(), !m_ifstream.eof()) {
        std::byte curr_byte;
        m_ifstream.read(reinterpret_cast<char*>(&curr_byte), 1);
        m_io_queue.push({curr_byte});
    }
    m_io_queue.push({});
}


// eventually, differentiate how to handle a local file vs source over the network
//
// probably best to differentiate this at io_queue somehow, and use this BytestreamSource
// class for streaming bytes over the network
//
// perhaps once some URL parsing functionality is in place, use that to understand if source is
// local file vs network
//
// local file io_queue -> likely no need to be multithreaded, upon initialization copy the file
// into a vector in memory & have a size_t variable to make note of position
//
// network io_queue    -> thread safe, one thread can take from the io_queue and the other will be
// the BytestreamSource that places in the queue from the network connection