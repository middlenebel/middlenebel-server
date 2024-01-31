#if !defined( LANG_CLJ_STRING_H )
#define LANG_CLJ_STRING_H

#include <iostream>
#include <string_view>

#include "lang.CljObject.hpp"

class CljString : public CljObject{
    public: 
        std::string std_string;

        // Constructors
        CljString(const char* str);
        CljString(const std::string_view sv);
        CljString(std::string std_string);
        CljString(int intValue);
        CljString(long longValue);

        // Operators
        friend std::ostream& operator << (std::ostream& os, const CljString& str);
};
#endif // !defined( LANG_CLJ_STRING_H )