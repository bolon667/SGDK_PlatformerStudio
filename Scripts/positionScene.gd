extends Area2D

onready var mainController = $"../../../../"
onready var camera2D = $"../../../../Camera2D"

var entityInst_id = -1
var cur_index: int

var can_move = false
var is_moving = false
var fixed_toggle_point = Vector2.ZERO

var highlight = false

var sprite_size: Vector2 = Vector2(32,32)

func _ready():
	pass # Replace with function body.
	cur_index = get_index()
	var entityInst_info = singleton.get_positionInst_by_instId(entityInst_id)
	fixed_toggle_point = get_viewport().get_mouse_position()
	updateCurMessage()
	_draw()

func updateCurMessage():
	$posNum.text = str(get_index())
	
func _draw():
	update()
	if(highlight):
		var color = Color(0.5,0.5,0)
		var line_thickness = 3
		var col_rect_pos: Vector2 = $CollisionShape2D.position
		#line_thickness = stepify(line_thickness*camera2D.zoom,0)
		var rect = Rect2(Vector2(-16,-16), Vector2(sprite_size.x,sprite_size.y))
		draw_rect(rect, color)
	
	
	

func _process(delta):
	pass
	if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
		move_entity()
		_draw()


func move_entity():
	if(!can_move):
		return
	
	var ref = get_viewport().get_mouse_position()
	if(Input.is_action_just_pressed("mouse1")):
		is_moving = true
		singleton.cur_entity_inst_ind = entityInst_id
		print("cur entity inst id: ", singleton.cur_entity_inst_ind)
	if(is_moving):
		global_position.x += (ref.x - fixed_toggle_point.x)*camera2D.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)*camera2D.zoom.y
	if(Input.is_action_just_released("mouse1")):
		is_moving = false
	if(Input.is_action_pressed("mouse2") and highlight):
		singleton.delete_entityInstance(entityInst_id)
		singleton.can_create_entity_obj = true
		var children = get_parent().get_children()

		queue_free()

		
		
	fixed_toggle_point = ref


func _on_Area2D_mouse_entered():
	if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
		updateCurMessage()
		can_move = true
		highlight = true
		singleton.can_create_entity_obj = false

func _on_Area2D_mouse_exited():
	if(!is_moving):
		if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
			highlight = false
			can_move = false
			singleton.save_entityInst_pos(entityInst_id, [position.x, position.y])
			singleton.can_create_entity_obj = true
