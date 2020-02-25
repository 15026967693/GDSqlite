#ifndef GDSQLITE_H
#define GDSQLITE_H




#include <Godot.hpp>
#include <Node2D.hpp>
#include <FuncRef.hpp>
#include <Variant.hpp>
#include <sqlite3.h>
#include <string>
#include <unordered_map>

namespace sqliteLib {

class GDSqlite : public godot::Node2D {
    GODOT_CLASS(GDSqlite, godot::Node2D )

private:
  sqlite3* db;
  const char* db_path;
  std::unordered_map<std::string,sqlite3_stmt*> stmt_dict;



public:
    static void _register_methods();

    GDSqlite();
    ~GDSqlite();

    godot::String get_db_path();
    void set_db_path(godot::String new_db_path);
    godot::String last_errmsg();

    bool create_stmt(godot::String key,godot::String value);

    bool delete_stmt(godot::String key);
    int query_stmt(godot::String key,godot::FuncRef* callback);
    bool exec_stmt(godot::String key);

    bool bind_value(godot::String key,int pos,godot::Variant variant);


    void _init();
    bool open_db(godot::String db_path);
    godot::String exec_sql(godot::String sql,godot::FuncRef* callback);
    static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName);
    bool close_db();

    // our initializer called by Godot

};

}

#endif
