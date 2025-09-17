

#include "html_tokenizer.hpp"
#include "html/codepoint.hpp"
#include "html/html_parse_error.hpp"


HTMLTokenizer::HTMLTokenizer(InputStream& t_input_stream, HTMLTreeBuilder& t_tree_builder)
    : m_input_stream{ t_input_stream }
    , m_tree_builder{ t_tree_builder }
{}


void HTMLTokenizer::get_next_token()
{
    switch (m_curr_state) {
    case State::Data:
        Data_handler();
        break;
    case State::RCDATA:
        RCDATA_handler();
        break;
    case State::RAWTEXT:
        RAWTEXT_handler();
        break;
    case State::ScriptData:
        ScriptData_handler();
        break;
    case State::PLAINTEXT:
        PLAINTEXT_handler();
        break;
    case State::TagOpen:
        TagOpen_handler();
        break;
    case State::EndTagOpen:
        EndTagOpen_handler();
        break;
    case State::TagName:
        TagName_handler();
        break;
    case State::RCDATALessThanSign:
        RCDATALessThanSign_handler();
        break;
    case State::RCDATAEndTagOpen:
        RCDATAEndTagOpen_handler();
        break;
    case State::RCDATAEndTagName:
        RCDATAEndTagName_handler();
        break;
    case State::RAWTEXTLessThanSign:
        RAWTEXTLessThanSign_handler();
        break;
    case State::RAWTEXTEndTagOpen:
        RAWTEXTEndTagOpen_handler();
        break;
    case State::RAWTEXTEndTagName:
        RAWTEXTEndTagName_handler();
        break;
    case State::ScriptDataLessThanSign:
        ScriptDataLessThanSign_handler();
        break;
    case State::ScriptDataEndTagOpen:
        ScriptDataEndTagOpen_handler();
        break;
    case State::ScriptDataEndTagName:
        ScriptDataEndTagName_handler();
        break;
    case State::ScriptDataEscapeStart:
        ScriptDataEscapeStart_handler();
        break;
    case State::ScriptDataEscapeStartDash:
        ScriptDataEscapeStartDash_handler();
        break;
    case State::ScriptDataEscaped:
        ScriptDataEscaped_handler();
        break;
    case State::ScriptDataEscapedDash:
        ScriptDataEscapedDash_handler();
        break;
    case State::ScriptDataEscapedDashDash:
        ScriptDataEscapedDashDash_handler();
        break;
    case State::ScriptDataEscapedLessThanSign:
        ScriptDataEscapedLessThanSign_handler();
        break;
    case State::ScriptDataEscapedEndTagOpen:
        ScriptDataEscapedEndTagOpen_handler();
        break;
    case State::ScriptDataEscapedEndTagName:
        ScriptDataEscapedEndTagName_handler();
        break;
    case State::ScriptDataDoubleEscapeStart:
        ScriptDataDoubleEscapeStart_handler();
        break;
    case State::ScriptDataDoubleEscaped:
        ScriptDataDoubleEscaped_handler();
        break;
    case State::ScriptDataDoubleEscapedDash:
        ScriptDataDoubleEscapedDash_handler();
        break;
    case State::ScriptDataDoubleEscapedDashDash:
        ScriptDataDoubleEscapedDashDash_handler();
        break;
    case State::ScriptDataDoubleEscapedLessThanSign:
        ScriptDataDoubleEscapedLessThanSign_handler();
        break;
    case State::ScriptDataDoubleEscapeEnd:
        ScriptDataDoubleEscapeEnd_handler();
        break;
    case State::BeforeAttributeName:
        BeforeAttributeName_handler();
        break;
    case State::AttributeName:
        AttributeName_handler();
        break;
    case State::AfterAttributeName:
        AfterAttributeName_handler();
        break;
    case State::BeforeAttributeValue:
        BeforeAttributeValue_handler();
        break;
    case State::AttributeValueDoubleQuoted:
        AttributeValueDoubleQuoted_handler();
        break;
    case State::AttributeValueSingleQuoted:
        AttributeValueSingleQuoted_handler();
        break;
    case State::AttributeValueUnquoted:
        AttributeValueUnquoted_handler();
        break;
    case State::AfterAttributeValueQuoted:
        AfterAttributeValueQuoted_handler();
        break;
    case State::SelfClosingStartTag:
        SelfClosingStartTag_handler();
        break;
    case State::BogusComment:
        BogusComment_handler();
        break;
    case State::MarkupDeclarationOpen:
        MarkupDeclarationOpen_handler();
        break;
    case State::CommentStart:
        CommentStart_handler();
        break;
    case State::CommentStartDash:
        CommentStartDash_handler();
        break;
    case State::Comment:
        Comment_handler();
        break;
    case State::CommentLessThanSign:
        CommentLessThanSign_handler();
        break;
    case State::CommentLessThanSignBang:
        CommentLessThanSignBang_handler();
        break;
    case State::CommentLessThanSignBangDash:
        CommentLessThanSignBangDash_handler();
        break;
    case State::CommentLessThanSignBangDashDash:
        CommentLessThanSignBangDashDash_handler();
        break;
    case State::CommentEndDash:
        CommentEndDash_handler();
        break;
    case State::CommentEnd:
        CommentEnd_handler();
        break;
    case State::CommentEndBang:
        CommentEndBang_handler();
        break;
    case State::DOCTYPE:
        DOCTYPE_handler();
        break;
    case State::BeforeDOCTYPEName:
        BeforeDOCTYPEName_handler();
        break;
    case State::DOCTYPEName:
        DOCTYPEName_handler();
        break;
    case State::AfterDOCTYPEName:
        AfterDOCTYPEName_handler();
        break;
    case State::AfterDOCTYPEPublicKeyword:
        AfterDOCTYPEPublicKeyword_handler();
        break;
    case State::BeforeDOCTYPEPublicIdentifier:
        BeforeDOCTYPEPublicIdentifier_handler();
        break;
    case State::DOCTYPEPublicIdentifierDoubleQuoted:
        DOCTYPEPublicIdentifierDoubleQuoted_handler();
        break;
    case State::DOCTYPEPublicIdentifierSingleQuoted:
        DOCTYPEPublicIdentifierSingleQuoted_handler();
        break;
    case State::AfterDOCTYPEPublicIdentifier:
        AfterDOCTYPEPublicIdentifier_handler();
        break;
    case State::BetweenDOCTYPEPublicAndSystemIdentifiers:
        BetweenDOCTYPEPublicAndSystemIdentifiers_handler();
        break;
    case State::AfterDOCTYPESystemKeyword:
        AfterDOCTYPESystemKeyword_handler();
        break;
    case State::BeforeDOCTYPESystemIdentifier:
        BeforeDOCTYPESystemIdentifier_handler();
        break;
    case State::DOCTYPESystemIdentifierDoubleQuoted:
        DOCTYPESystemIdentifierDoubleQuoted_handler();
        break;
    case State::DOCTYPESystemIdentifierSingleQuoted:
        DOCTYPESystemIdentifierSingleQuoted_handler();
        break;
    case State::AfterDOCTYPESystemIdentifier:
        AfterDOCTYPESystemIdentifier_handler();
        break;
    case State::BogusDOCTYPE:
        BogusDOCTYPE_handler();
        break;
    case State::CDATASection:
        CDATASection_handler();
        break;
    case State::CDATASectionBracket:
        CDATASectionBracket_handler();
        break;
    case State::CDATASectionEnd:
        CDATASectionEnd_handler();
        break;
    case State::CharacterReference:
        CharacterReference_handler();
        break;
    case State::NamedCharacterReference:
        NamedCharacterReference_handler();
        break;
    case State::AmbiguousAmpersand:
        AmbiguousAmpersand_handler();
        break;
    case State::NumericCharacterReference:
        NumericCharacterReference_handler();
        break;
    case State::HexadecimalCharacterReferenceStart:
        HexadecimalCharacterReferenceStart_handler();
        break;
    case State::DecimalCharacterReferenceStart:
        DecimalCharacterReferenceStart_handler();
        break;
    case State::HexadecimalCharacterReference:
        HexadecimalCharacterReference_handler();
        break;
    case State::DecimalCharacterReference:
        DecimalCharacterReference_handler();
        break;
    case State::NumericCharacterReferenceEnd:
        NumericCharacterReferenceEnd_handler();
        break;
    }
}


