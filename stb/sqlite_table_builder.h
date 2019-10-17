
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
    typedef std::vector<std::string> Index;
    typedef std::vector<Index> IndexArray;
public:
    builder();
    builder(std::string const& sqlite3_name);
    ~builder();
public:
    bool open(std::string const& sqlite3_name) noexcept;
    void close();
public:
    builder& table(std::string const& tableName);
    builder& add(std::string const& fieldName, std::string const& fieldType);
    builder& index(std::string const& fieldsName);
    builder& index(std::string const& fields1, std::string const& fields2);
    builder& index(std::string const& fields1, std::string const& fields2, std::string const& fields3);
    void build(bool rebuild = false) noexcept (false);
private:
    CppSQLite3DB mdb;
    std::string tableName;
    InfoArray infos;
    IndexArray indexs;
};

}

#endif
