#include <print>

#include <utility.hpp>
#include "html/html_tree_builder.hpp"


void HTMLTreeBuilder::token_dispatch(HTMLToken& token)
{
    /******************************************************************/
    /* TEMPORARY CODE - (just prints information about token for now) */
    /******************************************************************/

    std::print("TOKEN: ");

    switch (token.m_type) {
    case HTMLToken::Type::Uninitialized:
        std::println("Uninitialized");
        break;

    case HTMLToken::Type::DOCTYPE:
        std::println("DOCTYPE");
        break;

    case HTMLToken::Type::StartTag:
        std::println("Start Tag");
        break;

    case HTMLToken::Type::EndTag:
        std::println("End Tag");
        break;

    case HTMLToken::Type::Comment:
        std::println("Comment");
        break;

    case HTMLToken::Type::Character:
        std::println("Character");
        break;

    case HTMLToken::Type::EndOfFile:
        std::println("End of file");
        break;
    }

    /******************************************************************/

    // TODO: flesh out the entirety of the tree builder & document object so this can move to the next step
}