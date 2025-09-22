#include "html_tokenizer.hpp"
#include "html/codepoint.hpp"
#include "html/html_parse_error.hpp"
#include "html/html_token.hpp"


HTMLTokenizer::HTMLTokenizer(InputStream& t_input_stream, HTMLTreeBuilder& t_tree_builder)
    : m_input_stream{ t_input_stream }
    , m_tree_builder{ t_tree_builder }
{}


void HTMLTokenizer::get_next_token()
{ 
    switch (m_curr_state) {
    case State::Data:   Data_handler();   break;
    case State::RCDATA:   RCDATA_handler();   break;
    case State::RAWTEXT:   RAWTEXT_handler();   break;
    case State::ScriptData:   ScriptData_handler();   break;
    case State::PLAINTEXT:   PLAINTEXT_handler();   break;
    case State::TagOpen:   TagOpen_handler();   break;
    case State::EndTagOpen:   EndTagOpen_handler();   break;
    case State::TagName:   TagName_handler();   break;
    case State::RCDATALessThanSign:   RCDATALessThanSign_handler();   break;
    case State::RCDATAEndTagOpen:   RCDATAEndTagOpen_handler();   break;
    case State::RCDATAEndTagName:   RCDATAEndTagName_handler();   break;
    case State::RAWTEXTLessThanSign:   RAWTEXTLessThanSign_handler();   break;
    case State::RAWTEXTEndTagOpen:   RAWTEXTEndTagOpen_handler();   break;
    case State::RAWTEXTEndTagName:   RAWTEXTEndTagName_handler();   break;
    case State::ScriptDataLessThanSign:   ScriptDataLessThanSign_handler();   break;
    case State::ScriptDataEndTagOpen:   ScriptDataEndTagOpen_handler();   break;
    case State::ScriptDataEndTagName:   ScriptDataEndTagName_handler();   break;
    case State::ScriptDataEscapeStart:   ScriptDataEscapeStart_handler();   break;
    case State::ScriptDataEscapeStartDash:   ScriptDataEscapeStartDash_handler();   break;
    case State::ScriptDataEscaped:   ScriptDataEscaped_handler();   break;
    case State::ScriptDataEscapedDash:   ScriptDataEscapedDash_handler();   break;
    case State::ScriptDataEscapedDashDash:   ScriptDataEscapedDashDash_handler();   break;
    case State::ScriptDataEscapedLessThanSign:   ScriptDataEscapedLessThanSign_handler();   break;
    case State::ScriptDataEscapedEndTagOpen:   ScriptDataEscapedEndTagOpen_handler();   break;
    case State::ScriptDataEscapedEndTagName:   ScriptDataEscapedEndTagName_handler();   break;
    case State::ScriptDataDoubleEscapeStart:   ScriptDataDoubleEscapeStart_handler();   break;
    case State::ScriptDataDoubleEscaped:   ScriptDataDoubleEscaped_handler();   break;
    case State::ScriptDataDoubleEscapedDash:   ScriptDataDoubleEscapedDash_handler();   break;
    case State::ScriptDataDoubleEscapedDashDash:   ScriptDataDoubleEscapedDashDash_handler();   break;
    case State::ScriptDataDoubleEscapedLessThanSign:   ScriptDataDoubleEscapedLessThanSign_handler();   break;
    case State::ScriptDataDoubleEscapeEnd:   ScriptDataDoubleEscapeEnd_handler();   break;
    case State::BeforeAttributeName:   BeforeAttributeName_handler();   break;
    case State::AttributeName:   AttributeName_handler();   break;
    case State::AfterAttributeName:   AfterAttributeName_handler();   break;
    case State::BeforeAttributeValue:   BeforeAttributeValue_handler();   break;
    case State::AttributeValueDoubleQuoted:   AttributeValueDoubleQuoted_handler();   break;
    case State::AttributeValueSingleQuoted:   AttributeValueSingleQuoted_handler();   break;
    case State::AttributeValueUnquoted:   AttributeValueUnquoted_handler();   break;
    case State::AfterAttributeValueQuoted:   AfterAttributeValueQuoted_handler();   break;
    case State::SelfClosingStartTag:   SelfClosingStartTag_handler();   break;
    case State::BogusComment:   BogusComment_handler();   break;
    case State::MarkupDeclarationOpen:   MarkupDeclarationOpen_handler();   break;
    case State::CommentStart:   CommentStart_handler();   break;
    case State::CommentStartDash:   CommentStartDash_handler();   break;
    case State::Comment:   Comment_handler();   break;
    case State::CommentLessThanSign:   CommentLessThanSign_handler();   break;
    case State::CommentLessThanSignBang:   CommentLessThanSignBang_handler();   break;
    case State::CommentLessThanSignBangDash:   CommentLessThanSignBangDash_handler();   break;
    case State::CommentLessThanSignBangDashDash:   CommentLessThanSignBangDashDash_handler();   break;
    case State::CommentEndDash:   CommentEndDash_handler();   break;
    case State::CommentEnd:   CommentEnd_handler();   break;
    case State::CommentEndBang:   CommentEndBang_handler();   break;
    case State::DOCTYPE:   DOCTYPE_handler();   break;
    case State::BeforeDOCTYPEName:   BeforeDOCTYPEName_handler();   break;
    case State::DOCTYPEName:   DOCTYPEName_handler();   break;
    case State::AfterDOCTYPEName:   AfterDOCTYPEName_handler();   break;
    case State::AfterDOCTYPEPublicKeyword:   AfterDOCTYPEPublicKeyword_handler();   break;
    case State::BeforeDOCTYPEPublicIdentifier:   BeforeDOCTYPEPublicIdentifier_handler();   break;
    case State::DOCTYPEPublicIdentifierDoubleQuoted:   DOCTYPEPublicIdentifierDoubleQuoted_handler();   break;
    case State::DOCTYPEPublicIdentifierSingleQuoted:   DOCTYPEPublicIdentifierSingleQuoted_handler();   break;
    case State::AfterDOCTYPEPublicIdentifier:   AfterDOCTYPEPublicIdentifier_handler();   break;
    case State::BetweenDOCTYPEPublicAndSystemIdentifiers:   BetweenDOCTYPEPublicAndSystemIdentifiers_handler();   break;
    case State::AfterDOCTYPESystemKeyword:   AfterDOCTYPESystemKeyword_handler();   break;
    case State::BeforeDOCTYPESystemIdentifier:   BeforeDOCTYPESystemIdentifier_handler();   break;
    case State::DOCTYPESystemIdentifierDoubleQuoted:   DOCTYPESystemIdentifierDoubleQuoted_handler();   break;
    case State::DOCTYPESystemIdentifierSingleQuoted:   DOCTYPESystemIdentifierSingleQuoted_handler();   break;
    case State::AfterDOCTYPESystemIdentifier:   AfterDOCTYPESystemIdentifier_handler();   break;
    case State::BogusDOCTYPE:   BogusDOCTYPE_handler();   break;
    case State::CDATASection:   CDATASection_handler();   break;
    case State::CDATASectionBracket:   CDATASectionBracket_handler();   break;
    case State::CDATASectionEnd:   CDATASectionEnd_handler();   break;
    case State::CharacterReference:   CharacterReference_handler();   break;
    case State::NamedCharacterReference:   NamedCharacterReference_handler();   break;
    case State::AmbiguousAmpersand:   AmbiguousAmpersand_handler();   break;
    case State::NumericCharacterReference:   NumericCharacterReference_handler();   break;
    case State::HexadecimalCharacterReferenceStart:   HexadecimalCharacterReferenceStart_handler();   break;
    case State::DecimalCharacterReferenceStart:   DecimalCharacterReferenceStart_handler();   break;
    case State::HexadecimalCharacterReference:   HexadecimalCharacterReference_handler();   break;
    case State::DecimalCharacterReference:   DecimalCharacterReference_handler();   break;
    case State::NumericCharacterReferenceEnd:   NumericCharacterReferenceEnd_handler();   break;
    }
}


