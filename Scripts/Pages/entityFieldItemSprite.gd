extends Control



var root_path: String
# Called when the node enters the scene tree for the first time.
func _ready():
	print(singleton.cur_project_folder_path)
	$FileDialog.current_path = singleton.cur_project_folder_path + "/build/res/sprites/"
	root_path = $FileDialog.current_path
	pass # Replace with function body.
	
	var cur_entityDef = singleton.get_cur_entityDef()
	#return
	if(len(cur_entityDef["spritePath"]) > 0):
		change_spr_by_path(cur_entityDef["spritePath"])
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_changeSpriteBtn_button_down():
	print($FileDialog.current_path)
	$FileDialog.popup_centered()

func change_spr_by_path(path: String):
	#Update sprite in settings
	var img1 = Image.new()
	img1.load(path)
	var imgTex = ImageTexture.new()
	imgTex.create_from_image(img1, 1)
	$HBoxContainer/VBoxContainer/Control/Sprite.texture = imgTex;

func _on_FileDialog_file_selected(path):
	print(path)
	var spr_name: String
	var find_ind: int = path.find(root_path,0)
	if find_ind:
		spr_name = path.substr(find_ind+len(root_path))
		spr_name = spr_name.split(".")[0]
		spr_name = spr_name.split("-")[0]
		spr_name = spr_name.replace("/", "_")
	
	spr_name = "&spr_" + spr_name
	$HBoxContainer/VBoxContainer/TextEdit.text = spr_name
	change_spr_by_path(path)
	
	
	#Change defaultValue to sprite reference
	singleton.change_cur_field("defaultValue", spr_name)
	
	#Change spritePath if entity
	singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["spritePath"] = path
	pass # Replace with function body.
