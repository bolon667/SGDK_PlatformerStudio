extends Button

onready var engineTypeLabel = $EngineTypeLabel
var cur_path:String = ProjectSettings.globalize_path("res://") + "StudioType/SGDK/Projects/"
# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	load_info()
	load_icon()

func load_info():
	var json_path = cur_path + text + "/fast_info.json"
	var file = File.new()
	file.open(json_path, File.READ)
	
	var json_data = JSON.parse(file.get_as_text())

	engineTypeLabel.text = json_data.result["engineType"]

func load_icon():
	var icon_path = cur_path + text + "/icon.png"
	icon = load(icon_path)
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_prevProjectButton_button_down():
	get_tree().call_group("startUpMenu", "load_prev_project", text)
	pass # Replace with function body.
