extends Node

enum EditorMode {
  NONE,
  COLLISION,
  ENTITY,
  POSITION,
}

var cell_size: int = 8

var cur_engine: String = "platformer"

var cur_project_path: String = ""
var cur_project_folder_path: String = ""

var cur_editor_mode: int = EditorMode.NONE
var prev_editor_mode: int = EditorMode.NONE

var cur_entity_type: String
var cur_entity_type_ind: int = -1
var cur_entity_field_ind: int
var cur_entity_field_def_ind: int
var cur_entity_inst_ind: int
var cur_entity_instId: int
var cur_messagePack_ind: int = -1
var cur_message_ind: int = -1

var cur_level_ind: int = 0
var cur_level_layer_ind: int = 0

var entity_names_len: int = 0

var cur_level: int = 1

var entity_snap_to_grid: bool = false

var mergedFieldArr: Array

var can_create_entity_obj: bool = true

var entity_types = {
	"__header__": 
	{
		"fileType": "SGDK Studio Project JSON",
		"app": "SGDK Studio",
		"doc": "???",
		"appAuthor": "bolon667",
		"appVersion": "1.0 alpha",
		"url": "https://github.com/bolon667/SGDK_PlatformerStudio",
	},
	"jsonVersion": "1.0.0",
	"engineRootPath": "./StudioType/SGDK/Engines/platformer",
	"isOneScreen": false, #mb, i will delete that
	"runFromCurrentLevel": false,
	"defaultGridSize": 16,
	"defs": 
	{
		"layers":
			[
				
			],
		"entities": 
			[

			],
		"enums":
			[
				
			],
		"tilesets":
			[
				
			],
		"messagePacks":
			[
				
			],
		"levelFields":
			[
				
			],
	},
	"levels": 
		[
			
		],
	
}

const enum_template = {
	"identifier": "",
	"uid": -1,
	"values": [],
	"tags": [],
}

const enum_val_template = {
	"name": "test",
	"color": 0, 
}

const field_def_template = {	
	"identifier": "Field",
	"__type": "Int",
	"inCodeType": "s16",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "1",
	"canBeDeleted": true,
	"hasStruct": true,
}

const field_def_spriteDef = {	
	"identifier": "sprDef",
	"__type": "Sprite",
	"inCodeType": "SpriteDefinition*",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "NULL",
	"canBeDeleted": false,
	"hasStruct": true,
}

const field_def_triggerRectDef = {	
	"identifier": "Trigger rect",
	"__type": "AABB",
	"inCodeType": "AABB",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "{0, 0, 0, 0}",
	"canBeDeleted": false,
	"hasStruct": false,
}

const field_def_triggerType = {	
	"identifier": "Trigger type",
	"__type": "Integer",
	"inCodeType": "u8",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "0",
	"canBeDeleted": false,
	"hasStruct": false,
}

const field_def_triggerValue = {	
	"identifier": "Trigger value",
	"__type": "Integer",
	"inCodeType": "u8",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "0",
	"canBeDeleted": false,
	"hasStruct": false,
}

const field_def_triggerValue2 = {	
	"identifier": "Trigger value2",
	"__type": "Integer",
	"inCodeType": "u8",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "0",
	"canBeDeleted": false,
	"hasStruct": false,
}

const field_def_triggerValue3 = {	
	"identifier": "Trigger value3",
	"__type": "Integer",
	"inCodeType": "u8",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "0",
	"canBeDeleted": false,
	"hasStruct": false,
}

const field_def_hp = {	
	"identifier": "hp",
	"__type": "Integer",
	"inCodeType": "s16",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "1",
	"canBeDeleted": false,
	"hasStruct": true,
}

const entity_def_template = {
	"identifier": "testEntity",
	"pos": [0,0],
	"tags": [],
	"width": 16,
	"height": 16,
	"show": true,
	"isMerged": true,
	"spritePath": "",
	"addTrigger": false,
	"triggerAABB": [0,0,8,8],
	"triggerType": 1,
	"triggerValue": 1,
	"triggerValue2": 1,
	"triggerValue3": 1,
	"defId": -1, #id of entity definition, for quick search of entity instance, when you changed
	#entity name or field name in entity menu.
	"color": "#0048FF",
	"showName": true,
	"maxCount": -1,
	"limitScope": "PerLevel",
	"fieldDefs": []
}

const startPos_def_template = {
	"identifier": "testEntity",
	"pos": [0,0],
	"tags": [],
	"width": 16,
	"height": 16,
	"show": true,
	"spritePath": "",
	"addTrigger": false,
	"defId": -1, #id of entity definition, for quick search of entity instance, when you changed
	#entity name or field name in entity menu.
	"color": "#0048FF",
	"showName": true,
	"maxCount": 1,
	"limitScope": "PerLevel",
	"fieldDefs": []
}

