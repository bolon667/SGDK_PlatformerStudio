extends Node

var cur_entity_type: String
var cur_entity_type_ind: int = -1
var cur_entity_field_ind: int

var cur_level_ind: int = 0
var cur_level_layer_ind: int = 0

var entity_names_len: int = 0

var cur_level: int = 1

var can_create_entity_obj: bool = true

var entity_types = {
	"defs": 
	{
		"entities": 
		[

		]
	},
	"levels": 
		[
			
		]
	
}

const field_def_template = {	
	"identifier": "Field",
	"__type": "string",
	"canBeNull": true,
	"defaultValue": "test_string",
}

const entity_def_template = {
	"identifier": "testEntity",
	"pos": [0,0],
	"tags": [],
	"width": 16,
	"height": 16,
	"color": "#0048FF",
	"showName": true,
	"maxCount": -1,
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
	"__sprite": "",
	"__smartColor": "#FF9801",
	"width": 32,
	"height": 32,
	"px": [0,0],
	"fieldInstances": 
		[
			
		]
}

const field_inst_template = {
	"__identifier": "fieldInst",
	"__value": "fieldVal",
	"__type": "String",
	"__sprite": "",
	
}

func _ready():
	add_level()
	add_level_layer(cur_level_ind, "EntityLayer", "Entity")
	add_level_layer(cur_level_ind, "CollisionLayer", "Collision")

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

func change_name_of_cur_field(text: String):
	
	entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind]["identifier"] = text
	print(entity_types["defs"]["entities"][cur_entity_type_ind]["fieldDefs"][cur_entity_field_ind])

func get_cur_level_layer_names():
	var layer_names = []
	var layers = get_cur_level_layers()
	for layer in layers:
		layer_names.append(layer["__identifier"])
	return layer_names

func get_cur_level_layers():
	return entity_types["levels"][cur_level_ind]["layerInstances"]

func get_cur_entity_field_names():
	var entity_field_names = []
	if(entity_names_len == 0):
		 return entity_field_names
	print("DEBUG")
	print(cur_entity_type_ind)
	print(entity_names_len)
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
	
func get_entity_fields(entity_name: String):
	#TODO: check if it works???!?!??
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

func get_entityDef(entity_name: String):
	var ind = 0
	for entity in entity_types["defs"]["entities"]:
		if entity["identifier"] == entity_name:
			return {"ind": ind,"val": entity}
		ind += 1
	return {"ind": 0,"val": []}
	
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
	print("Debug")
	print(field_def_data)
	print(entity_types)
	entity_types["defs"]["entities"][ent_ind]["fieldDefs"].append(field_def_data)


func add_entity(entity_name: String):
	var entity_def_data = entity_def_template.duplicate(true)
	entity_def_data["identifier"] = entity_name
	entity_types["defs"]["entities"].append(entity_def_data)
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

func delete_fieldDef(entity_name: String, field_name: String):
	var entity_field = get_entity_field(entity_name, field_name)
	entity_types["defs"]["entities"][entity_field["entityInd"]]["fieldDefs"].remove(entity_field["fieldInd"])

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
