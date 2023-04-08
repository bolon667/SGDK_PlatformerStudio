extends Node

enum EditorMode {
  NONE,
  COLLISION,
  ENTITY,
  POSITION,
  LEVEL_MOVE,
}

var cell_size: int = 8

var cur_engine: String = "platformer"

var cur_project_path: String = ""
var cur_project_folder_path: String = ""

var cur_editor_mode: int = EditorMode.NONE
var prev_editor_mode: int = EditorMode.NONE

var in_modal_window: bool = false

var cur_entity_type: String
var cur_entity_type_ind: int = -1
var cur_entity_field_ind: int
var cur_entity_field_def_ind: int
var cur_entity_inst_ind: int
var cur_entity_instId: int
var cur_messagePack_ind: int = -1
var cur_message_ind: int = -1

var draw_tile_ind:int = 0

var cur_level_ind: int = 0
var cur_level_layer_ind: int = 0

var entity_names_len: int = 0

var cur_level: int = 1

var entity_snap_to_grid: bool = false

var mergedFieldArr: Array
var level_move_mode: bool = false


var entity_types = {
	"__header__": 
	{
		"fileType": "PS4SGDK Project JSON",
		"app": "Platformer Studio For SGDK",
		"doc": "???",
		"appAuthor": "bolon667",
		"appVersion": "1.4 alpha",
		"url": "https://github.com/bolon667/SGDK_PlatformerStudio",
	},
	"jsonVersion": "1.0.0",
	"engineRootPath": "./StudioType/SGDK/Engines/platformer",
	"isOneScreen": false, #mb, i will delete this
	"runFromCurrentLevel": false,
	"defaultGridSize": 16,
	"addEntityMergedSlots": 10,
	"addEntityBulletMergedSlots": 10,
	"entityLoadOptimization": 0,
	"chunkSizeX": 80,
	"chunkSizeY": 80,
	"turnOnGates": false,
	"defs": 
	{
		"layers":
			[
				
			],
		"entities": 
			[

			],
		"bulletEntities":
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
		"variables":
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

const field_def_damage = {	
	"identifier": "damage",
	"__type": "Integer",
	"inCodeType": "s16",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "1",
	"canBeDeleted": false,
	"hasStruct": true,
}

const field_def_damaged = {
	"identifier": "damaged",
	"__type": "Bool",
	"inCodeType": "bool",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "FALSE",
	"canBeDeleted": false,
	"hasStruct": true,
}

const field_def_onGround = {
	"identifier": "onGround",
	"__type": "Bool",
	"inCodeType": "bool",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "FALSE",
	"canBeDeleted": false,
	"hasStruct": true,
}

const bulletsEntity_def_template = {
	"identifier": "testBulletEntity",
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
	"triggerTypeName": "",
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
	"triggerTypeName": "",
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
	"pal0SpriteName": "",
	"pal1SpriteName": "",
	"pal2SpriteName": "",
	"pal3SpriteName": "",
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
	"localVars":
		[
		
		],
}

const level_layer_template = {
	"__identifier": "Layer",
	"__type": "Collision",
	"__cWid": 320,
	"__cHei": 224,
	"__gridSize": 8,
	"__tilesetRelPath": "",
	"visible": true,
	"intGridCsv": [],
	"entityInstances": [],
	"gateInstances": [],
}

const bulletEntity_inst_template = {
	"__identifier": "bulletInstance",
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

func change_load_image_mode_bga(mode: int, level_ind:int):
	entity_types["levels"][level_ind]["bgaMode"] = mode
func change_load_image_mode_bgb(mode: int, level_ind:int):
	entity_types["levels"][level_ind]["bgbMode"] = mode

func get_level_attr(attr: String, level_ind: int):
	if(!entity_types["levels"][level_ind].has(attr)):
		entity_types["levels"][level_ind][attr] = ""
	return entity_types["levels"][level_ind][attr]
		
func get_load_modes(level_ind: int):
	if(!entity_types["levels"][level_ind].has("bgaMode")):
		entity_types["levels"][level_ind]["bgaMode"] = 0
	if(!entity_types["levels"][level_ind].has("bgbMode")):
		entity_types["levels"][level_ind]["bgbMode"] = 0
	return[entity_types["levels"][level_ind]["bgaMode"], entity_types["levels"][level_ind]["bgbMode"]];

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
	#Autoupdating project to new enum trigger
	var new_enum = enum_template.duplicate(true)
	new_enum["identifier"] = "trigger"
	var enum_default_val_names: Array = get_files_no_ext(cur_project_folder_path + "/code_template/triggerType/")
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
	
func get_entity_load_opt_mode():
	return entity_types["entityLoadOptimization"]
	
func change_cell_size(new_size: int):
	cell_size = new_size
	entity_types["defaultGridSize"] = cell_size

func _ready():
	add_level(Vector2.ZERO)

func is_level_layer_exists(level_num: int, layer_type):
	for layer in entity_types["levels"][level_num]["layerInstances"]:
		if layer_type == layer["__type"]:
			return true
	
	return false

func change_addTrigger_all_entityInst_by_defId(defId: int, val: bool):
	for level in entity_types["levels"]:
		#Finding EntityLayer
		for layer in level["layerInstances"]:
			if layer["__identifier"] != "Entity":
				continue
			for entityInst in layer["entityInstances"]:
				if entityInst["defId"] != defId:
					continue
				entityInst["addTrigger"] = val
	print("addTrigger for ALL defId=", defId, " changed to ", val)

func change_property_all_entityInst_by_defId(property_name: String, property_value, defId: int):
	for level in entity_types["levels"]:
		#Finding EntityLayer
		for layer in level["layerInstances"]:
			if layer["__identifier"] != "Entity":
				continue
			for entityInst in layer["entityInstances"]:
				if entityInst["defId"] != defId:
					continue
				if(property_name == "Trigger rect"):
					var temp_str: String = property_value.substr(1, property_value.rfind("}"))
					var rect_arr = temp_str.split(",")
					var result_arr = [int(rect_arr[0]),int(rect_arr[1]),int(rect_arr[2]),int(rect_arr[3])]
					entityInst["triggerAABB"] = result_arr
				if(property_name == "Trigger type"):
					entityInst["triggerType"] = int(property_value)
				if(property_name == "Trigger type name"):
					entityInst["triggerTypeName"] = str(property_value)
				if(property_name == "Trigger value"):
					entityInst["triggerValue"] = int(property_value)
				if(property_name == "Trigger value2"):
					entityInst["triggerValue2"] = int(property_value)
				if(property_name == "Trigger value3"):
					entityInst["triggerValue3"] = int(property_value)
			
				for fieldInst in entityInst["fieldInstances"]:
					if fieldInst["__identifier"] == property_name:
						fieldInst["__value"] = property_value
						break
	
	print("Property ", property_name, " for ALL defId=", defId, " changed to ", property_value)

func update_project():
	print("autoupdate project")
	#Add bulletEntites def if not exists
	if(!entity_types["defs"].has("bulletEntities")):
		entity_types["defs"]["bulletEntities"] = []
	if !entity_types["defs"].has("variables"):
		entity_types["defs"]["variables"] = []
	#Add arr entityBulletInstances arr to all entities
	for level in entity_types["levels"]:
		var entityLayer = level["layerInstances"][0]
		if !entityLayer.has("entityBulletInstances"):
			entityLayer["entityBulletInstances"] = []
	if(!entity_types.has("chunkSizeX")):
		entity_types["chunkSizeX"] = 80;
	if(!entity_types.has("chunkSizeY")):
		entity_types["chunkSizeY"] = 80;
	#autoupdate triggers
	add_trigger_enum()

func get_chunkSizeX():
	return entity_types["chunkSizeX"] 
	
func get_chunkSizeY():
	return entity_types["chunkSizeY"] 

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
	
	update_project()


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

func get_files_no_ext(path):
	var files = []
	var dir = Directory.new()
	dir.open(path)
	dir.list_dir_begin(true)

	var file = dir.get_next()
	while file != '':
		file = file.substr(0, file.find("."))
		files += [file]
		file = dir.get_next()

	return files

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
	add_entity_def_to_entityCollection("entities", "EntitySimple")
	add_entity_def_to_entityCollection("bulletEntities", "BulletSimple")
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
	


func save_collisionMap(tile_map, size: Vector2, level_ind: int):
	var intGridCsv_arr: Array = []
	for y in range(size.y):
		for x in range(size.x):
			intGridCsv_arr.append(tile_map.get_cell(x,y)+1)
	var collision_layer_ind: int = 0
	for layer_inst in entity_types["levels"][level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Collision"):
			break
		collision_layer_ind += 1
	entity_types["levels"][level_ind]["layerInstances"][collision_layer_ind]["intGridCsv"] = intGridCsv_arr
	pass

func change_start_pos(start_pos_coord: Vector2):
	entity_types["levels"][cur_level_ind]["startPos"] = [int(start_pos_coord.x), int(start_pos_coord.y)]

func get_start_pos():
	return entity_types["levels"][cur_level_ind]["startPos"]
	
func get_start_pos_for_level(l_ind: int):
	return entity_types["levels"][l_ind]["startPos"]

func change_level_size(level_ind, size: Vector2):
	entity_types["levels"][level_ind]["pxWid"] = size.x
	entity_types["levels"][level_ind]["pxHei"] = size.y
	
func get_level_size(levelNum):
	return Vector2(entity_types["levels"][levelNum]["pxWid"], entity_types["levels"][levelNum]["pxHei"]);

func change_bgRelPath_all_levels():
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind]["bgRelPath"] = entity_types["levels"][cur_level_ind]["bgRelPath"]
	
func change_bgRelPath2_all_levels():
	for level_ind in range(len(entity_types["levels"])):
		entity_types["levels"][level_ind]["bgRelPath2"] = entity_types["levels"][cur_level_ind]["bgRelPath2"]

func change_bgRelPath(new_bg_path: String, level_ind: int):
	entity_types["levels"][level_ind]["bgRelPath"] = new_bg_path
	
func change_bgRelPath2(new_bg_path: String, level_ind: int):
	entity_types["levels"][cur_level_ind]["bgRelPath2"] = new_bg_path

func get_bgRelPath(level_ind: int):
	return entity_types["levels"][level_ind]["bgRelPath"]
func get_bgRelPath2(level_ind: int):
	return entity_types["levels"][level_ind]["bgRelPath2"]
	
func get_bgRelPathForLevel(levelNum:int):
	return entity_types["levels"][levelNum]["bgRelPath"]

func get_worldPos_vector2(level_ind: int):
	var resultPos: Vector2 = Vector2(entity_types["levels"][level_ind]["worldX"], entity_types["levels"][level_ind]["worldY"])
	#Make sure that the levels are in different positions on world map
	#For that, checking every level position
	#Fix world position, just in case
	#entity_types["levels"][level_ind]["worldX"] = resultPos.x
	return resultPos

func get_collisionMap(level_ind: int):
	var collision_layer_ind: int = 0
	#find collision_layer index in array
	for layer_inst in entity_types["levels"][level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Collision"):
			break
		collision_layer_ind += 1
	return entity_types["levels"][level_ind]["layerInstances"][collision_layer_ind]["intGridCsv"]

func get_collisionMapForLevel(levelNum:int):
	var collision_layer_ind: int = 0
	#find collision_layer index in array
	for layer_inst in entity_types["levels"][levelNum]["layerInstances"]:
		if(layer_inst["__type"] == "Collision"):
			break
		collision_layer_ind += 1
	return entity_types["levels"][levelNum]["layerInstances"][collision_layer_ind]["intGridCsv"]

func remove_gates(level_ind: int):
	entity_types["levels"][level_ind]["layerInstances"][0]["gateInstances"] = []
func fix_level_inst_ids(level_ind):
	
	var entity_layer_ind: int = -1
	var temp_entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	var entityInst_amount:int = len(entity_types["levels"][level_ind]["layerInstances"][entity_layer_ind]["entityInstances"])
	var cur_ind: int = 0
	while cur_ind < entityInst_amount:
		entity_types["levels"][level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][cur_ind]["instId"] = cur_ind
		cur_ind += 1
	print("InstIds are fixed on level ", level_ind)

func change_entityInstTriggerAABB(aabb: Array, level_ind: int, entityInst_id: int):
	var entity_layer_ind: int = -1
	var temp_entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind == -1:
		return
	entity_types["levels"][level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][entityInst_id]["triggerAABB"] = aabb

func change_cur_entityInst(key: String, val):
	var entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			break
		entity_layer_ind += 1
	entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][cur_entity_inst_ind][key] = val

func get_amount_of_levels():
	return len(entity_types["levels"])

func get_messagePack_arr(level_ind:int):
	if(!entity_types["levels"][level_ind].has("messagePacks")):
		entity_types["levels"][level_ind]["messagePacks"] = []
	return entity_types["levels"][level_ind]["messagePacks"]
	

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

func change_cur_fieldInst(field_name: String, field_val):
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		for field_inst in entity_inst["fieldInstances"]:
			if field_inst["__identifier"] == field_name:
				field_inst["__value"] = field_val

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
	var lastInstId: int = 0
	#find last instId in entityInstances arr
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if lastInstId < int(entity_inst["instId"]):
			lastInstId = entity_inst["instId"]
	#find last instId in gateInstances arr
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["gateInstances"]:
		if lastInstId < int(entity_inst["instId"]):
			lastInstId = entity_inst["instId"]
	# +1 to make sure, that instId is unique
	return lastInstId+1
	
func get_unique_entity_instId_level_ind(level_ind):
	var lastInstId: int = 0
	#find last instId in entityInstances arr
	for entity_inst in entity_types["levels"][level_ind]["layerInstances"][0]["entityInstances"]:
		if lastInstId < int(entity_inst["instId"]):
			lastInstId = entity_inst["instId"]
	#find last instId in gateInstances arr
	for entity_inst in entity_types["levels"][level_ind]["layerInstances"][0]["gateInstances"]:
		if lastInstId < int(entity_inst["instId"]):
			lastInstId = entity_inst["instId"]
	# +1 to make sure, that instId is unique
	return lastInstId+1

func get_unique_entity_defId():
	var defId = 0
	#find last instId
	for entity_def in entity_types["defs"]["entities"]:
		if defId < int(entity_def["defId"]):
			defId = entity_def["defId"]
		continue
	# +1 to make sure, that defId is unique
	return defId+1



func delete_gateInstance(level_ind:int, instId: int):
	var posInArray: int = 0
	if(!entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind].has("gateInstances")):
		entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["gateInstances"] = []
	for gate_inst in entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["gateInstances"]:
		if gate_inst["instId"] == instId:
			entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["gateInstances"].remove(posInArray)
		posInArray += 1

func delete_entityInstance(instId: int, level_ind: int):
	var posInArray: int = 0
	for entity_inst in entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"].remove(posInArray)
		posInArray += 1

func save_gateInst_pos(instId: int, posPx: Array):
	var posInArray: int = 0
	for gate_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["gateInstances"]:
		if gate_inst["instId"] == instId:
			gate_inst["px"] = [int(posPx[0]), int(posPx[1])]
			entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["gateInstances"][posInArray]["px"] = [int(posPx[0]), int(posPx[1])]
			break
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


func get_positionInstances(level_ind: int):
	#Finding entity layer
	var temp_cur_ind:int = 0
	var cur_ind: int = -1
	for val in entity_types["levels"][level_ind]["layerInstances"]:
		if val["__type"] == "Position":
			cur_ind = temp_cur_ind
			break
		temp_cur_ind += 1
	if cur_ind == -1:
		return []
		
	return entity_types["levels"][level_ind]["layerInstances"][cur_ind]["entityInstances"]

func get_entityInstances(level_ind: int):
	#Finding entity layer
	var cur_ind: int = 0
	for val in entity_types["levels"][level_ind]["layerInstances"]:
		if val["__type"] == "Entity":
			break
		cur_ind += 1
	return entity_types["levels"][level_ind]["layerInstances"][cur_ind]["entityInstances"]

func change_entityInst_by_instId(instId: int, level_ind: int, key:String, val):
	var cur_InstInd:int = 0
	for entity_inst in entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			print("instId found")
			print(val)
			print(key)
			entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_InstInd][key] = val
			break
		cur_InstInd+=1

func get_positionInst_by_instId(instId: int, level_ind: int):
	#Getting entity layer
	var cur_ind: int = 0
	for layer in entity_types["levels"][level_ind]["layerInstances"]:
		if layer["__type"] == "Position":
			break
		cur_ind += 1
	for entity_inst in entity_types["levels"][level_ind]["layerInstances"][cur_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			return entity_inst

func get_entityInst_ind_by_id(entityInst_id: int):
	var layer_ind: int = 0
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Entity":
			break
		layer_ind += 1
	var result_ind: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][layer_ind]["entityInstances"]:
		if entity_inst["instId"] == entityInst_id:
			return result_ind
		result_ind += 1

func get_entityInst_by_instInd_levelInd(instId:int, levelInd:int):
	#Getting entity layer
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][levelInd]["layerInstances"]:
		if layer["__type"] == "Entity":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	#Getting entity_inst_ind
	var entity_inst_ind: int = -1
	var temp_entity_inst_ind: int = 0
	for entityInst in entity_types["levels"][levelInd]["layerInstances"][entity_layer_ind]["entityInstances"]:
		if entityInst["instId"] == instId:
			entity_inst_ind = temp_entity_inst_ind
			break
		temp_entity_inst_ind += 1
	if(entity_inst_ind == -1):
		return
	return entity_types["levels"][levelInd]["layerInstances"][entity_layer_ind]["entityInstances"][entity_inst_ind]

	


