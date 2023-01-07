extends Node2D

export var camera_speed = 300
export var zoom_step = 0.1

var zoom = 1
var fixed_toggle_point = Vector2(0,0)

var can_move_mouse = false

var draw_tile_ind = 0
const erase_tile_ind = -1

var prev_cell_pos = Vector2.ZERO


# moves the map around just like in the editor
func move_map_around():
	var ref = get_viewport().get_mouse_position()
	$Camera2D.global_position.x -= (ref.x - fixed_toggle_point.x)*$Camera2D.zoom.x
	$Camera2D.global_position.y -= (ref.y - fixed_toggle_point.y)*$Camera2D.zoom.y
	fixed_toggle_point = ref


func _ready():
	pass # Replace with function body.
	var window_size = get_viewport_rect().size
	print(window_size)
	$Area2D/CollisionShape2D.shape.extents = window_size

func area2d_follow_camera():
	$Area2D.global_position = $Camera2D.global_position

func make_line_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int):
	var dx = pos1.x - pos0.x
	var dy = pos1.y - pos0.y
	var steps
	if (abs(dx) > abs(dy)):
		steps = abs(dx)
	else:
		steps = abs(dy)
	if(steps == 0):
		$TileMap.set_cell(pos1.x, pos1.y, tile_ind);
		return
	var x_increment = dx / steps;
	var y_increment = dy / steps;
	
	var v=0;
	var x = 0
	var y = 0
	while(v < steps):
		x = x + x_increment
		y = y + y_increment
		$TileMap.set_cell(pos0.x+x, pos0.y+y, tile_ind);
		v+=1
	

func move_camera(delta):
	var move_x = 0
	var move_y = 0
	var mouse_pos_local
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
		zoom -= zoom_step
		if(zoom < 0.5):
			zoom = 0.5
		$Camera2D.zoom = Vector2(zoom, zoom)
		var window_size = get_viewport_rect().size
		$Area2D/CollisionShape2D.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
	if(Input.is_action_just_released("wheel_down")):
		zoom += zoom_step
		$Camera2D.zoom = Vector2(zoom, zoom)
		var window_size = get_viewport_rect().size
		$Area2D/CollisionShape2D.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
		
	# This happens once 'move_map' is pressed
	if Input.is_action_just_pressed('wheel_btn'):
		var ref = get_viewport().get_mouse_position()
		fixed_toggle_point = ref
	# This happens while 'move_map' is pressed
	if Input.is_action_pressed('wheel_btn'):
		move_map_around()
	
	$Camera2D.position += Vector2(move_x*camera_speed*delta, move_y*camera_speed*delta)

func tile_map_handler(delta):
	var global_mouse_pos = get_global_mouse_position()
	var bg_sprite_size = $BGSprite.texture.get_size()
	if(global_mouse_pos.x < 0 or global_mouse_pos.y < 0):
		return
	if(global_mouse_pos.x > bg_sprite_size.x or global_mouse_pos.y > bg_sprite_size.y):
		return
	var cell_pos = $TileMap.world_to_map(global_mouse_pos)
	if(Input.is_action_pressed("mouse1")):
		make_line_tileMap(prev_cell_pos, cell_pos, draw_tile_ind)
	if(Input.is_action_pressed("mouse2")):
		make_line_tileMap(prev_cell_pos, cell_pos, erase_tile_ind)
	prev_cell_pos = cell_pos

func _process(delta):
	area2d_follow_camera()
	if(can_move_mouse):
		move_camera(delta)
		tile_map_handler(delta)
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_Area2D_mouse_entered():
	pass # Replace with function body.
	can_move_mouse = true


func _on_Area2D_mouse_exited():
	pass # Replace with function body.
	can_move_mouse = false
