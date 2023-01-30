extends Control

onready var messagePackContainer = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/MessagePackContainer
onready var messageContainer = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/ScrollContainer/MessageConainer
onready var addNewMessageBtn = $CanvasLayer/VBoxContainer/HBoxContainer/VBoxContainer/HBoxContainer2/VBoxContainer/AddNewMessage
onready var msgPackBtn_t = preload("res://Scenes/Pages/MessagePack/messagePackItem.tscn")
onready var msgBtn_t = preload("res://Scenes/Pages/MessagePack/messageItem.tscn")
onready var messagePackNameEdit = $CanvasLayer/VBoxContainer/TextEditContainer/HBoxContainer/MessagePackNameEdit
onready var messageTextEdit = $CanvasLayer/VBoxContainer/TextEditContainer/HBoxContainer2/MessageTextEdit
# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var messagePack_amount: int = 0
var message_arr_len:int = 0
# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	load_messagePacks()

func clear_messagePacks():
	var children = messagePackContainer.get_children()
	for child in children:
		child.queue_free()

func load_messagePacks():
	clear_messagePacks()
	var message_pack_arr: Array = singleton.get_messagePack_arr()
	for message_pack in message_pack_arr:
		var msg_pack_name: String = message_pack["name"]
		var msg_pack_node = msgPackBtn_t.instance()
		msg_pack_node.get_node("TextBtn").text = msg_pack_name
		messagePackContainer.add_child(msg_pack_node)
	messagePack_amount = len(message_pack_arr)
	
	if messagePack_amount == 0:
		addNewMessageBtn.disabled = true
		messagePackNameEdit.readonly = true
		messagePackNameEdit.text = ""
		block_message_edit()
		clear_messages()
	else:
		addNewMessageBtn.disabled = false
		
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func clear_messages():
	var children = messageContainer.get_children()
	for child in children:
		child.queue_free()

func load_messages():
	clear_messages()
	var message_pack_arr: Array = singleton.get_messagePack_arr()
	if(len(message_pack_arr) == 0):
		return
	messagePackNameEdit.text = message_pack_arr[singleton.cur_messagePack_ind]["name"]
	messagePackNameEdit.readonly = false
	message_arr_len = 0
	for message in message_pack_arr[singleton.cur_messagePack_ind]["messages"]:
		var msg_node = msgBtn_t.instance()
		msg_node.get_node("TextBtn").text = message
		messageContainer.add_child(msg_node)
		message_arr_len += 1
	
	
	if message_arr_len == 0:
		block_message_edit()
	
		

func block_message_edit():
	messageTextEdit.readonly = true
	messageTextEdit.text = ""


func update_message_edit(text: String):
	messageTextEdit.readonly = false
	messageTextEdit.text = text

func _on_ExitBtn_pressed():
	
	queue_free()


func _on_AddNewMessagePack_button_down():
	singleton.add_messagePack()
	load_messagePacks()
	pass # Replace with function body.


func _on_AddNewMessage_button_down():
	singleton.add_message()
	load_messages()


func _on_MessagePackNameEdit_text_changed():
	singleton.change_messagePack_name(messagePackNameEdit.text)
	load_messagePacks()
	pass # Replace with function body.


func _on_MessageTextEdit_text_changed():
	singleton.change_cur_message(messageTextEdit.text)
	load_messages()
