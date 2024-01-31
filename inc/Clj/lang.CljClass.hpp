#if !defined( LANG_CLJ_CLASS_H )
#define LANG_CLJ_CLASS_H

#include "lang.CljObject.hpp"
#include "lang.CljString.hpp"

class CljClass : public CljObject{
    CljString	getCanonicalName();
};
#endif // !defined( LANG_CLJ_CLASS_H )