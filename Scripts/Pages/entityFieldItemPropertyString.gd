extends HBoxContainer


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var defId: int = -1;
var fieldName: String = "";


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_TextEdit_text_changed():
	var fieldPropertyName: String = $Label.text
	var fieldPropertyValue: String = $TextEdit.text
	#var fieldName: String = singleton.get_cur_fieldDef_name()

	singleton.change_cur_field(fieldPropertyName, fieldPropertyValue)
	match fieldPropertyName:
		"identifier":
			get_tree().call_group("entityMenu", "load_entity_fields")
	match fieldName:
		"Trigger rect":
			var temp_str: String = fieldPropertyValue.substr(1, fieldPropertyValue.rfind("}"))
			var rect_arr = temp_str.split(",")
			var result_arr = [int(rect_arr[0]),int(rect_arr[1]),int(rect_arr[2]),int(rect_arr[3])]
			singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["triggerAABB"] = result_arr
		"Trigger type":
			singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["triggerType"] = int(fieldPropertyValue)
		"Trigger value":
			singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["triggerValue"] = int(fieldPropertyValue)
	#Change spritePath if entity
	#singleton.entity_types["defs"]["entities"][singleton.cur_entity_type_ind]["spritePath"] = path


func _on_applyToAllBtn_button_down():
	$ConfirmationDialog.popup_centered()


func _on_ConfirmationDialog_confirmed():
	var fieldPropertyName: String = $Label.text
	var fieldPropertyValue: String = $TextEdit.text
	var fieldName: String = singleton.get_cur_fieldDef_name()
	singleton.change_property_all_entityInst_by_defId(fieldName, fieldPropertyValue, defId)
	pass # Replace with function body.