const level_template = {
	"identifier": "Level1",
	"worldX": 0,
	"worldY": 0,
	"pxWid": 320,
	"pxHei": 224,
	"bgRelPath": "",
	"bgRelPath2": "",
	"bgaMode": 0,
	"bgbMode": 0,
	"musicName": "",
	"beforeLevelScript": "",
	"everyFrameScript": "",
	"afterLevelScript": "",
	"startPos": [0,0],
	"fieldInstances":
		[
			#ignore for now
		],
	"layerInstances":
		[
			
		],
	"messagePacks":
		[
			
		],
}

const level_layer_template = {
	"__identifier": "tLayer",
	"__type": "Collision",
	"__cWid": 320,
	"__cHei": 224,
	"__gridSize": 8,
	"__tilesetRelPath": "",
	"visible": true,
	"intGridCsv": [],
	"entityInstances": []
}

const entity_inst_template = {
	"__identifier": "enemyInstance",
	"__grid": [],
	"__pivot": [],
	"__tags": [],
	"__spritePath": "",
	"__smartColor": "#FF9801",
	"width": 32,
	"height": 32,
	"isMerged": true,
	"px": [0,0],
	"addTrigger": false,
	"triggerType": 1,
	"triggerValue": 1,
	"triggerValue2": 1,
	"triggerValue3": 1,
	"triggerAABB": [0,0,8,8],
	"instId": -1, #id of entity instance, to quicly find in databse
	"defId": -1, #id of entity definition, for quick search of entity instance, when you changed
	#entity name or field name in entity menu.
	"fieldInstances": 
		[
			
		]
}

const messagePack_template = {
	"name": "0",
	"messages": [],
}

const field_inst_template = {
	"__identifier": "fieldInst",
	"__value": "fieldVal",
	"__type": "String",
	"__inCodeType": "string",
	"__sprite": "",
	"fieldId": -1,
	"__hasStruct": true,
}

func change_load_image_mode_bga_all_levels():
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind]["bgaMode"] = entity_types["levels"][cur_level_ind]["bgaMode"]
func change_load_image_mode_bgb_all_levels():
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind]["bgbMode"] = entity_types["levels"][cur_level_ind]["bgbMode"]

func change_load_image_mode_bga(mode: int):
	entity_types["levels"][cur_level_ind]["bgaMode"] = mode
func change_load_image_mode_bgb(mode: int):
	entity_types["levels"][cur_level_ind]["bgbMode"] = mode

func get_level_attr(attr: String):
	if(!entity_types["levels"][cur_level_ind].has(attr)):
		entity_types["levels"][cur_level_ind][attr] = ""
	return entity_types["levels"][cur_level_ind][attr]
		
func get_load_modes():
	if(!entity_types["levels"][cur_level_ind].has("bgaMode")):
		entity_types["levels"][cur_level_ind]["bgaMode"] = 0
	if(!entity_types["levels"][cur_level_ind].has("bgbMode")):
		entity_types["levels"][cur_level_ind]["bgbMode"] = 0
	return[entity_types["levels"][cur_level_ind]["bgaMode"], entity_types["levels"][cur_level_ind]["bgbMode"]];

func change_all_level_attr(attr: String, val):
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind][attr] = val

func change_level_attr(attr: String, val):
	entity_types["levels"][cur_level_ind][attr] = val

func get_trigger_enum():
	for cur_enum in entity_types["defs"]["enums"]:
		if cur_enum["identifier"] == "trigger":
			return cur_enum

func get_show_trigger_rects():
	if !entity_types.has("debugSettings"):
		return false
	if !entity_types["debugSettings"].has("showTriggerRects"):
		return false
	return entity_types["debugSettings"]["showTriggerRects"]

func add_trigger_enum():
	if not entity_types["defs"].has("enums"):
		entity_types["defs"]["enums"] = []
	for cur_enum in entity_types["defs"]["enums"]:
		if cur_enum["identifier"] == "trigger":
			entity_types["defs"]["enums"] = []
			break
	#DEBUG stuff to autoupdate all projects to new enum trigger
	var new_enum = enum_template.duplicate(true)
	new_enum["identifier"] = "trigger"
	var enum_default_val_names: Array = ["change_level", "got_something", "got_damage", "show_message", "execute_custom_script", "force_player_up", "force_player_down", "force_player_left", "force_player_right"]
	for enum_val_name in enum_default_val_names:
		var temp_enum_val = enum_val_template.duplicate(true)
		temp_enum_val["name"] = enum_val_name
		new_enum["values"].append(temp_enum_val)
	entity_types["defs"]["enums"].append(new_enum)

func add_messagePack():
	var data: Dictionary = messagePack_template.duplicate(true)
	entity_types["levels"][cur_level_ind]["messagePacks"].append(data)
	
