
#ifndef __SQLITE_TABLE_BUILDER_H__
#define __SQLITE_TABLE_BUILDER_H__

#include "CppSQLite3.h"
#include <string>
#include <vector>

namespace stb {

extern std::string type_id;
extern std::string type_bool;
extern std::string type_int;
extern std::string type_string(int size);
extern std::string type_text;

typedef CppSQLite3Exception Exception;

class builder {
public:
    typedef struct {
        std::string fieldName;
        std::string fieldType;
    } FieldInfo;
    typedef std::vector<FieldInfo> InfoArray;
public:
    builder();
    builder(std::string const& sqlite3_name);
    ~builder();
public:
    bool open(std::string const& sqlite3_name) noexcept;
    void close();
public:
    builder& create(std::string const& tableName);
    builder& add(std::string const& fieldName, std::string const& fieldType);
    void build(bool rebuild = false) throw (Exception&);
private:
    CppSQLite3DB mdb;
    std::string tableName;
    InfoArray infos;
};

}

#endif
