#include <print>

#include "html/html_parser.hpp"


int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::println("USAGE: ./browser_render [path/to/html/file]");
        return 1;
    }

    HTMLParser html_parser{ argv[1] };

    html_parser.parse();

    return 0;
}