extends Area2D

#../../../../../

export var entity_name: String = "testEntity"
onready var ContainerRight: Control = $"../../../../../CanvasLayer/ContainerRight"
onready var VBoxContainerRight: VBoxContainer = ContainerRight.get_node("VBoxContainerRight")
onready var ContainerRightColorRect: ColorRect = $"../../../CanvasLayer/ContainerRight/ColorRect"
onready var mainController = $"../../../"
onready var camera2D = $"../../../../Camera2D"

onready var settings_field_t = preload("res://Scenes/Fields/string_field.tscn")
onready var settings_field_sprite_t = preload("res://Scenes/Fields/sprite_field.tscn")
onready var settings_field_aabb_t = preload("res://Scenes/Fields/aabb_field.tscn")
onready var settings_field_trigger_type_t = preload("res://Scenes/Fields/trigger_type_field.tscn")
onready var settings_head_t = preload("res://Scenes/Fields/settings_head.tscn")

var entityInst_id = -1
var level_ind = -1
var def_id = -1
var triggerAABB: Array = [0,0,0,0]
#var entity_definition: Dictionary = {
#	\"__identifier\":\"testIdentifierName\",
#	\"fieldInstances\": [],
#	}

var can_move = false
var is_moving = false
var fixed_toggle_point = Vector2.ZERO

var highlight = false

var sprite_size: Vector2

func _ready():
	pass # Replace with function body.
	#singleton.cur_entity_inst_ind = singleton.get_entityInst_ind_by_id(entityInst_id)
	
	#get_entityDef_by_defId
	var entityInst_info = singleton.get_entityInst_by_instInd_levelInd(entityInst_id, level_ind)
	#return
	triggerAABB = entityInst_info["triggerAABB"]
	if(entityInst_info.has("addTrigger")):
		$ColorRect.visible = entityInst_info["addTrigger"]
	#$Sprite.region_rect = Rect2(0, 0, 10,10)
	sprite_size = Vector2(8,8)
	#$Sprite.region_rect = Rect2(Vector2(0, 0), sprite_size)
	$CollisionShape2D.shape.extents = sprite_size/2
	$CollisionShape2D.position = Vector2(sprite_size.x/2, sprite_size.y/2)
	fixed_toggle_point = get_viewport().get_mouse_position()
	$ColorRect.rect_position = Vector2(triggerAABB[0], triggerAABB[1])
	$ColorRect.rect_size = Vector2(triggerAABB[2]-$ColorRect.rect_position.x, triggerAABB[3]-$ColorRect.rect_position.y)
	_draw()

func change_collision_rect(rect: Rect2):
	triggerAABB = [rect.position.x, rect.position.y, rect.size.x, rect.size.y]
	$ColorRect.rect_position = Vector2(triggerAABB[0], triggerAABB[1])
	$ColorRect.rect_size = Vector2(triggerAABB[2]-$ColorRect.rect_position.x, triggerAABB[3]-$ColorRect.rect_position.y)
	
	
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
	
func _draw():
	if(highlight):
		var color: Color = Color( 1, 0.84, 0, 1 )
		var line_thickness = 3
		var col_rect_pos: Vector2 = $CollisionShape2D.position
		#line_thickness = stepify(line_thickness*camera2D.zoom,0)
		var rect = Rect2(Vector2(-line_thickness,-line_thickness), Vector2(sprite_size.x+line_thickness+line_thickness,sprite_size.y+line_thickness+line_thickness))
		draw_rect(rect, color)
	
	
func _process(delta):
	if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY && !singleton.level_move_mode):
		move_entity()
		update()

