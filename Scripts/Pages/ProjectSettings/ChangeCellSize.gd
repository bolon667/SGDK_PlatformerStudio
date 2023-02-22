extends OptionButton


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	var cell_ind: int = 0
	if(singleton.cell_size == 16):
		cell_ind = 1
	select(cell_ind)


func _on_ChangeCellSize_item_selected(index):
	match index:
		0:
			print("changed to 8x8 cell size")
			singleton.change_cell_size(8)
			get_tree().call_group("tilemapEditorWindow", "update_tilemap_cell_size", Vector2(singleton.cell_size, singleton.cell_size))
		1:
			print("changed to 16x16 cell size")
			singleton.change_cell_size(16)
			get_tree().call_group("tilemapEditorWindow", "update_tilemap_cell_size", Vector2(singleton.cell_size, singleton.cell_size))
