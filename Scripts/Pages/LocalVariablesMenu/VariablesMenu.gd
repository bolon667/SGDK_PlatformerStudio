extends Control

onready var variableItem_t = preload("res://Scenes/Pages/LocalVariablesMenu/VariableItem.tscn")
onready var varContainer = $CanvasLayer/VBoxContainer/VBoxContainer/ScrollContainer/VarContainer

var level_ind = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	print("LEVEL IND", level_ind)
	load_vars()
	
func _on_AddVarBtn_button_down():
	var node = variableItem_t.instance()
	varContainer.add_child(node)
	pass # Replace with function body.

func load_vars():
	if(!singleton.entity_types["levels"][level_ind].has("localVars")):
		singleton.entity_types["levels"][level_ind]["localVars"] = []
		return
	for varInst in singleton.entity_types["levels"][level_ind]["localVars"]:
		var varNode = variableItem_t.instance()
		varNode.get_node("VBoxContainer/edit1").text = varInst["type"]
		varNode.get_node("VBoxContainer2/edit1").text = varInst["name"]
		varNode.get_node("VBoxContainer3/edit1").text = varInst["value"]
		varContainer.add_child(varNode)

func save_vars():
	singleton.entity_types["levels"][level_ind]["localVars"] = []
	var varJSONarr = []
	var variableInstTemplate = {
		"type": "s16",
		"name": "undefVar",
		"value": "",
	}
	#Get var info from lineEdits
	for varInst in varContainer.get_children():
		var varName = varInst.get_node("VBoxContainer2/edit1").text
		
		if varName == "":
			continue
		var tempVarInst = variableInstTemplate.duplicate(true)
		tempVarInst["type"] = varInst.get_node("VBoxContainer/edit1").text
		tempVarInst["name"] = varName
		tempVarInst["value"] = varInst.get_node("VBoxContainer3/edit1").text
		varJSONarr.append(tempVarInst)
		
	singleton.entity_types["levels"][level_ind]["localVars"] = varJSONarr
	pass

func _on_ExitBtn_button_down():
	save_vars()
	queue_free()
