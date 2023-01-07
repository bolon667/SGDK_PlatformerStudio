extends Node2D

var id_of_current_entity = 0


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	var tile_ids = $TileMapEditorWindow/TileMap.tile_set.get_tiles_ids()
	for tile_id in tile_ids:
		var texture = $TileMapEditorWindow/TileMap.tile_set.tile_get_texture(tile_id)
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