func add_message():
	if cur_messagePack_ind == -1:
		return
	var data: String = "test message"
	entity_types["levels"][cur_level_ind]["messagePacks"][cur_messagePack_ind]["messages"].append(data)

func delete_cur_messagePack():
	if cur_messagePack_ind == -1:
		return
	entity_types["levels"][cur_level_ind]["messagePacks"].remove(cur_messagePack_ind)
	
func delete_cur_message():
	
	entity_types["levels"][cur_level_ind]["messagePacks"][cur_messagePack_ind]["messages"].remove(cur_message_ind)

func change_messagePack_name(msgPack_name: String):
	entity_types["levels"][cur_level_ind]["messagePacks"][cur_messagePack_ind]["name"] = msgPack_name

func change_cur_message(text: String):
	entity_types["levels"][cur_level_ind]["messagePacks"][cur_messagePack_ind]["messages"][cur_message_ind] = text

func get_project_folder_path():
	print("cur project folder path: ", cur_project_folder_path)
	return cur_project_folder_path

func get_engine_root_path():
	return entity_types["engineRootPath"]

func get_cell_size():
	return cell_size
	
func change_cell_size(new_size: int):
	cell_size = new_size
	entity_types["defaultGridSize"] = cell_size

func _ready():
	add_level()

func is_level_layer_exists(level_num: int, layer_type):
	for layer in entity_types["levels"][level_num]["layerInstances"]:
		if layer_type == layer["__type"]:
			return true
	
	return false



func load_project(projectPath: String):
	var dict = {}
	var file = File.new()
	file.open(projectPath, file.READ)
	var text = file.get_as_text()
	file.close()
	var data_parse = JSON.parse(text)
	cur_project_path = projectPath
	if data_parse.error != OK:
		return
	
	entity_types = data_parse.result
	cell_size = entity_types["defaultGridSize"]
	#autoupdate crunch, should be deleted
	#TODO
	add_trigger_enum()
	


func get_sprite_size_from_path(path:String):
	var result
	var sprite_name = path.substr(path.find_last("/"))
	sprite_name = path.split(".")[0]
	var temp_1 = sprite_name.split("-")
	if len(temp_1) > 1:
		var sprite_info = temp_1[1]
		var info_arr = sprite_info.split("_")
		var t_width: int = int(info_arr[0])
		var t_height: int = int(info_arr[1])
				
		result = Vector2(t_width*8, t_height*8)
	return result

func load_project_last_paths(projects_path: String):
	var paths = {
		"last_project_paths": [],
	}

	var file = File.new()
	var file_path = "./InternalData/lastPath_arr.json"
	if file.file_exists(file_path):
		file.open(file_path, file.READ)
		var text = file.get_as_text()
		file.close()
		var data_parse = JSON.parse(text)
		if data_parse.error != OK:
			return paths
		
		#Checking if folders exists
		var directory = Directory.new();
		for project_name in data_parse.result["last_project_paths"]:
			if(directory.dir_exists(projects_path + project_name)):
				paths["last_project_paths"].append(project_name)
		save_project_last_paths_fast(paths)
		return paths
	else:
		var dir = Directory.new()
		dir.open("./")
		dir.make_dir("InternalData")
		
		file.open(file_path, file.WRITE)
		file.store_string(to_json(paths))
		file.close()
		return paths

func save_project_last_paths_fast(paths_dict: Dictionary):
	var file_path: String = "./InternalData/lastPath_arr.json"
	var file = File.new()
	file.open(file_path, file.WRITE)
	file.store_string(to_json(paths_dict))
	file.close()

func save_project_last_paths():
	var paths = {}
	var file = File.new()
	#Get cur paths
	var file_path = "./InternalData/lastPath_arr.json"
	file.open(file_path, file.READ)
	var text = file.get_as_text()
	file.close()
	var data_parse = JSON.parse(text)
	if data_parse.error != OK:
		return
	var paths_dict =  data_parse.result
	#Add new one
	var project_name:String = cur_project_folder_path.substr(cur_project_folder_path.rfind("/")+1)
	if(project_name == ""):
		return
	if(!paths_dict["last_project_paths"].has(project_name)):
		paths_dict["last_project_paths"].append(project_name)
	#Save
	var file2 = File.new()
	file2.open(file_path, file2.WRITE)
	file2.store_string(to_json(paths_dict))
	file2.close()

func update_cur_project_folder_path(path: String):
	cur_project_folder_path = path

func update_cur_project_path(path: String):
	cur_project_path = path

func create_new_project():
	entity_types["defaultGridSize"] = cell_size
	add_entity_def("EntitySimple")
	add_positionInstance()
	add_trigger_enum()
	save_project()

