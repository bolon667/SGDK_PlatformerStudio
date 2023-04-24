extends Area2D

onready var mainController = $"../../../../"
var camera2D

var entityInst_id = -1
var defId = -1
var masterDefId = -1
var level_ind = 0
var cur_index: int

var can_move = false
var is_moving = false
var fixed_toggle_point = Vector2.ZERO

var highlight = false

var sprite_size: Vector2 = Vector2(32,32)

func _ready():
	pass # Replace with function body.
	getMainCamera()
	cur_index = get_index()
	fixed_toggle_point = get_viewport().get_mouse_position()
	updateCurMessage()
	_draw()

func getMainCamera():
	camera2D = get_tree().get_nodes_in_group("mainCamera")[0]
	

func change_sprite_rect(rect: Rect2):
	$Sprite.region_rect = rect
	sprite_size = rect.size
	var draggableShape: Vector2 = Vector2(8,8)
	if(sprite_size.x > 8):
		draggableShape.x = sprite_size.x/2
	if(sprite_size.y > 8):
		draggableShape.y = sprite_size.y/2
	$CollisionShape2D.shape.extents = draggableShape
	$CollisionShape2D.position = Vector2(sprite_size.x/2, sprite_size.y/2)

func updateCurMessage():
	yield(get_tree(), "idle_frame")
	$posNum.text = str(get_index())
	
func _draw():
	if(can_move):
		var color: Color = Color( 1, 0.84, 0, 1 )
		var line_thickness = 3
		var col_rect_pos: Vector2 = $CollisionShape2D.position
		#line_thickness = stepify(line_thickness*camera2D.zoom,0)
		var rect = Rect2(Vector2(-line_thickness,-line_thickness), Vector2(sprite_size.x+line_thickness+line_thickness,sprite_size.y+line_thickness+line_thickness))
		draw_rect(rect, color)
	
	
	

func _process(delta):
	move_entity()
	update()


func _is_on_top():
	for entity in get_tree().get_nodes_in_group("entity_def_hovered"):
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
		is_moving = true
		singleton.cur_entity_inst_ind = entityInst_id
		print("cur entity inst id: ", singleton.cur_entity_inst_ind)
	if(is_moving):
		global_position.x += (ref.x - fixed_toggle_point.x)#*camera2D.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)#*camera2D.zoom.y
	if(Input.is_action_just_released("mouse1")):
		is_moving = false
	if(Input.is_action_pressed("mouse2") and highlight):
		pass
		#singleton.save_entitySlaveInst_pos(masterDefId, entityInst_id, [position.x, position.y])
		var entity_inst = singleton.delete_entitySlave_by_instId(masterDefId, entityInst_id);
		#singleton.delete_entityInstance(entityInst_id, level_ind)
		#for pos in get_parent().get_children():
		#	pos.updateCurMessage()

		queue_free()

		
		
	fixed_toggle_point = ref


func _on_Area2D_mouse_entered():
	add_to_group("entity_def_hovered")
	updateCurMessage()
	can_move = true

func _on_Area2D_mouse_exited():
	if(!is_moving):
		remove_from_group("entity_def_hovered")
		can_move = false
		print("Pre check")
		print("masterDefId: ", masterDefId, ", defId: ", defId)
		singleton.save_entitySlaveInst_pos(masterDefId, entityInst_id, [position.x, position.y])

