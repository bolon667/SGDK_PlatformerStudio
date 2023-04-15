extends Node2D

export var camera_speed = 300
export var zoom_step = 0.1

var zoom = 1
var fixed_toggle_point = Vector2(0,0)


var mouse_on_level = false
var highlight = false
var is_moving = false

const erase_tile_ind = -1

var cell_pos = Vector2.ZERO
var prev_cell_pos = Vector2.ZERO
var prev_cell_pos_temp = Vector2.ZERO

var local_mouse_pos

var level_size

var map_size_px:Vector2
var map_size_tiles:Vector2

var real_z_index = z_index

var first_sides_check = true


onready var temp_tile_map = $tempTileMap

onready var tile_map = $TileMap
onready var entity_obj_list = $EntityList
onready var position_obj_list = $PositionList

onready var bgA_spr = $bgA
onready var bgB_spr = $bgB

var cur_level_ind: int = 0


onready var camera = $"../../Camera2D"
#onready var camera = $Camera2D

onready var entity_obj_t = preload("res://Scenes/entityScene.tscn")
onready var position_obj_t = preload("res://Scenes/positionScene.tscn")
onready var gate_obj_t = preload("res://Scenes/gateScene.tscn")
onready var level_buttons_t = preload("res://Scenes/modalWindows/levelContainerContextButtons.tscn")


func draw_frame(rect: Rect2, color, line_thickness):
	var half_line_thickness = int(line_thickness/2)
	draw_line(Vector2(rect.position.x-half_line_thickness,rect.position.y-half_line_thickness), Vector2(rect.size.x+line_thickness, rect.position.y-half_line_thickness), color, line_thickness)
	draw_line(Vector2(rect.size.x+half_line_thickness, rect.position.y-half_line_thickness), Vector2(rect.size.x+half_line_thickness, rect.size.y+line_thickness), color, line_thickness)
	draw_line(Vector2(rect.size.x+half_line_thickness, rect.size.y+half_line_thickness), Vector2(rect.position.x-line_thickness, rect.size.y+half_line_thickness), color, line_thickness)
	draw_line(Vector2(rect.position.x-half_line_thickness, rect.size.y+half_line_thickness), Vector2(rect.position.x-half_line_thickness, rect.position.y-line_thickness), color, line_thickness)

func delete_self():
	#singleton.entity_types["turnOnGates"] = false
	
	disable_side_areas()
	yield(get_tree(), "idle_frame")
	#get_tree.
	singleton.delete_level(cur_level_ind)
	get_tree().call_group("tilemapEditorWindow", "load_levels")
	#queue_free()

func _draw():
	if(highlight):
		var color: Color = Color( 1, 0.84, 0, 1 )
		var line_thickness = 6
		#line_thickness = stepify(line_thickness*camera2D.zoom,0)
		#var rect = Rect2(Vector2(-line_thickness,-line_thickness), Vector2(level_size.x+line_thickness+line_thickness,level_size.y+line_thickness+line_thickness))
		var rect = Rect2(Vector2(0,0), Vector2(map_size_px.x,map_size_px.y))
		draw_frame(rect, color, line_thickness)
	
func turn_off_highlight():
	highlight = false
	update()

func change_bga(imgPath: String, bgaMode: int):
	singleton.change_bgRelPath(imgPath, cur_level_ind)
	print("bgA on level ", cur_level_ind, " has changed")
	var bgImage = Image.new()
	bgImage.load(imgPath)
	var imgTex = ImageTexture.new()
	imgTex.create_from_image(bgImage, 1)
	$bgA.texture = imgTex;
	if(bgaMode == 0):
		map_size_px = $bgA.texture.get_size()
		map_size_tiles = Vector2(map_size_px.x/singleton.cell_size, map_size_px.y/singleton.cell_size)
		singleton.change_level_size(cur_level_ind, map_size_px)
	updateAreas()

func change_bgb(imgPath: String, bgbMode: int):
	singleton.change_bgRelPath2(imgPath, cur_level_ind)
	print("bgB on level ", cur_level_ind, " has changed")
	var bgImage = Image.new()
	bgImage.load(imgPath)
	var imgTex = ImageTexture.new()
	imgTex.create_from_image(bgImage, 1)
	$bgB.texture = imgTex;
	if(bgbMode == 0):
		map_size_px = $bgB.texture.get_size()
		map_size_tiles = Vector2(map_size_px.x/singleton.cell_size, map_size_px.y/singleton.cell_size)
		singleton.change_level_size(cur_level_ind, map_size_px)
	updateAreas()

func move_level():
	
	if(!mouse_on_level or !singleton.level_move_mode):
		highlight = false
		return
	var on_top: bool = _is_on_top()
	if(highlight != on_top):
		highlight = _is_on_top()
		update()
	
	
	
	var ref = get_viewport().get_mouse_position()
	if(Input.is_action_just_pressed("mouse1") && _is_on_top()):
		get_tree().call_group("tempWindow", "queue_free")
		singleton.in_modal_window = false
		is_moving = true
		z_index = 100

	if(is_moving):
		global_position.x += (ref.x - fixed_toggle_point.x)*camera.zoom.x
		global_position.y += (ref.y - fixed_toggle_point.y)*camera.zoom.y
		
	if(Input.is_action_just_released("mouse1")):
		z_index = real_z_index
		is_moving = false
		
		global_position.x = (round(global_position.x/8))*8
		global_position.y = (round(global_position.y/8))*8
		#Save world coords of level
		singleton.entity_types["levels"][cur_level_ind]["worldX"] = global_position.x
		singleton.entity_types["levels"][cur_level_ind]["worldY"] = global_position.y
		
	if(Input.is_action_just_pressed("mouse2") and highlight):
		get_tree().call_group("tempWindow", "queue_free")
		var buttons_node = level_buttons_t.instance()
		buttons_node.level_container = self
		buttons_node.global_position = get_global_mouse_position()
		get_parent().add_child(buttons_node)
		
		
	fixed_toggle_point = ref

