using Godot;
using System;

public class ChooseBoolProperty : HBoxContainer
{
	private Node singleton;
	private Label infoLabel;
	private CheckButton checkButton;
	public int levelInd;

	[Export] private String levelAttrName = "";
	[Export] private String defaultVal = "0";
	
	public void loadContent(){
		GD.Print("I;m here");
		GD.Print(infoLabel.Text);
		String temp = infoLabel.Text.ToString();
		if(temp == "1"){
			checkButton.Pressed = true;
		} else {
			checkButton.Pressed = false;
		}
	}
	
	public override void _Ready()
	{
		singleton = (Node)GetNode("/root/singleton");
		infoLabel = (Label)GetNode("VBoxContainer/infoLabel");

		checkButton = (CheckButton)GetNode("VBoxContainer/HBoxContainer/CheckButton");
		levelInd = int.Parse(singleton.Get("cur_level_ind").ToString());
	}

	private void _on_applyAllLevelsBtn_button_down()
	{
		singleton.Call("change_all_level_attr", levelAttrName, infoLabel.Text);
	}


	private void _on_removeBtn_button_down()
	{
		singleton.Call("change_level_attr", levelAttrName, defaultVal, levelInd);
		infoLabel.Text = defaultVal;
	}
	
	private void _on_makeDefaultBtn_button_down()
	{
		singleton.Call("change_default_level_attr", levelAttrName, infoLabel.Text);
		
	}
	
	private void _on_CheckButton_toggled(bool button_pressed)
	{
		String tempText;
		if(button_pressed){
			tempText = "1";
		} else {
			tempText = "0";
		}
		infoLabel.Text = tempText;
		
		singleton.Call("change_level_attr", levelAttrName, tempText, levelInd);
	}

}