// https://html.spec.whatwg.org/#tokenization


void HTMLTokenizer::Data_handler()
{
    consume_next_input_character();
    if (m_curr_char == '&') {
        set_return_state_to(State::Data);
        switch_state_to(State::CharacterReference);
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::TagOpen);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(m_curr_char);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::RCDATA_handler()
{
    consume_next_input_character();
    if (m_curr_char == '&') {
        set_return_state_to(State::RCDATA);
        switch_state_to(State::CharacterReference);
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::RCDATALessThanSign);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(m_curr_char);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::RAWTEXT_handler()
{
    consume_next_input_character();
    if (m_curr_char == '<') {
        switch_state_to(State::RAWTEXTLessThanSign);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptData_handler()
{   
    consume_next_input_character();
    if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataLessThanSign);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::PLAINTEXT_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::TagOpen_handler()
{
    consume_next_input_character();
    if (m_curr_char == '!') {
        switch_state_to(State::MarkupDeclarationOpen);
    }
    else if (m_curr_char == '/') {
        switch_state_to(State::EndTagOpen);
    }
    else if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::StartTag>();
        reconsume_in(State::TagName);
    }
    else if (m_curr_char == '?') {
        handle_parse_error(ParseError::unexpected_question_mark_instead_of_tag_name);
        m_curr_token.initialize<HTMLToken::Type::Comment>();
        reconsume_in(State::BogusComment);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_before_tag_name);
        emit_character_token('<');
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::invalid_first_character_of_tag_name);
        emit_character_token('<');
        reconsume_in(State::Data);
    }
}


void HTMLTokenizer::EndTagOpen_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::EndTag>();
        reconsume_in(State::TagName);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_end_tag_name);
        switch_state_to(HTMLTokenizer::State::Data);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_before_tag_name);
        emit_character_token('<');
        emit_character_token('/');
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::invalid_first_character_of_tag_name);
        m_curr_token.initialize<HTMLToken::Type::Comment>();
        reconsume_in(State::BogusComment);
    }
}


void HTMLTokenizer::TagName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' ') {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '/') {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_data(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data(m_curr_char);
    }
}


void HTMLTokenizer::RCDATALessThanSign_handler()
{
    consume_next_input_character();
    if (m_curr_char == '/') {
        m_temporary_buffer.clear();
        switch_state_to(State::RCDATAEndTagOpen);
    }
    else {
        emit_character_token('<');
        reconsume_in(State::RCDATA);
    }
}


void HTMLTokenizer::RCDATAEndTagOpen_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::EndTag>();
        reconsume_in(State::RCDATAEndTagName);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        reconsume_in(State::RCDATA);
    }
}


void HTMLTokenizer::RCDATAEndTagName_handler()
{
    consume_next_input_character();
    if ((m_curr_char == '\t'
        ||  m_curr_char == Codepoint::line_feed
        ||  m_curr_char == Codepoint::form_feed
        ||  m_curr_char == ' '
        ) &&  appropriate_end_tag_token()
    ) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '/' && appropriate_end_tag_token()) {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '>' && appropriate_end_tag_token()) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
        m_temporary_buffer.push_back(m_curr_char);
    }
    else if (Codepoint::is_ascii_lower_alpha(m_curr_char)) {
        m_curr_token.append_data(m_curr_char);
        m_temporary_buffer.push_back(m_curr_char);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        emit_temp_buffer_as_char_tokens();
        reconsume_in(State::RCDATA);
    }
}


