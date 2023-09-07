extends Node2D


var level_container

var layer_id: int = 0
var image_mode_bga: int = 0
var image_mode_bgb: int = 0

var leftContainer

onready var msg_packs_menu_t = preload("res://Scenes/Pages/MessagePack/MessagePacksMenu.tscn")
onready var level_settings_menu_t = preload("res://Scenes/Pages/LevelSettings/LevelSettingsMenu.tscn")
onready var local_vars_menu_t = preload("res://Scenes/Pages/LocalVariablesMenu/VariablesMenu.tscn")

func _ready():
	singleton.in_modal_window = true
	print("level_container.cur_level_ind: ", level_container.cur_level_ind)
	leftContainer = get_tree().get_nodes_in_group("levelContainer")[0]
	$CanvasLayer/ColorRect.rect_size = $CanvasLayer/VBoxContainer.rect_size
	init_pos()
	load_image_modes()

func init_pos():
	$CanvasLayer.transform.origin= get_viewport().get_mouse_position()

func load_image_modes():
	var load_modes: Array = singleton.get_load_modes(level_container.cur_level_ind)
	image_mode_bga = load_modes[0]
	image_mode_bgb = load_modes[1]	
	$CanvasLayer/VBoxContainer/HBoxContainer/loadImageOptionBga.select(image_mode_bga)
	$CanvasLayer/VBoxContainer/HBoxContainer2/loadImageOptionBgb.select(image_mode_bgb)


func _on_DeleteLevelBtn_pressed():
	level_container.delete_self()
	yield(get_tree(), "idle_frame")
	yield(get_tree(), "idle_frame")
	var num: int = 0
	for levelContainer in get_tree().get_nodes_in_group("levelContainer"):
		levelContainer.cur_level_ind = num
		levelContainer.update_level_index()
		num += 1
	#get_tree().call_group("levelContainer", "update_level_ind")
	singleton.in_modal_window = false
	queue_free()



func _on_ChangeBgaBtn_pressed():
	layer_id = 0 # Now loading bga
	match(image_mode_bga):
		0: #map mode
			$CanvasLayer2/LoadBGFile.window_title = "Choose MAP for level"
			$CanvasLayer2/LoadBGFile.current_path = singleton.cur_project_folder_path + "/build/res/gfx/"
		1: #map mode (palette embeded) <- 16Tile like mode
			$CanvasLayer2/LoadBGFile.window_title = "Choose MAP for level (with palette for every tile)"
			$CanvasLayer2/LoadBGFile.current_path = singleton.cur_project_folder_path + "/build/res/gfx/"
		2: #image mode
			$CanvasLayer2/LoadBGFile.window_title = "Choose IMAGE for level"
			$CanvasLayer2/LoadBGFile.current_path = singleton.cur_project_folder_path + "/build/res/images/"
	$CanvasLayer2/LoadBGFile.popup_centered()


func _on_ChangeBgbBtn_pressed():
	layer_id = 1 # Now loading bgb
	match(image_mode_bgb):
		0: #map mode
			$CanvasLayer2/LoadBGFile.window_title = "Chose MAP for level"
			$CanvasLayer2/LoadBGFile.current_path = singleton.cur_project_folder_path + "/build/res/gfx/"
		1: #map mode (palette embeded) <- 16Tile like mode
			$CanvasLayer2/LoadBGFile.window_title = "Choose MAP for level (with palette for every tile)"
			$CanvasLayer2/LoadBGFile.current_path = singleton.cur_project_folder_path + "/build/res/gfx/"
		2: #image mode
			$CanvasLayer2/LoadBGFile.window_title = "Chose IMAGE for level"
			$CanvasLayer2/LoadBGFile.current_path = singleton.cur_project_folder_path + "/build/res/images/"
	$CanvasLayer2/LoadBGFile.popup_centered()
	print(singleton.cur_project_folder_path)


func _on_loadImageOptionBgb_item_selected(index):
	image_mode_bgb = index
	level_container.change_bgb_mode(image_mode_bgb)


func _on_loadImageOptionBga_item_selected(index):
	image_mode_bga = index
	level_container.change_bga_mode(image_mode_bga)


func _on_LoadBGFile_file_selected(path):
	var ext = path.get_extension();
	if(!(ext in ['png', 'bmp'])):
		return
	var localPath = path.replace(ProjectSettings.globalize_path("res://"), "./")
	var load_modes: Array = singleton.get_load_modes(level_container.cur_level_ind)
	print("load modes")
	print(load_modes)
	match layer_id:
		0: #bga
			level_container.change_bga(localPath, load_modes[0])
		1: #bgb
			level_container.change_bgb(localPath, load_modes[1])
	singleton.in_modal_window = false
	queue_free()
	


func _on_AddLevelBtn_pressed():
	var global_mouse_position = get_global_mouse_position()
	singleton.add_level(global_mouse_position)
	get_tree().call_group("tilemapEditorWindow", "load_last_level")
	singleton.in_modal_window = false
	queue_free()


func _on_DeleteBgaBtn_pressed():
	level_container.delete_bga()
	


func _on_DeleteBgbBtn_pressed():
	level_container.delete_bgb()


func _on_MessagePacksBtn_button_down():
	var node = msg_packs_menu_t.instance()
	node.level_ind = level_container.cur_level_ind
	leftContainer.add_child(node)
	singleton.in_modal_window = false
	queue_free()


func _on_LevelSettingsBtn_button_down():
	var node = level_settings_menu_t.instance()
	node.level_ind = level_container.cur_level_ind
	leftContainer.add_child(node)
	singleton.in_modal_window = false
	queue_free()


func _on_LocalVariablesBtn2_button_down():
	var node = local_vars_menu_t.instance()
	node.level_ind = level_container.cur_level_ind
	leftContainer.add_child(node)
	singleton.in_modal_window = false
	queue_free()


func _on_CopyLevelSettingsBtn_button_down():
	singleton.copy_level_settings(level_container.cur_level_ind)
	pass # Replace with function body.


func _on_PasteLevelSettingsBtn_button_down():
	get_tree().call_group("levelContainer", "paste_level_settings")
	#singleton.paste_level_settings(level_container.cur_level_ind)


func _on_CopyBgb_button_down():
	singleton.copy_bgb_settings(level_container.cur_level_ind)


func _on_PasteBgb_button_down():
	get_tree().call_group("levelContainer", "paste_bgb_settings")


func _on_CopyBga_button_down():
	singleton.copy_bga_settings(level_container.cur_level_ind)


func _on_PasteBga_button_down():
	get_tree().call_group("levelContainer", "paste_bga_settings")
