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


func _on_DeleteBtn_button_down():
	var cur_field_ind = get_index()
	
	singleton.delete_fieldDef("generators", "generators", cur_field_ind)
	
	get_tree().call_group("generatorMenu" , "clear_field_properties")
	
	
	queue_free()





func _on_TextBtn_button_down():
	var field_name = $TextBtn.text
	singleton.cur_entity_field_ind = get_index()
	
	get_tree().call_group("generatorMenu", "load_field_properties", singleton.cur_entity_type, field_name)
	
	
	