func get_run_level_ind():
	var lvl_ind: int = 0
	if entity_types.has("runFromCurrentLevel"):
		if entity_types["runFromCurrentLevel"]:
			lvl_ind = cur_level_ind
	return lvl_ind

func save_project():
	print("saving project")
	print("cur_path ", cur_project_path)
	var file = File.new()
	file.open(cur_project_path, File.WRITE)
	file.store_string(to_json(entity_types))
	file.close()
	


func save_collisionMap(tile_map, size: Vector2):
	var intGridCsv_arr: Array = []
	for y in range(size.y):
		for x in range(size.x):
			intGridCsv_arr.append(tile_map.get_cell(x,y)+1)
	var collision_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Collision"):
			break
		collision_layer_ind += 1
	entity_types["levels"][cur_level_ind]["layerInstances"][collision_layer_ind]["intGridCsv"] = intGridCsv_arr
	pass

func change_start_pos(start_pos_coord: Vector2):
	entity_types["levels"][cur_level_ind]["startPos"] = [int(start_pos_coord.x), int(start_pos_coord.y)]

func get_start_pos():
	return entity_types["levels"][cur_level_ind]["startPos"]
	
func get_start_pos_for_level(l_ind: int):
	return entity_types["levels"][l_ind]["startPos"]

func change_level_size(size: Vector2):
	entity_types["levels"][cur_level_ind]["pxWid"] = size.x
	entity_types["levels"][cur_level_ind]["pxHei"] = size.y
	
func get_level_size(levelNum):
	return Vector2(entity_types["levels"][levelNum]["pxWid"], entity_types["levels"][levelNum]["pxHei"]);

func change_bgRelPath_all_levels():
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind]["bgRelPath"] = entity_types["levels"][cur_level_ind]["bgRelPath"]
	
func change_bgRelPath2_all_levels():
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind]["bgRelPath2"] = entity_types["levels"][cur_level_ind]["bgRelPath2"]

func change_bgRelPath(new_bg_path: String):
	entity_types["levels"][cur_level_ind]["bgRelPath"] = new_bg_path
	
func change_bgRelPath2(new_bg_path: String):
	entity_types["levels"][cur_level_ind]["bgRelPath2"] = new_bg_path

func get_bgRelPath():
	return entity_types["levels"][cur_level_ind]["bgRelPath"]
func get_bgRelPath2():
	return entity_types["levels"][cur_level_ind]["bgRelPath2"]
	
func get_bgRelPathForLevel(levelNum:int):
	return entity_types["levels"][levelNum]["bgRelPath"]

func get_collisionMap():
	var collision_layer_ind: int = 0
	#find collision_layer index in array
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Collision"):
			break
		collision_layer_ind += 1
	return entity_types["levels"][cur_level_ind]["layerInstances"][collision_layer_ind]["intGridCsv"]

func get_collisionMapForLevel(levelNum:int):
	var collision_layer_ind: int = 0
	#find collision_layer index in array
	for layer_inst in entity_types["levels"][levelNum]["layerInstances"]:
		if(layer_inst["__type"] == "Collision"):
			break
		collision_layer_ind += 1
	return entity_types["levels"][levelNum]["layerInstances"][collision_layer_ind]["intGridCsv"]


func fix_cur_level_inst_ids():
	var entity_layer_ind: int = -1
	var temp_entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	var entityInst_amount:int = len(entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"])
	var cur_ind: int = 0
	while cur_ind < entityInst_amount:
		entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][cur_ind]["instId"] = cur_ind
		cur_ind += 1
	print("InstIds are fixed on level ", cur_level_ind)

func change_cur_entityInstTriggerAABB(aabb: Array):
	var entity_layer_ind: int = -1
	var temp_entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind == -1:
		return
	entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][cur_entity_inst_ind]["triggerAABB"] = aabb

func change_cur_entityInst(key: String, val):
	var entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			break
		entity_layer_ind += 1
	entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][cur_entity_inst_ind][key] = val

func get_amount_of_levels():
	return len(entity_types["levels"])

func get_messagePack_arr():
	if(!entity_types["levels"][cur_level_ind].has("messagePacks")):
		entity_types["levels"][cur_level_ind]["messagePacks"] = []
	return entity_types["levels"][cur_level_ind]["messagePacks"]
	

func change_entityInstName_by_defId(entity_name: String, defId: int):
	var amount_of_levels = get_amount_of_levels()
	for level_ind in range(amount_of_levels):
		var entity_layer_ind: int = 0
		for layer_inst in entity_types["levels"][level_ind]["layerInstances"]:
			if(layer_inst["__type"] == "Entity"):
				break
			entity_layer_ind += 1
		for entity_inst in entity_types["levels"][level_ind]["layerInstances"][entity_layer_ind]["entityInstances"]:
			if entity_inst["defId"] == defId:
				entity_inst["__identifier"] = entity_name
			continue

