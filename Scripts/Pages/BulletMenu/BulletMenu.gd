extends Control

export var entityCollection: String = ""
export var entityDefaultName: String = ""



onready var entity_item_t = preload("res://Scenes/Pages/BulletMenu/entityListItem.tscn")
onready var entity_field_t = preload("res://Scenes/Pages/BulletMenu/entityFieldItem.tscn")
onready var field_property_string_t = preload("res://Scenes/Pages/BulletMenu/entityFieldItemPropertyString.tscn")
onready var field_property_sprite_t = preload("res://Scenes/Pages/BulletMenu/entityFieldItemSprite.tscn")
onready var field_property_triggerType_t = preload("res://Scenes/Pages/BulletMenu/entityFieldItemTriggerType.tscn")
onready var field_property_triggerAABB_t = preload("res://Scenes/Pages/BulletMenu/entityFieldItemAABB.tscn")

onready var entity_field_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/ScrollContainer/ContainerChooseField 
onready var entity_list_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/EntityListContainer
onready var entity_name_edit = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer3/EntityNameEdit
onready var add_new_field_btn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/AddNewEntityField
onready var field_properties_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/ScrollContainer/ContainerFieldProperties
onready var add_trigger_btn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer5/addTriggerActivate
onready var can_add_new_entity = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer6/canAddNewEntityBtn
onready var change_amount_of_entity = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer6/changeAmountOfEntity


var add_trigger_activated: bool = false

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	print(get_path())
	load_list_of_entity()


func load_add_new_entity():
	can_add_new_entity.disabled = false
	change_amount_of_entity.readonly = false
	if(singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind].has("canAddNewEntity")):
		var val = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["canAddNewEntity"]
		can_add_new_entity.pressed = val
		if(val):
			can_add_new_entity.text = "On"
			if singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind].has("amountOfEntity"):
				var val2 = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["amountOfEntity"]
				change_amount_of_entity.text = str(val2)
		else:
			can_add_new_entity.text = "Off"
	else:
		can_add_new_entity.pressed = false
		can_add_new_entity.text = "Off"
		change_amount_of_entity.text = ""
func load_entity_has_trigger():
	add_trigger_btn.disabled = false
	if(singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind].has("addTrigger")):
		add_trigger_activated = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["addTrigger"]
	else:
		singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["addTrigger"] = false
		add_trigger_activated = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["addTrigger"]
	if add_trigger_activated:
		add_trigger_btn.text = "On"
	else:
		add_trigger_btn.text = "Off"
		
func clear_list_of_entity():
	var entity_list_container_children = entity_list_container.get_children()
	for child in entity_list_container_children:
		child.queue_free()

func clear_field_properties():
	var children = field_properties_container.get_children()
	for child in children:
		child.queue_free()

func load_field_properties(entity_name: String, field_name: String):
	
	clear_field_properties()
	var defId = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["defId"]
	var field_data = singleton.get_cur_entity_one_field(entityCollection)
	match field_data["identifier"]:
		"sprDef":
			var key
			var val
			var field_property_node
			
			key = "defaultValue"
			val = field_data[key]
			field_property_node = field_property_sprite_t.instance()
			field_property_node.get_node("HBoxContainer/Label").text = key
			field_property_node.get_node("HBoxContainer/VBoxContainer/TextEdit").text = str(val)
			field_properties_container.add_child(field_property_node)
		"Trigger type":
			var key
			var val
			var field_property_node
			
			key = "defaultValue"
			val = field_data[key]
			field_property_node = field_property_triggerType_t.instance()
			#field_property_node.get_node("HBoxContainer/Label").text = key
			#field_property_node.get_node("HBoxContainer/VBoxContainer/TextEdit").text = str(val)
			field_properties_container.add_child(field_property_node)
		"Trigger rect":
			var key
			var val
			var field_property_node
			
			key = "defaultValue"
			val = field_data[key]
			field_property_node = field_property_triggerAABB_t.instance()
			#field_property_node.get_node("HBoxContainer/Label").text = key
			#field_property_node.get_node("HBoxContainer/VBoxContainer/TextEdit").text = str(val)
			field_properties_container.add_child(field_property_node)
			
		_:
			for key in field_data:
				if key == "fieldId":
					continue
				var val = field_data[key]
				var field_property_node = field_property_string_t.instance()
				field_property_node.defId = defId
				field_property_node.fieldName = field_name
				field_property_node.get_node("Label").text = key
				field_property_node.get_node("TextEdit").text = str(val)
				field_properties_container.add_child(field_property_node)
	pass

func enitity_name_edit_readonly(is_on: bool):
	entity_name_edit.readonly = is_on

