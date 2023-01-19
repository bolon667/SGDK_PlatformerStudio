extends Node

enum EditorMode {
  NONE,
  COLLISION,
  ENTITY,
  START_POS,
}

var cell_size: int = 16

var cur_engine: String = "platformer"

var cur_project_path: String = ""

var cur_editor_mode: int = EditorMode.COLLISION
var prev_editor_mode: int = EditorMode.COLLISION

var cur_entity_type: String
var cur_entity_type_ind: int = -1
var cur_entity_field_ind: int
var cur_entity_inst_ind: int

var cur_level_ind: int = 0
var cur_level_layer_ind: int = 0

var entity_names_len: int = 0

var cur_level: int = 1

var can_create_entity_obj: bool = true

var entity_types = {
	"__header__": 
	{
		"fileType": "SGDK Studio Project JSON",
		"app": "SGDK Studio",
		"doc": "???",
		"appAuthor": "bolon667",
		"appVersion": "1.0.0 alpha",
		"url": "https://github.com/bolon667/SGDK_OneScreenPlatformerStudio",
	},
	"jsonVersion": "1.0.0",
	"engineRootPath": "./StudioType/SGDK/Engines/platformer",
	"isOneScreen": true,
	"defaultGridSize": 16,
	"defs": 
	{
		"entities": 
		[

		]
	},
	"levels": 
		[
			
		],
	"messagePacks":
		[
			
		],
}

const field_def_template = {	
	"identifier": "Field",
	"__type": "Int",
	"inCodeType": "s16",
	"fieldId": -1,
	"canBeNull": true,
	"defaultValue": "0",
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


const entity_def_template = {
	"identifier": "testEntity",
	"pos": [0,0],
	"tags": [],
	"width": 16,
	"height": 16,
	"show": true,
	"spritePath": "",
	"triggerAABB": [0,0,8,8],
	"triggerType": 0,
	"triggerValue": 0,
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
	"pxWid": 500,
	"pxHei": 500,
	"bgRelPath": "",
	"bgRelPath2": "",
	"startPos": [0,0],
	"fieldInstances":
		[
			#ignore for now
		],
	"layerInstances":
		[
			
		],
	
}

const level_layer_template = {
	"__identifier": "tLayer",
	"__type": "Collision",
	"__cWid": 500,
	"__cHei": 500,
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
	"px": [0,0],
	"triggerAABB": [0,0,8,8],
	"instId": -1, #id of entity instance, to quicly find in databse
	"defId": -1, #id of entity definition, for quick search of entity instance, when you changed
	#entity name or field name in entity menu.
	"fieldInstances": 
		[
			
		]
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



func get_engine_root_path():
	return entity_types["engineRootPath"]

func get_cell_size():
	return cell_size

func _ready():
	add_level()
	add_level_layer(cur_level_ind, "EntityLayer", "Entity")
	add_level_layer(cur_level_ind, "CollisionLayer", "Collision")



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
	
	singleton.entity_types = data_parse.result

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

func load_project_last_paths():
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
		
		return data_parse.result
	else:
		var dir = Directory.new()
		dir.open("./")
		dir.make_dir("InternalData")
		
		file.open(file_path, file.WRITE)
		file.store_string(to_json(paths))
		file.close()
		return paths
		
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
	paths_dict["last_project_paths"].append(cur_project_path)
	#Save
	var file2 = File.new()
	file2.open(file_path, file2.WRITE)
	file2.store_string(to_json(paths_dict))
	file2.close()

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

func change_level_size(size: Vector2):
	entity_types["levels"][cur_level_ind]["pxWid"] = size.x
	entity_types["levels"][cur_level_ind]["pxHei"] = size.y
	
func get_level_size():
	return Vector2(entity_types["levels"][cur_level_ind]["pxWid"], entity_types["levels"][cur_level_ind]["pxHei"]);

func change_bgRelPath(new_bg_path: String):
	entity_types["levels"][cur_level_ind]["bgRelPath"] = new_bg_path

func get_bgRelPath():
	return entity_types["levels"][cur_level_ind]["bgRelPath"]
	
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


func change_cur_entityInst(key: String, val):
	var entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			break
		entity_layer_ind += 1
	entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"][cur_entity_inst_ind][key] = val

func change_entityInstName_by_defId(entity_name: String, defId: int):
	var entity_layer_ind: int = 0
	for layer_inst in entity_types["levels"][cur_level_ind]["layerInstances"]:
		if(layer_inst["__type"] == "Entity"):
			break
		entity_layer_ind += 1
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][entity_layer_ind]["entityInstances"]:
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
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			entity_inst["px"] = [int(posPx[0]), int(posPx[1])]

func get_level_count():
	var level_count: int = 0
	for level in entity_types["levels"]:
		level_count += 1
	return level_count
func get_entityInstances():
	return entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]

