# GDSqlite
godot sqlite 链接库

## 示例代码
```python
	#初始化一个对象实例
	var sqlite=GDSqlite.new()
	#打开数据库
	sqlite.open_db("/home/jiayang/godot-demo/GDSqlite/test.db")
	#获取回调函数
	var cb=funcref(self,"sql_cb")
	#执行sql，返回值为错误信息
	print(sqlite.exec_sql("insert into  test values(2)",cb))
	#预编译sql指令
	print(sqlite.create_stmt("test","insert into test values(?)"))
	#绑定值(现阶段只做了text和int,因为c++和c的兼容性，目前sqlite3_bind_value用不了)
	print(sqlite.bind_value("test",1,22))
	#预编译sql指令
	print(sqlite.create_stmt("test2","select * from test"))
	#执行查询预编译指令
	print(sqlite.query_stmt("test2",cb))
	#获取sqlite最后的错误
	print(sqlite.last_errmsg())
	#直接执行sql
	print(sqlite.exec_sql("insert into test values(23)",null))
	#执行非查询的预编译指令
	print(sqlite.exec_stmt("test"))
	#删除预编译指令并进行回收，也可执行close_db一下自动回收所有并且关闭数据库
	print(sqlite.delete_stmt("test"))
	#关闭sqlite
	print(sqlite.close_db())
```
## api 说明
### 设置数据库地址留用api
  void set_db_path(godot::String new_db_path);


### 获取最后的报错信息
    godot::String last_errmsg();



###  在内部的map使用指定的key键值保存预编译的sql指令
    bool create_stmt(godot::String key,godot::String value);

### 回收不使用的sql语句，key为保存预编译指令的键值  关闭数据库也会自动回收
    bool delete_stmt(godot::String key);

### 使用预编译指令进行查询，key为保存预编译指令的键值 ，callback 为回调函数,回调第一个参数是每条记录包装的Dictionary对象，一条记录执行一遍
    int query_stmt(godot::String key,godot::FuncRef* callback);

### 执行非查询预编译sql ，key为保存预编译指令的键值
    bool exec_stmt(godot::String key);

### 为指定的key键值对应的预编译指令绑定制定的值，pos为1开始（不是0），目前支持int text类型的值

    bool bind_value(godot::String key,int pos,godot::Variant variant);
### 打开指定路径的sqlite数据库
    bool open_db(godot::String db_path);

### 直接执行sql并执行回调，回调第一个参数是每条记录包装的Dictionary对象
    godot::String exec_sql(godot::String sql,godot::FuncRef* callback);
   ### 关闭数据库并且回收所有预编译sql
    bool close_db();

# 编译指令

Linux（测试版正式版请自行调整优化等级等参数）
```bash
g++ -o src/GDLibrary.os -c -std=c++17 -fPIC -g3 -Og -fPIC -I. -Igodot-cpp/godot_headers -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Isrc src/GDLibrary.cpp

g++ -o src/GDSqlite.os -c -std=c++17 -fPIC -g3 -Og -fPIC -I. -Igodot-cpp/godot_headers -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Isrc src/GDSqlite.cpp

gcc -o src/Sqlite3.os -c -fPIC -g3 -Og -fPIC src/sqlite3.c

g++ -o GDsqlite3/bin/x11/libsqlite3.so -shared src/GDLibrary.os src/GDSqlite.os src/Sqlite3.os  -Lgodot-cpp/bin -lgodot-cpp.linux.debug.64

```
windows待完成（cl不熟，GDNative貌似gcc编译出来有问题可能是电脑有问题待定）
```bash

```

由于容量限制GDSqliteLib/godot-cpp/bin/libgodot-cpp.linux.debug.64.a未上传
