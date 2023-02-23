extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

onready var runFromCurLevelBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/HBoxContainer/runFromCurLevelBtn
onready var showTriggerRectsBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer/debugContainer/showTriggerRectsBtn


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	if(singleton.entity_types.has("runFromCurrentLevel")):
		runFromCurLevelBtn.pressed = singleton.entity_types["runFromCurrentLevel"]
	if(!singleton.entity_types.has("debugSettings")):
		singleton.entity_types["debugSettings"] = {}
	elif(singleton.entity_types["debugSettings"].has("showTriggerRects")):
		showTriggerRectsBtn.pressed = singleton.entity_types["debugSettings"]["showTriggerRects"]


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_ExitBtn_button_down():
	queue_free()


func _on_runFromCurLevelBtn_toggled(button_pressed):
	singleton.entity_types["runFromCurrentLevel"] = button_pressed
	print("run from current level is ", button_pressed)


func _on_showTriggerRectsBtn_toggled(button_pressed):
	singleton.entity_types["debugSettings"]["showTriggerRects"] = button_pressed