void HTMLTokenizer::RAWTEXTLessThanSign_handler()
{
    consume_next_input_character();
    if (m_curr_char == '/') {
        m_temporary_buffer.clear();
        switch_state_to(State::RAWTEXTEndTagOpen);
    }
    else {
        emit_character_token('<');
        reconsume_in(State::RAWTEXT);
    }
}


void HTMLTokenizer::RAWTEXTEndTagOpen_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::EndTag>();
        reconsume_in(State::RAWTEXTEndTagName);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        reconsume_in(State::RAWTEXT);
    }
}


void HTMLTokenizer::RAWTEXTEndTagName_handler()
{
    consume_next_input_character();
    if ((m_curr_char == '\t'
        ||  m_curr_char == Codepoint::line_feed
        ||  m_curr_char == Codepoint::form_feed
        ||  m_curr_char == ' '
        ) && appropriate_end_tag_token()
    ) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '/' && appropriate_end_tag_token()) {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '>' && appropriate_end_tag_token()) {
        switch_state_to(State::Data);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
        m_temporary_buffer.push_back(m_curr_char);
    }
    else if (Codepoint::is_ascii_lower_alpha(m_curr_char)) {
        m_curr_token.append_data(m_curr_char);
        m_temporary_buffer.push_back(m_curr_char);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        emit_temp_buffer_as_char_tokens();
        reconsume_in(State::RAWTEXT);
    }
}


void HTMLTokenizer::ScriptDataLessThanSign_handler()
{
    consume_next_input_character();
    if (m_curr_char == '/') {

    }
    else if (m_curr_char == '!') {
        switch_state_to(State::ScriptDataEscapeStart);
        emit_character_token('<');
        emit_character_token('!');
    }
    else {
        emit_character_token('<');
        reconsume_in(State::ScriptData);
    }
}


void HTMLTokenizer::ScriptDataEndTagOpen_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::EndTag>();
        reconsume_in(State::ScriptData);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        reconsume_in(State::ScriptData);
    }
}


void HTMLTokenizer::ScriptDataEndTagName_handler()
{
    consume_next_input_character();
    if ((m_curr_char == '\t'
        ||  m_curr_char == Codepoint::line_feed
        ||  m_curr_char == Codepoint::form_feed
        ||  m_curr_char == ' '
        ) && appropriate_end_tag_token()
    ) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '/' && appropriate_end_tag_token()) {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '>' && appropriate_end_tag_token()) {
        switch_state_to(State::Data);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
        m_temporary_buffer.push_back(m_curr_char);
    }
    else if (Codepoint::is_ascii_lower_alpha(m_curr_char)) {
        m_curr_token.append_data(m_curr_char);
        m_temporary_buffer.push_back(m_curr_char);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        emit_temp_buffer_as_char_tokens();
        reconsume_in(State::ScriptData);
    }
}


void HTMLTokenizer::ScriptDataEscapeStart_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::ScriptDataEscapedDash);
        emit_character_token('-');
    }
    else {
        reconsume_in(State::ScriptData);
    }
}


void HTMLTokenizer::ScriptDataEscapeStartDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::ScriptDataEscapedDashDash);
        emit_character_token('-');
    }
    else {
        reconsume_in(State::ScriptData);
    }
}


void HTMLTokenizer::ScriptDataEscaped_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::ScriptDataEscapedDash);
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataEscapedLessThanSign);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_script_html_comment_like_text);
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptDataEscapedDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::ScriptDataEscapedDashDash);
        emit_character_token('-');
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataEscapedLessThanSign);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        switch_state_to(State::ScriptDataEscaped);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_script_html_comment_like_text);
        emit_end_of_file_token();
    }
    else {
        switch_state_to(State::ScriptDataEscaped);
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptDataEscapedDashDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        emit_character_token('-');
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataEscapedLessThanSign);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::ScriptData);
        emit_character_token('>');
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        switch_state_to(State::ScriptDataEscaped);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_script_html_comment_like_text);
        emit_end_of_file_token();
    }
    else {
        switch_state_to(State::ScriptDataEscaped);
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptDataEscapedLessThanSign_handler()
{
    consume_next_input_character();
    if (m_curr_char == '/') {
        m_temporary_buffer.clear();
        switch_state_to(State::ScriptDataEscapedEndTagOpen);
    }
    else if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_temporary_buffer.clear();
        emit_character_token('<');
        reconsume_in(State::ScriptDataDoubleEscapeStart);
    }
    else {
        emit_character_token('<');
        reconsume_in(State::ScriptDataEscaped);
    }
}


void HTMLTokenizer::ScriptDataEscapedEndTagOpen_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::EndTag>();
        reconsume_in(State::ScriptDataEscapedEndTagName);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        reconsume_in(State::ScriptData);
    }
}


void HTMLTokenizer::ScriptDataEscapedEndTagName_handler()
{
    consume_next_input_character();
    if ((m_curr_char == '\t'
        ||  m_curr_char == Codepoint::line_feed
        ||  m_curr_char == Codepoint::form_feed
        ||  m_curr_char == ' '
        ) && appropriate_end_tag_token()
    ) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '/' && appropriate_end_tag_token()) {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '>' && appropriate_end_tag_token()) {
        switch_state_to(State::Data);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
        m_temporary_buffer.push_back(m_curr_char);
    }
    else if (Codepoint::is_ascii_lower_alpha(m_curr_char)) {
        m_curr_token.append_data(m_curr_char);
        m_temporary_buffer.push_back(m_curr_char);
    }
    else {
        emit_character_token('<');
        emit_character_token('/');
        emit_temp_buffer_as_char_tokens();
        reconsume_in(State::ScriptDataEscaped);
    }
}