func show_fields_of_entity():
	ContainerRight.visible = true
	var settings_head_node = settings_head_t.instance()
	var entity_definition = singleton.get_entityInst_by_instInd_levelInd(entityInst_id, level_ind)
	if(!entity_definition):
		return
	settings_head_node.get_node("Label").text = entity_definition["__identifier"]
	settings_head_node.get_node("InfoLabel").text = "level_Id = " + str(level_ind) + ";\nentityInst_Id = " + str(entityInst_id) + ";\ndef_Id = " + str(def_id) + ";"
	
	VBoxContainerRight.add_child(settings_head_node)
	for field_inst in entity_definition["fieldInstances"]:
		var settings_field_node
		if field_inst["__identifier"] == "Trigger type":
			settings_field_node = settings_field_trigger_type_t.instance()
			settings_field_node.level_ind = level_ind
			settings_field_node.entityInst_id = entityInst_id
			if(!entity_definition.has("triggerTypeName")):
				entity_definition["triggerTypeName"] = "0"
			settings_field_node.triggerType_name = entity_definition["triggerTypeName"]
			settings_field_node.get_node("Label").text = str(field_inst["__identifier"])
		elif field_inst["__type"] == "Sprite":
			settings_field_node = settings_field_sprite_t.instance()
			settings_field_node.level_ind = level_ind
			settings_field_node.entityInst_id = entityInst_id
			settings_field_node.get_node("FileDialog").current_path = singleton.cur_project_folder_path + "/build/res/sprites/"
			
			settings_field_node.get_node("HBoxContainer/Label").text = field_inst["__identifier"]
			settings_field_node.get_node("HBoxContainer/VBoxContainer/TextEdit").text = field_inst["__value"]
		elif field_inst["__type"] == "AABB":
			settings_field_node = settings_field_aabb_t.instance()
			settings_field_node.level_ind = level_ind
			settings_field_node.entityInst_id = entityInst_id
			settings_field_node.entity_scene = self
			settings_field_node.get_node("HBoxContainer/Label").text = field_inst["__identifier"]
			
			var val:String = field_inst["__value"]
			val = val.substr(1, val.find("}")-1)
			var info_arr:Array = val.split(",")
			
			settings_field_node.get_node("HBoxContainer/VBoxContainer/GridContainer/TextEdit").text = info_arr[0]
			settings_field_node.get_node("HBoxContainer/VBoxContainer/GridContainer/TextEdit2").text = info_arr[1]
			settings_field_node.get_node("HBoxContainer/VBoxContainer/GridContainer/TextEdit3").text = info_arr[2]
			settings_field_node.get_node("HBoxContainer/VBoxContainer/GridContainer/TextEdit4").text = info_arr[3]

		else:
			settings_field_node = settings_field_t.instance()
			settings_field_node.level_ind = level_ind
			settings_field_node.entityInst_id = entityInst_id
			
			settings_field_node.get_node("Label").text = field_inst["__identifier"]
			settings_field_node.get_node("LineEdit").text = field_inst["__value"]
			
		VBoxContainerRight.add_child(settings_field_node)
	
func remove_fields_of_entity():
	ContainerRight.visible = false
	var children = VBoxContainerRight.get_children()
	for n in children:
		VBoxContainerRight.remove_child(n)
		n.queue_free()

func move_entity():
	
	if(!can_move):
		highlight = false
		return
	highlight = _is_on_top()
	
	
	var ref = get_viewport().get_mouse_position()
	if(Input.is_action_just_pressed("mouse1") && highlight):
		singleton.cur_entity_instId = entityInst_id
		singleton.cur_entity_inst_ind = singleton.get_entityInst_ind_by_id(entityInst_id)
		remove_fields_of_entity()
		show_fields_of_entity()
		is_moving = true
		
		print("Found entity instInd: ", singleton.cur_entity_inst_ind)
		var entityInst_info = singleton.get_entityInst_by_instInd_levelInd(entityInst_id, level_ind)
		
		if !entityInst_info.has("addTrigger"):
			var entityDef = singleton.get_entityDef_by_defId(entityInst_info["defId"])
			entityInst_info["addTrigger"] = entityDef["addTrigger"]
			pass
	if(is_moving):
		global_position.x += (ref.x - fixed_toggle_point.x)*camera2D.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)*camera2D.zoom.y
		
	if(Input.is_action_just_released("mouse1")):
		is_moving = false
		if(singleton.entity_snap_to_grid):
			global_position.x = (round(global_position.x/8))*8
			global_position.y = (round(global_position.y/8))*8
	if(Input.is_action_pressed("mouse2") and highlight):
		remove_fields_of_entity()
		singleton.delete_entityInstance(entityInst_id, level_ind)
		queue_free()
		
		
	fixed_toggle_point = ref

func _is_on_top():
	for entity in get_tree().get_nodes_in_group("entity_hovered"):
		if entity.get_index() > get_index():
			return false
	return true

func _on_Area2D_mouse_entered():
	if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY):
		singleton.cur_entity_inst_ind = entityInst_id
		add_to_group("entity_hovered")
		can_move = true

func _on_Area2D_mouse_exited():
	if(!is_moving):
		if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY):
			remove_from_group("entity_hovered")
			can_move = false
			singleton.save_entityInst_pos(entityInst_id, [position.x, position.y])
