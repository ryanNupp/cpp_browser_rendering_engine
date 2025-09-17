#pragma once

#include <fstream>


class IOQueue;


class BytestreamSource {
/*
    Intended to be a byte stream from a network connection
    readBytes() can run on it's own thread and push bytes into m_io_queue

    FOR NOW: only handles local files 
*/
public:
    BytestreamSource(std::string_view t_filepath, IOQueue& t_io_queue);
    void read_bytes();

private:
    std::ifstream m_ifstream;
    IOQueue& m_io_queue;
};