extends Area2D

export var entity_name: String = "testEntity"
onready var ContainerRight: Control = $"../../../CanvasLayer/ContainerRight"
onready var VBoxContainerRight: VBoxContainer = $"../../../CanvasLayer/ContainerRight/VBoxContainerRight"
onready var ContainerRightColorRect: ColorRect = $"../../../CanvasLayer/ContainerRight/ColorRect"
onready var mainController = $"../../../"
onready var settings_field_t = preload("res://Scenes/Fields/string_field.tscn")
onready var settings_head_t = preload("res://Scenes/Fields/settings_head.tscn")
onready var camera2D = $"../../../Camera2D"

var entityInst_id = -1
#var entity_definition: Dictionary = {
#	"__identifier":"testIdentifierName",
#	"fieldInstances": [],
#	}

var can_move = false
var is_moving = false
var fixed_toggle_point = Vector2.ZERO

var highlight = false

func _ready():
	pass # Replace with function body.
	
	print("chheck ",$CollisionShape2D.shape.extents)
	print($Sprite.texture.get_size()/2)
	$CollisionShape2D.shape.extents = $Sprite.texture.get_size()/2
	
	fixed_toggle_point = get_viewport().get_mouse_position()
	_draw()
	
	
func _draw():
	update()
	if(highlight):
		var sprite_size = $Sprite.texture.get_size()
		var color = Color(0.5,0.5,0)
		var line_thickness = 3
		#line_thickness = stepify(line_thickness*camera2D.zoom,0)
		var rect = Rect2(Vector2((-sprite_size.x/2)-line_thickness,(-sprite_size.y/2)-line_thickness), Vector2(sprite_size.x+line_thickness+line_thickness,sprite_size.y+line_thickness+line_thickness))
		draw_rect(rect, color)
	
	
	

func _process(delta):
	pass
	if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY):
		move_entity()
		_draw()

func show_fields_of_entity():
	ContainerRight.visible = true
	var settings_head_node = settings_head_t.instance()
	var entity_definition = singleton.get_entityInst_by_instId(entityInst_id)

	settings_head_node.get_node("Label").text = entity_definition["__identifier"]
	
	VBoxContainerRight.add_child(settings_head_node)
	for field_inst in entity_definition["fieldInstances"]:
		var settings_field_node = settings_field_t.instance()
		
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
		return
	
	var ref = get_viewport().get_mouse_position()
	if(Input.is_action_just_pressed("mouse1")):
		is_moving = true
	if(is_moving):
		global_position.x += (ref.x - fixed_toggle_point.x)*camera2D.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)*camera2D.zoom.y
	if(Input.is_action_just_released("mouse1")):
		is_moving = false
	fixed_toggle_point = ref


func _on_Area2D_mouse_entered():
	if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY):
		remove_fields_of_entity()
		show_fields_of_entity()
		can_move = true
		highlight = true
		singleton.can_create_entity_obj = false

func _on_Area2D_mouse_exited():
	if(!is_moving):
		if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY):
			remove_fields_of_entity()
			highlight = false
			can_move = false
			singleton.save_entityInst_pos(entityInst_id, [position.x, position.y])
			singleton.can_create_entity_obj = true
