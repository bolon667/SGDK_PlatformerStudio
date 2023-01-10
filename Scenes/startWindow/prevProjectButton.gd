extends Button


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_prevProjectButton_button_down():
	get_tree().call_group("startUpMenu", "load_prev_project", text)
	pass # Replace with function body.
