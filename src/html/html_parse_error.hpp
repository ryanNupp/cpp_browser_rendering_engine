#pragma once

#include <print>


enum class ParseError {
    abrupt_closing_of_empty_comment,
    abrupt_doctype_public_identifier,
    abrupt_doctype_system_identifier,
    absence_of_digits_in_numeric_character_reference,
    cdata_in_html_content,
    character_reference_outside_unicode_range,
    control_character_in_input_stream,
    control_character_reference,
    duplicate_attribute,
    end_tag_with_attributes,
    end_tag_with_trailing_solidus,
    eof_before_tag_name,
    eof_in_cdata,
    eof_in_comment,
    eof_in_doctype,
    eof_in_script_html_comment_like_text,
    eof_in_tag,
    incorrectly_closed_comment,
    incorrectly_opened_comment,
    invalid_character_sequence_after_doctype_name,
    invalid_first_character_of_tag_name,
    missing_attribute_value,
    missing_doctype_name,
    missing_doctype_public_identifier,
    missing_doctype_system_identifier,
    missing_end_tag_name,
    missing_quote_before_doctype_public_identifier,
    missing_quote_before_doctype_system_identifier,
    missing_semicolon_after_character_reference,
    missing_whitespace_after_doctype_public_keyword,
    missing_whitespace_after_doctype_system_keyword,
    missing_whitespace_before_doctype_name,
    missing_whitespace_between_attributes,
    missing_whitespace_between_doctype_public_and_system_identifiers,
    nested_comment,
    noncharacter_character_reference,
    noncharacter_in_input_stream,
    non_void_html_element_start_tag_with_trailing_solidus,
    null_character_reference,
    surrogate_character_reference,
    surrogate_in_input_stream,
    unexpected_character_after_doctype_system_identifier,
    unexpected_character_in_attribute_name,
    unexpected_character_in_unquoted_attribute_value,
    unexpected_equals_sign_before_attribute_name,
    unexpected_null_character,
    unexpected_question_mark_instead_of_tag_name,
    unexpected_solidus_in_tag,
    unknown_named_character_reference
};


static void handle_parse_error(ParseError err)
/*
    TODO: Definitely do something better than this...
*/
{
    std::print("PARSE ERROR: ");

    switch (err) {
    case ParseError::abrupt_closing_of_empty_comment:                                  std::println("abrupt_closing_of_empty_comment");
    case ParseError::abrupt_doctype_public_identifier:                                 std::println("abrupt_doctype_public_identifier");
    case ParseError::abrupt_doctype_system_identifier:                                 std::println("abrupt_doctype_system_identifier");
    case ParseError::absence_of_digits_in_numeric_character_reference:                 std::println("absence_of_digits_in_numeric_character_reference");
    case ParseError::cdata_in_html_content:                                            std::println("cdata_in_html_content");
    case ParseError::character_reference_outside_unicode_range:                        std::println("character_reference_outside_unicode_range");
    case ParseError::control_character_in_input_stream:                                std::println("control_character_in_input_stream");
    case ParseError::control_character_reference:                                      std::println("control_character_reference");
    case ParseError::duplicate_attribute:                                              std::println("duplicate_attribute");
    case ParseError::end_tag_with_attributes:                                          std::println("end_tag_with_attributes");
    case ParseError::end_tag_with_trailing_solidus:                                    std::println("end_tag_with_trailing_solidus");
    case ParseError::eof_before_tag_name:                                              std::println("eof_before_tag_name");
    case ParseError::eof_in_cdata:                                                     std::println("eof_in_cdata");
    case ParseError::eof_in_comment:                                                   std::println("eof_in_comment");
    case ParseError::eof_in_doctype:                                                   std::println("eof_in_doctype");
    case ParseError::eof_in_script_html_comment_like_text:                             std::println("eof_in_script_html_comment_like_text");
    case ParseError::eof_in_tag:                                                       std::println("eof_in_tag");
    case ParseError::incorrectly_closed_comment:                                       std::println("incorrectly_closed_comment");
    case ParseError::incorrectly_opened_comment:                                       std::println("incorrectly_opened_comment");
    case ParseError::invalid_character_sequence_after_doctype_name:                    std::println("invalid_character_sequence_after_doctype_name");
    case ParseError::invalid_first_character_of_tag_name:                              std::println("invalid_first_character_of_tag_name");
    case ParseError::missing_attribute_value:                                          std::println("missing_attribute_value");
    case ParseError::missing_doctype_name:                                             std::println("missing_doctype_name");
    case ParseError::missing_doctype_public_identifier:                                std::println("missing_doctype_public_identifier");
    case ParseError::missing_doctype_system_identifier:                                std::println("missing_doctype_system_identifier");
    case ParseError::missing_end_tag_name:                                             std::println("missing_end_tag_name");
    case ParseError::missing_quote_before_doctype_public_identifier:                   std::println("missing_quote_before_doctype_public_identifier");
    case ParseError::missing_quote_before_doctype_system_identifier:                   std::println("missing_quote_before_doctype_system_identifier");
    case ParseError::missing_semicolon_after_character_reference:                      std::println("missing_semicolon_after_character_reference");
    case ParseError::missing_whitespace_after_doctype_public_keyword:                  std::println("missing_whitespace_after_doctype_public_keyword");
    case ParseError::missing_whitespace_after_doctype_system_keyword:                  std::println("missing_whitespace_after_doctype_system_keyword");
    case ParseError::missing_whitespace_before_doctype_name:                           std::println("missing_whitespace_before_doctype_name");
    case ParseError::missing_whitespace_between_attributes:                            std::println("missing_whitespace_between_attributes");
    case ParseError::missing_whitespace_between_doctype_public_and_system_identifiers: std::println("missing_whitespace_between_doctype_public_and_system_identifiers");
    case ParseError::nested_comment:                                                   std::println("nested_comment");
    case ParseError::noncharacter_character_reference:                                 std::println("noncharacter_character_reference");
    case ParseError::noncharacter_in_input_stream:                                     std::println("noncharacter_in_input_stream");
    case ParseError::non_void_html_element_start_tag_with_trailing_solidus:            std::println("non_void_html_element_start_tag_with_trailing_solidus");
    case ParseError::null_character_reference:                                         std::println("null_character_reference");
    case ParseError::surrogate_character_reference:                                    std::println("surrogate_character_reference");
    case ParseError::surrogate_in_input_stream:                                        std::println("surrogate_in_input_stream");
    case ParseError::unexpected_character_after_doctype_system_identifier:             std::println("unexpected_character_after_doctype_system_identifier");
    case ParseError::unexpected_character_in_attribute_name:                           std::println("unexpected_character_in_attribute_name");
    case ParseError::unexpected_character_in_unquoted_attribute_value:                 std::println("unexpected_character_in_unquoted_attribute_value");
    case ParseError::unexpected_equals_sign_before_attribute_name:                     std::println("unexpected_equals_sign_before_attribute_name");
    case ParseError::unexpected_null_character:                                        std::println("unexpected_null_character");
    case ParseError::unexpected_question_mark_instead_of_tag_name:                     std::println("unexpected_question_mark_instead_of_tag_name");
    case ParseError::unexpected_solidus_in_tag:                                        std::println("unexpected_solidus_in_tag");
    case ParseError::unknown_named_character_reference:                                std::println("unknown_named_character_reference");
    }
}