func change_fiendInst_value_by_levelInd_entityInstId(fieldName: String, fieldValue, level_ind, entityInst_id):
	print("changed ", fieldName, "value to ", fieldValue)
	var inst_ind: int = 0
	#finding instId
	for entity_inst in entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == entityInst_id:
			break
		inst_ind += 1
	var cur_fieldInst_ind: int = 0
	for fieldInst in entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][inst_ind]["fieldInstances"]:
		if fieldInst["__identifier"] == fieldName:
			break
		cur_fieldInst_ind += 1
	entity_types["levels"][level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][inst_ind]["fieldInstances"][cur_fieldInst_ind]["__value"] = fieldValue
	
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
	
func add_gateInstance(level_ind: int, gate_node):
	var gate_instance = get_gateInstance_t_defId(0, level_ind)
	gate_instance["triggerAABB"] = gate_node.triggerAABB
	gate_instance["height"] = gate_node.triggerAABB[3]
	gate_instance["width"] = gate_node.triggerAABB[2]	
	gate_instance["px"] = [int(gate_node.position.x), int(gate_node.position.y)]
	gate_instance["triggerValue"] = gate_node.to_level_id
	gate_instance["triggerValue2"] = gate_node.side_id
	gate_instance["triggerValue3"] = gate_node.side_offset
	
	var layer_ind = 0
	if(!entity_types["levels"][level_ind]["layerInstances"][layer_ind].has("gateInstances")):
		entity_types["levels"][level_ind]["layerInstances"][layer_ind]["gateInstances"] = []
	entity_types["levels"][level_ind]["layerInstances"][layer_ind]["gateInstances"].append(gate_instance)
	return gate_instance
	

