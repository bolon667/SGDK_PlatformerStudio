extends Node2D

var side_offset: int = 0
var inst_id: int = -1
var to_level_id:int = 0
var side_id: int = 0;
# side_id reference
#0 = left
#1 = right
#2 = up
#3 = down
var triggerAABB: Array = [0,0,0,0]

func _ready():
	$ColorRect.rect_position = Vector2(triggerAABB[0], triggerAABB[1])
	$ColorRect.rect_size = Vector2(triggerAABB[2]-$ColorRect.rect_position.x, triggerAABB[3]-$ColorRect.rect_position.y)