func _is_on_top():
	for entity in get_tree().get_nodes_in_group("level_hovered"):
		if entity.get_index() > get_index():
			return false
	return true

func change_cur_entity_pic(pic_path: String, level_ind: int, entity_inst_id: int):
	var children = entity_obj_list.get_children()
	for entity_obj in children: # && (entity_obj.entityInst_id == singleton.cur_entity_inst_ind
		if ((cur_level_ind == level_ind) && (entity_obj.entityInst_id == entity_inst_id)):
			print("CHANGE PIC OF ENTITY")
			var img1 = Image.new()
			img1.load(pic_path)
			var imgTex = ImageTexture.new()
			imgTex.create_from_image(img1, 1)
			entity_obj.get_node("Sprite").texture = imgTex;
			
			var temp_sprite_size = singleton.get_sprite_size_from_path(pic_path)
			if temp_sprite_size:
				entity_obj.sprite_size = temp_sprite_size
				entity_obj.change_sprite_rect(Rect2(0,0,temp_sprite_size.x, temp_sprite_size.y))
				entity_obj.get_node("CollisionShape2D").shape.extents = Vector2(temp_sprite_size.x/2, temp_sprite_size.y/2)
			else:
				entity_obj.sprite_size = entity_obj.get_node("Sprite").texture.get_size()
				entity_obj.change_sprite_rect(Rect2(0,0,entity_obj.sprite_size.x, entity_obj.sprite_size.y))
				entity_obj.get_node("CollisionShape2D").shape.extents = entity_obj.sprite_size/2


func update_tilemap_cell_size():
	#Changing tilemap cell size
	tile_map.cell_size = Vector2(singleton.cell_size, singleton.cell_size)
	if(singleton.cell_size == 8):
		tile_map.tile_set = load("res://TileSets/8x8TileSet.tres")
	if(singleton.cell_size == 16):
		tile_map.tile_set = load("res://TileSets/16x16TileSet.tres")
	temp_tile_map.cell_size = tile_map.cell_size
	temp_tile_map.tile_set = tile_map.tile_set

func load_tileMap():
	#clear all from scene
	clean_tileMap()
	level_size = singleton.get_level_size(cur_level_ind)
	print("level_size: ", level_size)
	var collision_map_size = Vector2(level_size.x/singleton.cell_size, level_size.y/singleton.cell_size)
	var intGridCsv:Array = singleton.get_collisionMap(cur_level_ind)
	var intGridCsvLen = len(intGridCsv)
	if len(intGridCsv) == 0:
		return
	for y in range(collision_map_size.y):
		for x in range(collision_map_size.x):
			if(x+(y*collision_map_size.x) > intGridCsvLen-1):
				break
			var tile_value: int = intGridCsv[x+(y*collision_map_size.x)]-1
			tile_map.set_cell(x, y, tile_value) 

func clear_positions_on_scene():
	var children = position_obj_list.get_children()
	for child in children:
		child.queue_free()
		
func load_positions_on_scene():
	clear_positions_on_scene()
	var entity_instantes = singleton.get_positionInstances(cur_level_ind)
	print("AMOUNT: ", len(entity_instantes))
	for entity_inst in entity_instantes:
		var entity_node = position_obj_t.instance()
		var entity_pos = entity_inst["px"]
		entity_node.position = Vector2(entity_pos[0], entity_pos[1])
		entity_node.entityInst_id = entity_inst["instId"]
		entity_node.level_ind = cur_level_ind
		
		
		entity_node.get_node("CollisionShape2D").shape = RectangleShape2D.new()
		var draggableShape: Vector2 = Vector2(16,16)

		entity_node.get_node("CollisionShape2D").shape.extents = draggableShape
		position_obj_list.add_child(entity_node)
		

func clear_entities_on_scene():
	var children = entity_obj_list.get_children()
	for child in children:
		child.queue_free()

func load_entities_on_scene():
	clear_entities_on_scene()
	#singleton.fix_level_inst_ids(cur_level_ind)
	singleton.remove_gates(cur_level_ind)
	var entity_instantes = singleton.get_entityInstances(cur_level_ind)
	for entity_inst in entity_instantes:
		var sprite_rect: Rect2
		var entity_node = entity_obj_t.instance()
		var entity_pos = entity_inst["px"]
		entity_node.position = Vector2(entity_pos[0], entity_pos[1])
		entity_node.entityInst_id = entity_inst["instId"]
		entity_node.def_id = entity_inst["defId"]
		entity_node.level_ind = cur_level_ind
		
		if(!entity_inst.has("triggerAABB")):
			continue
		entity_node.triggerAABB = entity_inst["triggerAABB"].duplicate(true)
		
		if len(entity_inst["__spritePath"]) > 0:
			var img1 = Image.new()
			img1.load(entity_inst["__spritePath"])
			var imgTex = ImageTexture.new()
			imgTex.create_from_image(img1, 1)
			entity_node.get_node("Sprite").texture = imgTex;
			
			#break
			var sprite_name = entity_inst["__spritePath"].substr(entity_inst["__spritePath"].find_last("/"))
			sprite_name = sprite_name.split(".")[0]
			var temp_1 = sprite_name.split("-")
			if len(temp_1) > 1:
				var sprite_info = temp_1[1]
				var info_arr = sprite_info.split("_")
				var t_width: int = int(info_arr[0])
				var t_height: int = int(info_arr[1])
				var t_time: int = int(info_arr[2])
				
				sprite_rect = Rect2(0,0,t_width*8,t_height*8)
			else:
				var texture_size = entity_node.get_node("Sprite").texture.get_size()
				sprite_rect = Rect2(0,0,texture_size.x,texture_size.y)
				
				#texture_size.x
		#entity_node.get_node("ColorRect").rect_position = 
		entity_node.get_node("CollisionShape2D").shape = RectangleShape2D.new()
		var draggableShape: Vector2 = Vector2(8,8);
		if(sprite_rect.size.x > 8):
			draggableShape.x = sprite_rect.size.x/2
		if(sprite_rect.size.y > 8):
			draggableShape.y = sprite_rect.size.y/2
		entity_node.get_node("CollisionShape2D").shape.extents = draggableShape
		entity_node.sprite_size = sprite_rect.size
		entity_obj_list.add_child(entity_node)
		
		
		entity_node.change_sprite_rect(sprite_rect)	

