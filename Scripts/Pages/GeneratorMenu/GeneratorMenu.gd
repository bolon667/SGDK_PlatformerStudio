extends Control


export var entityCollection: String = "generators"
export var entityDefaultName: String = "generator"

onready var entity_item_t = preload("res://Scenes/Pages/GeneratorMenu/entityListItem.tscn")
onready var entity_field_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItem.tscn")

onready var field_property_string_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItemPropertyString.tscn")
onready var field_property_image_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItemImage.tscn")
onready var field_property_pal_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItemPalType.tscn")

onready var field_property_triggerRect_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItemAABB.tscn")
onready var field_property_num_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItemPropertyNumber.tscn")
onready var field_property_bool_t = preload("res://Scenes/Pages/GeneratorMenu/entityFieldItemPropertyBool.tscn")

onready var entity_field_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/ScrollContainer/ContainerChooseField 
onready var entity_list_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/EntityListContainer
onready var entity_name_edit = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer3/EntityNameEdit
onready var add_new_field_btn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/AddNewEntityField
onready var field_properties_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/ScrollContainer/ContainerFieldProperties
onready var add_trigger_btn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer5/addTriggerActivate
onready var can_add_new_entity = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer6/canAddNewEntityBtn
onready var change_amount_of_entity = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer6/changeAmountOfEntity
onready var complex_menu_btn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer3/complexEntityEditorBtn


var add_type: int = 0

func _ready():
	load_list_of_entity()
	
func clear_entity_fields():
	entity_name_edit.text = singleton.cur_entity_type
	
	#free all children
	var tempChildrens = entity_field_container.get_children()
	for child in tempChildrens:
		child.queue_free()
	
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

func _on_ExitBtn_button_down():
	queue_free()


func _on_AddNewEntityBtn_button_down():
	#Get new Entity name
	var entity_name = entityDefaultName
	var final_entity_name = entity_name

	if(singleton.is_entity_name_exists(entityCollection,entity_name)):
		var i:int = 0;
		while(i<100):
			final_entity_name = entity_name + str(i)
			if(!singleton.is_entity_name_exists(entityCollection, final_entity_name)):
				
				break
			i+=1
	
	
	#Add entity to database
	singleton.add_entity_def_to_entityCollection(entityCollection, final_entity_name, add_type)
	
	clear_list_of_entity()
	load_list_of_entity()

func clear_field_properties():
	var children = field_properties_container.get_children()
	for child in children:
		child.queue_free()

func load_field_properties(entity_name: String, field_name: String):
	
	clear_field_properties()
	var defId = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["defId"]
	var field_data = singleton.get_cur_entity_one_field(entityCollection)
	match field_data["__type"]:
		"Sprite":
			var key
			var val
			var field_property_node
			
			key = "defaultValue"
			val = field_data[key]
			field_property_node = field_property_image_t.instance()
			field_property_node.get_node("HBoxContainer/Label").text = key
			field_property_node.get_node("HBoxContainer/VBoxContainer/TextEdit").text = str(val)
			field_properties_container.add_child(field_property_node)
			return
		"Palette":
			var key
			var val
			var field_property_node
			
			key = "defaultValue"
			val = field_data[key]
			field_property_node = field_property_pal_t.instance()
			field_property_node.defId = defId
			field_properties_container.add_child(field_property_node)
			return
			
		"Integer":
			var key = "defaultValue"
			var val = field_data[key]
			var field_property_node = field_property_num_t.instance()
			field_property_node.defId = defId
			field_property_node.fieldName = field_name
			field_property_node.get_node("Label").text = key
			field_property_node.get_node("TextEdit").text = str(val)
			field_properties_container.add_child(field_property_node)
		"Bool":
			var key = "defaultValue"
			var val = field_data[key]
			var field_property_node = field_property_bool_t.instance()
			field_property_node.defId = defId
			field_property_node.fieldName = field_name
			field_property_node.get_node("Label").text = key

			field_property_node.get_node("CheckButton").pressed = bool(val)
			field_properties_container.add_child(field_property_node)
		"Text":
			var key = "defaultValue"
			var val = field_data[key]
			var field_property_node = field_property_string_t.instance()
			field_property_node.defId = defId
			field_property_node.fieldName = field_name
			field_property_node.get_node("Label").text = key
			field_property_node.get_node("TextEdit").text = str(val)
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
			return

func clear_list_of_entity():
	var entity_list_container_children = entity_list_container.get_children()
	for child in entity_list_container_children:
		child.queue_free()
		
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
		entity_item.defId = entity_dict["defId"]
		entity_list_container.add_child(entity_item)

func enitity_name_edit_readonly(is_on: bool):
	entity_name_edit.readonly = is_on

func _on_OptionButton_item_selected(index):
	add_type = index


func _on_EntityNameEdit_text_changed():
	print("singleton.cur_entity_type_ind: ", singleton.cur_entity_type_ind)
	print("singleton.cur_entity_type: ", singleton.cur_entity_type)
	
	var text = entity_name_edit.text
	var defId = singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["defId"]
	print(defId, " defId")
	singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["identifier"] = text
	singleton.change_entityInstName_by_defId(text, defId, "Generator")
	
	clear_list_of_entity()
	load_list_of_entity()
