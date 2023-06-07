extends Control

onready var sprOptItem_t = preload("res://Scenes/Pages/SpriteOptimizationsMenu/SprOptItem.tscn")
onready var sprOptContainer = $CanvasLayer/VBoxContainer/VBoxContainer/ScrollContainer/sprOptContainer

var level_ind = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	load_sprOpts()
	
func _on_AddVarBtn_button_down():
	var node = sprOptItem_t.instance()
	sprOptContainer.add_child(node)
	pass # Replace with function body.

func load_sprOpts():
	if(!singleton.entity_types["defs"].has("SpriteOptList")):
		singleton.entity_types["defs"]["SpriteOptList"]  = []
		return
	for sprOptInst in singleton.entity_types["defs"]["SpriteOptList"]:
		var sprOptNode = sprOptItem_t.instance()
		sprOptNode.get_node("VBoxContainer/edit1").text = sprOptInst["sprDefName"]
		#if(!sprOptInst.has("onlyOnLevel")):
		#	sprOptInst["onlyOnLevel"] = false
		sprOptNode.get_node("VBoxContainer3/IfOnLevelBtn").pressed = sprOptInst["onlyOnLevel"]
		sprOptContainer.add_child(sprOptNode)

func save_sprOpts():
	singleton.entity_types["defs"]["SpriteOptList"] = []
	var varJSONarr = []
	var sprOptInstTemplate = {
		"sprDefName": "",
		"optMode": 0,
		"onlyOnLevel": false,
	}
	#Get var info from lineEdits
	for sprOptInst in sprOptContainer.get_children():
		var sprDefName = sprOptInst.get_node("VBoxContainer/edit1").text
		
		if sprDefName == "":
			continue
		var tempSprOptInst = sprOptInstTemplate.duplicate(true)
		tempSprOptInst["sprDefName"] = sprDefName
		tempSprOptInst["onlyOnLevel"] = sprOptInst.get_node("VBoxContainer3/IfOnLevelBtn").pressed
		varJSONarr.append(tempSprOptInst)
		
	singleton.entity_types["defs"]["SpriteOptList"] = varJSONarr
	pass

func _on_ExitBtn_button_down():
	save_sprOpts()
	queue_free()