func _ready():
	load_level()

func updateAreas():
	#Change area for mouse to be able to interact with level
	$AreaLevel/CollisionShape2D.shape.extents = (map_size_px/2)
	$AreaLevel/CollisionShape2D.position = (map_size_px/2)
	
	var adjacent_side_width = 4
	var adjacent_side_width_half = adjacent_side_width/2
	var map_size_px_y_half =  map_size_px.y/2
	var map_size_px_x_half =  map_size_px.x/2
	
	#Change area of adjacent sides
	#Left side
	$AreaLeft/CollisionShape2D.shape.extents = Vector2(adjacent_side_width, map_size_px_y_half-5)
	$AreaLeft/CollisionShape2D.position = Vector2(-adjacent_side_width_half, map_size_px_y_half)
	#Right side
	$AreaRight/CollisionShape2D.shape.extents = Vector2(adjacent_side_width, map_size_px_y_half-5)
	$AreaRight/CollisionShape2D.position = Vector2(map_size_px.x+adjacent_side_width_half, map_size_px_y_half)
	#Up side
	$AreaUp/CollisionShape2D.shape.extents = Vector2(map_size_px_x_half-5, adjacent_side_width_half)
	$AreaUp/CollisionShape2D.position = Vector2(map_size_px_x_half, -adjacent_side_width_half)
	#Down side
	$AreaDown/CollisionShape2D.shape.extents = Vector2(map_size_px_x_half-5, adjacent_side_width_half)
	$AreaDown/CollisionShape2D.position = Vector2(map_size_px_x_half, map_size_px.y+adjacent_side_width_half)
	

func update_level_index():
	$Label.text = "Level_" + str(cur_level_ind)

func load_level():
	update_tilemap_cell_size()
	#move to world position
	set_global_position(singleton.get_worldPos_vector2(cur_level_ind))
	
	map_size_px = singleton.get_level_size(cur_level_ind)
	map_size_tiles = Vector2(map_size_px.x/singleton.cell_size, map_size_px.y/singleton.cell_size)

	updateAreas()
	
	$Label.text = "Level_" + str(cur_level_ind)
	
	#get image of level background
	var bgRelPath: String = singleton.get_bgRelPath(cur_level_ind)
	if bgRelPath:
		bgA_spr.texture = load(bgRelPath)
	else:
		bgA_spr.texture = null
	var bgRelPath2: String = singleton.get_bgRelPath2(cur_level_ind)
	if bgRelPath2:
		bgB_spr.texture = load(bgRelPath2)
	else:
		bgB_spr.texture = null
		
	load_positions_on_scene()
	#add entities on scene from database (singleton.entity_types)
	load_entities_on_scene()
	#Fill tilemap with data from json (singleton.entity_types)
	load_tileMap()
	
func move_level_off():
	turn_off_highlight()
	disable_side_areas()
	enable_position_list()
	enable_entity_list()
	get_tree().call_group("tempWindow", "queue_free")

func move_level_on():
	if(singleton.entity_types["turnOnGates"]):
		enable_side_areas()
	clean_temp_tileMap()
	disable_position_list()
	disable_entity_list()
	
func change_bga_mode(mode: int):
	singleton.change_load_image_mode_bga(mode, cur_level_ind)
func change_bgb_mode(mode: int):
	singleton.change_load_image_mode_bgb(mode, cur_level_ind)
func enable_side_areas():
	$AreaDown/CollisionShape2D.disabled = false
	$AreaLeft/CollisionShape2D.disabled = false
	$AreaRight/CollisionShape2D.disabled = false
	$AreaUp/CollisionShape2D.disabled = false
	
	first_sides_check = true
	yield(get_tree(), "idle_frame")
	yield(get_tree(), "idle_frame")
	
	first_sides_check = false
	
func disable_side_areas():
	$AreaDown/CollisionShape2D.disabled = true
	$AreaLeft/CollisionShape2D.disabled = true
	$AreaRight/CollisionShape2D.disabled = true
	$AreaUp/CollisionShape2D.disabled = true

func disable_position_list():
	for positionInst in $PositionList.get_children():
		positionInst.get_node("CollisionShape2D").disabled = true
func enable_position_list():
	for positionInst in $PositionList.get_children():
		positionInst.get_node("CollisionShape2D").disabled = false
func disable_entity_list():
	for entityInst in $EntityList.get_children():
		entityInst.get_node("CollisionShape2D").disabled = true
func enable_entity_list():
	for entityInst in $EntityList.get_children():
		entityInst.get_node("CollisionShape2D").disabled = false

func area2d_follow_camera():
	pass
	#$Area2D.global_position = camera.global_position