void HTMLTokenizer::Data_handler()
/*
    https://html.spec.whatwg.org/#data-state
*/
{
    consume_next_input_character();
    switch(m_current_input_character) {
    case '&':
        set_return_state_to<State::Data>();
        switch_state_to<State::CharacterReference>();
        break;
    case '<':
        switch_state_to<State::TagOpen>();
        break;
    case '\0':
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(m_current_input_character);
        break;
    case Codepoint::end_of_file:
        emit_end_of_file_token();
        break;
    default: // Anything else
        emit_character_token(m_current_input_character);
    }
}


void HTMLTokenizer::RCDATA_handler()
/*
    https://html.spec.whatwg.org/#rcdata-state
*/
{
    consume_next_input_character();
    switch(m_current_input_character) {
    case '&':
        set_return_state_to<State::RCDATA>();
        switch_state_to<State::CharacterReference>();
        break;
    case '<':
        switch_state_to<State::RCDATALessThanSign>();
        break;
    case '\0':
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(m_current_input_character);
        break;
    case Codepoint::end_of_file:
        emit_end_of_file_token();
        break;
    default: // Anything else
        emit_character_token(m_current_input_character);
    }
}


void HTMLTokenizer::RAWTEXT_handler()
/*
    https://html.spec.whatwg.org/#rawtext-state
*/
{
    consume_next_input_character();
    switch (m_current_input_character) {
    case '<':
        switch_state_to<State::RAWTEXTLessThanSign>();
        break;
    case '\0':
        handle_parse_error(ParseError::unexpected_null_character);
        emit_character_token(Codepoint::replacement_character);
        
    case Codepoint::end_of_file:
    default: // Anything else
        emit_character_token(m_current_input_character);
    }
}