void HTMLTokenizer::ScriptDataDoubleEscapeStart_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ||  m_curr_char == '/'
    ||  m_curr_char == '>'
    ) {
        constexpr std::array<char32_t, 6> string = {'s', 't', 'r', 'i', 'n', 'g'};
        if (Codepoint::utf32_str_equal(m_temporary_buffer, string)) {
            switch_state_to(State::ScriptDataDoubleEscaped);
        }
        else {
            switch_state_to(State::ScriptDataEscaped);
        }
        emit_character_token(m_curr_char);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_temporary_buffer.push_back(Codepoint::to_lowercase(m_curr_char));
        emit_character_token(m_curr_char);
    }
    else if (Codepoint::is_ascii_lower_alpha(m_curr_char)) {
        m_temporary_buffer.push_back(m_curr_char);
        emit_character_token(m_curr_char);
    }
    else {
        reconsume_in(State::ScriptDataEscaped);
    }
}


void HTMLTokenizer::ScriptDataDoubleEscaped_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::ScriptDataDoubleEscapedDash);
        emit_character_token('-');
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataDoubleEscapedLessThanSign);
        emit_character_token('<');
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_script_html_comment_like_text);
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptDataDoubleEscapedDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::ScriptDataDoubleEscapedDashDash);
        emit_character_token('-');
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataDoubleEscapedLessThanSign);
        emit_character_token('<');
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        switch_state_to(State::ScriptDataDoubleEscaped);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_script_html_comment_like_text);
        emit_end_of_file_token();
    }
    else {
        switch_state_to(State::ScriptDataDoubleEscaped);
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptDataDoubleEscapedDashDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        emit_character_token('-');
    }
    else if (m_curr_char == '<') {
        switch_state_to(State::ScriptDataDoubleEscapedLessThanSign);
        emit_character_token('<');
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::ScriptData);
        emit_character_token('>');
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        switch_state_to(State::ScriptDataDoubleEscaped);
        emit_character_token(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_script_html_comment_like_text);
        emit_end_of_file_token();
    }
    else {
        switch_state_to(State::ScriptDataDoubleEscaped);
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::ScriptDataDoubleEscapedLessThanSign_handler()
{
    consume_next_input_character();
    if (m_curr_char == '/') {
        m_temporary_buffer.clear();
        switch_state_to(State::ScriptDataDoubleEscapeEnd);
        emit_character_token('/');
    }
    else {
        reconsume_in(State::ScriptDataDoubleEscaped);
    }
}


void HTMLTokenizer::ScriptDataDoubleEscapeEnd_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ||  m_curr_char == '/'
    ||  m_curr_char == '>'
    ) {
        constexpr char32_t string[] = {'s', 't', 'r', 'i', 'n', 'g'};
        if (Codepoint::utf32_str_equal(m_temporary_buffer, string)) {
            switch_state_to(State::ScriptDataEscaped);
        }
        else {
            switch_state_to(State::ScriptDataDoubleEscaped);
        }
        emit_character_token(m_curr_char);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_temporary_buffer.push_back(Codepoint::to_lowercase(m_curr_char));
        emit_character_token(m_curr_char);
    }
    else if (Codepoint::is_ascii_lower_alpha(m_curr_char)) {
        m_temporary_buffer.push_back(m_curr_char);
        emit_character_token(m_curr_char);
    }
    else {
        reconsume_in(State::ScriptDataDoubleEscaped);
    }
}


void HTMLTokenizer::BeforeAttributeName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '/'
         ||  m_curr_char == '>'
         ||  m_curr_char == Codepoint::end_of_file
    ) {
        reconsume_in(State::AfterAttributeName);
    }
    else if (m_curr_char == '=') {
        handle_parse_error(ParseError::unexpected_equals_sign_before_attribute_name);
        m_curr_token.start_new_attribute();
        switch_state_to(State::AttributeName);
    }
    else {
        m_curr_token.start_new_attribute();
        reconsume_in(State::AttributeName);
    }
}


void HTMLTokenizer::AttributeName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ||  m_curr_char == '/'
    ||  m_curr_char == '>'
    ||  m_curr_char == Codepoint::end_of_file
    ) {
        reconsume_in(State::AfterAttributeName);
    }
    else if (m_curr_char == '=') {
        switch_state_to(State::BeforeAttributeValue);
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.attribute_name_append(Codepoint::to_lowercase(m_curr_char));
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.attribute_name_append(Codepoint::replacement_character);
    }
    else {
        if (m_curr_char == '"'
        ||  m_curr_char == '\''
        ||  m_curr_char == '<'
        ) {
            handle_parse_error(ParseError::unexpected_character_in_attribute_name);
        }
        m_curr_token.attribute_name_append(m_curr_char);
    }
    /*
        ---------------------------------------------------------------------------------------
        https://html.spec.whatwg.org/#attribute-name-state
        ---------------------------------------------------------------------------------------
        When the user agent leaves the attribute name state (and before emitting the tag token,
        if appropriate), the complete attribute's name must be compared to the other attributes
        on the same token;
        
        if there is already an attribute on the token with the exact same name, then this is a
        duplicate-attributeparse error and the new attribute must be removed from the token.
        ---------------------------------------------------------------------------------------
        NOTE:
        If an attribute is so removed from a token, it, and the value that gets associated with
        it, if any, are never subsequently used by the parser, and are therefore effectively
        discarded. Removing the attribute in this way does not change its status as the
        "current attribute" for the purposes of the tokenizer, however.
        ---------------------------------------------------------------------------------------
        TODO: look into above info.....
    */
}


void HTMLTokenizer::AfterAttributeName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '/') {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '=') {
        switch_state_to(State::BeforeAttributeValue);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_tag_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        m_curr_token.start_new_attribute();
        reconsume_in(State::AttributeName);
    }
}


void HTMLTokenizer::BeforeAttributeValue_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '"') {
        switch_state_to(State::AttributeValueDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        switch_state_to(State::AttributeValueSingleQuoted);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_attribute_value);
        switch_state_to(State::Data);
        emit_current_tag_token();
    }
    else {
        reconsume_in(State::AttributeValueUnquoted);
    }
}


