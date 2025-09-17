#pragma once


#include "html/html_token.hpp"
// #include "dom/document.hpp"


class HTMLTreeBuilder {
public:
    // HTMLTreeBuilder(Document t_document);

    void token_dispatch(HTMLToken& token);

private:
    // Document m_document;   // The model of our document (the DOM)
};