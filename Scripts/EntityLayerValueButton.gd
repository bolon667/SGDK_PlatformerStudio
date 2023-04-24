extends Button

signal choose_entity


var defId: int = -1

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_EntityLayerButton_button_down():
	var ind = get_index()
	#This is such a mess, i will fix that eventually, but now, i need to add as much
	#features as possible, to create a solid foundation for this engine
	singleton.cur_entity_defId = defId
	singleton.cur_entity_type_ind = ind
	emit_signal("choose_entity", defId, text)
