#pragma once


#include "html/html_token.hpp"
// #include "dom/document.hpp"


class HTMLTreeBuilder {
public:
    HTMLTreeBuilder(HTMLToken& t_token);

    void receive_character_token(char32_t character);

    void receive_token();

private:
    HTMLToken& m_token;

    // Document m_document;   // The model of our document (the DOM)
};