func add_level_layer(level_num: int, layer_name: String, layer_type: String):
	var level_layer_data = level_layer_template.duplicate(true)
	level_layer_data["__identifier"] = layer_name
	level_layer_data["__type"] = layer_type
	entity_types["levels"][level_num]["layerInstances"].append(level_layer_data)

func get_level_name(level_num: int):
	return entity_types["levels"][level_num]["identifier"]

func add_level(level_world_pos: Vector2):
	var level_data = level_template.duplicate(true)
	level_data["identifier"] = "Level_" + str(cur_level_ind)
	level_data["bgRelPath"] = ProjectSettings.globalize_path("res://") + "InternalData/defaultBG.png"
			
	level_data["worldX"] = level_world_pos.x
	level_data["worldY"] = level_world_pos.y
	
	
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

func delete_level(level_ind: int):
	var level_count: int = get_level_count()
	if(level_count < 2):
		#You must have at least one level
		return
	entity_types["levels"].remove(level_ind)
	level_count = get_level_count()
	

func change_fieldDef(entityCollectionDef: String, field_property_name: String, field_property_value: String):
	for fieldDef in entity_types["defs"][entityCollectionDef][cur_entity_type_ind]["fieldDefs"]:
		if fieldDef["identifier"] == field_property_name:
			fieldDef["defaultValue"] = field_property_value
			break
	pass

