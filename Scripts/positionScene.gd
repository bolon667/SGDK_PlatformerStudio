extends Area2D

onready var mainController = $"../../../../"
onready var camera2D = $"../../../../Camera2D"

var entityInst_id = -1
var level_ind = 0
var cur_index: int

var can_move = false
var is_moving = false
var fixed_toggle_point = Vector2.ZERO

var highlight = false

var sprite_size: Vector2 = Vector2(32,32)

var second_pos_obj_arr = []
var second_pos_obj_arr_ids = []


func print_test():
	print(second_pos_obj_arr)
	print(second_pos_obj_arr_ids)

func clean_second_pos_massive(check_obj):
	for ind in len(second_pos_obj_arr):
		if check_obj == second_pos_obj_arr[ind]:
			second_pos_obj_arr[ind] = null

func update_second_pos_obj(new_obj):
	if not(new_obj in second_pos_obj_arr):
		if new_obj != self:
			second_pos_obj_arr.append(new_obj)
	

func _ready():
	pass # Replace with function body.
	cur_index = get_index()
	var entityInst_info = singleton.get_positionInst_by_instId(entityInst_id, level_ind)
	fixed_toggle_point = get_viewport().get_mouse_position()
	updateCurMessage()
	update()

func updateCurMessage():
	yield(get_tree(), "idle_frame")
	$posNum.text = str(get_index())


func _draw():
	for second_pos_obj in second_pos_obj_arr:
		if (second_pos_obj):
			#Draw arrow
			var color: Color = Color( 1, 0, 0, 0.75)
			var to_pos_local: Vector2 = second_pos_obj.position - position
			draw_line(Vector2.ZERO, to_pos_local, color, 5)
			var arrow_part = to_pos_local.normalized()*30
			draw_line(to_pos_local, to_pos_local+arrow_part.rotated(deg2rad(190)), color, 5)
			draw_line(to_pos_local, to_pos_local+arrow_part.rotated(deg2rad(170)), color, 5)
			
			
	if(highlight):
		var color: Color = Color( 1, 0.84, 0, 1 )
		var line_thickness = 3
		var col_rect_pos: Vector2 = $CollisionShape2D.position
		#line_thickness = stepify(line_thickness*camera2D.zoom,0)
		var rect = Rect2(Vector2(-16,-16), Vector2(sprite_size.x,sprite_size.y))
		draw_rect(rect, color)
		
		
	
	
	

func _process(delta):
	if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
		update()
		move_entity()
		


func _is_on_top():
	for entity in get_tree().get_nodes_in_group("position_hovered"):
		if entity.get_index() > get_index():
			return false
	return true

func move_entity():
	if(!can_move):
		highlight = false
		return
	highlight = _is_on_top()
	var ref = get_viewport().get_mouse_position()
	if(Input.is_action_just_pressed("mouse1") and highlight):
		if(Input.is_action_pressed("shift_key")):
			singleton.update_first_pos_obj(self)
			print("Making a line")
		is_moving = true
		singleton.cur_entity_inst_ind = entityInst_id
		print("cur pos inst id: ", singleton.cur_entity_inst_ind, " from level_ind: " + str(level_ind))
	if(Input.is_action_pressed("shift_key")):
		is_moving = false
	if(is_moving):
		global_position.x += (ref.x - fixed_toggle_point.x)*camera2D.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)*camera2D.zoom.y
	
		
	#if(Input.is_action_just_released("shift_key")):
	
	if(Input.is_action_just_released("mouse1")):
		is_moving = false
		if(singleton.first_pos_obj):
			singleton.first_pos_obj.update_second_pos_obj(self)
		
		
		#$Line2D.points.set(0, Vector2(5,5))
		singleton.first_pos_obj = null
		
		#get_tree().call_group("positionScene", "print_test")
		
	#	singleton.first_pos_obj = null
	if(Input.is_action_pressed("mouse2") and highlight):
		singleton.delete_entityInstance(entityInst_id, level_ind)
		for pos in get_parent().get_children():
			pos.updateCurMessage()
		get_tree().call_group("positionScene", "clean_second_pos_massive", self)
		#first_pos_obj.second_pos_obj = null
		queue_free()

	
		
	fixed_toggle_point = ref


func _on_Area2D_mouse_entered():
	if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
		add_to_group("position_hovered")
		updateCurMessage()
		can_move = true

func _on_Area2D_mouse_exited():
	if(!is_moving):
		if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
			remove_from_group("position_hovered")
			#singleton.save_entityInst_pos(self, singleton.first_pos_obj)
			can_move = false
