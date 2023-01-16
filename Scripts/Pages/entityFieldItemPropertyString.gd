extends HBoxContainer


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_TextEdit_text_changed():
	var fieldPropertyName: String = $Label.text
	var fieldPropertyValue: String = $TextEdit.text
	singleton.change_cur_field(fieldPropertyName, fieldPropertyValue)
	if fieldPropertyName == "identifier":
		#singleton.change_name_of_cur_fieldDef(text)
		
		get_tree().call_group("entityMenu", "load_entity_fields")
	