func make_rect_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int, tileMap: TileMap):
	var startPosX: int
	var startPosY: int
	var curPosX: int
	var curPosY: int
	var lastPosY: int
	var lastPosX: int
	if(pos0.x < pos1.x):
		startPosX = pos0.x
		lastPosX = pos1.x
	else:
		startPosX = pos1.x
		lastPosX = pos0.x
	
	if(pos0.y < pos1.y):
		startPosY = pos0.y
		lastPosY = pos1.y
	else:
		startPosY = pos1.y
		lastPosY = pos0.y
		
	curPosX = startPosX
	curPosY = startPosY
	while(curPosY <= lastPosY):
		while(curPosX <= lastPosX):
			curPosX += 1
			if(curPosX < 0 or curPosY < 0):
				continue
			if(curPosX >= map_size_tiles.x or curPosY >= map_size_tiles.y):
				continue
			tileMap.set_cell(curPosX, curPosY, tile_ind)
			
		curPosY+=1
		curPosX = startPosX

func make_line_temp_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int):
	var line_positions = []
	var dx = pos1.x - pos0.x
	var dy = pos1.y - pos0.y
	var steps
	if (abs(dx) > abs(dy)):
		steps = abs(dx)
	else:
		steps = abs(dy)
	if(steps == 0):
		temp_tile_map.set_cell(pos1.x, pos1.y, tile_ind);
		return
	var x_increment = dx / steps;
	var y_increment = dy / steps;
	
	var v=0;
	var x = 0
	var y = 0
	while(v < steps):
		x = x + x_increment
		y = y + y_increment
		temp_tile_map.set_cell(pos0.x+x, pos0.y+y, tile_ind);
		line_positions.append({"x": pos0.x+x,"y": pos0.y+y})
		v+=1
	

	return 

func change_entity_trigger_rect_by_instId(instId: int, rect: Rect2):
	var children = entity_obj_list.get_children()
	for entity_obj in children:
		if entity_obj.entityInst_id == singleton.cur_entity_instId:
			entity_obj.triggerAABB = [rect.position.x, rect.position.y, rect.size.x, rect.size.y]
			entity_obj.get_node("ColorRect").rect_position = rect.position
			entity_obj.get_node("ColorRect").rect_size = Vector2(rect.size.x-rect.position.x, rect.size.y-rect.position.y)
			break
	pass

func make_line_tileMap(pos0: Vector2, pos1: Vector2, tile_ind: int, tile_map):
	var dx = pos1.x - pos0.x
	var dy = pos1.y - pos0.y
	var steps
	if (abs(dx) > abs(dy)):
		steps = abs(dx)
	else:
		steps = abs(dy)
	if(steps == 0):
		if(pos1.x < 0 or pos1.y < 0):
			return
		if(pos1.x >= map_size_tiles.x or pos1.y >= map_size_tiles.y):
			return
		if(tile_ind == 4):
			tile_map.set_cell(pos1.x, pos1.y, 4)
			tile_map.set_cell(pos1.x, pos1.y+1, 0)
		elif(tile_ind == 5):
			tile_map.set_cell(pos1.x, pos1.y, 5)
			tile_map.set_cell(pos1.x, pos1.y+1, 0)
		elif(tile_ind == 8):
			tile_map.set_cell(pos1.x, pos1.y, 8)
			tile_map.set_cell(pos1.x+1, pos1.y, 9)
		elif(tile_ind == 9):
			tile_map.set_cell(pos1.x, pos1.y, 9)
			tile_map.set_cell(pos1.x-1, pos1.y, 8)
		elif(tile_ind == 10):
			tile_map.set_cell(pos1.x, pos1.y, 10)
			tile_map.set_cell(pos1.x+1, pos1.y, 11)
		elif(tile_ind == 11):
			tile_map.set_cell(pos1.x, pos1.y, 11)
			tile_map.set_cell(pos1.x-1, pos1.y, 10)
		elif(tile_ind == 12):
			tile_map.set_cell(pos1.x, pos1.y, 12)
			tile_map.set_cell(pos1.x+1, pos1.y, 13)
		elif(tile_ind == 13):
			tile_map.set_cell(pos1.x, pos1.y, 13)
			tile_map.set_cell(pos1.x-1, pos1.y, 12)
		elif(tile_ind == 14):
			tile_map.set_cell(pos1.x, pos1.y, 14)
			tile_map.set_cell(pos1.x+1, pos1.y, 15)
		elif(tile_ind == 15):
			tile_map.set_cell(pos1.x, pos1.y, 15)
			tile_map.set_cell(pos1.x-1, pos1.y, 14)
		tile_map.set_cell(pos1.x, pos1.y, tile_ind)
		return
	var x_increment = dx / steps;
	var y_increment = dy / steps;
	
	var v=0;
	var x = 0
	var y = 0
	var real_pos_x = 0
	var real_pos_y = 0
	while(v < steps):
		x = x + x_increment
		y = y + y_increment
		
		
		v+=1
		real_pos_x = pos0.x+x
		real_pos_y = pos0.y+y
		if(real_pos_x < 0 or real_pos_y < 0):
			continue
		if(real_pos_x >= map_size_tiles.x or real_pos_y >= map_size_tiles.y):
			continue
		tile_map.set_cell(pos0.x+x, pos0.y+y, tile_ind);
		


