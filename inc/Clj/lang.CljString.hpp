#if !defined( LANG_CLJ_STRING_H )
#define LANG_CLJ_STRING_H

#include "lang.CljObject.hpp"

class CljString : public CljObject{
    private: std::string std__string;
    public: CljString();
    public: CljString(std::string std__string);
    public: CljString(int intValue);
    public: CljString(long longValue);
};
#endif // !defined( LANG_CLJ_STRING_H )