#ifndef COT_MODEL_H
#define COT_MODEL_H

#include <string>
#include "sql.h"
#include "type.h"

#define BEGIN_MODEL(name) \
    class name: public SqlQueryPart \
    { \
        public: \
            static std::string stringify() { return # name ; }\
        typedef TypePair<TypeNil,

#define FIELD(name, ValueType) \
                ValueType> name ## List; \
    ValueType::castType name; \
    class _ ## name ## _: public SqlQueryPart \
    { \
        public: \
            static std::string stringify() { return # name ; } \
            typedef ValueType itemType; \
    };\
    typedef TypePair< TypePair<name ## List::tail, name ## List::head>,

#define END_MODEL \
        TypeNil> wrongTypeList;\
        typedef wrongTypeList::head typeList;\
    };

#endif //COT_MODEL_H