func tile_map_handler(delta):
	
	#var bg_sprite_size = bgA_spr.texture.get_size()

	#if(local_mouse_pos.x < 0 or local_mouse_pos.y < 0):
	#	return
	#if(local_mouse_pos.x > bg_sprite_size.x or local_mouse_pos.y > bg_sprite_size.y):
	#	return
	
	cell_pos = tile_map.world_to_map(local_mouse_pos)
	#rect drawing mode
	if(Input.is_action_pressed("shift_key")):
		
		if(Input.is_action_just_pressed("mouse1")):
			prev_cell_pos = cell_pos
		if(Input.is_action_just_released("mouse1")):
			#draw rect
			print("draw rect")
			make_rect_tileMap(prev_cell_pos, cell_pos, singleton.draw_tile_ind, tile_map)
			singleton.save_collisionMap(tile_map, map_size_tiles, cur_level_ind)
			
		if(Input.is_action_just_pressed("mouse2")):
			prev_cell_pos = cell_pos
		if(Input.is_action_just_released("mouse2")):
			#erase rect
			print("erase rect")
			make_rect_tileMap(prev_cell_pos, cell_pos, erase_tile_ind, tile_map)
			singleton.save_collisionMap(tile_map, map_size_tiles, cur_level_ind)
			
	else: #standart point mode
		if(Input.is_action_pressed("mouse1")):
			make_line_tileMap(prev_cell_pos, cell_pos, singleton.draw_tile_ind, tile_map)
			
		elif(Input.is_action_just_released("mouse1")):
			singleton.save_collisionMap(tile_map, map_size_tiles, cur_level_ind)
			
		if(Input.is_action_pressed("mouse2")):
			make_line_tileMap(prev_cell_pos, cell_pos, erase_tile_ind, tile_map)
			
		elif(Input.is_action_just_released("mouse2")):
			singleton.save_collisionMap(tile_map, map_size_tiles, cur_level_ind)
			
		prev_cell_pos = cell_pos
	
	

func clean_tileMap():
	var used_tile_positions = tile_map.get_used_cells()
	for tile_pos in used_tile_positions:
		tile_map.set_cell(tile_pos[0], tile_pos[1], -1)

func clean_temp_tileMap():
	var used_tile_positions = temp_tile_map.get_used_cells()
	for tile_pos in used_tile_positions:
		temp_tile_map.set_cell(tile_pos[0], tile_pos[1], -1)

func temp_tile_map_handler(delta):
	clean_temp_tileMap()

	#var bg_sprite_size = bgA_spr.texture.get_size()
	#if(local_mouse_pos.x < 0 or local_mouse_pos.y < 0):
	#	return
	#if(local_mouse_pos.x > bg_sprite_size.x or local_mouse_pos.y > bg_sprite_size.y):
	#	return
		
	
	#var cell_pos = $TileMap.world_to_map(global_mouse_pos)
	#rect drawing mode
	if(Input.is_action_pressed("shift_key")):
		if(Input.is_action_just_pressed("mouse1")):
			prev_cell_pos_temp = cell_pos
		elif(Input.is_action_pressed("mouse1")):
			#draw fake rect
			make_rect_tileMap(prev_cell_pos_temp, cell_pos, singleton.draw_tile_ind, temp_tile_map)
		else:
			prev_cell_pos_temp = cell_pos
			make_line_tileMap(prev_cell_pos_temp, cell_pos, singleton.draw_tile_ind, temp_tile_map)
		if(Input.is_action_just_pressed("mouse2")):
			if(Input.is_action_pressed("mouse2")):
				make_line_tileMap(prev_cell_pos_temp, cell_pos, singleton.draw_tile_ind, temp_tile_map)
		
	else: #standart point mode
		make_line_tileMap(prev_cell_pos_temp, cell_pos, singleton.draw_tile_ind, temp_tile_map)
		prev_cell_pos_temp = cell_pos

func delete_all_highlighted_entity_objs():
	var children = entity_obj_list.get_children()
	for child in children:
		if child.highlight == true:
			singleton.delete_entityInstance(child.entityInst_id, cur_level_ind)
			child.queue_free()

func position_list_handler():
	var position_obj_node = position_obj_t.instance()
	
	if(Input.is_action_just_pressed("mouse1")):
		var can_create_entity: bool = true
		for entityScene in get_tree().get_nodes_in_group("positionScene"):
			if entityScene.can_move == true:
				can_create_entity = false
				break
		if(can_create_entity):
			var mouse_pos = get_local_mouse_position()
			position_obj_node.position = Vector2(mouse_pos.x, mouse_pos.y)
			#Got uid for entityInst
			var entity_inst = singleton.add_positionInstance()
			#Put entityInst in database
			position_obj_node.entityInst_id = entity_inst["instId"]
			
			var savePos = [position_obj_node.position.x, position_obj_node.position.y]
			
			position_obj_list.add_child(position_obj_node)
			
			singleton.save_entityInst_pos(position_obj_node.entityInst_id, savePos)
		
	if(Input.is_action_pressed("mouse2")):
		pass
		#singleton.can_create_entity_obj = true

func entity_list_handler():
	
	var entity_obj_node = entity_obj_t.instance()
	
	if(Input.is_action_just_pressed("mouse1") and singleton.cur_entity_type_ind != -1):
		var can_create_entity: bool = true
		for entityScene in get_tree().get_nodes_in_group("entityScene"):
			if entityScene.can_move == true:
				can_create_entity = false
				break
		if(can_create_entity):
			var mouse_pos = get_local_mouse_position()
			if singleton.entity_snap_to_grid:
				var x_pos = (round((mouse_pos.x)/8)) * 8
				var y_pos = (round((mouse_pos.y)/8)) * 8
				entity_obj_node.position = Vector2(x_pos, y_pos)
			else:
				entity_obj_node.position = Vector2(mouse_pos.x, mouse_pos.y)
			#Got uid for entityInst
			var entity_inst = singleton.add_cur_entityInstance()
			#Put entityInst in database
			entity_obj_node.entityInst_id = entity_inst["instId"]
			entity_obj_node.level_ind = cur_level_ind
			
			var savePos = [entity_obj_node.position.x, entity_obj_node.position.y]
			
			entity_obj_list.add_child(entity_obj_node)
			
			singleton.save_entityInst_pos(entity_obj_node.entityInst_id, savePos)
			
			#If sprite path, not null, than changing sprite of new entity obj
			if len(entity_inst["__spritePath"]) > 0:
				var pic_path = entity_inst["__spritePath"]
				var img1 = Image.new()
				img1.load(pic_path)
				var imgTex = ImageTexture.new()
				imgTex.create_from_image(img1, 1)
				entity_obj_node.get_node("Sprite").texture = imgTex;
				
				var temp_sprite_size = singleton.get_sprite_size_from_path(pic_path)
				if temp_sprite_size:
					entity_obj_node.sprite_size = temp_sprite_size
					entity_obj_node.change_sprite_rect(Rect2(0,0,temp_sprite_size.x, temp_sprite_size.y))
					entity_obj_node.get_node("CollisionShape2D").shape.extents = Vector2(temp_sprite_size.x/2, temp_sprite_size.y/2)
				else:
					entity_obj_node.sprite_size = entity_obj_node.get_node("Sprite").texture.get_size()
					entity_obj_node.change_sprite_rect(Rect2(0,0,entity_obj_node.sprite_size.x, entity_obj_node.sprite_size.y))
					entity_obj_node.get_node("CollisionShape2D").shape.extents = entity_obj_node.sprite_size/2
			#Pretty long block
		
	if(Input.is_action_pressed("mouse2")):
		get_tree().call_group("tilemapEditorWindow", "remove_fields_of_entity")
		
		
	
	

