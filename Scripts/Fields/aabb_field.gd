extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var cur_aabb:Array =  [0,0,0,0]


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	load_aabb()

func load_aabb():
	pass
	cur_aabb[0] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit.text)
	cur_aabb[1] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit2.text)
	cur_aabb[2] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit3.text)
	cur_aabb[3] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit4.text)
	
	$HBoxContainer/VBoxContainer/realVal.text = "{" + str(cur_aabb[0]) + "," + str(cur_aabb[1]) + "," + str(cur_aabb[2]) + "," + str(cur_aabb[3]) + "}"
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func update_trigger_rect():
	#singleton.get_cur_entityInstance_t()
	get_tree().call_group("tilemapEditorWindow", "change_entity_trigger_rect_by_instId", singleton.cur_entity_inst_ind, Rect2(cur_aabb[0], cur_aabb[1], cur_aabb[2], cur_aabb[3]))
	var new_val:String = "{" + str(cur_aabb[0]) + "," + str(cur_aabb[1]) + "," + str(cur_aabb[2]) + "," + str(cur_aabb[3]) + "}"
	singleton.change_fiendInst_by_instId("Trigger rect", new_val)
	singleton.change_cur_entityInst("triggerAABB", cur_aabb)
	$HBoxContainer/VBoxContainer/realVal.text = new_val
	pass

func _on_TextEdit_text_changed():
	cur_aabb[0] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit.text)
	update_trigger_rect()


func _on_TextEdit2_text_changed():
	cur_aabb[1] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit2.text)
	update_trigger_rect()

func _on_TextEdit3_text_changed():
	cur_aabb[2] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit3.text)
	update_trigger_rect()

func _on_TextEdit4_text_changed():
	cur_aabb[3] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit4.text)
	update_trigger_rect()
