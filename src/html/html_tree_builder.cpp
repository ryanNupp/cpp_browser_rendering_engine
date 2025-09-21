#include <print>

#include "html/html_tree_builder.hpp"



#include "utf_32_util.hpp"
void print_char32_array(std::span<const char32_t> char32_array)
{
    char char_buf[5];
    for (auto ch : char32_array) {
        UTF32_Util::char_utf32_to_utf8(ch, char_buf);
        std::print("{}", char_buf);
    }
    std::println();
}


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
        std::println("Force quirks: {}", token.m_doctype_data->m_force_quirks);
        // NAME
        std::println("Name missing: {}", token.m_doctype_data->m_name_missing);
        std::print("Name: ");
        print_char32_array(token.m_data);
        // PUBLIC IDENTIFIER
        std::println("Public identifier missing: {}", token.m_doctype_data->m_public_identifier_missing);
        std::print("Public identifier: ");
        print_char32_array(token.m_doctype_data->m_public_identifier);
        // SYSTEM IDENTIFIER
        std::println("System identifier missing: {}", token.m_doctype_data->m_system_identifier_missing);
        std::print("System identifier: ");
        print_char32_array(token.m_doctype_data->m_system_identifier);
        break;

    case HTMLToken::Type::StartTag:
    case HTMLToken::Type::EndTag:
        std::println("{}", (token.m_type == HTMLToken::Type::StartTag) ? "Start Tag" : "End Tag" );
        std::print("Tag name: ");
        print_char32_array(token.m_data);
        std::println("Self closing: {}", token.m_self_closing);
        std::println("Attributes:");
        for (auto attr : token.m_attributes) {
            std::print("    Attribute name:  ");
            print_char32_array(attr.m_name);
            std::print("    Attribute value: ");
            print_char32_array(attr.m_value);
            std::println();
        }
        break;

    case HTMLToken::Type::Comment:
        std::println("Comment");
        std::print("Data: ");
        print_char32_array(token.m_data);
        break;

    case HTMLToken::Type::Character:
        std::println("Character");
        {
            char char_buf[5];
            UTF32_Util::char_utf32_to_utf8(token.m_data[0], char_buf);
            std::println("Data: {}", char_buf);
        }
        break;

    case HTMLToken::Type::EndOfFile:
        std::println("End of file");
        break;
    }

    std::println("\n\n\n");

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