func change_cur_field(entityCollectionDef: String, field_property_name: String, field_property_value: String):
	entity_types["defs"][entityCollectionDef][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind][field_property_name] = field_property_value
	pass

func get_cur_fieldDef_name():
	return entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["identifier"]

func change_name_of_cur_fieldDef(text: String):
	
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["identifier"] = text
	var fieldId = entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["fieldId"]
	change_entityInstFieldName_by_fieldId(text, fieldId)
	print(entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind])
	
func get_entityMeged_ids_dict(entityCollectionDef: String):
	var cur_ind: int = 0
	var dict:Dictionary = {}
	for entity in entity_types["defs"][entityCollectionDef]:
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


func get_cur_entityDef(entityCollection: String):
	return entity_types["defs"][entityCollection][cur_entity_type_ind]

func get_entityCollection_fields(entityCollection: String):
	return entity_types["defs"][entityCollection][cur_entity_type_ind]["fieldDefs"]

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
	var amount = len(entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["entityInstances"])
	amount += len(entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["gateInstances"])
	return amount

func get_TriggerAmount_by_levelNum(level_num: int):
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Entity":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind < 0:
		return
	#Find amount of triggers
	var triggerAmount:int = 0
	for entityInstance in entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["entityInstances"]:
		if entityInstance.has("addTrigger"):
			if(entityInstance["addTrigger"]):
				triggerAmount += 1
	triggerAmount += len(entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["gateInstances"])
	return triggerAmount


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

