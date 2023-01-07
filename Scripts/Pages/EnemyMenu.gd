extends Control


onready var entity_item_t = preload("res://Scenes/Pages/entityListItem.tscn")


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	for entity_type in singleton.entity_types:
		var val = singleton.entity_types[entity_type]
		var entity_item = entity_item_t.instance()
		entity_item.get_node("TextBtn").text = entity_type
		$HBoxContainer/VBoxChooseEntity/ScrollContainer/EntityListContainer.add_child(entity_item)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
