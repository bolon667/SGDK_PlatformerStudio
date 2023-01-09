extends Area2D

export var entity_name: String = "testEntity"
onready var VBoxContainerLeft: VBoxContainer = $"../../../CanvasLayer/VBoxContainerLeft"
onready var mainController = $"../../../"
onready var settings_nodes = preload("res://Scenes/Fields/string_field.tscn")
onready var camera2D = $"../../../Camera2D"

	
export var entity_fields: Dictionary = {"spriteDefinition": "&spr_test"}

var can_move = false
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
	move_entity()
	_draw()

func show_settings_of_entity():
	var settings_node = settings_nodes.instance()
	for key in entity_fields:
		var value = entity_fields[key] 
		print(key)
		print(value)
		settings_node.get_node("Label").text = str(key)
		settings_node.get_node("LineEdit").text = str(value)
	VBoxContainerLeft.add_child(settings_node)
	
func remove_settings_of_entity():
	var children = VBoxContainerLeft.get_children()
	for n in children:
		VBoxContainerLeft.remove_child(n)
		n.queue_free()

func move_entity():
	if(!can_move):
		
		return
	
	var ref = get_viewport().get_mouse_position()
	if(Input.is_action_pressed("mouse1")):
		
		global_position.x += (ref.x - fixed_toggle_point.x)*camera2D.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)*camera2D.zoom.y
	fixed_toggle_point = ref


func _on_Area2D_mouse_entered():
	remove_settings_of_entity()
	show_settings_of_entity()
	can_move = true
	highlight = true
	singleton.can_create_entity_obj = false

func _on_Area2D_mouse_exited():
	remove_settings_of_entity()
	highlight = false
	can_move = false
	singleton.can_create_entity_obj = true
