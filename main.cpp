
#include "stdio.h"
#include "stb/sqlite_table_builder.h"

int main() {
    try {
        stb::builder b("test.sql");
        b.create("table1").
            add("id", stb::type_id).
            add("name", stb::type_string(256)).
            build();
    } catch (stb::Exception& e) {
        printf("%s\n", e.errorMessage());
    }
    return 0;
}