func get_gateInstances_by_levelNum(level_num: int):
	var temp_entity_layer_ind: int = 0
	var entity_layer_ind: int = -1
	for layer in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if layer["__type"] == "Entity":
			entity_layer_ind = temp_entity_layer_ind
			break
		temp_entity_layer_ind += 1
	if entity_layer_ind < 0:
		return
	if(!entity_types["levels"][level_num]["layerInstances"][entity_layer_ind].has("gateInstances")):
		entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["gateInstances"] = []
	return entity_types["levels"][level_num]["layerInstances"][entity_layer_ind]["gateInstances"]

	
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

func get_cur_entity_one_field(entityCollection: String):
	return entity_types["defs"][entityCollection][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]

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

func get_entity_names(entityCollectionDef: String):
	var entity_names = []
	for entity in entity_types["defs"][entityCollectionDef]:
		
		entity_names.append(entity["identifier"])
	return entity_names
	
func get_def_entityCollection_names(collectionName: String):
	var entity_names = []
	for entity in entity_types["defs"][collectionName]:
		if entity["show"]:
			entity_names.append(entity["identifier"])
	return entity_names

func get_gateInstance_t_defId(defId: int, level_ind: int):
	var def = entity_types["defs"]["entities"][defId].duplicate(true)
	var entity_inst = entity_inst_template.duplicate(true)
	entity_inst["__identifier"] = def["identifier"]
	entity_inst["triggerAABB"] = def["triggerAABB"]
	entity_inst["triggerType"] = def["triggerType"]
	entity_inst["triggerValue"] = def["triggerValue"]
	
	if(!def.has("triggerTypeName")):
		def["triggerTypeName"] = ""
	entity_inst["triggerTypeName"] = def["triggerTypeName"]
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
	entity_inst["instId"] = get_unique_entity_instId_level_ind(level_ind)
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
	entity_inst["addTrigger"] = true
	entity_inst["triggerTypeName"] = "changeLevelv2"
	return entity_inst

