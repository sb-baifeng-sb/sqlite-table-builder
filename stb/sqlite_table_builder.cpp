
#include "sqlite_table_builder.h"

namespace stb {

std::string type_id = "integer primary key AUTOINCREMENT";
std::string type_bool = "boolean";
std::string type_int = "integer";
std::string type_string(int size) {
    char temp[64] = {0};
    sprintf(temp, "varchar(%d)", size);
    return temp;
}
std::string type_text = "text";

static std::string _defaultValue(std::string const& type) {
    if (type == type_id) {
        return "";
    } else if (type == type_bool) {
        return "default '0'";
    } else if (type == type_int) {
        return "default '0'";
    }
    return "default ''";
}

builder::builder() {

}

builder::builder(std::string const& sqlite3_name) {
    try {
        this->open(sqlite3_name);
    } catch (Exception& e) {
        printf("open error: %s\n", e.errorMessage());
    }
}

builder::~builder() {
    this->close();
}

bool builder::open(std::string const& sqlite3_name) noexcept {
    try {
        this->mdb.open(sqlite3_name.c_str());
    } catch (Exception& e) {
        return false;
    }
    return true;
}

void builder::close() {
    this->mdb.close();
}

builder& builder::create(std::string const& tableName) {
    this->tableName = tableName;
    this->infos.clear();
    return *this;
}

builder& builder::add(std::string const& fieldName, std::string const& fieldType) {
    this->infos.push_back(FieldInfo{fieldName, fieldType});
    return *this;
}

void builder::build(bool rebuild) noexcept (false) {
    if (rebuild) {
        auto stmt = this->mdb.compileStatement("drop table ?");
        stmt.bind(0, this->tableName.c_str());
        stmt.execDML();
    }
    std::string sql = std::string("create table ") + (rebuild ? "" : "if not exists ") + this->tableName;
    sql += "(";
    char temp[1024];
    for (int i = 0; i < (int)this->infos.size(); i++) {
        auto& item = this->infos[i];
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "`%s` %s not null", item.fieldName.c_str(), item.fieldType.c_str());
        sql += temp;
        sql = sql + " " + _defaultValue(item.fieldType);
        if (i+1 < (int)this->infos.size()) {
            sql += ", ";
        }
    }
    sql += ");";
    printf("%s\n", sql.c_str());
    auto stmt = this->mdb.compileStatement(sql.c_str());
    stmt.execDML();
}

}