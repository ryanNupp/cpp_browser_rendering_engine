#pragma once

#include <span>
#include <vector>
#include <memory>


class HTMLToken {
public:

    enum class Type {
        Uninitialized,
        DOCTYPE,
        StartTag,
        EndTag,
        Comment,
        Character,
        EndOfFile
    };

    struct Attribute {
        std::vector<char32_t> m_name;
        std::vector<char32_t> m_value;
    };

    struct DOCTYPEData {
        std::vector<char32_t> m_public_identifier;
        std::vector<char32_t> m_system_identifier;
        bool m_name_missing{ true };
        bool m_public_identifier_missing{ true };
        bool m_system_identifier_missing{ true };
        bool m_force_quirks{ false };
    };

    // clear everything and set type to uninitalized
    inline void clear()
    {
        m_type = Type::Uninitialized;
        m_data.clear();
        m_attributes.clear();
        m_doctype_data.reset(nullptr);
        m_self_closing = false;
    }


    template <HTMLToken::Type T>
    inline constexpr void initialize()
    {
        static_assert(T != HTMLToken::Type::Uninitialized, "Use clear() to uninitialize an HTMLToken");

        m_type = T;

        if constexpr (T == Type::DOCTYPE) {
            m_doctype_data = std::make_unique<DOCTYPEData>();
        }
    }


    inline void append_data(const char32_t character)
    {    m_data.push_back(character);    }

    inline void append_data_multiple(std::span<const char32_t> characters)
    {    m_data.insert(m_data.end(), characters.begin(), characters.end());    }

    inline void start_new_attribute()
    {    m_attributes.emplace_back();    }

    inline void attribute_name_append(const char32_t character)
    {    m_attributes.back().m_name.push_back(character);    }

    inline void attribute_value_append(const char32_t character)
    {    m_attributes.back().m_value.push_back(character);    }

    inline void set_self_closing_flag()
    {    m_self_closing = true;    }

    inline void set_doctype_force_quirks_flag()
    {    m_doctype_data->m_force_quirks = true;    }

    inline void set_doctype_name_not_missing()
    {    m_doctype_data->m_name_missing = false;    }

    inline void set_doctype_public_identifier_not_missing()
    {    m_doctype_data->m_public_identifier_missing = false;    }

    inline void set_doctype_system_identifier_not_missing()
    {    m_doctype_data->m_system_identifier_missing = false;    }

    inline void append_doctype_public_identifier(const char32_t character)
    {    m_doctype_data->m_public_identifier.push_back(character);    }

    inline void append_doctype_system_identifier(const char32_t character)
    {    m_doctype_data->m_public_identifier.push_back(character);    }

    // -- 

    inline HTMLToken::Type get_type()
    {    return m_type;    }

    inline std::span<const char32_t> get_tag_name()
    {    return m_data;    }

private:

    Type m_type{ Type::Uninitialized };

    std::vector<char32_t> m_data;
    /*
        Data representation per each type:
          DOCTYPE             -> Name
          StartTag & EndTag   -> Tag name
          Comment             -> Comment characters
          Character           -> Character
    */

    std::vector<Attribute> m_attributes;

    std::unique_ptr<DOCTYPEData> m_doctype_data;

    bool m_self_closing{ false };


    // Allow the tree builder to be able to access data
    friend class HTMLTreeBuilder;
};