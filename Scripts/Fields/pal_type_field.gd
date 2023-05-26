extends HBoxContainer

var level_ind = 0
var entityInst_id = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_OptionButton_item_selected(index):
	singleton.change_fiendInst_value_by_levelInd_entityInstId($Label.text, index, level_ind, entityInst_id)
	pass # Replace with function body.
