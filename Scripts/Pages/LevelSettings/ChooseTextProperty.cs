using Godot;
using System;

public class ChooseTextProperty : HBoxContainer
{
	private Node singleton;
	//private LineEdit lineEdit;
	[Export] private String levelAttrName = "";
	
	public void loadContent(){
		//nothing
	}
	
	public override void _Ready()
	{
		singleton = (Node)GetNode("/root/singleton");
	}

	private void _on_OptionButton_item_selected(int index)
	{
		singleton.Call("change_level_attr", levelAttrName, index);
	}
	private void _on_applyAllLevelsBtn_button_down()
	{
		singleton.Call("change_all_level_attr", levelAttrName, "0");
	}

	private void _on_removeBtn_button_down()
	{
		singleton.Call("change_level_attr", levelAttrName, "0");
	}
	
	private void _on_makeDefaultBtn_button_down()
	{
		//singleton.Call("change_default_level_attr", levelAttrName, infoLabel.Text);
		
	}
}





