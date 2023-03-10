extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

onready var runFromCurLevelBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HBoxContainer/runFromCurLevelBtn
onready var showTriggerRectsBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/debugContainer/showTriggerRectsBtn
onready var addSlotsLabel = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HboxContainer3/addSlotsLabel
onready var addSlotsSlider = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HboxContainer3/addSlotsSlider

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	load_run_from_cur_level()
	load_shot_trigger_rects()
	load_add_EntityMerged_slots()

func load_run_from_cur_level():
	if(singleton.entity_types.has("runFromCurrentLevel")):
		runFromCurLevelBtn.pressed = singleton.entity_types["runFromCurrentLevel"]
func load_shot_trigger_rects():
	if(!singleton.entity_types.has("debugSettings")):
		singleton.entity_types["debugSettings"] = {}
	elif(singleton.entity_types["debugSettings"].has("showTriggerRects")):
		showTriggerRectsBtn.pressed = singleton.entity_types["debugSettings"]["showTriggerRects"]
func load_add_EntityMerged_slots():
	if(singleton.entity_types.has("addEntityMergedSlots")):
		addSlotsSlider.value = singleton.entity_types["addEntityMergedSlots"]


func _on_ExitBtn_button_down():
	queue_free()


func _on_runFromCurLevelBtn_toggled(button_pressed):
	singleton.entity_types["runFromCurrentLevel"] = button_pressed
	print("run from current level is ", button_pressed)


func _on_showTriggerRectsBtn_toggled(button_pressed):
	singleton.entity_types["debugSettings"]["showTriggerRects"] = button_pressed




func _on_addSlotsSlider_value_changed(value):
	var int_val: int = int(value)
	addSlotsLabel.text = str(int_val)
	singleton.entity_types["addEntityMergedSlots"] = int_val
