extends HBoxContainer


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var enum_amount: int = 0
var label_text: String = ""
var triggerType_name: String = ""
var level_ind = 0
var entityInst_id = 0


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	$Label.text = label_text
	load_trigger_enum()
	

func load_trigger_enum():
	var trigger_enum = singleton.get_trigger_enum()
	var optionButton = $VBoxContainer/OptionButton
	if not trigger_enum:
		return
	var cur_ind = 0
	for enum_item in trigger_enum["values"]:
		optionButton.add_item(enum_item["name"], cur_ind)
		cur_ind += 1
	enum_amount = cur_ind
	#finding index of cur triggerType
	for i in range(enum_amount):
		print(optionButton.get_item_text(i), " == ", triggerType_name)
		print(optionButton.get_item_text(i) == triggerType_name)
		if optionButton.get_item_text(i) == triggerType_name:
			optionButton.select(i)
			break

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_LineEdit_text_changed(new_text):
	return
	if($Label.text == "Trigger type"):
		print("change trigger type")
		singleton.change_entityInst_by_instId(entityInst_id, level_ind, "triggerType", int(new_text))
		
	#$VBoxContainer/OptionButton.
	var select_ind: int = int(new_text)
	if(select_ind > enum_amount):
		select_ind = enum_amount
	
	
	singleton.change_fiendInst_value("Trigger type", str(select_ind))
	
	$VBoxContainer/OptionButton.select(select_ind)
	pass # Replace with function body.


func _on_OptionButton_item_selected(index):
	print("change optionsButton index")
	#$VBoxContainer/LineEdit.text = str(index)
	var triggerTypeName = $VBoxContainer/OptionButton.get_item_text(index)
	#singleton.change_entityInst_by_instId(singleton.cur_entity_inst_ind, "triggerType", index)
	#singleton.change_entityInst_by_instId(singleton.cur_entity_inst_ind, "triggerType", index)
	singleton.change_entityInst_by_instId(entityInst_id, level_ind, "triggerTypeName", triggerTypeName)
	singleton.change_fiendInst_value_by_levelInd_entityInstId("Trigger type", index, level_ind, entityInst_id)
	#singleton.change_fiendInst_value("Trigger type", str(index))