func _physics_process(delta):
	area2d_follow_camera()
	move_level()
	if(mouse_on_level && !singleton.level_move_mode):
		local_mouse_pos = get_local_mouse_position()
		if(singleton.cur_editor_mode == singleton.EditorMode.ENTITY):
			entity_list_handler()
		if(singleton.cur_editor_mode == singleton.EditorMode.POSITION):
			position_list_handler()
		if(singleton.cur_editor_mode == singleton.EditorMode.COLLISION):
			temp_tile_map_handler(delta)
			tile_map_handler(delta)


func _on_Area2D_mouse_entered():
	print("level ", cur_level_ind, " entered")
	singleton.cur_level_ind = cur_level_ind
	prev_cell_pos = $TileMap.world_to_map(get_local_mouse_position())
	cell_pos = prev_cell_pos
	add_to_group("level_hovered")
	mouse_on_level = true
	pass # Replace with function body.


func _on_Area2D_mouse_exited():
	if(!is_moving):
		print("level ", cur_level_ind, " exited")
		remove_from_group("level_hovered")
		clean_temp_tileMap()
		mouse_on_level = false
		update()


func _on_AreaLeft_area_entered(area):
	if(!(first_sides_check || highlight)):
		return
	var levelContainerAdjacent = area.get_parent()
	if(levelContainerAdjacent == self):
		return
	var trigger_width = 1
	is_moving = false
	
	var pos_y_start = global_position.y
	var pos_y_end = global_position.y + map_size_px.y
	var pos_y_start_2 = levelContainerAdjacent.global_position.y
	var pos_y_end_2 = levelContainerAdjacent.global_position.y + levelContainerAdjacent.map_size_px.y
	
	var first_diff = pos_y_start_2 - pos_y_start
	var last_diff = pos_y_end_2 - pos_y_end
	
	#Put gates on this levelContainer
	var gate_node = gate_obj_t.instance()
	gate_node.side_offset = int(first_diff)
	gate_node.to_level_id = levelContainerAdjacent.cur_level_ind
	gate_node.side_id = 0
	if(first_diff > 0):
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.y - first_diff + last_diff
		gate_node.position.y = first_diff
		
		gate_node.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node.get_node("Sprite").position.y = (gate_size/2)
	else:
		first_diff = 0
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.y - first_diff + last_diff
		gate_node.position.y = first_diff
		gate_node.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node.get_node("Sprite").position.y =(gate_size/2)
	$Gates/Left.add_child(gate_node)
	var gate_inst_dict = singleton.add_gateInstance(cur_level_ind, gate_node)
	gate_node.inst_id = gate_inst_dict["instId"]
	
	#Put gates on adjacent levelContainer
	
	var first_diff_mirr = pos_y_start - pos_y_start_2
	var last_diff_mirr = pos_y_end - pos_y_end_2
	
	var gate_node_adjacent = gate_obj_t.instance()
	gate_node_adjacent.side_offset = int(first_diff_mirr)
	gate_node_adjacent.to_level_id = cur_level_ind
	gate_node_adjacent.side_id = 1
	gate_node_adjacent.get_node("Sprite").flip_h = true
	gate_node_adjacent.position.x = levelContainerAdjacent.map_size_px.x-trigger_width
	if(first_diff_mirr > 0):
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.y - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.x = levelContainerAdjacent.map_size_px.x-trigger_width
		gate_node_adjacent.position.y = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node_adjacent.get_node("Sprite").position.y = (gate_size/2)
	else:
		first_diff_mirr = 0
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.y - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.x = levelContainerAdjacent.map_size_px.x-trigger_width
		gate_node_adjacent.position.y = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node_adjacent.get_node("Sprite").position.y = (gate_size/2)
	
	var adjacent_gates = levelContainerAdjacent.get_node("Gates/Right")
	if(adjacent_gates == null):
		return
	var gate_inst_dict_adjucent = singleton.add_gateInstance(levelContainerAdjacent.cur_level_ind, gate_node_adjacent)
	gate_node_adjacent.inst_id = gate_inst_dict_adjucent["instId"]
	adjacent_gates.add_child(gate_node_adjacent)
		

