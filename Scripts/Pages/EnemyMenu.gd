extends Control


onready var entity_item_t = preload("res://Scenes/Pages/entityListItem.tscn")
onready var entity_field_t = preload("res://Scenes/Pages/entityFieldItem.tscn")

onready var entity_field_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/ScrollContainer/ContainerChooseField 
onready var entity_list_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/EntityListContainer
onready var entity_name_edit = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/EntityNameEdit
onready var add_new_field_btn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/AddNewEntityField

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	print(get_path())
	load_list_of_entity()


func clear_list_of_entity():
	var entity_list_container_children = entity_list_container.get_children()
	for child in entity_list_container_children:
		child.queue_free()

func load_list_of_entity():
	var entity_names = singleton.get_entity_names()
	var entity_names_len = len(entity_names)
	print(entity_names)
	print("entity_names_len: ", entity_names_len)
	if(entity_names_len == 0):
		entity_name_edit.readonly = true
		add_new_field_btn.disabled = true
	else:
		entity_name_edit.readonly = false
		add_new_field_btn.disabled = false
	for entity_type in entity_names:
		var entity_item = entity_item_t.instance()
		entity_item.get_node("HBoxContainer/TextBtn").text = entity_type
		entity_list_container.add_child(entity_item)
		

func add_new_entity_button():
	pass

func add_new_field_button():
	pass

func get_entity_list():
	clear_list_of_entity()
	load_list_of_entity()
	get_entity_fields(singleton.cur_entity_type)

func get_entity_fields(entity_name: String):
	#change entity_name field to what written on button
	
	entity_name_edit.text = entity_name
	
	#free all children
	var tempChildrens = entity_field_container.get_children()
	for child in tempChildrens:
		child.queue_free()
	
	var entity_field_names = singleton.get_entity_field_names(entity_name)
	#show children, generated from array
	for entity_field_name in entity_field_names:
		var entity_field_node = entity_field_t.instance()
		entity_field_node.get_node("TextBtn").text = entity_field_name
		entity_field_container.add_child(entity_field_node)


func _on_AddNewEntityBtn_button_down():
	#Get new Entity name
	var entity_name = "Entity"
	var final_entity_name = entity_name

	if(singleton.is_entity_name_exists(entity_name)):
		var i=2;
		while(i<100):
			final_entity_name = entity_name + str(i)
			if(!singleton.is_entity_name_exists(final_entity_name)):
				
				break
			i+=1
	
	#Add entity to database
	singleton.add_entity(final_entity_name)
	var file = File.new()
	file.open("res://test.json", File.WRITE)
	file.store_string(to_json(singleton.entity_types))
	file.close()
	
	clear_list_of_entity()
	load_list_of_entity()
	


func _on_EntityNameEdit_breakpoint_toggled(row):
	#if you changed name, then trying to find that name array
	
	print(row)
	pass # Replace with function body.


func _on_AddNewEntityField_button_down():
	#Adding button to list of field buttons
	var field_name = "Field"
	var final_field_name = field_name
	
	if(singleton.is_entity_have_field(singleton.cur_entity_type, final_field_name)):
		var i=2;
		while(i<100):
			final_field_name = field_name + str(i)
			if(!singleton.is_entity_have_field(singleton.cur_entity_type, final_field_name)):
				break
			i+=1
	
	singleton.add_field_to_entity(singleton.cur_entity_type, final_field_name)
	get_entity_fields(singleton.cur_entity_type)
	
	#Adding this field to database
	
	
	
	print(singleton.entity_types)
	
	


func _on_EntityNameEdit_text_changed():
	print("singleton.cur_entity_type_ind: ", singleton.cur_entity_type_ind)
	print("singleton.cur_entity_type: ", singleton.cur_entity_type)
	#TODO: filter not ASCII symbols
	singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["identifier"] = entity_name_edit.text
	clear_list_of_entity()
	load_list_of_entity()


func _on_ExitBtn_button_down():
	queue_free()