void HTMLTokenizer::ScriptData_handler()
/*
    https://html.spec.whatwg.org/#script-data-state
*/
{   

}


void HTMLTokenizer::PLAINTEXT_handler()
/*
    https://html.spec.whatwg.org/#plaintext-state
*/
{

}


void HTMLTokenizer::TagOpen_handler()
{

}


void HTMLTokenizer::EndTagOpen_handler()
{

}


void HTMLTokenizer::TagName_handler()
{

}


void HTMLTokenizer::RCDATALessThanSign_handler()
{

}


void HTMLTokenizer::RCDATAEndTagOpen_handler()
{

}


void HTMLTokenizer::RCDATAEndTagName_handler()
{

}


void HTMLTokenizer::RAWTEXTLessThanSign_handler()
{

}


void HTMLTokenizer::RAWTEXTEndTagOpen_handler()
{

}


void HTMLTokenizer::RAWTEXTEndTagName_handler()
{

}


void HTMLTokenizer::ScriptDataLessThanSign_handler()
{

}


void HTMLTokenizer::ScriptDataEndTagOpen_handler()
{

}


void HTMLTokenizer::ScriptDataEndTagName_handler()
{

}


void HTMLTokenizer::ScriptDataEscapeStart_handler()
{

}


void HTMLTokenizer::ScriptDataEscapeStartDash_handler()
{

}


void HTMLTokenizer::ScriptDataEscaped_handler()
{

}


void HTMLTokenizer::ScriptDataEscapedDash_handler()
{

}


void HTMLTokenizer::ScriptDataEscapedDashDash_handler()
{

}


void HTMLTokenizer::ScriptDataEscapedLessThanSign_handler()
{

}


void HTMLTokenizer::ScriptDataEscapedEndTagOpen_handler()
{

}


void HTMLTokenizer::ScriptDataEscapedEndTagName_handler()
{

}


void HTMLTokenizer::ScriptDataDoubleEscapeStart_handler()
{

}


void HTMLTokenizer::ScriptDataDoubleEscaped_handler()
{

}


void HTMLTokenizer::ScriptDataDoubleEscapedDash_handler()
{

}


void HTMLTokenizer::ScriptDataDoubleEscapedDashDash_handler()
{

}


