extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var in_bounds: bool = true

# Called when the node enters the scene tree for the first time.
func _ready():
	singleton.prev_editor_mode = singleton.cur_editor_mode
	singleton.cur_editor_mode = singleton.EditorMode.START_POS
	set_process(false)
	yield(get_tree(), "idle_frame")
	yield(get_tree(), "idle_frame")
	set_process(true)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("mouse1") and in_bounds:
		get_tree().call_group("tilemapEditorWindow", "change_start_pos")
		#singleton.cur_editor_mode = singleton.prev_editor_mode
		singleton.cur_editor_mode = singleton.EditorMode.NONE
		queue_free()


func _on_Area2D_mouse_entered():
	in_bounds = true


func _on_Area2D_mouse_exited():
	in_bounds = false