func change_entityInstFieldName_by_fieldId(field_name: String, fieldId: int):
	print(entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"])
	print("THIS")
	
	#[cur_entity_field_ind]["fieldInstances"]
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		for field_inst in entity_inst["fieldInstances"]:
			if field_inst["fieldId"] == fieldId:
				field_inst["__identifier"] = field_name
				break


func get_unique_entity_fieldId():
	var fieldId = 0
	for entity_def in entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"]:
		if entity_def["fieldId"] == fieldId:
			fieldId += 1
		continue
	return fieldId
	


func get_unique_entity_instId():
	var instId = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			instId += 1
		continue
	return instId

func get_unique_entity_defId():
	var defId = 0
	for entity_def in entity_types["defs"]["entities"]:
		if entity_def["defId"] == defId:
			defId += 1
		continue
	return defId

func delete_entityInstance(instId: int):
	var posInArray: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"].remove(posInArray)
		posInArray += 1

func save_entityInst_pos(instId: int, posPx: Array):
	var posInArray: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			entity_inst["px"] = [int(posPx[0]), int(posPx[1])]
			entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][posInArray]["px"] = [int(posPx[0]), int(posPx[1])]
			break
		posInArray += 1

func get_level_count():
	var level_count: int = 0
	for level in entity_types["levels"]:
		level_count += 1
	return level_count


func get_positionInstances():
	#Finding entity layer
	var temp_cur_ind:int = 0
	var cur_ind: int = -1
	for val in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if val["__type"] == "Position":
			cur_ind = temp_cur_ind
			break
		temp_cur_ind += 1
	if cur_ind == -1:
		return []
		
	return entity_types["levels"][cur_level_ind]["layerInstances"][cur_ind]["entityInstances"]

func get_entityInstances():
	#Finding entity layer
	var cur_ind: int = 0
	for val in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if val["__type"] == "Entity":
			break
		cur_ind += 1
	return entity_types["levels"][cur_level_ind]["layerInstances"][cur_ind]["entityInstances"]

func change_entityInst_by_instId(instId: int, key:String, val):
	var cur_InstInd:int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			print("instId found")
			print(val)
			print(key)
			entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_InstInd][key] = val
			break
		cur_InstInd+=1

func get_positionInst_by_instId(instId: int):
	#Getting entity layer
	var cur_ind: int = 0
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Position":
			break
		cur_ind += 1
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			return entity_inst

func get_entityInst_ind_by_id(entityInst_id: int):
	var result_ind: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == entityInst_id:
			return result_ind
		result_ind += 1

func get_cur_entityInst():
	#Getting entity layer
	var temp_cur_ind: int = 0
	var cur_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Entity":
			cur_ind = temp_cur_ind
			break
		temp_cur_ind += 1
	return entity_types["levels"][cur_level_ind]["layerInstances"][cur_ind]["entityInstances"][cur_entity_inst_ind]

	


func change_fiendInst_value(fieldName: String, fieldValue):
	print("changed ", fieldName, "value to ", fieldValue)
	var cur_fieldInst_ind: int = 0
	for fieldInst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_entity_inst_ind]["fieldInstances"]:
		if fieldInst["__identifier"] == fieldName:
			break
		cur_fieldInst_ind += 1
	entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_entity_inst_ind]["fieldInstances"][cur_fieldInst_ind]["__value"] = fieldValue
	
func change_sprite_by_instId(spritePath: String):
	var instId = cur_entity_inst_ind
	var cur_inst_ind: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			entity_inst["__spritePath"] = spritePath
			break
		cur_inst_ind += 1

func add_positionInstance_to_level(levelInd: int):
	var entity_instance = {"px": [0,0], "instId": -1}
	entity_instance["instId"] = get_unique_entity_instId()
	var temp_layer_id:int = 0
	var layer_id:int = -1
	for layer in entity_types["levels"][levelInd]["layerInstances"]:
		if layer["__type"] == "Position":
			layer_id = temp_layer_id
			break
		temp_layer_id += 1
	
	if(layer_id == -1):
		return
	entity_types["levels"][levelInd]["layerInstances"][layer_id]["entityInstances"].append(entity_instance)
	return entity_instance

func add_positionInstance():
	var entity_instance = {"px": [0,0], "instId": -1}
	entity_instance["instId"] = get_unique_entity_instId()
	var temp_layer_id:int = 0
	var layer_id:int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Position":
			layer_id = temp_layer_id
			break
		temp_layer_id += 1
	
	if(layer_id == -1):
		return
	entity_types["levels"][cur_level_ind]["layerInstances"][layer_id]["entityInstances"].append(entity_instance)
	return entity_instance

