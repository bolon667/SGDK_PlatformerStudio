extends Node2D

var id_of_current_entity = 0

onready var entity_menu_t = preload("res://Scenes/Pages/EntityMenu.tscn")

onready var TileMapEditorWindow = $TileMapEditorWindow
onready var TileMapEditorWindow_tileMap = $TileMapEditorWindow/BGSprite/TileMap
onready var TileMapEditorWindow_bg_sprite = $TileMapEditorWindow/BGSprite/


var level_count: int = 0
# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	var tile_ids = TileMapEditorWindow_tileMap.tile_set.get_tiles_ids()
	for tile_id in tile_ids:
		var texture = TileMapEditorWindow_tileMap.tile_set.tile_get_texture(tile_id)
		$CanvasLayer/VBoxContainer/ChangeTileMode.add_icon_item(texture, str(tile_id), tile_id)
		
	level_count = singleton.get_level_count()
	for cur_level_ind in range(level_count):
		$CanvasLayer/VBoxContainer/ChangeCurLevel.add_item("Level_"+str(cur_level_ind), cur_level_ind)

func _process(delta):
	if(Input.is_action_just_pressed("save_project")):
		var texture_size = TileMapEditorWindow_bg_sprite.texture.get_size()
		var collision_map_size = Vector2(texture_size.x/singleton.cell_size, texture_size.y/singleton.cell_size)
		singleton.save_collisionMap(TileMapEditorWindow_tileMap, collision_map_size)
		singleton.save_project()
		

func _on_LoadBackgroundBtn_button_down():
	$CanvasLayer/LoadBGFile.popup_centered()


func _on_LoadBGFile_file_selected(path):
	
	var ext = path.get_extension();
	if(!(ext in ['png', 'jpg', 'bmp'])):
		return
	print(ext)
	singleton.change_bgRelPath(path)
	
	var bgImage = Image.new()
	bgImage.load(path)
	var imgTex = ImageTexture.new()
	imgTex.create_from_image(bgImage, 1)
	
	$TileMapEditorWindow/BGSprite.texture = imgTex;
	var level_size = $TileMapEditorWindow/BGSprite.texture.get_size()
	singleton.change_level_size(level_size);
	
	

	

func _on_GenCodeBtn_button_down():
	$CanvasLayer/VBoxContainer/GenCodeBtn.call("test")


func _on_ChangeTileMode_item_selected(index):
	#pass
	$TileMapEditorWindow.draw_tile_ind = index


func _on_addNewEntityTypeBtn_button_down():
	#get_tree().change_scene("res://Scenes/Pages/EntityMenu.tscn")
	var inst = entity_menu_t.instance()
	add_child(inst)



func _on_TileMapEditorWindow_mouse_entered():
	print("mouse entered")
	pass # Replace with function body.


func _on_Button_button_down():
	print("test")
	pass # Replace with function body.


func _on_buildProjectBtn_pressed():
	$CanvasLayer/VBoxContainer/buildProjectBtn.call("buildProject")
	pass # Replace with function body.


func _on_Button_pressed():
	print(singleton.get_entityInstanAmount_by_levelNum(0))
	pass # Replace with function body.


func _on_addNewLevelBtn_button_down():
	pass # Replace with function body.
	level_count+=1
	singleton.add_level()
	$CanvasLayer/VBoxContainer/ChangeCurLevel.add_item("Level_"+str(level_count-1), level_count-1)


func _on_deleteCurLevelBtn_button_down():
	pass # Replace with function body.


func _on_ChangeCurLevel_item_selected(index):
	pass # Replace with function body.
	singleton.cur_level_ind = index
	get_tree().call_group("tilemapEditorWindow", "load_level")
