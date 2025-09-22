#pragma once

#include <vector>

#include "html/codepoint.hpp"
#include "html/input_stream.hpp"
#include "html/html_tree_builder.hpp"
#include "html/html_token.hpp"


class HTMLTokenizer {
public:

    explicit HTMLTokenizer(InputStream& t_input_stream, HTMLToken& t_token,
                           HTMLTreeBuilder& t_tree_builder );

    void get_next_token();

    inline bool get_eof_status()
    {
        return m_is_eof;
    }

    enum class State {
        Data,
        RCDATA,
        RAWTEXT,
        ScriptData,
        PLAINTEXT,
        TagOpen,
        EndTagOpen,
        TagName,
        RCDATALessThanSign,
        RCDATAEndTagOpen,
        RCDATAEndTagName,
        RAWTEXTLessThanSign,
        RAWTEXTEndTagOpen,
        RAWTEXTEndTagName,
        ScriptDataLessThanSign,
        ScriptDataEndTagOpen,
        ScriptDataEndTagName,
        ScriptDataEscapeStart,
        ScriptDataEscapeStartDash,
        ScriptDataEscaped,
        ScriptDataEscapedDash,
        ScriptDataEscapedDashDash,
        ScriptDataEscapedLessThanSign,
        ScriptDataEscapedEndTagOpen,
        ScriptDataEscapedEndTagName,
        ScriptDataDoubleEscapeStart,
        ScriptDataDoubleEscaped,
        ScriptDataDoubleEscapedDash,
        ScriptDataDoubleEscapedDashDash,
        ScriptDataDoubleEscapedLessThanSign,
        ScriptDataDoubleEscapeEnd,
        BeforeAttributeName,
        AttributeName,
        AfterAttributeName,
        BeforeAttributeValue,
        AttributeValueDoubleQuoted,
        AttributeValueSingleQuoted,
        AttributeValueUnquoted,
        AfterAttributeValueQuoted,
        SelfClosingStartTag,
        BogusComment,
        MarkupDeclarationOpen,
        CommentStart,
        CommentStartDash,
        Comment,
        CommentLessThanSign,
        CommentLessThanSignBang,
        CommentLessThanSignBangDash,
        CommentLessThanSignBangDashDash,
        CommentEndDash,
        CommentEnd,
        CommentEndBang,
        DOCTYPE,
        BeforeDOCTYPEName,
        DOCTYPEName,
        AfterDOCTYPEName,
        AfterDOCTYPEPublicKeyword,
        BeforeDOCTYPEPublicIdentifier,
        DOCTYPEPublicIdentifierDoubleQuoted,
        DOCTYPEPublicIdentifierSingleQuoted,
        AfterDOCTYPEPublicIdentifier,
        BetweenDOCTYPEPublicAndSystemIdentifiers,
        AfterDOCTYPESystemKeyword,
        BeforeDOCTYPESystemIdentifier,
        DOCTYPESystemIdentifierDoubleQuoted,
        DOCTYPESystemIdentifierSingleQuoted,
        AfterDOCTYPESystemIdentifier,
        BogusDOCTYPE,
        CDATASection,
        CDATASectionBracket,
        CDATASectionEnd,
        CharacterReference,
        NamedCharacterReference,
        AmbiguousAmpersand,
        NumericCharacterReference,
        HexadecimalCharacterReferenceStart,
        DecimalCharacterReferenceStart,
        HexadecimalCharacterReference,
        DecimalCharacterReference,
        NumericCharacterReferenceEnd
    };


private:

    InputStream& m_input_stream;
    HTMLTreeBuilder& m_tree_builder;

    State m_curr_state{ State::Data };
    State m_return_state{ State::Data };

    char32_t m_curr_char;
    bool m_reconsume{ false };
    bool m_is_eof{ false };

    HTMLToken& m_token;

    std::vector<char32_t> m_temporary_buffer;

    std::vector<char32_t> m_appropriate_end_tag_name;

    int m_character_reference_code{ 0 };
    

    /* tokenizer helper functions */

    inline void switch_state_to(HTMLTokenizer::State state)
    {
        m_curr_state = state;
    }

    inline void switch_to_return_state()
    {
        m_curr_state = m_return_state;
    }

    inline void reconsume_in(HTMLTokenizer::State state)
    {
        m_curr_state = state;
        m_reconsume = true;
    }

    inline void reconsume_in_return_state()
    {
        m_curr_state = m_return_state;
        m_reconsume = true;
    }

    inline void set_return_state_to(HTMLTokenizer::State state)
    {
        m_return_state = state;
    }

    inline void consume_next_input_character()
    {
        if (m_reconsume) {
            m_reconsume = false;
        }
        else {
            m_curr_char = m_input_stream.consume();
        }
    }

    inline void consume_many(const size_t amount)
    {
        m_curr_char = m_input_stream.consume_many(amount);
    }

    inline void emit_current_token()
    {
        m_tree_builder.receive_token();
        m_token.clear();
    }

    inline void emit_current_tag_token()
    {
        if (m_token.get_type() == HTMLToken::Type::StartTag) {
            auto tag_token_name{ m_token.get_tag_name() };
            m_appropriate_end_tag_name.assign(tag_token_name.begin(),
                                              tag_token_name.end()   );
        }
        emit_current_token();
    }

    inline void emit_character_token(const char32_t character)
    {
        m_tree_builder.receive_character_token(character);
    }