func add_cur_entityInstance():
	var entity_instance = get_cur_entityInstance_t()
	entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"].append(entity_instance)
	return entity_instance

func add_level_layer(level_num: int, layer_name: String, layer_type: String):
	var level_layer_data = level_layer_template.duplicate(true)
	level_layer_data["__identifier"] = layer_name
	level_layer_data["__type"] = layer_type
	entity_types["levels"][level_num]["layerInstances"].append(level_layer_data)

func get_level_name(level_num: int):
	return entity_types["levels"][level_num]["identifier"]

func add_level():
	var level_data = level_template.duplicate(true)
	level_data["identifier"] = "Level_" + str(cur_level_ind)
	level_data["bgRelPath"] = ProjectSettings.globalize_path("res://") + "InternalData/defaultBG.png"
	#if len(entity_types["levels"]) > 0:
	#	var prev_level = entity_types["levels"][cur_level_ind]
	#	level_data["pxHei"] = prev_level["pxHei"]
	#	level_data["pxWid"] = prev_level["pxWid"]
	#	level_data["bgRelPath"] = prev_level["bgRelPath"]
	#	level_data["bgRelPath2"] = prev_level["bgRelPath2"]
	
	entity_types["levels"].append(level_data)
	
	var last_level_ind: int = len(entity_types["levels"])-1
	#Add layers
	
	add_level_layer(last_level_ind, "Entity", "Entity")
	add_level_layer(last_level_ind, "Positions", "Position")
	add_level_layer(last_level_ind, "Collision", "Collision")
	
	#Add position instance
	add_positionInstance_to_level(last_level_ind)
	
	#cur_level_ind += 1
	#cur_level += 1

func delete_cur_level():
	var level_count: int = get_level_count()
	if(level_count < 2):
		#You must have at least one level
		return
	entity_types["levels"].remove(cur_level_ind)
	level_count = get_level_count()
	if(cur_level_ind > level_count-1):
		cur_level_ind = level_count-1
	

func change_cur_field(field_property_name: String, field_property_value: String):
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind][field_property_name] = field_property_value
	pass

func get_cur_fieldDef_name():
	return entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["identifier"]

func change_name_of_cur_fieldDef(text: String):
	
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["identifier"] = text
	var fieldId = entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["fieldId"]
	change_entityInstFieldName_by_fieldId(text, fieldId)
	print(entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind])
	
func get_entityMeged_ids_dict():
	var cur_ind: int = 0
	var dict:Dictionary = {}
	for entity in entity_types["defs"]["entities"]:
		var entity_name = entity["identifier"]
		if !dict.has(entity_name):
			dict[entity_name] = cur_ind
			cur_ind += 1
	return dict

func get_cur_level_layer_names():
	var layer_names = []
	var layers = get_cur_level_layers()
	for layer in layers:
		layer_names.append(layer["__identifier"])
	return layer_names

func get_cur_level_layers():
	return entity_types["levels"][cur_level_ind]["layerInstances"]

func get_level(curLevel: int):
	return entity_types["levels"][curLevel]

func get_cur_level():
	return entity_types["levels"][cur_level_ind]

func get_cur_entityDef():
	return entity_types["defs"]["entities"][cur_entity_type_ind]

func get_entity_fields():
	return entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"]

func get_entityInstanAmount_by_levelNum(level_num: int):
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Entity":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind < 0:
		return
	return len(entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["entityInstances"])

func get_messagePacks_by_levelNum(level_num: int):
	if(entity_types["levels"][level_num].has("messagePacks")):
		return entity_types["levels"][level_num]["messagePacks"]

func get_positionInstances_by_levelNum(level_num: int):
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Position":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind < 0:
		return
	
	return entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["entityInstances"]

func get_positionInstancesAmount_by_levelNum(level_num: int):
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Position":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind < 0:
		return
	
	return len(entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["entityInstances"])


	
func get_entityInstances_by_levelNum(level_num: int):
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Entity":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind < 0:
		return
	
	return entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["entityInstances"]

func get_cur_entity_field_names():
	var entity_field_names = []
	if(entity_names_len == 0):
		 return entity_field_names
	for entity_field in entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"]:
		entity_field_names.append(entity_field["identifier"])
	return entity_field_names

func get_entity_field_names(entity_name: String):
	var entity_field_names = []
	var ent_ind = -1
	var temp_ent_ind = 0
	#find entity by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			ent_ind = temp_ent_ind
			break
		temp_ent_ind += 1
	if ent_ind == -1:
		return []
	#check all fields for this entity type
	for entity_field in entity_types["defs"]["entities"][ent_ind]["fieldDefs"]:
		entity_field_names.append(entity_field["identifier"])

	return entity_field_names
	