func load_list_of_entity():
	var entity_names = singleton.get_def_entityCollection_names(entityCollection)
	var entity_names_len = len(entity_names)
	singleton.entity_names_len = entity_names_len
	print("entity_names_len: ", entity_names_len)
	print(singleton.cur_entity_type_ind)
	
	
	if(entity_names_len == 0):
		entity_name_edit.readonly = true
		add_new_field_btn.disabled = true
	else:
		entity_name_edit.readonly = false
		add_new_field_btn.disabled = false
	if(singleton.cur_entity_type_ind == -1):
		print("readonly")
		entity_name_edit.readonly = true
	
	for entity_dict in entity_names:
		var entity_item = entity_item_t.instance()
		entity_item.get_node("HBoxContainer/TextBtn").text = entity_dict["name"]
		
		entity_list_container.add_child(entity_item)
		

func add_new_entity_button():
	pass

func add_new_field_button():
	pass

func get_entity_list():
	clear_list_of_entity()
	load_list_of_entity()
	#load_entity_fields()

func clear_entity_fields():
	entity_name_edit.text = singleton.cur_entity_type
	
	#free all children
	var tempChildrens = entity_field_container.get_children()
	for child in tempChildrens:
		child.queue_free()

func load_entity_pic():
	singleton.get_cur_entityDef(entityCollection)
	pass

func load_entity_fields():
	#free all children
	clear_entity_fields()
	if(singleton.entity_names_len == 0):
		return
	
	#var entity_field_names = singleton.get_cur_entity_field_names()
	var entity_fields = singleton.get_entityCollection_fields(entityCollection)
	#show children, generated from array
	for field in entity_fields:
		var field_name = field["identifier"]
		
		var entity_field_node = entity_field_t.instance()
		entity_field_node.get_node("TextBtn").text = field_name
		entity_field_node.get_node("DeleteBtn").disabled = !field["canBeDeleted"]
		entity_field_container.add_child(entity_field_node)


func _on_AddNewEntityBtn_button_down():
	#Get new Entity name
	var entity_name = entityDefaultName
	var final_entity_name = entity_name

	if(singleton.is_entity_name_exists(entityCollection, entity_name)):
		var i:int = 2;
		while(i<100):
			final_entity_name = entity_name + str(i)
			if(!singleton.is_entity_name_exists(entityCollection, final_entity_name)):
				
				break
			i+=1
	
	
	#Add entity to database
	singleton.add_entity_def_to_entityCollection(entityCollection, final_entity_name)
	
	clear_list_of_entity()
	load_list_of_entity()
	


func _on_AddNewEntityField_button_down():
	#Adding button to list of field buttons
	var field_name = "Field"
	var final_field_name = field_name
	
	if(singleton.is_entity_have_field(entityCollection, singleton.cur_entity_type, final_field_name)):
		var i=2;
		while(i<100):
			final_field_name = field_name + str(i)
			if(!singleton.is_entity_have_field(entityCollection, singleton.cur_entity_type, final_field_name)):
				break
			i+=1
	
	singleton.add_field_to_entity_def(entityCollection, "entityBulletInstances", singleton.cur_entity_type, final_field_name)
	load_entity_fields()
	
	#Adding this field to database
	
	
	
	print(singleton.entity_types)
	
	


func _on_EntityNameEdit_text_changed():
	print("singleton.cur_entity_type_ind: ", singleton.cur_entity_type_ind)
	print("singleton.cur_entity_type: ", singleton.cur_entity_type)
	
	#TODO: filter not ASCII symbols
	var text = entity_name_edit.text
	var defId = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["defId"]
	print(defId, " defId")
	singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["identifier"] = text
	singleton.change_entityInstName_by_defId(text, defId)
	
	clear_list_of_entity()
	load_list_of_entity()


func _on_ExitBtn_button_down():
	singleton.cur_entity_type_ind = -1
	get_tree().call_group("leftContainer", "clear_layer_values")
	get_tree().call_group("tilemapEditorWindow", "load_entities_on_scene")
	
	queue_free()


func _on_entityMergedActive_toggled(button_pressed):
	pass # Replace with function body.


func _on_addTriggerActivate_button_down():
	add_trigger_activated = !add_trigger_activated
	singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["addTrigger"] = add_trigger_activated
	if add_trigger_activated:
		add_trigger_btn.text = "On"
	else:
		add_trigger_btn.text = "Off"
	var defId = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["defId"]
	singleton.change_addTrigger_all_entityInst_by_defId(defId, add_trigger_activated)

func _on_canAddNewEntityBtn_toggled(button_pressed):
	#can_add_new_entity change_amount_of_entity
	can_add_new_entity.pressed =  button_pressed
	if(button_pressed):
		can_add_new_entity.text = "On"
	else:
		can_add_new_entity.text = "Off"
		
	singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["canAddNewEntity"] = button_pressed
	pass # Replace with function body.


func _on_changeAmountOfEntity_text_changed():
	var val: int = int(change_amount_of_entity.text)
	
	singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["amountOfEntity"] = val
	pass # Replace with function body.