func get_entityInstance_t_defId(defId: int):
	var def = entity_types["defs"]["entities"][defId].duplicate(true)
	var entity_inst = entity_inst_template.duplicate(true)
	entity_inst["__identifier"] = def["identifier"]
	entity_inst["triggerAABB"] = def["triggerAABB"]
	entity_inst["triggerType"] = def["triggerType"]
	entity_inst["triggerValue"] = def["triggerValue"]
	
	if(!def.has("triggerTypeName")):
		def["triggerTypeName"] = ""
	entity_inst["triggerTypeName"] = def["triggerTypeName"]
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

func get_cur_entityInstance_t():
	var def = entity_types["defs"]["entities"][cur_entity_type_ind].duplicate(true)
	var entity_inst = entity_inst_template.duplicate(true)
	entity_inst["__identifier"] = def["identifier"]
	entity_inst["triggerAABB"] = def["triggerAABB"]
	entity_inst["triggerType"] = def["triggerType"]
	entity_inst["triggerValue"] = def["triggerValue"]
	
	if(!def.has("triggerTypeName")):
		def["triggerTypeName"] = ""
	entity_inst["triggerTypeName"] = def["triggerTypeName"]
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

func get_entity_defs(entityCollectionDef: String):
	return entity_types["defs"][entityCollectionDef]

