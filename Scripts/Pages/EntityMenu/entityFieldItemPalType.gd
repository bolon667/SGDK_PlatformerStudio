extends Control



var triggerType_name: String = "";
var entityCollectionDef = "entities";

var defId

var root_path: String
# Called when the node enters the scene tree for the first time.

func _ready():
	load_pal_data()
	
func load_pal_data():
	var entity_field = singleton.get_cur_fieldDef()
	var optionButton = $HBoxContainer/VBoxContainer/OptionButton
	print("------------")
	print(entity_field)
	optionButton.select(int(entity_field["defaultValue"]))

func _on_OptionButton_item_selected(index):
	print("change optionsButton index")
	
	singleton.change_cur_field(entityCollectionDef, "defaultValue", index)


func _on_applyAllBtn_button_down():
	pass # Replace with function body.
	#var fieldName: String = singleton.get_cur_fieldDef_name()

	
	#singleton.change_property_all_entityInst_by_defId("Trigger type name", triggerType_name, singleton.cur_entity_type_ind)
