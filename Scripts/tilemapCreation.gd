extends Node2D

var id_of_current_entity = 0

onready var entity_menu_t = preload("res://Scenes/Pages/EntityMenu.tscn")



onready var TileMapEditorWindow = $TileMapEditorWindow
onready var TileMapEditorWindow_tileMap = $TileMapEditorWindow/BGSprite/TileMap



# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	var tile_ids = TileMapEditorWindow_tileMap.tile_set.get_tiles_ids()
	for tile_id in tile_ids:
		var texture = TileMapEditorWindow_tileMap.tile_set.tile_get_texture(tile_id)
		$CanvasLayer/VBoxContainer/ChangeTileMode.add_icon_item(texture, str(tile_id), tile_id)

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func _on_LoadBackgroundBtn_button_down():
	$CanvasLayer/LoadBGFile.popup_centered()


func _on_LoadBGFile_file_selected(path):
	pass # Replace with function body.
	var ext = path.get_extension();
	if(!(ext in ['png', 'jpg', 'bmp'])):
		return
	print(ext)
	var bgImage = Image.new()
	bgImage.load(path)
	var imgTex = ImageTexture.new()
	imgTex.create_from_image(bgImage, 1)
	
	$TileMapEditorWindow/BGSprite.texture = imgTex;


	


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
