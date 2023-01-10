extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func load_prev_project(projectPath: String):
	singleton.load_project(projectPath)
	get_tree().change_scene("res://Scenes/mapEditorScene.tscn")



func _on_NewProjBtn_button_down():
	$createProjectDialog.popup_centered()


func _on_createProjectDialog_file_selected(path):
	singleton.cur_project_path = path
	singleton.save_project()
	
	get_tree().change_scene("res://Scenes/mapEditorScene.tscn")


func _on_LoadProjBtn_button_down():
	$loadProjectDialog.popup_centered()
	


func _on_loadProjectDialog_file_selected(path):
	singleton.cur_project_path = path
	singleton.load_project(path)
	
	get_tree().change_scene("res://Scenes/mapEditorScene.tscn")
