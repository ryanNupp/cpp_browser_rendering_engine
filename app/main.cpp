#include <print>
#include <thread>

#include "bytestream/bytestream.hpp"
#include "bytestream/io_queue.hpp"
#include "bytestream/text_decoder.hpp"
#include "html/codepoint.hpp"

#include "utility.hpp"


int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::println("USAGE: ./browser_render [path/to/html/file]");
        return 1;
    }
    
    // This project is very much still WIP... just testing bytestream -> io_queue -> text_decoder functionality here

    IOQueue io_queue;

    BytestreamSource bytestream_source{ argv[1], io_queue };
    std::thread bytestream_thread{ &BytestreamSource::read_bytes, &bytestream_source };

    TextDecoder_UTF_8 text_decoder{ io_queue };

    char buffer[5];
    while (true) {
        
        char32_t codept{ text_decoder.decode() };

        if (codept == Codepoint::end_of_file) {
            break;
        }
        else {
            char_utf32_to_utf8(codept, buffer);
            std::print("{}", buffer);
        }
    }

    bytestream_thread.join();
    return 0;
}