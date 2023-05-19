extends Control

var entityName: String = "NULL"

var in_area: bool = false

export var entityCollection: String = "entities"

onready var entity_layer_value_btn_t = preload("res://Scenes/EntityLayerValueButton.tscn")
onready var entityScene_t = preload("res://Scenes/Pages/EntityMenu/ComplexEntityMenu/entitySceneComplexDef.tscn")

onready var collShapeMain = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/MainArea/Area2D/CollisionShape2D
onready var entity_list_container = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/EntityListContainer
onready var entityNameEdit = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer3/EntityNameEdit
onready var mainArea = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer/VBoxContainer/MainArea
onready var area2D_collShape = mainArea.get_node("Area2D/CollisionShape2D")
onready var centerArea = mainArea.get_node("CenterArea")

var masterDefId: int = -1
var slaveDefId: int = -1

var max_amount_of_slaves: int = 10
var cur_amount_of_slaves: int = 0


func load_list_of_entity():
	var entity_names = singleton.get_def_entityCollection_names(entityCollection)
	var entity_names_len = len(entity_names)
	singleton.entity_names_len = entity_names_len
	
	for entity_dict in entity_names:
		var entity_item = entity_layer_value_btn_t.instance()
		entity_item.text = entity_dict["name"]
		entity_item.defId = entity_dict["defId"]
		entity_item.connect("choose_entity", self, "_on_entity_button_down")
		entity_list_container.add_child(entity_item)

func load_slaves():
	#finding index of this masterDefId
	var target_ind: int = 0
	for entity_def in singleton.entity_types["defs"]["entities"]:
		if(entity_def["defId"] == masterDefId):
			break
		target_ind += 1
		
	for entityInst in singleton.entity_types["defs"]["entities"][target_ind]["subordinates"]:
		var entity_node = entityScene_t.instance()
		entity_node.position = Vector2(entityInst["px"][0], entityInst["px"][1])
		entity_node.defId = entityInst["defId"]
		entity_node.masterDefId = masterDefId
		entity_node.entityInst_id = entityInst["instId"]
		
		if(entityInst["__spritePath"]):
			entity_node.get_node("Sprite").texture = load(entityInst["__spritePath"])
			var temp_sprite_size = singleton.get_sprite_size_from_path(entityInst["__spritePath"])
			if temp_sprite_size:
				entity_node.sprite_size = temp_sprite_size
				entity_node.change_sprite_rect(Rect2(0,0,temp_sprite_size.x, temp_sprite_size.y))
				entity_node.get_node("CollisionShape2D").shape.extents = Vector2(temp_sprite_size.x/2, temp_sprite_size.y/2)
			else:
				entity_node.sprite_size = entity_node.get_node("Sprite").texture.get_size()
				entity_node.change_sprite_rect(Rect2(0,0,entity_node.sprite_size.x, entity_node.sprite_size.y))
				entity_node.get_node("CollisionShape2D").shape.extents = entity_node.sprite_size/2

		var colorRect = entity_node.get_node("ColorRect")
		
		if(entityInst["addTrigger"]):
			var triggerAABB = entityInst["triggerAABB"]
			colorRect.rect_position = Vector2(triggerAABB[0], triggerAABB[1])
			colorRect.rect_size = Vector2(triggerAABB[2]-colorRect.rect_position.x, triggerAABB[3]-colorRect.rect_position.y)
		else:
			colorRect.visible = false
		centerArea.add_child(entity_node)

func load_root_sprite():
	#load root (master) sprite
	var entity_def = singleton.get_entityDef_by_defId(masterDefId)
	if(entity_def == null):
		return
	centerArea.get_node("Sprite").texture = load(entity_def["spritePath"])
	
# Called when the node enters the scene tree for the first time.
func _ready():
	singleton.can_move_map = false
	singleton.cur_editor_mode = singleton.EditorMode.NONE
	max_amount_of_slaves = singleton.entity_types["entitySlavesAmount"]
	get_tree().get_root().connect("size_changed", self, "_on_size_changed")
	load_list_of_entity()
	load_root_sprite()
	load_slaves()
	update_main_area_res()
	entityNameEdit.text = entityName

func _on_size_changed():
	collShapeMain.shape.extents = get_viewport_rect().size

func _process(delta):
	if(in_area):
		var can_create_entity: bool = true
		var entities = get_tree().get_nodes_in_group("entitySceneDef")
		cur_amount_of_slaves = len(entities)
		for entityScene in entities:
			if entityScene.can_move == true:
				can_create_entity = false
				break
		if(can_create_entity):
			if(Input.is_action_just_pressed("mouse1")):
				add_cur_entity()
func update_main_area_res():
	#Not working as espected >:*
	pass
	#area2D_collShape.shape.extents = mainArea.rect_size

func add_cur_entity():
	if(slaveDefId == -1):
		return
	if(cur_amount_of_slaves >= max_amount_of_slaves):
		return
	var mouse_pos = centerArea.get_local_mouse_position()
	var entity_node = entityScene_t.instance()
	entity_node.position = mouse_pos
	
	print("Create entitySlave with defId: ", slaveDefId)
	var entity_def = singleton.get_entityDef_by_defId(slaveDefId)
	
	if(entity_def == null):
		return
	print(entity_def["spritePath"])
	print(entity_def["identifier"])
	if(entity_def["spritePath"]):
		entity_node.get_node("Sprite").texture = load(singleton.cur_project_folder_path+entity_def["spritePath"])
		var temp_sprite_size = singleton.get_sprite_size_from_path(entity_def["spritePath"])
		if temp_sprite_size:
			entity_node.sprite_size = temp_sprite_size
			entity_node.change_sprite_rect(Rect2(0,0,temp_sprite_size.x, temp_sprite_size.y))
			entity_node.get_node("CollisionShape2D").shape.extents = Vector2(temp_sprite_size.x/2, temp_sprite_size.y/2)
		else:
			entity_node.sprite_size = entity_node.get_node("Sprite").texture.get_size()
			entity_node.change_sprite_rect(Rect2(0,0,entity_node.sprite_size.x, entity_node.sprite_size.y))
			entity_node.get_node("CollisionShape2D").shape.extents = entity_node.sprite_size/2
	
	var colorRect = entity_node.get_node("ColorRect")
	if(entity_def["addTrigger"]):
		var triggerAABB = entity_def["triggerAABB"]
		colorRect.rect_position = Vector2(triggerAABB[0], triggerAABB[1])
		colorRect.rect_size = Vector2(triggerAABB[2]-colorRect.rect_position.x, triggerAABB[3]-colorRect.rect_position.y)
	else:
		colorRect.visible = false
		
	var entity_inst = singleton.add_cur_entitySlave(masterDefId, slaveDefId);
	entity_inst["px"] = [int(mouse_pos.x),int(mouse_pos.y)]
	#entity_inst["defId"] = slaveDefId
	entity_node.entityInst_id = entity_inst["instId"]
	entity_node.defId = slaveDefId
	entity_node.masterDefId = masterDefId
	#entity_inst["defId"] = entity_node.defId
	centerArea.add_child(entity_node)

func _on_entity_button_down(ind, text):
	#entityNameEdit.text = text
	print(ind, ", ", text)
	slaveDefId = ind
	

func _on_ExitBtn_button_down():
	singleton.can_move_map = true
	queue_free()


func _on_Area2D_mouse_entered():
	print("in area")
	in_area = true

func _on_Area2D_mouse_exited():
	print("out of area")
	in_area = false