void HTMLTokenizer::ScriptDataDoubleEscapedLessThanSign_handler()
{

}


void HTMLTokenizer::ScriptDataDoubleEscapeEnd_handler()
{

}


void HTMLTokenizer::BeforeAttributeName_handler()
{

}


void HTMLTokenizer::AttributeName_handler()
{

}


void HTMLTokenizer::AfterAttributeName_handler()
{

}


void HTMLTokenizer::BeforeAttributeValue_handler()
{

}


void HTMLTokenizer::AttributeValueDoubleQuoted_handler()
{

}


void HTMLTokenizer::AttributeValueSingleQuoted_handler()
{

}


void HTMLTokenizer::AttributeValueUnquoted_handler()
{

}


void HTMLTokenizer::AfterAttributeValueQuoted_handler()
{

}


void HTMLTokenizer::SelfClosingStartTag_handler()
{

}


void HTMLTokenizer::BogusComment_handler()
{

}


void HTMLTokenizer::MarkupDeclarationOpen_handler()
{

}


void HTMLTokenizer::CommentStart_handler()
{

}


void HTMLTokenizer::CommentStartDash_handler()
{

}


void HTMLTokenizer::Comment_handler()
{

}


void HTMLTokenizer::CommentLessThanSign_handler()
{

}


void HTMLTokenizer::CommentLessThanSignBang_handler()
{

}


void HTMLTokenizer::CommentLessThanSignBangDash_handler()
{

}


void HTMLTokenizer::CommentLessThanSignBangDashDash_handler()
{

}


void HTMLTokenizer::CommentEndDash_handler()
{

}


void HTMLTokenizer::CommentEnd_handler()
{

}


void HTMLTokenizer::CommentEndBang_handler()
{

}


void HTMLTokenizer::DOCTYPE_handler()
{

}


void HTMLTokenizer::BeforeDOCTYPEName_handler()
{

}


void HTMLTokenizer::DOCTYPEName_handler()
{

}


void HTMLTokenizer::AfterDOCTYPEName_handler()
{

}


void HTMLTokenizer::AfterDOCTYPEPublicKeyword_handler()
{

}


void HTMLTokenizer::BeforeDOCTYPEPublicIdentifier_handler()
{

}


void HTMLTokenizer::DOCTYPEPublicIdentifierDoubleQuoted_handler()
{

}


void HTMLTokenizer::DOCTYPEPublicIdentifierSingleQuoted_handler()
{

}


void HTMLTokenizer::AfterDOCTYPEPublicIdentifier_handler()
{

}


void HTMLTokenizer::BetweenDOCTYPEPublicAndSystemIdentifiers_handler()
{

}


void HTMLTokenizer::AfterDOCTYPESystemKeyword_handler()
{

}


void HTMLTokenizer::BeforeDOCTYPESystemIdentifier_handler()
{

}


void HTMLTokenizer::DOCTYPESystemIdentifierDoubleQuoted_handler()
{

}


void HTMLTokenizer::DOCTYPESystemIdentifierSingleQuoted_handler()
{

}


void HTMLTokenizer::AfterDOCTYPESystemIdentifier_handler()
{

}


void HTMLTokenizer::BogusDOCTYPE_handler()
{

}


void HTMLTokenizer::CDATASection_handler()
{

}


void HTMLTokenizer::CDATASectionBracket_handler()
{

}


void HTMLTokenizer::CDATASectionEnd_handler()
{

}


void HTMLTokenizer::CharacterReference_handler()
{

}


void HTMLTokenizer::NamedCharacterReference_handler()
{

}


void HTMLTokenizer::AmbiguousAmpersand_handler()
{

}


void HTMLTokenizer::NumericCharacterReference_handler()
{

}


void HTMLTokenizer::HexadecimalCharacterReferenceStart_handler()
{

}


void HTMLTokenizer::DecimalCharacterReferenceStart_handler()
{

}


void HTMLTokenizer::HexadecimalCharacterReference_handler()
{

}


void HTMLTokenizer::DecimalCharacterReference_handler()
{

}


void HTMLTokenizer::NumericCharacterReferenceEnd_handler()
{

}