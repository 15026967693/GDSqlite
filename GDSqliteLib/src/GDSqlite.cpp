#include "GDSqlite.h"
using namespace sqliteLib;

void GDSqlite::_register_methods() {
    godot::register_method("open_db", &GDSqlite::open_db);
    godot::register_method("exec_sql", &GDSqlite::exec_sql);
   godot::register_method("create_stmt", &GDSqlite::create_stmt);
    godot::register_method("delete_stmt", &GDSqlite::delete_stmt);
      godot::register_method("query_stmt", &GDSqlite::query_stmt);
    godot::register_method("exec_stmt", &GDSqlite::exec_stmt);
   godot::register_method("bind_value", &GDSqlite::bind_value);
    godot::register_method("close_db", &GDSqlite::close_db);
    godot::register_method("last_errmsg", &GDSqlite::last_errmsg);
    godot::register_property<GDSqlite, godot::String>("db_path", &GDSqlite::set_db_path, &GDSqlite::get_db_path, godot::String("test"));


}

GDSqlite::GDSqlite() {
}

GDSqlite::~GDSqlite() {

}


void GDSqlite::_init() {

}

bool GDSqlite::open_db(godot::String db_path){
  const char * utf8_path=db_path.utf8().get_data();
  int rc = sqlite3_open(utf8_path, &this->db);
  if( rc ){

    return false;
  }
  this->db_path=db_path.utf8().get_data();
  return true;

}


godot::String GDSqlite::get_db_path(){
  return godot::String(this->db_path);
}


void GDSqlite::set_db_path(godot::String new_db_path){
  this->db_path=new_db_path.utf8().get_data();


}

int  GDSqlite::sql_callback(void *NotUsed, int argc, char **argv, char **azColName){
  godot::FuncRef* callback=(godot::FuncRef*)NotUsed;
  godot::Dictionary dict;
 for(int i=0; i<argc; i++){
   //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   dict[azColName[i]]=argv[i] ? argv[i] : "NULL";

 }
 callback->call_func(dict);
 //printf("\n");
 return 0;
}
bool GDSqlite::create_stmt(godot::String key,godot::String sql){
  this->delete_stmt(key);
  sqlite3_stmt * stmt;
  int rc= sqlite3_prepare_v2(
    this->db,            /* Database handle */
  sql.utf8().get_data(),       /* SQL statement, UTF-8 encoded */
    -1,              /* Maximum length of zSql in bytes. */
    &stmt,  /* OUT: Statement handle */
    NULL     /* OUT: Pointer to unused portion of zSql */
  );
  if(rc){
    return false;
  }

  this->stmt_dict[std::string(key.utf8().get_data())]=stmt;
  return true;

}

bool GDSqlite::delete_stmt(godot::String key){
      std::unordered_map<std::string, sqlite3_stmt*>::iterator iter=this->stmt_dict.find(std::string(key.utf8().get_data()));
      if (iter != this->stmt_dict.end())
      {
        int rc=sqlite3_finalize(iter->second);
        if(rc){
          return false;
        }
        this->stmt_dict.erase(iter);
      }

      return true;

}

bool GDSqlite::exec_stmt(godot::String key){
      std::unordered_map<std::string, sqlite3_stmt*>::iterator iter=this->stmt_dict.find(std::string(key.utf8().get_data()));
      if (iter != this->stmt_dict.end())
      {
        int rc=sqlite3_step(iter->second);
        if(rc!=SQLITE_DONE){
            rc=sqlite3_reset(iter->second);
          return false;
        }
          rc=sqlite3_reset(iter->second);
      }
      return true;


}

int GDSqlite::query_stmt(godot::String key,godot::FuncRef* callback){
      std::unordered_map<std::string, sqlite3_stmt*>::iterator iter=this->stmt_dict.find(std::string(key.utf8().get_data()));
      if (iter != this->stmt_dict.end())
      {
        while(sqlite3_step(iter->second)==SQLITE_ROW){
          godot::Dictionary dict;
          for(int i=0; i<sqlite3_column_count(iter->second); i++){

            dict[godot::String(sqlite3_column_name(iter->second,i))]=godot::String((const char*)sqlite3_column_text(iter->second,i));

          }
          callback->call_func(dict);

        }
          sqlite3_reset(iter->second);
      }


}



bool GDSqlite::close_db(){

    std::unordered_map<std::string, sqlite3_stmt*>::iterator iter;
    for (iter = this->stmt_dict.begin(); iter !=  this->stmt_dict.end(); iter++)
    {
      int rc=sqlite3_finalize(iter->second);
      if(rc){
        return false;
      }

    }
     return  sqlite3_close(this->db)==SQLITE_OK;

}

  bool GDSqlite::bind_value(godot::String key,int pos,godot::Variant variant){

    std::unordered_map<std::string, sqlite3_stmt*>::iterator iter=this->stmt_dict.find(std::string(key.utf8().get_data()));
    if (iter != this->stmt_dict.end())
    {
      switch(variant.get_type()){
        case godot::Variant::BOOL:
        {
          bool v=variant;
           int rc=sqlite3_bind_int64(iter->second, pos, (int)v);
           if(rc){
             return false;
           }
        }
          break;
        case godot::Variant::INT:
        {
            int i=variant;
          int rc=  sqlite3_bind_int64(iter->second, pos, i);
            if(rc){
              return false;
            }
        }
          break;
        case godot::Variant::STRING:
        {
          godot::String str=variant;
         const char* c=str.utf8().get_data();
         int rc=sqlite3_bind_text(iter->second, pos,c,str.length(),NULL);
         if(rc){
           return false;
         }
        }
          break;
        case godot::Variant::REAL:
          {
            double d=variant;
            int rc=  sqlite3_bind_double(iter->second, pos, d);
              if(rc){
                return false;
              }
          }
          break;

      }

    }
    return true;


  }

godot::String GDSqlite::last_errmsg(){
  const char* msg=sqlite3_errmsg(this->db);
  return godot::String(msg);


}




godot::String GDSqlite::exec_sql(godot::String sql,godot::FuncRef* callback){
  char *zErrMsg = 0;
    godot::String result;
  int  rc = sqlite3_exec(db, sql.utf8().get_data(), GDSqlite::sql_callback, callback, &zErrMsg);
  if( rc!=SQLITE_OK ){
    result=godot::String(zErrMsg);
    sqlite3_free(zErrMsg);
  }
  return result;

}
