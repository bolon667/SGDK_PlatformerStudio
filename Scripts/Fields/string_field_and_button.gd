extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var root_path: String

# Called when the node enters the scene tree for the first time.
func _ready():
	$FileDialog.current_path = "./StudioType/SGDK/Engines/" + singleton.cur_engine + "/build/res/sprites/"
	root_path = $FileDialog.current_path
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_ActionBtn_button_down():
	$FileDialog.popup_centered()
	pass # Replace with function body.


func _on_FileDialog_file_selected(path: String):
	var spr_name: String
	var find_ind: int = path.find(root_path,0)
	if find_ind:
		spr_name = path.substr(find_ind+len(root_path))
		spr_name = spr_name.split(".")[0]
		spr_name = spr_name.split("-")[0]
		spr_name = spr_name.replace("/", "_")
	
	spr_name = "&spr_" + spr_name
	$HBoxContainer/VBoxContainer/TextEdit.text = spr_name
	
	#update value in database
	singleton.change_fiendInst_by_instId($HBoxContainer/Label.text, spr_name)
	singleton.change_sprite_by_instId(path)
	
	#change entity obj sprite
	get_tree().call_group("tilemapEditorWindow", "change_cur_entity_pic", path)
	#var temp_str = "StudioType/SGDK/Engines/" + singleton.cur_engine + "/build"
	#print(root_path)
	pass # Replace with function body.


func _on_TextEdit_text_changed():
	singleton.change_fiendInst_by_instId(singleton.cur_entity_inst_ind, $HBoxContainer/Label.text, $HBoxContainer/VBoxContainer/TextEdit.text)
