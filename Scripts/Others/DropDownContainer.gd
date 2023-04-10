extends VBoxContainer

export var defaultOpen:bool = false
export var groupName:String = "test"

func _ready():
	$pressGroupBtn.text = groupName
	$pressGroupBtn.pressed = defaultOpen
		

func _on_pressGroupBtn_toggled(button_pressed):
	$GroupContainer.visible = button_pressed

func get_container():
	return $GroupContainer
