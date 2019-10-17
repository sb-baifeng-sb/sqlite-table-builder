
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

builder& builder::table(std::string const& tableName) {
    this->tableName = tableName;
    this->infos.clear();
    this->indexs.clear();
    return *this;
}

builder& builder::add(std::string const& fieldName, std::string const& fieldType) {
    this->infos.push_back(FieldInfo{fieldName, fieldType});
    return *this;
}

builder& builder::index(std::string const& fieldsName) {
    Index arr;
    arr.push_back(fieldsName);
    this->indexs.push_back(arr);
    return *this;
}

builder& builder::index(std::string const& fields1, std::string const& fields2) {
    Index arr;
    arr.push_back(fields1);
    arr.push_back(fields2);
    this->indexs.push_back(arr);
    return *this;
}

builder& builder::index(std::string const& fields1, std::string const& fields2, std::string const& fields3) {
    Index arr;
    arr.push_back(fields1);
    arr.push_back(fields2);
    arr.push_back(fields3);
    this->indexs.push_back(arr);
    return *this;
}

void builder::build(bool rebuild) noexcept (false) {
    char temp[1024] = {0};

    // 删除表
    if (rebuild) {
        sprintf(temp, "drop table %s;", this->tableName.c_str());
        this->mdb.execDML(temp);
    }

    // 创建表
    std::string sql = std::string("create table ") + (rebuild ? "" : "if not exists ") + this->tableName;
    sql += "(";
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
    this->mdb.execDML(sql.c_str());

    // 创建索引
    for (int i = 0; i < (int)this->indexs.size(); i++) {
        auto& index = this->indexs[i];
        memset(temp, 0, sizeof(temp));
        if (index.size() == 1) {
            sprintf(temp, "create index if not exists %s on %s (%s);", (index[0]).c_str(), this->tableName.c_str(), index[0].c_str());
        } else if (index.size() == 2) {
            sprintf(temp, "create index if not exists %s on %s (%s, %s);", (index[0]+"_"+index[1]).c_str(), this->tableName.c_str(), index[0].c_str(), index[1].c_str());
        } else if (index.size() == 3) {
            sprintf(temp, "create index if not exists %s on %s (%s, %s, %s);", (index[0]+"_"+index[1]+"_"+index[2]).c_str(), this->tableName.c_str(), index[0].c_str(), index[1].c_str(), index[2].c_str());
        } else {
            throw Exception(CPPSQLITE_ERROR, "不支持的索引字段数量");
        }
        printf("%s\n", temp);
        this->mdb.execDML(temp);
    }
}

}