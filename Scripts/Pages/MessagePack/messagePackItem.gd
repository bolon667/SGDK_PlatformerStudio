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
	singleton.cur_messagePack_ind = get_index()
	singleton.delete_cur_messagePack()
	get_tree().call_group("messageMenu", "load_messagePacks")
	queue_free()


func _on_TextBtn_button_down():
	singleton.cur_messagePack_ind = get_index()
	singleton.cur_message_ind = -1
	get_tree().call_group("messageMenu", "load_messages")
	get_tree().call_group("messageMenu","block_message_edit")
