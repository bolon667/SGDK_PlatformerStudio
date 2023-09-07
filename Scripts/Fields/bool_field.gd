extends HBoxContainer


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var level_ind = 0
var entityInst_id = 0

var entityObj

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_CheckButton_toggled(button_pressed):
	var fieldName: String = $Label.text
	match(fieldName):
		"hFlip":
			singleton.change_entityInst_root_by_instId(entityInst_id, level_ind, "hFlip", button_pressed)
			entityObj.get_node("Sprite").flip_h = button_pressed
		"vFlip":
			singleton.change_entityInst_root_by_instId(entityInst_id, level_ind, "vFlip", button_pressed)
			entityObj.get_node("Sprite").flip_v = button_pressed
	singleton.change_fiendInst_value_by_levelInd_entityInstId(fieldName, button_pressed, level_ind, entityInst_id)