func get_entityDef(entityCollection: String, entity_name: String):
	var ind = 0
	for entity in entity_types["defs"][entityCollection]:
		if entity["identifier"] == entity_name:
			return {"ind": ind,"val": entity}
		ind += 1
	return {"ind": 0,"val": []}

func add_fieldDef_to_entity_in_entityCollection(entityCollection: String, fieldDef: Dictionary):
	entity_types["defs"][entityCollection][cur_entity_type_ind]["fieldDefs"].append(fieldDef.duplicate(true))

func add_field_to_entity_def(entityDefCollection: String, entityInstCollection: String, entity_name: String, field_name:String):
	var ent_ind:int = -1
	var temp_ent_ind:int = 0
	#find entity ind by name
	for entity in entity_types["defs"][entityDefCollection]:
		if entity["identifier"] == entity_name:
			ent_ind = temp_ent_ind
			break
		temp_ent_ind += 1
	if ent_ind == -1:
		return
	var field_def_data = field_def_template.duplicate(true)
	field_def_data["identifier"] = field_name
	field_def_data["fieldId"] = get_unique_entity_fieldId()
	entity_types["defs"][entityDefCollection][ent_ind]["fieldDefs"].append(field_def_data)
	
	#Doing same thing to all entityInstances of this type in game
	add_field_all_entityInst(entityInstCollection, field_name)
	
func add_field_all_entityInst(entityInstCollection: String, field_name:String):
	for level in entity_types["levels"]:
		#finding entity layer
		for layer in level["layerInstances"]:
			if(layer["__identifier"] == "Entity"):
				#Finding entity of this type
				for entityInst in layer[entityInstCollection]:
					if(entityInst["defId"] == cur_entity_type_ind):
						#entity found
						var field_def_data = field_inst_template.duplicate(true)
						field_def_data["__identifier"] = field_name
						entityInst["fieldInstances"].append(field_def_data)
						
				break

