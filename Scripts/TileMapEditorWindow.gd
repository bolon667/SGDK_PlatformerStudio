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

var level_size

var map_size_px:Vector2
var map_size_tiles:Vector2


var levelContainer_t = preload("res://Scenes/levelContainer.tscn")
onready var VBoxContainerRight: VBoxContainer = $"../CanvasLayer/ContainerRight/VBoxContainerRight"
onready var ContainerRight: Control = $"../CanvasLayer/ContainerRight"
onready var world: Control = $World

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

func load_levels():
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
	print(window_size)
	$Area2D/CollisionShape2D.shape.extents = window_size
	load_levels()
	


func area2d_follow_camera():
	$Area2D.global_position = camera.global_position



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
	if(Input.is_action_just_released("wheel_up") && !singleton.in_modal_window):
		print("wheel_up")
		print("camera.zoom", camera.zoom)
		if(zoom == move_mode_trigger_zoom):
			singleton.level_move_mode = false
			get_tree().call_group("levelContainer", "move_level_off")
		zoom -= zoom_step
		
		
		if(zoom < 0.5):
			zoom = 0.5
		camera.zoom = Vector2(zoom, zoom)
		var window_size = get_viewport_rect().size
		$Area2D/CollisionShape2D.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
	if(Input.is_action_just_released("wheel_down")):
		zoom += zoom_step
		if(zoom > move_mode_trigger_zoom):
			zoom = move_mode_trigger_zoom
			if(!singleton.level_move_mode):
				print("level move mode activated")
				get_tree().call_group("levelContainer", "move_level_on")
				singleton.level_move_mode = true
		
		camera.zoom = Vector2(zoom, zoom)
		var window_size = get_viewport_rect().size
		$Area2D/CollisionShape2D.shape.extents = Vector2((window_size.x*zoom)-2, window_size.y*zoom)
		
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

func _on_Area2D_mouse_entered():
	print("area entered")
	focused_editor_window = true
	pass # Replace with function body.


func _on_Area2D_mouse_exited():
	print("area existed")
	focused_editor_window = false