func get_entity_fields_backup_posibly_need_to_delete(entity_name: String):
	var entity_fields = []
	var ent_ind = -1
	var temp_ent_ind = 0
	#find entity ind by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			ent_ind = temp_ent_ind
			break
		temp_ent_ind += 1
	if ent_ind == -1:
		return []
	#check all fields for this entity type
	

	return entity_types["defs"]["entities"][ent_ind]["fieldDefs"]

func get_cur_entity_one_field():
	return entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]

func get_entity_types():
	return entity_types

func get_entity_one_field(entity_name: String, field_name: String):
	#TODO: check if it works???!?!??
	var ent_ind = -1
	var temp_ent_ind = 0
	#find entity ind by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			ent_ind = temp_ent_ind
			break
		temp_ent_ind += 1
	if ent_ind == -1:
		return {}
	#check all fields for this entity type
	
	for field in entity_types["defs"]["entities"][ent_ind]["fieldDefs"]:
		if(field["identifier"] == field_name):
			return field

func get_entity_names():
	var entity_names = []
	for entity in entity_types["defs"]["entities"]:
		
		entity_names.append(entity["identifier"])
	return entity_names
	
func get_def_entity_names():
	var entity_names = []
	for entity in entity_types["defs"]["entities"]:
		if entity["show"]:
			entity_names.append(entity["identifier"])
	return entity_names


func get_cur_entityInstance_t():
	var def = entity_types["defs"]["entities"][cur_entity_type_ind].duplicate(true)
	var entity_inst = entity_inst_template.duplicate(true)
	entity_inst["__identifier"] = def["identifier"]
	entity_inst["triggerAABB"] = def["triggerAABB"]
	entity_inst["triggerType"] = def["triggerType"]
	entity_inst["triggerValue"] = def["triggerValue"]
	if(!def.has("triggerValue2")):
		def["triggerValue2"] = 0
	entity_inst["triggerValue2"] = def["triggerValue2"]
	if(!def.has("triggerValue3")):
		def["triggerValue3"] = 0
	entity_inst["triggerValue3"] = def["triggerValue3"]
	entity_inst["addTrigger"] = def["addTrigger"]
	
	#Loading sprite path
	entity_inst["__spritePath"] = def["spritePath"]
	
	entity_inst["defId"] = def["defId"]
	entity_inst["instId"] = get_unique_entity_instId()
	var filed_inst_arr = []
	#Get all fields from entityDef, and copy name,value to fields in entityInstance
	#Entity definition - template for entity
	#Entity instance - actual entity
	for field in def["fieldDefs"]:	
		var field_inst = field_inst_template.duplicate(true)
		field_inst["__identifier"] = field["identifier"]
		field_inst["__value"] = field["defaultValue"]
		field_inst["__type"] = field["__type"]
		field_inst["fieldId"] = field["fieldId"]
		field_inst["__hasStruct"] = field["hasStruct"]
		filed_inst_arr.append(field_inst)
		if(field["identifier"] == "Trigger type"):
			entity_inst["triggerType"] = int(field["defaultValue"])
		elif(field["identifier"] == "Trigger value"):
			entity_inst["triggerValue"] = int(field["defaultValue"])
		elif(field["identifier"] == "Trigger value2"):
			entity_inst["triggerValue2"] = int(field["defaultValue"])
		elif(field["identifier"] == "Trigger value3"):
			entity_inst["triggerValue3"] = int(field["defaultValue"])
			
	entity_inst["fieldInstances"] = filed_inst_arr
	return entity_inst
	

func get_entityDef_by_defId(defId: int):
	for entityDef in entity_types["defs"]["entities"]:
		if entityDef["defId"] == defId:
			return entityDef

func get_entityDef_by_ind(entity_ind: int):
	return entity_types["defs"]["entities"][entity_ind]

func get_entity_defs():
	return entity_types["defs"]["entities"]

func get_entityDef(entity_name: String):
	var ind = 0
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			return {"ind": ind,"val": entity}
		ind += 1
	return {"ind": 0,"val": []}

func add_fieldDef_to_entity(fieldDef: Dictionary):
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"].append(fieldDef.duplicate(true))

func add_field_to_entity(entity_name: String, field_name:String):
	var ent_ind:int = -1
	var temp_ent_ind:int = 0
	#find entity ind by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			ent_ind = temp_ent_ind
			break
		temp_ent_ind += 1
	if ent_ind == -1:
		return
	var field_def_data = field_def_template.duplicate(true)
	field_def_data["identifier"] = field_name
	field_def_data["fieldId"] = get_unique_entity_fieldId()
	entity_types["defs"]["entities"][ent_ind]["fieldDefs"].append(field_def_data)