func change_entityInst_by_instId(instId: int, key:String, val):
	var cur_InstInd:int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			print("instId found")
			print(val)
			print(key)
			entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_InstInd][key] = val
			print(entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_InstInd])
			break
		cur_InstInd+=1


func get_entityInst_by_instId(instId: int):
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			return entity_inst
			
func change_fiendInst_by_instId(fieldName: String, fieldValue):
	var instId = cur_entity_inst_ind
	var cur_inst_ind: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			break
		cur_inst_ind += 1
	var cur_fieldInst_ind: int = 0
	for fieldInst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_inst_ind]["fieldInstances"]:
		if fieldInst["__identifier"] == fieldName:
			break
		cur_fieldInst_ind += 1
	entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"][cur_inst_ind]["fieldInstances"][cur_fieldInst_ind]["__value"] = fieldValue
	
func change_sprite_by_instId(spritePath: String):
	var instId = cur_entity_inst_ind
	var cur_inst_ind: int = 0
	for entity_inst in entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"]:
		if entity_inst["instId"] == instId:
			entity_inst["__spritePath"] = spritePath
			break
		cur_inst_ind += 1

func add_cur_entityInstance():
	var entity_instance = get_cur_entityInstance_t()
	entity_types["levels"][cur_level_ind]["layerInstances"][cur_level_layer_ind]["entityInstances"].append(entity_instance)
	return entity_instance["instId"]

func add_level_layer(level_num: int, layer_name: String, layer_type: String):
	var level_layer_data = level_layer_template.duplicate()
	level_layer_data["__identifier"] = layer_name
	level_layer_data["__type"] = layer_type
	entity_types["levels"][level_num]["layerInstances"].append(level_layer_data)

func add_level():
	var level_data = level_template.duplicate()
	level_data["identifier"] = "Level_" + str(cur_level)
	entity_types["levels"].append(level_data)
	cur_level += 1

func change_cur_field(field_property_name: String, field_property_value: String):
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind][field_property_name] = field_property_value
	pass

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
	var def = entity_types["defs"]["entities"][cur_entity_type_ind]
	var entity_inst = entity_inst_template.duplicate()
	entity_inst["__identifier"] = def["identifier"]
	entity_inst["defId"] = def["defId"]
	entity_inst["instId"] = get_unique_entity_instId()
	var filed_inst_arr = []
	#Get all fields from entityDef, and copy name,value to fields in entityInstance
	#Entity definition - template for entity
	#Entity instance - actual entity
	for field in def["fieldDefs"]:	
		var field_inst = field_inst_template.duplicate()
		field_inst["__identifier"] = field["identifier"]
		field_inst["__value"] = field["defaultValue"]
		field_inst["__type"] = field["__type"]
		field_inst["fieldId"] = field["fieldId"]
		field_inst["__hasStruct"] = field["hasStruct"]
		filed_inst_arr.append(field_inst)
	entity_inst["fieldInstances"] = filed_inst_arr
	return entity_inst
	

func get_entityDef_by_ind(entity_ind: int):
	return entity_types["defs"]["entities"][entity_ind]

func get_entityDef(entity_name: String):
	var ind = 0
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			return {"ind": ind,"val": entity}
		ind += 1
	return {"ind": 0,"val": []}

func add_fieldDef_to_entity(fieldDef: Dictionary):
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"].append(fieldDef)

func add_field_to_entity(entity_name: String, field_name:String):
	var ent_ind = 0
	var temp_ent_ind = 0
	#find entity ind by name
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			ent_ind = temp_ent_ind
			break
		temp_ent_ind += 1
	var field_def_data = field_def_template.duplicate(true)
	field_def_data["identifier"] = field_name
	field_def_data["fieldId"] = get_unique_entity_fieldId()
	print("Debug")
	print(field_def_data)
	print(entity_types)
	entity_types["defs"]["entities"][ent_ind]["fieldDefs"].append(field_def_data)


func add_entity_def(entity_name: String):
	var entity_def_data = entity_def_template.duplicate(true)
	entity_def_data["identifier"] = entity_name
	entity_def_data["defId"] = get_unique_entity_defId()
	entity_types["defs"]["entities"].append(entity_def_data)
	
	add_fieldDef_to_entity(field_def_spriteDef)
	add_fieldDef_to_entity(field_def_triggerRectDef)
	add_fieldDef_to_entity(field_def_triggerType)
	add_fieldDef_to_entity(field_def_triggerValue)
	
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
	return mergedFieldDef_arr
		

func delete_fieldDef(field_ind: int):
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"].remove(field_ind)
	

func delete_entityDef(entity_name: String):
	var entity_def = get_entityDef(entity_name)
	entity_types["defs"]["entities"].remove(entity_def["ind"])

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
