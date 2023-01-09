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

var local_mouse_pos

onready var temp_tile_map = $BGSprite/tempTileMap
onready var tile_map = $BGSprite/TileMap
onready var entity_obj_list = $EntityList

onready var camera = get_parent().get_node("Camera2D")
#onready var camera = $Camera2D

onready var entity_obj_t = preload("res://Scenes/entityScene.tscn")


# moves the map around just like in the editor
func move_map_around():
	var ref = get_viewport().get_mouse_position()
	camera.global_position.x -= (ref.x - fixed_toggle_point.x)*camera.zoom.x
	camera.global_position.y -= (ref.y - fixed_toggle_point.y)*camera.zoom.y
	fixed_toggle_point = ref


func _ready():
	var window_size = get_viewport_rect().size
	print(window_size)
	$Area2D/CollisionShape2D.shape.extents = window_size

func area2d_follow_camera():
	$Area2D.global_position = camera.global_position

func make_line_temp_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int):
	var line_positions = []
	var dx = pos1.x - pos0.x
	var dy = pos1.y - pos0.y
	var steps
	if (abs(dx) > abs(dy)):
		steps = abs(dx)
	else:
		steps = abs(dy)
	if(steps == 0):
		temp_tile_map.set_cell(pos1.x, pos1.y, tile_ind);
		return
	var x_increment = dx / steps;
	var y_increment = dy / steps;
	
	var v=0;
	var x = 0
	var y = 0
	while(v < steps):
		x = x + x_increment
		y = y + y_increment
		temp_tile_map.set_cell(pos0.x+x, pos0.y+y, tile_ind);
		line_positions.append({"x": pos0.x+x,"y": pos0.y+y})
		v+=1
	return 

func make_rect_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int, tileMap: TileMap):
	var startPosX: int
	var startPosY: int
	var curPosX: int
	var curPosY: int
	var lastPosY: int
	var lastPosX: int
	if(pos0.x < pos1.x):
		startPosX = pos0.x
		lastPosX = pos1.x
	else:
		startPosX = pos1.x
		lastPosX = pos0.x
	
	if(pos0.y < pos1.y):
		startPosY = pos0.y
		lastPosY = pos1.y
	else:
		startPosY = pos1.y
		lastPosY = pos0.y
		
	curPosX = startPosX
	curPosY = startPosY
	while(curPosY <= lastPosY):
		while(curPosX <= lastPosX):
			tileMap.set_cell(curPosX, curPosY, tile_ind)
			curPosX += 1
		curPosY+=1
		curPosX = startPosX

func make_line_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int):
	var dx = pos1.x - pos0.x
	var dy = pos1.y - pos0.y
	var steps
	if (abs(dx) > abs(dy)):
		steps = abs(dx)
	else:
		steps = abs(dy)
	if(steps == 0):
		tile_map.set_cell(pos1.x, pos1.y, tile_ind);
		return
	var x_increment = dx / steps;
	var y_increment = dy / steps;
	
	var v=0;
	var x = 0
	var y = 0
	while(v < steps):
		x = x + x_increment
		y = y + y_increment
		tile_map.set_cell(pos0.x+x, pos0.y+y, tile_ind);
		v+=1
	

func move_camera(delta):
	var move_x = 0
	var move_y = 0
	if(Input.is_action_pressed("ui_right")):
		move_x += 1
		print("move right")
	if(Input.is_action_pressed("ui_left")):
		move_x -= 1
	if(Input.is_action_pressed("ui_down")):
		move_y += 1
	if(Input.is_action_pressed("ui_up")):
		move_y -= 1
	if(Input.is_action_just_released("wheel_up")):
		print("wheel_up")
		print("camera.zoom", camera.zoom)
		zoom -= zoom_step
		if(zoom < 0.5):
			zoom = 0.5
		camera.zoom = Vector2(zoom, zoom)
		var window_size = get_viewport_rect().size
		$Area2D/CollisionShape2D.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
	if(Input.is_action_just_released("wheel_down")):
		zoom += zoom_step
		camera.zoom = Vector2(zoom, zoom)
		var window_size = get_viewport_rect().size
		$Area2D/CollisionShape2D.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
		
	# This happens once 'move_map' is pressed
	if Input.is_action_just_pressed('wheel_btn'):
		var ref = get_viewport().get_mouse_position()
		fixed_toggle_point = ref
	# This happens while 'move_map' is pressed
	if Input.is_action_pressed('wheel_btn'):
		move_map_around()
	
	#camera.position += Vector2(move_x*camera_speed*delta, move_y*camera_speed*delta)