void HTMLTokenizer::AttributeValueDoubleQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '"') {
        switch_state_to(State::AfterAttributeValueQuoted);
    }
    else if (m_curr_char == '&') {
        set_return_state_to(State::AttributeValueDoubleQuoted);
        switch_state_to(State::CharacterReference);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.attribute_value_append(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        m_curr_token.attribute_value_append(m_curr_char);
    }
}


void HTMLTokenizer::AttributeValueSingleQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\'') {
        switch_state_to(State::AfterAttributeValueQuoted);
    }
    else if (m_curr_char == '&') {
        set_return_state_to(State::AttributeValueSingleQuoted);
        switch_state_to(State::CharacterReference);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.attribute_value_append(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        m_curr_token.attribute_value_append(m_curr_char);
    }
}


void HTMLTokenizer::AttributeValueUnquoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '&') {
        set_return_state_to(State::AttributeValueUnquoted);
        switch_state_to(State::CharacterReference);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.attribute_value_append(Codepoint::replacement_character);
    }
    else if (m_curr_char == '"'
         ||  m_curr_char == '\''
         ||  m_curr_char == '<'
         ||  m_curr_char == '='
         ||  m_curr_char == '`'
    ) {
        handle_parse_error(ParseError::unexpected_character_in_unquoted_attribute_value);
        m_curr_token.attribute_value_append(m_curr_char);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        m_curr_token.attribute_value_append(m_curr_char);
    }
}


void HTMLTokenizer::AfterAttributeValueQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::BeforeAttributeName);
    }
    else if (m_curr_char == '/') {
        switch_state_to(State::SelfClosingStartTag);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_tag_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_whitespace_between_attributes);
        reconsume_in(State::BeforeAttributeName);
    }
}


void HTMLTokenizer::SelfClosingStartTag_handler()
{
    consume_next_input_character();
    if (m_curr_char == '>') {
        m_curr_token.set_self_closing_flag();
        switch_state_to(State::Data);
        emit_current_tag_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_tag);
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::unexpected_solidus_in_tag);
        reconsume_in(State::BeforeAttributeName);
    }
}


void HTMLTokenizer::BogusComment_handler()
{
    consume_next_input_character();
    if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_current_token();
        emit_end_of_file_token();
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_data(Codepoint::replacement_character);
    }
    else {
        m_curr_token.append_data(m_curr_char);
    }
}


void HTMLTokenizer::MarkupDeclarationOpen_handler()
{
    if (Codepoint::utf32_str_equal(m_input_stream.peek_many(2), {{'-','-'}})) {
        consume_many(2);
        m_curr_token.initialize<HTMLToken::Type::Comment>();
        switch_state_to(State::CommentStart);
        return;
    }
    
    constexpr char32_t doctype[] = {'d','o','c','t','y','p','e'};
    constexpr char32_t cdata[]   = {'[','C','D','A','T','A','['};
    auto next_7_chars{ m_input_stream.peek_many(7) };

    if (Codepoint::is_ascii_case_insensitive_match(next_7_chars, doctype)) {
        consume_many(7);
        switch_state_to(State::DOCTYPE);
    }
    else if (Codepoint::utf32_str_equal(next_7_chars, cdata)) {
        consume_many(7);
        // TODO: if - adjusted current node && it's not an element in the HTML namespace
        if (true) {
            switch_state_to(State::CDATASection);
        }
        else {
            handle_parse_error(ParseError::cdata_in_html_content);
            m_curr_token.initialize<HTMLToken::Type::Comment>();
            m_curr_token.append_data_multiple(cdata);
            switch_state_to(State::BogusComment);
        }
    }
    else {
        handle_parse_error(ParseError::incorrectly_opened_comment);
        m_curr_token.initialize<HTMLToken::Type::Comment>();
        switch_state_to(State::BogusComment);
    }
}


void HTMLTokenizer::CommentStart_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::CommentStartDash);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::abrupt_closing_of_empty_comment);
        switch_state_to(State::Data);
        emit_current_token();
    }
    else {
        reconsume_in(State::Comment);
    } 
}


void HTMLTokenizer::CommentStartDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::CommentEnd);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::abrupt_closing_of_empty_comment);
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_comment);
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data('-');
        reconsume_in(State::Comment);
    }
}


void HTMLTokenizer::Comment_handler()
{
    consume_next_input_character();
    if (m_curr_char == '<') {
        m_curr_token.append_data(m_curr_char);
        switch_state_to(State::CommentLessThanSign);
    }
    else if (m_curr_char == '-') {
        switch_state_to(State::CommentEndDash);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_data(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_comment);
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data(m_curr_char);
    }
}


void HTMLTokenizer::CommentLessThanSign_handler()
{
    consume_next_input_character();
    if (m_curr_char == '!') {
        m_curr_token.append_data(m_curr_char);
        switch_state_to(State::CommentLessThanSignBang);
    }
    else if (m_curr_char == '<') {
        m_curr_token.append_data(m_curr_char);
    }
    else {
        reconsume_in(State::Comment);
    }
}


void HTMLTokenizer::CommentLessThanSignBang_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::CommentLessThanSignBangDash);
    }
    else {
        reconsume_in(State::Comment);
    }
}


void HTMLTokenizer::CommentLessThanSignBangDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::CommentLessThanSignBangDashDash);
    }
    else {
        reconsume_in(State::CommentEndDash);
    }
}


void HTMLTokenizer::CommentLessThanSignBangDashDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '>' || m_curr_char == Codepoint::end_of_file) {
        reconsume_in(State::CommentEnd);
    }
    else {
        handle_parse_error(ParseError::nested_comment);
        reconsume_in(State::CommentEnd);
    }
}