    inline void emit_end_of_file_token()
    {
        m_token.clear();
        m_token.initialize<HTMLToken::Type::EndOfFile>();
        emit_current_token();
        m_is_eof = true;
    }

    inline void emit_temp_buffer_as_char_tokens()
    {
        for (auto ch : m_temporary_buffer) {
            emit_character_token(ch);
        }
    }

    inline bool consumed_as_part_of_an_attribute()
    {
        return m_return_state == State::AttributeValueDoubleQuoted
            || m_return_state == State::AttributeValueSingleQuoted
            || m_return_state == State::AttributeValueUnquoted;
    }

    inline void flush_code_points_as_char_ref()
    {
        if (consumed_as_part_of_an_attribute()) {
            for (auto ch : m_temporary_buffer) {
                m_token.attribute_value_append(ch);
            }
        }
        else {
            for (auto ch : m_temporary_buffer) {
                emit_character_token(ch);
            }
        }
    }

    inline bool appropriate_end_tag_token()
    {
        return Codepoint::utf32_str_equal(m_token.get_tag_name(),
                                          m_appropriate_end_tag_name   );
    }


    /* tokenizer state handler functions */

    inline void Data_handler();
    inline void RCDATA_handler();
    inline void RAWTEXT_handler();
    inline void ScriptData_handler();
    inline void PLAINTEXT_handler();
    inline void TagOpen_handler();
    inline void EndTagOpen_handler();
    inline void TagName_handler();
    inline void RCDATALessThanSign_handler();
    inline void RCDATAEndTagOpen_handler();
    inline void RCDATAEndTagName_handler();
    inline void RAWTEXTLessThanSign_handler();
    inline void RAWTEXTEndTagOpen_handler();
    inline void RAWTEXTEndTagName_handler();
    inline void ScriptDataLessThanSign_handler();
    inline void ScriptDataEndTagOpen_handler();
    inline void ScriptDataEndTagName_handler();
    inline void ScriptDataEscapeStart_handler();
    inline void ScriptDataEscapeStartDash_handler();
    inline void ScriptDataEscaped_handler();
    inline void ScriptDataEscapedDash_handler();
    inline void ScriptDataEscapedDashDash_handler();
    inline void ScriptDataEscapedLessThanSign_handler();
    inline void ScriptDataEscapedEndTagOpen_handler();
    inline void ScriptDataEscapedEndTagName_handler();
    inline void ScriptDataDoubleEscapeStart_handler();
    inline void ScriptDataDoubleEscaped_handler();
    inline void ScriptDataDoubleEscapedDash_handler();
    inline void ScriptDataDoubleEscapedDashDash_handler();
    inline void ScriptDataDoubleEscapedLessThanSign_handler();
    inline void ScriptDataDoubleEscapeEnd_handler();
    inline void BeforeAttributeName_handler();
    inline void AttributeName_handler();
    inline void AfterAttributeName_handler();
    inline void BeforeAttributeValue_handler();
    inline void AttributeValueDoubleQuoted_handler();
    inline void AttributeValueSingleQuoted_handler();
    inline void AttributeValueUnquoted_handler();
    inline void AfterAttributeValueQuoted_handler();
    inline void SelfClosingStartTag_handler();
    inline void BogusComment_handler();
    inline void MarkupDeclarationOpen_handler();
    inline void CommentStart_handler();
    inline void CommentStartDash_handler();
    inline void Comment_handler();
    inline void CommentLessThanSign_handler();
    inline void CommentLessThanSignBang_handler();
    inline void CommentLessThanSignBangDash_handler();
    inline void CommentLessThanSignBangDashDash_handler();
    inline void CommentEndDash_handler();
    inline void CommentEnd_handler();
    inline void CommentEndBang_handler();
    inline void DOCTYPE_handler();
    inline void BeforeDOCTYPEName_handler();
    inline void DOCTYPEName_handler();
    inline void AfterDOCTYPEName_handler();
    inline void AfterDOCTYPEPublicKeyword_handler();
    inline void BeforeDOCTYPEPublicIdentifier_handler();
    inline void DOCTYPEPublicIdentifierDoubleQuoted_handler();
    inline void DOCTYPEPublicIdentifierSingleQuoted_handler();
    inline void AfterDOCTYPEPublicIdentifier_handler();
    inline void BetweenDOCTYPEPublicAndSystemIdentifiers_handler();
    inline void AfterDOCTYPESystemKeyword_handler();
    inline void BeforeDOCTYPESystemIdentifier_handler();
    inline void DOCTYPESystemIdentifierDoubleQuoted_handler();
    inline void DOCTYPESystemIdentifierSingleQuoted_handler();
    inline void AfterDOCTYPESystemIdentifier_handler();
    inline void BogusDOCTYPE_handler();
    inline void CDATASection_handler();
    inline void CDATASectionBracket_handler();
    inline void CDATASectionEnd_handler();
    inline void CharacterReference_handler();
    inline void NamedCharacterReference_handler();
    inline void AmbiguousAmpersand_handler();
    inline void NumericCharacterReference_handler();
    inline void HexadecimalCharacterReferenceStart_handler();
    inline void DecimalCharacterReferenceStart_handler();
    inline void HexadecimalCharacterReference_handler();
    inline void DecimalCharacterReference_handler();
    inline void NumericCharacterReferenceEnd_handler();
};