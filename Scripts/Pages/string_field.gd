extends HBoxContainer


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var level_ind = 0
var entityInst_id = 0

var entityObj
# Called when the node enters the scene tree for the first time.
func _ready():
	entityObj = get_node("../../../").entityObj
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_LineEdit_text_changed(new_text):
	if entityObj.is_in_group("entityScene"):
		if($Label.text == "Trigger type"):
			print("change trigger type")
			singleton.change_entityInst_root_by_instId(entityInst_id, level_ind, "triggerType", int(new_text))
			
			pass
		elif($Label.text == "Trigger value"):
			print("change trigger value")
			singleton.change_entityInst_root_by_instId(entityInst_id, level_ind, "triggerValue", int(new_text))
			pass
		elif($Label.text == "Trigger value2"):
			print("change trigger value")
			singleton.change_entityInst_root_by_instId(entityInst_id, level_ind, "triggerValue2", int(new_text))
			pass
		elif($Label.text == "Trigger value3"):
			print("change trigger value")
			singleton.change_entityInst_root_by_instId(entityInst_id, level_ind, "triggerValue3", int(new_text))
			pass
			#trigger_type
	elif entityObj.is_in_group("generatorScene"):
		entityObj.get_node("textLabel").text = new_text
		
	singleton.change_fiendInst_value_by_levelInd_entityInstId($Label.text, new_text, level_ind, entityInst_id)
	pass # Replace with function body.