void HTMLTokenizer::CommentEndDash_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        switch_state_to(State::CommentEnd);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_comment);
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data('-');
        reconsume_in(State::Comment);
    }
}


void HTMLTokenizer::CommentEnd_handler()
{
    consume_next_input_character();
    if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == '!') {
        switch_state_to(State::CommentEndBang);
    }
    else if (m_curr_char == '-') {
        m_curr_token.append_data('-');
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_comment);
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data_multiple({{'-','-'}});
        reconsume_in(State::Comment);
    }
}


void HTMLTokenizer::CommentEndBang_handler()
{
    consume_next_input_character();
    if (m_curr_char == '-') {
        m_curr_token.append_data_multiple({{'-','-','!'}});
        reconsume_in(State::CommentEndDash);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::incorrectly_closed_comment);
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_comment);
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data_multiple({{'-','-','!'}});
        reconsume_in(State::Comment);
    }
}


void HTMLTokenizer::DOCTYPE_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::BeforeDOCTYPEName);
    }
    else if (m_curr_char == '>') {
        reconsume_in(State::BeforeDOCTYPEName);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.initialize<HTMLToken::Type::DOCTYPE>();
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_whitespace_before_doctype_name);
        reconsume_in(State::BeforeDOCTYPEName);
    }
}


void HTMLTokenizer::BeforeDOCTYPEName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.initialize<HTMLToken::Type::DOCTYPE>();
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
        switch_state_to(State::DOCTYPEName);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.initialize<HTMLToken::Type::DOCTYPE>();
        m_curr_token.append_data(Codepoint::replacement_character);
        switch_state_to(State::DOCTYPEName);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_doctype_name);
        m_curr_token.initialize<HTMLToken::Type::DOCTYPE>();
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.initialize<HTMLToken::Type::DOCTYPE>();
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.initialize<HTMLToken::Type::DOCTYPE>();
        m_curr_token.append_data(m_curr_char);
        switch_state_to(State::DOCTYPEName);
    }
}


void HTMLTokenizer::DOCTYPEName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::AfterDOCTYPEName);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (Codepoint::is_ascii_upper_alpha(m_curr_char)) {
        m_curr_token.append_data(Codepoint::to_lowercase(m_curr_char));
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_data(Codepoint::replacement_character);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_data(m_curr_char);
    }
}


void HTMLTokenizer::AfterDOCTYPEName_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        constexpr char32_t str_public[]{'p','u','b','l','i','c'};
        constexpr char32_t str_system[]{'s','y','s','t','e','m'};
        auto next_6_chars{ m_input_stream.peek_many(6) };

        if (Codepoint::is_ascii_case_insensitive_match(next_6_chars, str_public)) {
            consume_many(6);
            switch_state_to(State::AfterDOCTYPEPublicKeyword);
        }
        else if (Codepoint::is_ascii_case_insensitive_match(next_6_chars, str_system)) {
            consume_many(6);
            switch_state_to(State::AfterDOCTYPESystemKeyword);
        }
        else {
            handle_parse_error(ParseError::invalid_character_sequence_after_doctype_name);
            m_curr_token.set_doctype_force_quirks_flag();
            reconsume_in(State::BogusDOCTYPE);
        }
    }
}


void HTMLTokenizer::AfterDOCTYPEPublicKeyword_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::BeforeDOCTYPEPublicIdentifier);
    }
    else if (m_curr_char == '"') {
        handle_parse_error(ParseError::missing_whitespace_after_doctype_public_keyword);
        m_curr_token.set_doctype_public_identifier_not_missing();
        switch_state_to(State::DOCTYPEPublicIdentifierDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        handle_parse_error(ParseError::missing_whitespace_after_doctype_public_keyword);
        m_curr_token.set_doctype_public_identifier_not_missing();
        switch_state_to(State::DOCTYPEPublicIdentifierSingleQuoted);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_doctype_public_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_tag_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_quote_before_doctype_public_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::BeforeDOCTYPEPublicIdentifier_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '"') {
        m_curr_token.set_doctype_public_identifier_not_missing();
        switch_state_to(State::DOCTYPEPublicIdentifierDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        m_curr_token.set_doctype_public_identifier_not_missing();
        switch_state_to(State::DOCTYPEPublicIdentifierSingleQuoted);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_doctype_public_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_tag_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_quote_before_doctype_public_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::DOCTYPEPublicIdentifierDoubleQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '"') {
        switch_state_to(State::AfterDOCTYPEPublicIdentifier);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_doctype_public_identifier(Codepoint::replacement_character);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::abrupt_doctype_public_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_doctype_public_identifier(m_curr_char);
    }
}


void HTMLTokenizer::DOCTYPEPublicIdentifierSingleQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\'') {
        switch_state_to(State::AfterDOCTYPEPublicIdentifier);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_doctype_public_identifier(Codepoint::replacement_character);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::abrupt_doctype_public_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_doctype_public_identifier(m_curr_char);
    }
}


void HTMLTokenizer::AfterDOCTYPEPublicIdentifier_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::BetweenDOCTYPEPublicAndSystemIdentifiers);
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == '"') {
        handle_parse_error(ParseError::missing_whitespace_between_doctype_public_and_system_identifiers);
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        handle_parse_error(ParseError::missing_whitespace_between_doctype_public_and_system_identifiers);
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierSingleQuoted);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_quote_before_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::BetweenDOCTYPEPublicAndSystemIdentifiers_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == '"') {
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierSingleQuoted);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_quote_before_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::AfterDOCTYPESystemKeyword_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        switch_state_to(State::BeforeDOCTYPESystemIdentifier);
    }
    else if (m_curr_char == '"') {
        handle_parse_error(ParseError::missing_whitespace_after_doctype_system_keyword);
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        handle_parse_error(ParseError::missing_whitespace_after_doctype_system_keyword);
        switch_state_to(State::DOCTYPESystemIdentifierSingleQuoted);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_quote_before_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::BeforeDOCTYPESystemIdentifier_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '"') {
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierDoubleQuoted);
    }
    else if (m_curr_char == '\'') {
        m_curr_token.set_doctype_system_identifier_not_missing();
        switch_state_to(State::DOCTYPESystemIdentifierSingleQuoted);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::missing_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::missing_quote_before_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::DOCTYPESystemIdentifierDoubleQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '"') {
        switch_state_to(State::AfterDOCTYPESystemIdentifier);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_doctype_system_identifier(m_curr_char);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::abrupt_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_doctype_system_identifier(m_curr_char);
    }
}


