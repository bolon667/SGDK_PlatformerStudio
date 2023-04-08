extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

onready var runFromCurLevelBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HBoxContainer/runFromCurLevelBtn
onready var showTriggerRectsBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/debugContainer/showTriggerRectsBtn
onready var addSlotsLabel = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HboxContainer3/addSlotsLabel
onready var addBulletSlotsLabel = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HboxContainer4/addBulletSlotsLabel

onready var addSlotsSlider = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HboxContainer3/addSlotsSlider
onready var addBulletSlotsSlider = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HboxContainer4/addBulletSlotsSlider

onready var turnOnGates = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/turnOnGatesBtn
onready var entityOptBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HBoxContainer3/entityOptBtn

onready var chunkSizeXEdit = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HBoxContainer4/chunkSizeX
onready var chunkSizeYEdit = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HBoxContainer4/chunkSizeY


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	load_run_from_cur_level()
	load_shot_trigger_rects()
	load_add_EntityMerged_slots()
	load_add_EntityBulletMerged_slots()
	load_turnOnGates()
	load_entity_optimization()
	load_chunk_size()

func load_chunk_size():
	chunkSizeXEdit.text = str(singleton.entity_types["chunkSizeX"])
	chunkSizeYEdit.text = str(singleton.entity_types["chunkSizeY"])
	

func load_entity_optimization():
	if(singleton.entity_types.has("entityLoadOptimization")):
		entityOptBtn.select(singleton.entity_types["entityLoadOptimization"])

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

func load_add_EntityBulletMerged_slots():
	if(singleton.entity_types.has("addEntityBulletMergedSlots")):
		addBulletSlotsSlider.value = singleton.entity_types["addEntityBulletMergedSlots"]
		
func load_turnOnGates():
	if(singleton.entity_types.has("turnOnGates")):
		turnOnGates.pressed = singleton.entity_types["turnOnGates"]

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

func _on_turnOnGatesBtn_toggled(button_pressed):
	singleton.entity_types["turnOnGates"] = button_pressed

func _on_addBulletSlotsSlider_value_changed(value):
	var int_val: int = int(value)
	addBulletSlotsLabel.text = str(int_val)
	singleton.entity_types["addEntityBulletMergedSlots"] = int_val

func _on_entityOptBtn_item_selected(index):
	singleton.entity_types["entityLoadOptimization"] = index


func _on_chunkSizeX_text_changed(new_text):
	singleton.entity_types["chunkSizeX"] = int(new_text)


func _on_chunkSizeY_text_changed(new_text):
	singleton.entity_types["chunkSizeY"] = int(new_text)
