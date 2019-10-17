
#include "stdio.h"
#include "stb/sqlite_table_builder.h"

int main() {
    try {
        stb::builder b("test.sql");
        b.table("table1").
            add("id", stb::type_id).
            add("name", stb::type_string(256)).
            add("avatar", stb::type_string(256)).
            add("size", stb::type_int).
            index("name").
            index("size", "name").
            index("avatar").
            build(true);
    } catch (stb::Exception& e) {
        printf("%s\n", e.errorMessage());
    }
    return 0;
}