void HTMLTokenizer::DOCTYPESystemIdentifierSingleQuoted_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\'') {
        switch_state_to(State::AfterDOCTYPESystemIdentifier);
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        m_curr_token.append_doctype_system_identifier(Codepoint::replacement_character);
    }
    else if (m_curr_char == '>') {
        handle_parse_error(ParseError::abrupt_doctype_system_identifier);
        m_curr_token.set_doctype_force_quirks_flag();
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        m_curr_token.append_doctype_system_identifier(m_curr_char);
    }
}


void HTMLTokenizer::AfterDOCTYPESystemIdentifier_handler()
{
    consume_next_input_character();
    if (m_curr_char == '\t'
    ||  m_curr_char == Codepoint::line_feed
    ||  m_curr_char == Codepoint::form_feed
    ||  m_curr_char == ' '
    ) {
        // ignore the character
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_doctype);
        m_curr_token.set_doctype_force_quirks_flag();
        emit_current_token();
        emit_end_of_file_token();
    }
    else {
        handle_parse_error(ParseError::unexpected_character_after_doctype_system_identifier);
        reconsume_in(State::BogusDOCTYPE);
    }
}


void HTMLTokenizer::BogusDOCTYPE_handler()
{
    consume_next_input_character();
    if (m_curr_char == '>') {
        switch_state_to(State::Data);
        emit_current_token();
    }
    else if (m_curr_char == '\0') {
        handle_parse_error(ParseError::unexpected_null_character);
        // ignore the character
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        emit_current_token();
        emit_end_of_file_token();
    }
    // else, ignore the character
}


void HTMLTokenizer::CDATASection_handler()
{
    consume_next_input_character();
    if (m_curr_char == ']') {
        switch_state_to(State::CDATASectionBracket);
    }
    else if (m_curr_char == Codepoint::end_of_file) {
        handle_parse_error(ParseError::eof_in_cdata);
        emit_end_of_file_token();
    }
    else {
        emit_character_token(m_curr_char);
    }
}


void HTMLTokenizer::CDATASectionBracket_handler()
{
    consume_next_input_character();
    if (m_curr_char == ']') {
        switch_state_to(State::CDATASectionEnd);
    }
    else {
        emit_character_token(']');
        reconsume_in(State::CDATASection);
    }
}


void HTMLTokenizer::CDATASectionEnd_handler()
{
    consume_next_input_character();
    if (m_curr_char == ']') {
        emit_character_token(']');
    }
    else if (m_curr_char == '>') {
        switch_state_to(State::Data);
    }
    else {
        emit_character_token(']');
        emit_character_token(']');
        reconsume_in(State::CDATASection);
    }
}


void HTMLTokenizer::CharacterReference_handler()
{
    m_temporary_buffer.clear();
    m_temporary_buffer.push_back('&');
    consume_next_input_character();
    if (Codepoint::is_ascii_alphanumeric(m_curr_char)) {
        reconsume_in(State::NamedCharacterReference);
    }
    else if (m_curr_char == '#') {
        m_temporary_buffer.push_back(m_curr_char);
        switch_state_to(State::NumericCharacterReference);
    }
    else {
        flush_code_points_as_char_ref();
        reconsume_in_return_state();
    }
}


void HTMLTokenizer::NamedCharacterReference_handler()
{
    // TODO: come back to this one...
    //
    // https://html.spec.whatwg.org/#named-character-reference-state
}


void HTMLTokenizer::AmbiguousAmpersand_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_alphanumeric(m_curr_char)) {
        if (consumed_as_part_of_an_attribute()) {
            m_curr_token.attribute_value_append(m_curr_char);
        }
        else {
            emit_character_token(m_curr_char);
        }
    }
    else if (m_curr_char == ';') {
        handle_parse_error(ParseError::unknown_named_character_reference);
        reconsume_in_return_state();
    }
    else {
        reconsume_in_return_state();
    }
}


void HTMLTokenizer::NumericCharacterReference_handler()
{
    m_character_reference_code = 0;
    consume_next_input_character();
    if (m_curr_char == 'x' || m_curr_char == 'X') {
        m_temporary_buffer.push_back(m_curr_char);
        switch_state_to(State::HexadecimalCharacterReferenceStart);
    }
    else {
        reconsume_in(State::DecimalCharacterReferenceStart);
    }
}


void HTMLTokenizer::HexadecimalCharacterReferenceStart_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_hex_digit(m_curr_char)) {
        reconsume_in(State::HexadecimalCharacterReference);
    }
    else {
        handle_parse_error(ParseError::absence_of_digits_in_numeric_character_reference);
        flush_code_points_as_char_ref();
        reconsume_in_return_state();
    }
}


void HTMLTokenizer::DecimalCharacterReferenceStart_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_digit(m_curr_char)) {
        reconsume_in(State::DecimalCharacterReference);
    }
    else {
        handle_parse_error(ParseError::absence_of_digits_in_numeric_character_reference);
        flush_code_points_as_char_ref();
        reconsume_in_return_state();
    }
}