func _on_AreaRight_area_entered(area):
	if(!highlight):
		return
	var levelContainerAdjacent = area.get_parent()
	if(levelContainerAdjacent == self):
		return
	var trigger_width = 1
	
	is_moving = false
	#global_position.x = levelContainerAdjacent.global_position.x - levelContainerAdjacent.map_size_px.x
	
	var pos_y_start = global_position.y
	var pos_y_end = global_position.y + map_size_px.y
	var pos_y_start_2 = levelContainerAdjacent.global_position.y
	var pos_y_end_2 = levelContainerAdjacent.global_position.y + levelContainerAdjacent.map_size_px.y
	
	var first_diff = pos_y_start_2 - pos_y_start
	var last_diff = pos_y_end_2 - pos_y_end
	
	var gate_node = gate_obj_t.instance()
	gate_node.side_offset = int(first_diff)
	gate_node.to_level_id = levelContainerAdjacent.cur_level_ind
	gate_node.side_id = 1
	gate_node.get_node("Sprite").flip_h = true
	
	
	
	if(first_diff > 0):
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.y - first_diff + last_diff
		gate_node.position.x = map_size_px.x-trigger_width
		gate_node.position.y = first_diff
		gate_node.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node.get_node("Sprite").position.y = gate_node.position.y + (gate_size/2)
	else:
		first_diff = 0
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.y - first_diff + last_diff
		print(last_diff)
		gate_node.position.x = map_size_px.x-trigger_width
		gate_node.position.y = first_diff
		gate_node.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node.get_node("Sprite").position.y = gate_node.position.y + (gate_size/2)
	$Gates/Right.add_child(gate_node)
	var gate_inst_dict = singleton.add_gateInstance(cur_level_ind, gate_node)
	gate_node.inst_id = gate_inst_dict["instId"]
	
	
	#Put gateInst in database
	
	#Put gates on adjacent levelContainer
	
	var first_diff_mirr = pos_y_start - pos_y_start_2
	var last_diff_mirr = pos_y_end - pos_y_end_2

	var gate_node_adjacent = gate_obj_t.instance()
	gate_node_adjacent.side_offset = int(first_diff_mirr)
	gate_node_adjacent.to_level_id = cur_level_ind
	gate_node_adjacent.side_id = 0
	if(first_diff_mirr > 0):
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.y - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.y = first_diff_mirr
		
		gate_node_adjacent.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node_adjacent.get_node("Sprite").position.y = (gate_size/2)
	else:
		first_diff_mirr = 0
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.y - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.y = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, trigger_width, gate_size]
		gate_node_adjacent.get_node("Sprite").position.y = (gate_size/2)
	
	var adjacent_gates = levelContainerAdjacent.get_node("Gates/Left")
	if(adjacent_gates == null):
		return
	
	var gate_inst_dict_adjucent = singleton.add_gateInstance(levelContainerAdjacent.cur_level_ind, gate_node_adjacent)
	gate_node_adjacent.inst_id = gate_inst_dict_adjucent["instId"]
	
	adjacent_gates.add_child(gate_node_adjacent)
	


func _on_AreaUp_area_entered(area):
	if(!(first_sides_check || highlight)):
		return
	var levelContainerAdjacent = area.get_parent()
	if(levelContainerAdjacent == self):
		return
	var trigger_width = 1
	is_moving = false
	#global_position.y = levelContainerAdjacent.global_position.y + levelContainerAdjacent.map_size_px.y
	
	var pos_x_start = global_position.x
	var pos_x_end = global_position.x + map_size_px.x
	var pos_x_start_2 = levelContainerAdjacent.global_position.x
	var pos_x_end_2 = levelContainerAdjacent.global_position.x + levelContainerAdjacent.map_size_px.x
	
	var first_diff = pos_x_start_2 - pos_x_start
	var last_diff = pos_x_end_2 - pos_x_end

	#Put gates on this levelContainer
	var gate_node = gate_obj_t.instance()
	gate_node.side_offset = int(first_diff)
	gate_node.to_level_id = levelContainerAdjacent.cur_level_ind
	gate_node.side_id = 2
	gate_node.get_node("Sprite").rotation_degrees = 90
	if(first_diff > 0):
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.x - first_diff + last_diff
		gate_node.position.x = first_diff
		gate_node.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node.get_node("Sprite").position.x = (gate_size/2)
	else:
		first_diff = 0
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.x - first_diff + last_diff
		gate_node.position.x = first_diff
		gate_node.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node.get_node("Sprite").position.x = (gate_size/2)
	$Gates/Up.add_child(gate_node)
	var gate_inst_dict = singleton.add_gateInstance(cur_level_ind, gate_node)
	gate_node.inst_id = gate_inst_dict["instId"]
	
	#Put gates on adjacent levelContainer
	
	var first_diff_mirr = pos_x_start - pos_x_start_2
	var last_diff_mirr = pos_x_end - pos_x_end_2
	#var size_y_diff_mirr = ((levelContainerAdjacent.global_position.y+levelContainerAdjacent.map_size_px.y) - (global_position.y + map_size_px.y))
	
	var gate_node_adjacent = gate_obj_t.instance()
	gate_node_adjacent.side_offset = int(first_diff_mirr)
	gate_node_adjacent.to_level_id = cur_level_ind
	gate_node_adjacent.side_id = 3
	gate_node_adjacent.get_node("Sprite").rotation_degrees = -90
	gate_node_adjacent.position.y = (levelContainerAdjacent.map_size_px.y-trigger_width)
	
	if(first_diff_mirr > 0):
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.x - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.x = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node_adjacent.get_node("Sprite").position.x = (gate_size/2)
	else:
		first_diff_mirr = 0
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.x - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.x = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node_adjacent.get_node("Sprite").position.x = (gate_size/2)
	
	var adjacent_gates = levelContainerAdjacent.get_node("Gates/Down")
	if(adjacent_gates == null):
		return
		
	var gate_inst_dict_adjucent = singleton.add_gateInstance(levelContainerAdjacent.cur_level_ind, gate_node_adjacent)
	gate_node_adjacent.inst_id = gate_inst_dict_adjucent["instId"]
	adjacent_gates.add_child(gate_node_adjacent)


