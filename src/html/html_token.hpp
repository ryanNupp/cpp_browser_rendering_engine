#pragma once

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
    void clear()
    {
        m_type = Type::Uninitialized;
        m_data.clear();
        m_attributes.clear();
        m_doctype_data.reset(nullptr);
        m_self_closing = false;
    }


    template <HTMLToken::Type T>
    constexpr void initialize()
    {
        static_assert(T != HTMLToken::Type::Uninitialized, "Use clear() to uninitialize an HTMLToken");

        m_type = T;

        if constexpr (T == Type::DOCTYPE) {
            m_doctype_data = std::make_unique<DOCTYPEData>();
        }
        else if constexpr (T == Type::StartTag || T == Type::EndTag) {
            m_self_closing = false;
        }
        else if constexpr (T == Type::Comment) {

        }
        else if constexpr (T == Type::Character) {

        }
        else if constexpr (T == Type::EndOfFile) {

        }
    }


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