void HTMLTokenizer::HexadecimalCharacterReference_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_digit(m_curr_char)) {
        m_character_reference_code *= 16;
        m_character_reference_code += (static_cast<int>(m_curr_char) - 0x0030);
    }
    else if (Codepoint::is_ascii_upper_hex_digit(m_curr_char)) {
        m_character_reference_code *= 16;
        m_character_reference_code += (static_cast<int>(m_curr_char) - 0x0037);
    }
    else if (Codepoint::is_ascii_lower_hex_digit(m_curr_char)) {
        m_character_reference_code *= 16;
        m_character_reference_code += (static_cast<int>(m_curr_char) - 0x0057);
    }
    else if (m_curr_char == ';') {
        switch_state_to(State::NumericCharacterReferenceEnd);
    }
    else {
        handle_parse_error(ParseError::missing_semicolon_after_character_reference);
        reconsume_in(State::NumericCharacterReferenceEnd);
    }
}


void HTMLTokenizer::DecimalCharacterReference_handler()
{
    consume_next_input_character();
    if (Codepoint::is_ascii_digit(m_curr_char)) {
        m_character_reference_code *= 10;
        m_character_reference_code += (static_cast<int>(m_curr_char) - 0x0030);
    }
    else if (m_curr_char == ';') {
        switch_state_to(State::NumericCharacterReferenceEnd);
    }
    else {
        handle_parse_error(ParseError::missing_semicolon_after_character_reference);
        reconsume_in(State::NumericCharacterReferenceEnd);
    }
}


void HTMLTokenizer::NumericCharacterReferenceEnd_handler()
{   
    if (m_character_reference_code == 0x00) {
        handle_parse_error(ParseError::null_character_reference);
        m_character_reference_code = 0xFFFD;
    }
    else if (m_character_reference_code > 0x10FFFF) {
        handle_parse_error(ParseError::character_reference_outside_unicode_range);
        m_character_reference_code = 0xFFFD;
    }
    else if (Codepoint::is_surrogate(m_character_reference_code)) {
        handle_parse_error(ParseError::surrogate_character_reference);
        m_character_reference_code = 0xFFFD;
    }
    else if (Codepoint::is_noncharacter(m_character_reference_code)) {
        handle_parse_error(ParseError::noncharacter_character_reference);
    }
    else if (m_character_reference_code == 0x0D
         ||  (Codepoint::is_control(m_character_reference_code)
             && !Codepoint::is_ascii_whitespace(m_character_reference_code))
    ) {
        handle_parse_error(ParseError::control_character_reference);
        switch (m_character_reference_code) {
        case 0x80: m_character_reference_code = 0x20AC; break;// EURO SIGN (€)
        case 0x82: m_character_reference_code = 0x201A; break;// SINGLE LOW-9 QUOTATION MARK (‚)
        case 0x83: m_character_reference_code = 0x0192; break;// LATIN SMALL LETTER F WITH HOOK (ƒ)
        case 0x84: m_character_reference_code = 0x201E; break;// DOUBLE LOW-9 QUOTATION MARK („)
        case 0x85: m_character_reference_code = 0x2026; break;// HORIZONTAL ELLIPSIS (…)
        case 0x86: m_character_reference_code = 0x2020; break;// DAGGER (†)
        case 0x87: m_character_reference_code = 0x2021; break;// DOUBLE DAGGER (‡)
        case 0x88: m_character_reference_code = 0x02C6; break;// MODIFIER LETTER CIRCUMFLEX ACCENT (ˆ)
        case 0x89: m_character_reference_code = 0x2030; break;// PER MILLE SIGN (‰)
        case 0x8A: m_character_reference_code = 0x0160; break;// LATIN CAPITAL LETTER S WITH CARON (Š)
        case 0x8B: m_character_reference_code = 0x2039; break;// SINGLE LEFT-POINTING ANGLE QUOTATION MARK (‹)
        case 0x8C: m_character_reference_code = 0x0152; break;// LATIN CAPITAL LIGATURE OE (Œ)
        case 0x8E: m_character_reference_code = 0x017D; break;// LATIN CAPITAL LETTER Z WITH CARON (Ž)
        case 0x91: m_character_reference_code = 0x2018; break;// LEFT SINGLE QUOTATION MARK (‘)
        case 0x92: m_character_reference_code = 0x2019; break;// RIGHT SINGLE QUOTATION MARK (’)
        case 0x93: m_character_reference_code = 0x201C; break;// LEFT DOUBLE QUOTATION MARK (“)
        case 0x94: m_character_reference_code = 0x201D; break;// RIGHT DOUBLE QUOTATION MARK (”)
        case 0x95: m_character_reference_code = 0x2022; break;// BULLET (•)
        case 0x96: m_character_reference_code = 0x2013; break;// EN DASH (–)
        case 0x97: m_character_reference_code = 0x2014; break;// EM DASH (—)
        case 0x98: m_character_reference_code = 0x02DC; break;// SMALL TILDE (˜)
        case 0x99: m_character_reference_code = 0x2122; break;// TRADE MARK SIGN (™)
        case 0x9A: m_character_reference_code = 0x0161; break;// LATIN SMALL LETTER S WITH CARON (š)
        case 0x9B: m_character_reference_code = 0x203A; break;// SINGLE RIGHT-POINTING ANGLE QUOTATION MARK (›)
        case 0x9C: m_character_reference_code = 0x0153; break;// LATIN SMALL LIGATURE OE (œ)
        case 0x9E: m_character_reference_code = 0x017E; break;// LATIN SMALL LETTER Z WITH CARON (ž)
        case 0x9F: m_character_reference_code = 0x0178; break;// LATIN CAPITAL LETTER Y WITH DIAERESIS (Ÿ) 
        }
    }
    m_temporary_buffer.clear();
    m_temporary_buffer.push_back(static_cast<char32_t>(m_character_reference_code));
    flush_code_points_as_char_ref();
    switch_to_return_state();
}