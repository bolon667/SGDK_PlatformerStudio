extends Control


var triggerType_name: String = "";

var root_path: String
# Called when the node enters the scene tree for the first time.
func _ready():
	get_triggerType_name()
	load_trigger_enum()
	print("HERE")
	pass
	
func get_triggerType_name():
	if(singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind].has("triggerTypeName")):
		triggerType_name = singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["triggerTypeName"]

func load_trigger_enum():
	var trigger_enum = singleton.get_trigger_enum()
	var optionButton = $HBoxContainer/VBoxContainer/OptionButton
	if not trigger_enum:
		return
	var enum_values_count = 0
	for enum_item in trigger_enum["values"]:
		optionButton.add_item(enum_item["name"], enum_values_count)
		enum_values_count += 1
	#finding index of cur triggerType
	for i in range(enum_values_count):
		if optionButton.get_item_text(i) == triggerType_name:
			optionButton.select(i)
			break
	


func _on_OptionButton_item_selected(index):
	print("change optionsButton index")
	triggerType_name = $HBoxContainer/VBoxContainer/OptionButton.get_item_text(index)

	singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["triggerTypeName"] = triggerType_name


func _on_applyAllBtn_button_down():
	pass # Replace with function body.
	singleton.change_property_all_entityInst_by_defId("Trigger type name", triggerType_name, singleton.cur_entity_type_ind)
