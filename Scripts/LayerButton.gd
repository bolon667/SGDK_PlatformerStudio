extends Button

var type: String


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):


func _on_LayerButton_button_down():
	singleton.cur_level_layer_ind = get_index()
	print("singleton.cur_level_layer_ind", singleton.cur_level_layer_ind)
	get_tree().call_group("leftContainer", "load_layer_values", self)
	pass # Replace with function body.
