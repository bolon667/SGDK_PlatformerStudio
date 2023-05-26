extends Node2D

export var camera_speed = 300
export var zoom_step = 0.1

var zoom = 1
var fixed_toggle_point = Vector2(0,0)

var focused_editor_window = false

var draw_tile_ind = 0
const erase_tile_ind = -1

var cell_pos = Vector2.ZERO
var prev_cell_pos = Vector2.ZERO
var prev_cell_pos_temp = Vector2.ZERO

var drawing_rect_mode: bool = false

var rect_pos_1:Vector2 = Vector2.ZERO
var rect_pos_2:Vector2 = Vector2.ZERO


var local_mouse_pos

var level_size

var map_size_px:Vector2
var map_size_tiles:Vector2

var levelContainer_t = preload("res://Scenes/levelContainer.tscn")
onready var VBoxContainerRight: VBoxContainer = $"../CanvasLayer/ContainerRight/ScrollContainer/VBoxContainerRight"
onready var ContainerRight: Control = $"../CanvasLayer/ContainerRight"
onready var world: Control = $World
onready var collisionShape = $Area2D/CollisionShape2D
onready var colorRect = $ColorRect

#onready var camera = get_parent().get_node("Camera2D")
onready var camera = $Camera2D


func remove_fields_of_entity():
	ContainerRight.visible = false
	var children = VBoxContainerRight.get_children()
	for n in children:
		VBoxContainerRight.remove_child(n)
		n.queue_free()

func clean_levels():
	print("clear levels")
	for levelContainer in world.get_children():
		levelContainer.queue_free()

func list_files_in_directory_png(path):
	var files = []
	var dir = Directory.new()
	dir.open(path)
	dir.list_dir_begin()

	while true:
		var file = dir.get_next()
		if file == "":
			break
		elif not file.begins_with("."):
			if file.ends_with(".png"):
				files.append(file)

	dir.list_dir_end()

	return files

func generate_tilesets():
	var baseBath = singleton.cur_project_folder_path + "/code_template/collisionMapTypes/"
	if(singleton.cell_size == 8):
		var ts = TileSet.new()
		var filePaths = list_files_in_directory_png(baseBath + "/8x8/")
		for filePath in filePaths:
			print(filePath)
			var id = int(filePath)-1
			#var id = ts.get_last_unused_tile_id()
			ts.create_tile(id)
			ts.tile_set_name(id, str(id))
			
			var texture = ImageTexture.new()
			var image = Image.new()
			image.load(baseBath + "/8x8/" + filePath)
			texture.create_from_image(image)
			
			ts.tile_set_texture(id, texture)
		ResourceSaver.save("res://TileSets/8x8TileSet.tres", ts)
	if(singleton.cell_size == 16):
		var ts = TileSet.new()
		var filePaths = list_files_in_directory_png(baseBath + "/16x16/")
		for filePath in filePaths:
			print(filePath)
			var id = int(filePath)-1
			#var id = ts.get_last_unused_tile_id()
			ts.create_tile(id)
			ts.tile_set_name(id, str(id))
			
			var texture = ImageTexture.new()
			var image = Image.new()
			image.load(baseBath + "/16x16/" + filePath)
			texture.create_from_image(image)
			
			ts.tile_set_texture(id, texture)
		ResourceSaver.save("res://TileSets/16x16TileSet.tres", ts)

func load_levels():
	generate_tilesets()
	clean_levels()
	print("load levels")
	singleton.cur_entity_inst_ind = 0
	var amount_of_levels: int = singleton.get_amount_of_levels()
	var level_ind: int = 0
	var word_pos_x = 0
	while (level_ind < amount_of_levels):
		print("Adding levelInd ", level_ind, " to world")
		var levelContainer_node = levelContainer_t.instance()
		levelContainer_node.cur_level_ind = level_ind
		world.add_child(levelContainer_node)
		level_ind += 1

#func save_world():
	
	#for level_container in $World.get_children():
	#	level_container.call("save_level")

# moves the map around just like in the editor
func move_map_around():
	var ref = get_viewport().get_mouse_position()
	camera.global_position.x -= (ref.x - fixed_toggle_point.x)*camera.zoom.x
	camera.global_position.y -= (ref.y - fixed_toggle_point.y)*camera.zoom.y
	fixed_toggle_point = ref

func _ready():
	var window_size = get_viewport_rect().size
	get_tree().get_root().connect("size_changed", self, "_on_size_changed")
	print(window_size)
	collisionShape.shape.extents = window_size
	load_levels()

func _on_size_changed():
	collisionShape.shape.extents = get_viewport_rect().size

func area2d_follow_camera():
	$Area2D.global_position = camera.global_position

func _draw():
	pass

