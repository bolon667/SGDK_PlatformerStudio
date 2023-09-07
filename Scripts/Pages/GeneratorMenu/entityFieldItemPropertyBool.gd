extends HBoxContainer


var defId: int = -1;
var fieldName: String = "";
var entityCollectionDef = "entities";

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_CheckButton_toggled(button_pressed):
	var fieldPropertyName: String = $Label.text
	var fieldPropertyValue: bool = button_pressed
	#var fieldName: String = singleton.get_cur_fieldDef_name()

	singleton.change_cur_field(entityCollectionDef, fieldPropertyName, fieldPropertyValue)
	match fieldPropertyName:
		"identifier":
			get_tree().call_group("entityMenu", "load_entity_fields")

func _on_applyToAllBtn_button_down():
	$ConfirmationDialog.popup_centered()


func _on_ConfirmationDialog_confirmed():
	var fieldPropertyName: String = $Label.text
	var fieldPropertyValue: int = int($TextEdit.text)
	var fieldName: String = singleton.get_cur_fieldDef_name()
	singleton.change_property_all_entityInst_by_defId(fieldName, fieldPropertyValue, defId)
	pass # Replace with function body.
