extends Control

onready var layer_list = $VBoxContainer/LayerList
onready var layer_value_list = $VBoxContainer/LayerValuesList/LayerValuesContainer

onready var entity_menu_t = preload("res://Scenes/Pages/EntityMenu.tscn")
onready var layer_button_t = preload("res://Scenes/LayerButton.tscn")

onready var entity_layer_value_btn_t = preload("res://Scenes/EntityLayerValueButton.tscn")


# Called when the node enters the scene tree for the first time.
func _ready():
	var layers = singleton.get_cur_level_layers()
	for layer in layers:
		var layer_button_node = layer_button_t.instance()
		layer_button_node.text = layer["__identifier"]
		layer_button_node.type = layer["__type"]
		layer_list.add_child(layer_button_node)

func clear_layer_values():
	var children = layer_value_list.get_children()
	for child in children:
		child.queue_free()

func load_layer_values(btn: Button):
	#Clear all layer values
	clear_layer_values()
	#Next, load new one
	print(btn.type)
	match btn.type:
		"Entity":
			singleton.cur_editor_mode = singleton.EditorMode.ENTITY
			print("load entity values")
			var entity_names = singleton.get_entity_names()
			print(entity_names)
			for entity_name in entity_names:
				var entity_layer_value_btn_node = entity_layer_value_btn_t.instance()
				entity_layer_value_btn_node.text = entity_name
				layer_value_list.add_child(entity_layer_value_btn_node)
		"Collision":
			singleton.cur_editor_mode = singleton.EditorMode.COLLISION
			print("Collision mode")
			singleton.cur_entity_type_ind = -1 #Can't create entity


func _on_addNewEntityTypeBtn_button_down():
	#If entity mode pressed
	var inst = entity_menu_t.instance()
	add_child(inst)