func tile_map_handler(delta):
	
	var bg_sprite_size = $BGSprite.texture.get_size()
	if(local_mouse_pos.x < 0 or local_mouse_pos.y < 0):
		return
	if(local_mouse_pos.x > bg_sprite_size.x or local_mouse_pos.y > bg_sprite_size.y):
		return
	cell_pos = tile_map.world_to_map(local_mouse_pos)
	#rect drawing mode
	if(Input.is_action_pressed("shift_key")):
		
		if(Input.is_action_just_pressed("mouse1")):
			prev_cell_pos = cell_pos
		if(Input.is_action_just_released("mouse1")):
			#draw rect
			print("draw rect")
			make_rect_tileMap(prev_cell_pos, cell_pos, draw_tile_ind, tile_map)
		if(Input.is_action_just_pressed("mouse2")):
			prev_cell_pos = cell_pos
		if(Input.is_action_just_released("mouse2")):
			#erase rect
			print("erase rect")
			make_rect_tileMap(prev_cell_pos, cell_pos, erase_tile_ind, tile_map)
	else: #standart point mode
		if(Input.is_action_pressed("mouse1")):
			make_line_tileMap(prev_cell_pos, cell_pos, draw_tile_ind)
		if(Input.is_action_pressed("mouse2")):
			make_line_tileMap(prev_cell_pos, cell_pos, erase_tile_ind)
		prev_cell_pos = cell_pos
	
	
	

func clean_temp_tileMap():
	var used_tile_positions = temp_tile_map.get_used_cells()
	for tile_pos in used_tile_positions:
		temp_tile_map.set_cell(tile_pos[0], tile_pos[1], -1)

func temp_tile_map_handler(delta):
	clean_temp_tileMap()

	var bg_sprite_size = $BGSprite.texture.get_size()
	if(local_mouse_pos.x < 0 or local_mouse_pos.y < 0):
		return
	if(local_mouse_pos.x > bg_sprite_size.x or local_mouse_pos.y > bg_sprite_size.y):
		return
	#var cell_pos = $TileMap.world_to_map(global_mouse_pos)
	#rect drawing mode
	if(Input.is_action_pressed("shift_key")):
		if(Input.is_action_just_pressed("mouse1")):
			prev_cell_pos_temp = cell_pos
		elif(Input.is_action_pressed("mouse1")):
			#draw fake rect
			make_rect_tileMap(prev_cell_pos_temp, cell_pos, draw_tile_ind, temp_tile_map)
		else:
			prev_cell_pos_temp = cell_pos
			make_line_temp_tileMap(prev_cell_pos_temp, cell_pos, draw_tile_ind)
		if(Input.is_action_just_pressed("mouse2")):
			if(Input.is_action_pressed("mouse2")):
				make_line_temp_tileMap(prev_cell_pos_temp, cell_pos, draw_tile_ind)
		
	else: #standart point mode
		make_line_temp_tileMap(prev_cell_pos_temp, cell_pos, draw_tile_ind)
		prev_cell_pos_temp = cell_pos

func delete_all_highlighted_entity_objs():
	var children = entity_obj_list.get_children()
	for child in children:
		if child.highlight == true:
			child.queue_free()

func entity_list_handler():
	var entity_obj_node = entity_obj_t.instance()
	if(Input.is_action_just_pressed("mouse1") and singleton.can_create_entity_obj and singleton.cur_entity_type_ind != -1):
		entity_obj_node.global_position = get_global_mouse_position()
		entity_obj_list.add_child(entity_obj_node)
	if(Input.is_action_pressed("mouse2")):
		delete_all_highlighted_entity_objs()
		singleton.can_create_entity_obj = true
	
	

func _process(delta):
	area2d_follow_camera()
	if(focused_editor_window):
		local_mouse_pos = $BGSprite.get_local_mouse_position()
		move_camera(delta)
		entity_list_handler()
		temp_tile_map_handler(delta)
		tile_map_handler(delta)


func _on_Area2D_mouse_entered():
	print("area entered")
	focused_editor_window = true
	pass # Replace with function body.


func _on_Area2D_mouse_exited():
	print("area existed")
	clean_temp_tileMap()
	focused_editor_window = false
