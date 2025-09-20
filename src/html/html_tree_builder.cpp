#include <print>

#include "utf_32_util.hpp"
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


// TODO: streamline token passoff / dispatch:
//
//  --  HTMLParser creates HTMLToken m_curr_token object, HTMLTreeBuilder & HTMLTokenizer both have access by reference
//
//  --  Since tree builder only accesses token once told to from the tokenizer, tell token_dispatch token type at
//      compile time
//
//  --  Potentially have some seperate specific dispatch functions where necessary, probably for character literals have a
//      special one using non-type template parameter
//
//  --  Probably for character tokens from m_curr_char (known @ runtime) just pass character by parameter,
//      UNLESS - look into if m_curr_token is safe to use for character tokens, (probably not)