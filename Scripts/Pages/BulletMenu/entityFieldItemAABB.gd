extends Control

var entityCollection = "bulletEntities"
var defId: int = -1
var cur_aabb:Array =  [0,0,0,0]
var level_ind = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	load_aabb()
	

func load_aabb():
	var cur_entityDef = singleton.get_cur_entityDef(entityCollection)
	cur_aabb = cur_entityDef["triggerAABB"]
	$HBoxContainer/VBoxContainer/GridContainer/TextEdit.text = str(cur_aabb[0])
	$HBoxContainer/VBoxContainer/GridContainer/TextEdit2.text = str(cur_aabb[1])
	$HBoxContainer/VBoxContainer/GridContainer/TextEdit3.text = str(cur_aabb[2])
	$HBoxContainer/VBoxContainer/GridContainer/TextEdit4.text = str(cur_aabb[3])
	
	$HBoxContainer/VBoxContainer/realVal.text = "{" + str(cur_aabb[0]) + "," + str(cur_aabb[1]) + "," + str(cur_aabb[2]) + "," + str(cur_aabb[3]) + "}"

func update_aabb():
	var new_val:String = "{" + str(cur_aabb[0]) + "," + str(cur_aabb[1]) + "," + str(cur_aabb[2]) + "," + str(cur_aabb[3]) + "}"
	$HBoxContainer/VBoxContainer/realVal.text = new_val
	singleton.entity_types["defs"][entityCollection][singleton.cur_entity_type_ind]["triggerAABB"] == cur_aabb

func _on_TextEdit_text_changed():
	cur_aabb[0] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit.text)
	update_aabb()


func _on_TextEdit2_text_changed():
	cur_aabb[1] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit2.text)
	update_aabb()

func _on_TextEdit3_text_changed():
	cur_aabb[2] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit3.text)
	update_aabb()

func _on_TextEdit4_text_changed():
	cur_aabb[3] = int($HBoxContainer/VBoxContainer/GridContainer/TextEdit4.text)
	update_aabb()