func add_entity_def_to_entityCollection(entityCollection: String, entity_name: String):
	var entity_def_data = entity_def_template.duplicate(true)
	entity_def_data["identifier"] = entity_name
	entity_def_data["defId"] = get_unique_entity_defId()
	entity_types["defs"][entityCollection].append(entity_def_data)
	
	cur_entity_type_ind = -1
	
	
	add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_spriteDef)
	add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_triggerRectDef)
	
	if(entityCollection == "entities"):
		add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_triggerType)
		add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_triggerValue)
		add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_triggerValue2)
		add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_triggerValue3)
	if(entityCollection == "bulletEntities"):
		add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_damage)
	
	add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_hp)
	add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_damaged)
	add_fieldDef_to_entity_in_entityCollection(entityCollection, field_def_onGround)
	
	
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


func get_merged_fieldDef(entityCollectionDef: String):
	var mergedFieldDef_arr = []
	var mergedFieldDef_names = []
	for entity in entity_types["defs"][entityCollectionDef]:
		for field in entity["fieldDefs"]:
			var fieldDef_name = field["identifier"]
			if !(fieldDef_name in mergedFieldDef_names):
				mergedFieldDef_arr.append(field)
				mergedFieldDef_names.append(fieldDef_name)
				pass
	mergedFieldArr = mergedFieldDef_arr
	return mergedFieldDef_arr

func get_global_variables():
	return entity_types["defs"]["variables"];

func get_merged_varInst_levels() -> Array:
	var merged_arr:Array = []
	var merged_names:Array = []
	for level in entity_types["levels"]:
		if !level.has("localVars"):
			level["localVars"] = []
			continue
		for varInst in level["localVars"]:
			if !(varInst["name"] in merged_names):
				merged_arr.append(varInst)
				merged_names.append(varInst["name"])
	return merged_arr


func get_merged_fieldDefs_v2(entityCollectionDef: String):
	var result: Dictionary = {}
	
	var mergedFields_arr = get_merged_fieldDef(entityCollectionDef)
	
	#for ALL entity types
	for entityDef in entity_types["defs"][entityCollectionDef]:
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

func delete_fieldDef(entityDefCollection: String, entityInstCollection: String, field_ind: int):
	print("Deleting ", field_ind, " from game")
	var fieldName: String = entity_types["defs"][entityDefCollection][cur_entity_type_ind]["fieldDefs"][field_ind]["identifier"]
	entity_types["defs"][entityDefCollection][cur_entity_type_ind]["fieldDefs"].remove(field_ind)
	
	delete_all_fieldInst_by_fieldId(entityInstCollection, fieldName)

func delete_all_fieldInst_by_fieldId(entityCollection: String, fieldName: String):
	
	for level in entity_types["levels"]:
		#finding entity layer
		for layer in level["layerInstances"]:
			if(layer["__identifier"] == "Entity"):
				#Finding entity of this type
				for entityInst in layer[entityCollection]:
					if(entityInst["defId"] == cur_entity_type_ind):
						#entity found
						var temp_fieldInstances = []
						for fieldInst in entityInst["fieldInstances"]:
							if(fieldInst["__identifier"] != fieldName):
								temp_fieldInstances.append(fieldInst)
						entityInst["fieldInstances"] = temp_fieldInstances.duplicate(true)
						
				break


func delete_entityDef(entityCollection: String, entity_name: String):
	var entity_def = get_entityDef(entityCollection, entity_name)
	entity_types["defs"][entityCollection].remove(entity_def["ind"])
	
	if(entityCollection == "entities"):
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

func is_entity_name_exists(entityCollection: String, entity_name: String):
	for entity in entity_types["defs"][entityCollection]:
		if entity["identifier"] == entity_name:
			return true
	return false
func is_entity_have_field(entityCollection: String, entity_name: String, field_name: String):
	var ent
	#find entity by name
	for entity in entity_types["defs"][entityCollection]:
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

func get_addEntityMergedSlots():
	if !entity_types.has("addEntityMergedSlots"):
		entity_types["addEntityMergedSlots"] = 10
	return entity_types["addEntityMergedSlots"]

func get_entityDefs():
	return entity_types["defs"]["entities"]

#"testEntity": {"pos": [0,0], "spriteDefinition": "&spr_test"},
