extends Node2D
var GDSqlite=load("res://GDSqliteBin/GDSqlite.gdns")
# Declare member variables here. Examples:
# var a = 2
# var b = "text"
func sql_cb(arg):
	print(arg)
	
# Called when the node enters the scene tree for the first time.
func _ready():
	var sqlite=GDSqlite.new()
	sqlite.open_db("/home/jiayang/godot-demo/GDSqlite/test.db")
	var cb=funcref(self,"sql_cb")
	#print(sqlite.exec_sql("delete from test",cb))
	#print(sqlite.exec_sql("insert into  test values(2)",cb))
	print(sqlite.create_stmt("test","insert into test values(?)"))
	print(sqlite.bind_value("test",1,22))
	print(sqlite.create_stmt("test2","select * from test"))
	#print(sqlite.bind_value("test2",1,20))
	print("querystmt",sqlite.query_stmt("test2",cb))
	print("querystmt",sqlite.last_errmsg())
	print(sqlite.exec_sql("insert into test values(23)",null))
	print("exec:",sqlite.exec_stmt("test"))
	print(sqlite.delete_stmt("test"))
	print(sqlite.exec_sql("select * from test",cb))
	print("close",sqlite.close_db())
	print(sqlite.last_errmsg())
	
	#print(sqlite.exec_sql("insert into test values(1)",cb))
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
