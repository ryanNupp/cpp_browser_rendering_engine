#pragma once

#include <span>


namespace Codepoint {

    inline constexpr char32_t line_feed              = 0x000A;
    inline constexpr char32_t form_feed              = 0x000C;
    inline constexpr char32_t carriage_return        = 0x000D;
    inline constexpr char32_t replacement_character  = 0xFFFD;

    // sentinel value to represent end-of-file, not an actual unicode codepoint
    inline constexpr char32_t end_of_file            = 0xFFFF'FFFF;



    // Codepoint utility functions

    inline bool is_leading_surrogate(char32_t codept)
    {
        return (codept >= 0xD800) && (codept <= 0xDBFF);
    }


    inline bool is_trailing_surrogate(char32_t codept)
    {
        return (codept >= 0xDC00) && (codept <= 0xDFFF);
    }


    inline bool is_surrogate(char32_t codept)
    {
        return is_leading_surrogate(codept) || is_trailing_surrogate(codept);
    }


    inline bool is_scalar_value(char32_t codept)
    {
        return !is_surrogate(codept);
    }


    inline bool is_noncharacter(char32_t codept)
    /*
        https://infra.spec.whatwg.org/#noncharacter
    */
    {
        return ((codept >= 0xFDD0) && (codept <= 0xFDEF))
            || ((codept & 0xFFFE) == 0xFFFE && (codept >> 16));
    }


    inline bool is_ascii_code_point(char32_t codept)
    {
        return (codept >= 0x0000) && (codept <= 0x007F);
    }


    inline bool is_ascii_tab_or_newline(char32_t codept)
    {
        return (codept == '\t')
            || (codept == Codepoint::line_feed)
            || (codept == Codepoint::carriage_return);
    }


    inline bool is_ascii_whitespace(char32_t codept)
    {
        return (codept == '\t')
            || (codept == Codepoint::line_feed)
            || (codept == Codepoint::form_feed)
            || (codept == Codepoint::carriage_return)
            || (codept == ' ');
    }


    inline bool is_c0_control(char32_t codept)
    {
        return (codept >= 0x0000) && (codept <= 0x001F);
    }


    inline bool is_c0_control_or_space(char32_t codept)
    {
        return is_c0_control(codept) || (codept == 0x0020);
    }


    inline bool is_control(char32_t codept)
    {
        return is_c0_control(codept) || ((codept >= 0x007F) && (codept <= 0x009F));
    }


    inline bool is_ascii_digit(char32_t codept)
    {
        return (codept >= '0') && (codept <= '9');
    }


    inline bool is_ascii_upper_hex_digit(char32_t codept)
    {
        return (codept >= 'A') && (codept <= 'F');
    }


    inline bool is_ascii_lower_hex_digit(char32_t codept)
    {
        return (codept >= 'a') && (codept <= 'f');
    }


    inline bool is_ascii_hex_digit(char32_t codept)
    {
        return is_ascii_upper_hex_digit(codept) || is_ascii_lower_hex_digit(codept);
    }


    inline bool is_ascii_upper_alpha(char32_t codept)
    {
        return (codept >= 'A') && (codept <= 'Z');
    }


    inline bool is_ascii_lower_alpha(char32_t codept)
    {
        return (codept >= 'a') && (codept <= 'z');
    }


    inline bool is_ascii_alpha(char32_t codept)
    {
        return is_ascii_lower_alpha(codept) || is_ascii_upper_alpha(codept);
    }


    inline bool is_ascii_alphanumeric(char32_t codept)
    {
        return is_ascii_alpha(codept) || is_ascii_digit(codept);
    }


    inline char32_t to_lowercase(char32_t codept)
    {
        return codept + 0x0020;
    }


    inline bool is_ascii_case_insensitive_match(std::span<const char32_t> a, std::span<const char32_t> b)
    {
        if (a.size() != b.size()) {
            return false;
        }
        for (auto i{ 0 }; i < a.size(); ++i) {
            char32_t char_a{ (is_ascii_upper_alpha(a[i])) ? to_lowercase(a[i]) : a[i]};
            char32_t char_b{ (is_ascii_upper_alpha(b[i])) ? to_lowercase(b[i]) : b[i]};
            if (char_a != char_b) {
                return false;
            }
        }
        return true;
    }


    inline bool utf32_str_equal(std::span<const char32_t> a, std::span<const char32_t> b)
    {
        if (a.size() != b.size()) {
            return false;
        }
        return std::equal(a.begin(), a.end(), b.begin());
    }

} // namespace Codepoint