func move_camera(delta):
	var move_x = 0
	var move_y = 0
	var move_mode_trigger_zoom = 1.5
	if(Input.is_action_pressed("ui_right")):
		move_x += 1
		print("move right")
	if(Input.is_action_pressed("ui_left")):
		move_x -= 1
	if(Input.is_action_pressed("ui_down")):
		move_y += 1
	if(Input.is_action_pressed("ui_up")):
		move_y -= 1
	if(Input.is_action_just_released("wheel_up") && !singleton.in_modal_window && focused_editor_window):
		print("wheel_up")
		print("camera.zoom", camera.zoom)
		var window_size = get_viewport_rect().size
		if(zoom == move_mode_trigger_zoom):
			singleton.level_move_mode = false
			get_tree().call_group("levelContainer", "move_level_off")
		zoom -= zoom_step
		
		if(zoom < 0.5):
			zoom = 0.5
		else:
			
			#var screen_center = window_size / 2
			#var screen_center_global = camera.global_position
			#var local_mouse_pos = get_viewport().get_mouse_position()
			
			#var newCameraPos:Vector2
			#newCameraPos = camera.global_position + local_mouse_pos - screen_center
			#newCameraPos.x *= camera.zoom.x
			#newCameraPos.y *= camera.zoom.y
			
			#camera.global_position = newCameraPos
			camera.zoom = Vector2(zoom, zoom)
		
		
		
		collisionShape.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
	if(Input.is_action_just_released("wheel_down") && focused_editor_window):
		zoom += zoom_step
		var window_size = get_viewport_rect().size
		if(zoom > move_mode_trigger_zoom):
			zoom = move_mode_trigger_zoom
			if(!singleton.level_move_mode):
				print("level move mode activated")
				get_tree().call_group("levelContainer", "move_level_on")
				singleton.level_move_mode = true
		else:
			
			camera.zoom = Vector2(zoom, zoom)
		
		
		collisionShape.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
	
	if(singleton.can_move_map):
		# This happens once 'move_map' is pressed
		if Input.is_action_just_pressed('wheel_btn'):
			var ref = get_viewport().get_mouse_position()
			singleton.in_modal_window = false
			get_tree().call_group("tempWindow", "queue_free")
			fixed_toggle_point = ref
		# This happens while 'move_map' is pressed
		if Input.is_action_pressed('wheel_btn'):
			move_map_around()
	
	#camera.position += Vector2(move_x*camera_speed*delta, move_y*camera_speed*delta)

func _physics_process(delta):
	move_camera(delta)
	area2d_follow_camera()
	select_rect()


func select_levelContainers_in_rect(rect_pos_1:Vector2, rect_pos_2:Vector2):
	print("run select_levelContainers_in_rect(...)")
	for levelContainer in world.get_children():
		if not levelContainer.is_in_group("leftContainer"):
			continue
		var level_size:Vector2 = levelContainer.map_size_px
		var level_pos:Vector2 = levelContainer.global_position
		
		#Swap positions if necessary
		if rect_pos_1.x > rect_pos_2.x:
			var temp_x = rect_pos_2.x
			rect_pos_2.x = rect_pos_1.x
			rect_pos_1.x = temp_x
		if rect_pos_1.y > rect_pos_2.y:
			var temp_y = rect_pos_2.y
			rect_pos_2.y = rect_pos_1.y
			rect_pos_1.y = temp_y
			
		if((rect_pos_1.x < (level_pos.x+level_size.x)) and (rect_pos_2.x > level_pos.x)):
			if((rect_pos_1.y < (level_pos.y+level_size.y)) and (rect_pos_2.y > level_pos.y)):
				print(levelContainer.cur_level_ind)
				levelContainer.make_selected()

func deselect_levelContainers():
	#get_tree().call_group("connectedLevelContainers", "make_deselected")
	#Efficient way was too buggy, not worth it
	get_tree().call_group("levelContainer", "make_deselected")
	
	#world.add

func is_mouse_on_levelContainer():
	return len(get_tree().get_nodes_in_group("level_hovered")) != 0

func select_rect():
	if singleton.level_move_mode == false:
		return
	#if singleton.cur_editor_mode == singleton.EditorMode.NONE:
	#	return
	if (not drawing_rect_mode) and is_mouse_on_levelContainer():
		return
		
	if Input.is_action_just_pressed("mouse1"):
		colorRect.visible = true
		drawing_rect_mode = true
		deselect_levelContainers()
		
		rect_pos_1 = get_local_mouse_position()
		
	if Input.is_action_pressed("mouse1"):
		rect_pos_2 = get_local_mouse_position()
		
	if Input.is_action_just_released("mouse1"):
		colorRect.visible = false
		drawing_rect_mode = false
		
		
		select_levelContainers_in_rect(rect_pos_1, rect_pos_2)
	
	if rect_pos_1.x < rect_pos_2.x:
		colorRect.rect_position.x = rect_pos_1.x
		colorRect.rect_size.x = rect_pos_2.x-rect_pos_1.x
	else:
		colorRect.rect_position.x = rect_pos_2.x
		colorRect.rect_size.x = rect_pos_1.x - rect_pos_2.x
	
	if rect_pos_1.y < rect_pos_2.y:
		colorRect.rect_position.y = rect_pos_1.y
		colorRect.rect_size.y = rect_pos_2.y-rect_pos_1.y
	else:
		colorRect.rect_position.y = rect_pos_2.y
		colorRect.rect_size.y = rect_pos_1.y - rect_pos_2.y
		
	
func _on_Area2D_mouse_entered():
	print("area entered")
	focused_editor_window = true
	pass # Replace with function body.

func _on_Area2D_mouse_exited():
	print("area existed")
	focused_editor_window = false
