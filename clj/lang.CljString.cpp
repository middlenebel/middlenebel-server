
#include "inc/lang.CljString.hpp"

CljString::CljString(const char* str){
    std_string = str;
}

CljString::CljString(const std::string_view sv){
    std_string = {sv.begin(), sv.end()};
}
std::ostream& operator<<(std::ostream& os, const CljString& str){
    os.write( str.std_string.c_str(),  str.std_string.length() );
    return os;
}
 
// std::istream& operator>>(std::istream& is, T& obj)
// {
//     // read obj from stream
//     if (/* T could not be constructed */)
//         is.setstate(std::ios::failbit);
//     return is;
// }