func add_entity_def(entity_name: String):
	var entity_def_data = entity_def_template.duplicate(true)
	entity_def_data["identifier"] = entity_name
	entity_def_data["defId"] = get_unique_entity_defId()
	entity_types["defs"]["entities"].append(entity_def_data)
	
	cur_entity_type_ind = -1
	
	add_fieldDef_to_entity(field_def_spriteDef)
	add_fieldDef_to_entity(field_def_triggerRectDef)
	add_fieldDef_to_entity(field_def_triggerType)
	add_fieldDef_to_entity(field_def_triggerValue)
	add_fieldDef_to_entity(field_def_triggerValue2)
	add_fieldDef_to_entity(field_def_triggerValue3)
	add_fieldDef_to_entity(field_def_hp)
	
	print(entity_def_data)
	

func get_entity_field(entity_name: String, field_name:String):
	var ent_ind = 0
	var result = {"entityInd": 0, "fieldInd": 0, "val": null}
	#find entity by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			break
		ent_ind += 1
	result["entityInd"] = ent_ind
	var field_ind = 0
	for entity_field in entity_types["defs"]["entities"][ent_ind]:
		#print(entity_field)
		continue
		if(entity_field["identifier"] == field_name):
			result["fieldInd"] = field_ind
			result["val"] = entity_types["defs"]["entities"][ent_ind]["fieldDefs"][field_ind]
			break
		field_ind += 1
	
	return result


func get_merged_fieldDef():
	var mergedFieldDef_arr = []
	var mergedFieldDef_names = []
	for entity in entity_types["defs"]["entities"]:
		for field in entity["fieldDefs"]:
			var fieldDef_name = field["identifier"]
			if !(fieldDef_name in mergedFieldDef_names):
				mergedFieldDef_arr.append(field)
				mergedFieldDef_names.append(fieldDef_name)
				pass
	mergedFieldArr = mergedFieldDef_arr
	return mergedFieldDef_arr


func get_merged_fieldDefs_v2():
	var result: Dictionary = {}
	
	var mergedFields_arr = get_merged_fieldDef()
	
	#for ALL entity types
	for entityDef in entity_types["defs"]["entities"]:
		var temp_mergedFieldDef_arr = []
		for mergedField in mergedFields_arr: 
			#var replace_field_val: bool = false
			var applyFieldVal = mergedField
			for entityField in entityDef["fieldDefs"]:	
				#check if cur entity contains mergedField
				if mergedField["identifier"] == entityField["identifier"]:
					#if so, then replacing default merged value with new one
					
					#replace_field_val = true
					applyFieldVal = entityField
					break
			temp_mergedFieldDef_arr.append(applyFieldVal)
		result[entityDef["identifier"]] = temp_mergedFieldDef_arr.duplicate(true)
	return result

func delete_fieldDef(field_ind: int):
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"].remove(field_ind)
	

func delete_entityDef(entity_name: String):
	var entity_def = get_entityDef(entity_name)
	entity_types["defs"]["entities"].remove(entity_def["ind"])
	
	delete_entityDef_from_game(entity_def["ind"])

func delete_entityDef_from_game(defId: int):
	var level_amount = get_level_count()
	var cur_level: int = 0
	while cur_level < level_amount:
		delete_entityDef_from_level(cur_level, defId)
		cur_level += 1
		
	

func delete_entityDef_from_level(levelInd: int, defId: int):
	var layer_ind: int = -1
	var temp_layer_ind: int = 0
	for layer in entity_types["levels"][levelInd]["layerInstances"]:
		if layer["__type"] == "Entity":
			layer_ind = temp_layer_ind
		temp_layer_ind += 1
	if layer_ind == -1:
		return
	
	var new_entityInst_arr = []
	for entity_inst in  entity_types["levels"][levelInd]["layerInstances"][layer_ind]["entityInstances"]:
		if entity_inst["defId"] == defId:
			continue
		new_entityInst_arr.append(entity_inst)
	entity_types["levels"][levelInd]["layerInstances"][layer_ind]["entityInstances"] = new_entityInst_arr

func is_entity_name_exists(entity_name: String):
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			return true
	return false
func is_entity_have_field(entity_name: String, field_name: String):
	var ent
	#find entity by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			ent = entity
			break
	if(!ent):
		return false
	var fieldDefs = ent["fieldDefs"]
	
	var cur_field_ind = 0
	for fieldDef in fieldDefs:
		print("is_entity_have_field: ", ent["fieldDefs"][cur_field_ind]["identifier"])
		if ent["fieldDefs"][cur_field_ind]["identifier"] == field_name:
			
			return true
			break
		cur_field_ind += 1
	
	return false

func get_entityDefs():
	return entity_types["defs"]["entities"]

#"testEntity": {"pos": [0,0], "spriteDefinition": "&spr_test"},
