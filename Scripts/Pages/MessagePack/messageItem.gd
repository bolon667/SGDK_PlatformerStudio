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
	singleton.cur_message_ind = get_index()
	singleton.delete_cur_message()
	get_tree().call_group("messageMenu", "load_messages")
	queue_free()


func _on_TextBtn_button_down():
	singleton.cur_message_ind = get_index()
	get_tree().call_group("messageMenu", "update_message_edit", $TextBtn.text)