func _on_AreaDown_area_entered(area):
	if(!highlight):
		return
	var levelContainerAdjacent = area.get_parent()
	if(levelContainerAdjacent == self):
		return
	var trigger_width = 1
	is_moving = false
	
	var pos_x_start = global_position.x
	var pos_x_end = global_position.x + map_size_px.x
	var pos_x_start_2 = levelContainerAdjacent.global_position.x
	var pos_x_end_2 = levelContainerAdjacent.global_position.x + levelContainerAdjacent.map_size_px.x
	
	var first_diff = pos_x_start_2 - pos_x_start
	var last_diff = pos_x_end_2 - pos_x_end

	#Put gates on this levelContainer
	var gate_node = gate_obj_t.instance()
	gate_node.side_offset = int(first_diff)
	gate_node.to_level_id = cur_level_ind
	gate_node.side_id = 3
	gate_node.get_node("Sprite").rotation_degrees = -90
	gate_node.position.y = (map_size_px.y-trigger_width)
	
	if(first_diff > 0):
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.x - first_diff + last_diff
		gate_node.position.x = first_diff
		gate_node.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node.get_node("Sprite").position.x = (gate_size/2)
	else:
		first_diff = 0
		if(last_diff > 0):
			last_diff = 0
		var gate_size = map_size_px.x - first_diff + last_diff
		gate_node.position.x = first_diff
		gate_node.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node.get_node("Sprite").position.x = (gate_size/2)
	$Gates/Down.add_child(gate_node)
	var gate_inst_dict = singleton.add_gateInstance(cur_level_ind, gate_node)
	gate_node.inst_id = gate_inst_dict["instId"]
	
	#Put gates on adjacent levelContainer
	
	var first_diff_mirr = pos_x_start - pos_x_start_2
	var last_diff_mirr = pos_x_end - pos_x_end_2 

	#Put gates on this levelContainer
	var gate_node_adjacent = gate_obj_t.instance()
	gate_node_adjacent.side_offset = int(first_diff_mirr)
	gate_node_adjacent.to_level_id = levelContainerAdjacent.cur_level_ind
	gate_node_adjacent.side_id = 2
	gate_node_adjacent.get_node("Sprite").rotation_degrees = 90
	if(first_diff_mirr > 0):
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.x - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.x = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node_adjacent.get_node("Sprite").position.x = (gate_size/2)
	else:
		first_diff_mirr = 0
		if(last_diff_mirr > 0):
			last_diff_mirr = 0
		var gate_size = levelContainerAdjacent.map_size_px.x - first_diff_mirr + last_diff_mirr
		gate_node_adjacent.position.x = first_diff_mirr
		gate_node_adjacent.triggerAABB = [0, 0, gate_size, trigger_width]
		gate_node_adjacent.get_node("Sprite").position.x = (gate_size/2)
	
	var adjacent_gates = levelContainerAdjacent.get_node("Gates/Up")
	if(adjacent_gates == null):
		return
	var gate_inst_dict_adjucent = singleton.add_gateInstance(levelContainerAdjacent.cur_level_ind, gate_node_adjacent)
	gate_node_adjacent.inst_id = gate_inst_dict_adjucent["instId"]
	adjacent_gates.add_child(gate_node_adjacent)


func _on_AreaLeft_area_exited(area):
	var gate_count:int = 0
	for gate in $Gates/Left.get_children():
		gate_count += 1
		singleton.delete_gateInstance(cur_level_ind, gate.inst_id)
		gate.queue_free()
	if(gate_count == 0):
		pass
	var levelContainerAdjacent = area.get_parent()
	var adjacent_node = levelContainerAdjacent.get_node("Gates/Right")
	if adjacent_node == null:
		return
	for gate in adjacent_node.get_children():
		singleton.delete_gateInstance(levelContainerAdjacent.cur_level_ind, gate.inst_id)
		gate.queue_free()

func delete_bga():
	$bgA.texture = null;
	singleton.change_bgRelPath("", cur_level_ind)

func delete_bgb():
	$bgB.texture = null;
	singleton.change_bgRelPath2("", cur_level_ind)

func _on_AreaRight_area_exited(area):
	for gate in $Gates/Right.get_children():
		singleton.delete_gateInstance(cur_level_ind, gate.inst_id)
		print("Deleting ", gate.inst_id)
		gate.queue_free()
	var levelContainerAdjacent = area.get_parent()
	var adjacent_node = levelContainerAdjacent.get_node("Gates/Left")
	if adjacent_node == null:
		return
	for gate_adjucent in adjacent_node.get_children():
		singleton.delete_gateInstance(levelContainerAdjacent.cur_level_ind, gate_adjucent.inst_id)
		print("Deleting2 ", gate_adjucent.inst_id)
		gate_adjucent.queue_free()


func _on_AreaUp_area_exited(area):
	for gate in $Gates/Up.get_children():
		singleton.delete_gateInstance(cur_level_ind, gate.inst_id)
		gate.queue_free()
	var levelContainerAdjacent = area.get_parent()
	var adjacent_node = levelContainerAdjacent.get_node("Gates/Down")
	if adjacent_node == null:
		return
	for gate_adjucent in adjacent_node.get_children():
		singleton.delete_gateInstance(levelContainerAdjacent.cur_level_ind, gate_adjucent.inst_id)
		gate_adjucent.queue_free()


func _on_AreaDown_area_exited(area):
	for gate in $Gates/Down.get_children():
		singleton.delete_gateInstance(cur_level_ind, gate.inst_id)
		gate.queue_free()
	var levelContainerAdjacent = area.get_parent()
	var adjacent_node = levelContainerAdjacent.get_node("Gates/Up")
	if adjacent_node == null:
		return
	for gate in adjacent_node.get_children():
		singleton.delete_gateInstance(levelContainerAdjacent.cur_level_ind, gate.inst_id)
		gate.queue_free()
