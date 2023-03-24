extends Node2D


var level_container

var layer_id: int = 0

func _ready():
	singleton.in_modal_window = true
	init_pos()
	update_load_image_modes()

func init_pos():
	$CanvasLayer.transform.origin= get_viewport().get_mouse_position()

func update_load_image_modes():
	var load_modes: Array = singleton.get_load_modes(level_container.cur_level_ind)
	$CanvasLayer/VBoxContainer/HBoxContainer/loadImageOptionBga.select(load_modes[0])
	$CanvasLayer/VBoxContainer/HBoxContainer2/loadImageOptionBgb.select(load_modes[1])


func _on_DeleteLevelBtn_pressed():
	level_container.delete_self()
	singleton.in_modal_window = false
	queue_free()



func _on_ChangeBgaBtn_pressed():
	layer_id = 0 # Now loading bga
	$CanvasLayer2/LoadBGFile.popup_centered()


func _on_ChangeBgbBtn_pressed():
	layer_id = 1 # Now loading bgb
	$CanvasLayer2/LoadBGFile.popup_centered()


func _on_loadImageOptionBgb_item_selected(index):
	level_container.change_bgb_mode(index)


func _on_loadImageOptionBga_item_selected(index):
	level_container.change_bga_mode(index)


func _on_LoadBGFile_file_selected(path):
	var ext = path.get_extension();
	if(!(ext in ['png', 'bmp'])):
		return
	var localPath = path.replace(ProjectSettings.globalize_path("res://"), "./")
	var load_modes: Array = singleton.get_load_modes(level_container.cur_level_ind)
	match layer_id:
		0: #bga
			level_container.change_bga(localPath)
		1: #bgb
			level_container.change_bgb(localPath)
	singleton.in_modal_window = false
	queue_free()
	


func _on_AddLevelBtn_pressed():
	var global_mouse_position = get_global_mouse_position()
	singleton.add_level(global_mouse_position)
	get_tree().call_group("tilemapEditorWindow", "load_levels")
	singleton.in_modal_window = false
	queue_free()


func _on_DeleteBgaBtn_pressed():
	level_container.delete_bga()
	


func _on_DeleteBgbBtn_pressed():
	level_container.delete_bgb()
