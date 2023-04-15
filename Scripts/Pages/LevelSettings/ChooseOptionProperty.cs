using Godot;
using System;

public class ChooseOptionProperty : HBoxContainer
{
	private Node singleton;
	private Label infoLabel;
	private OptionButton optionButton;
	public int levelInd;

	[Export] private String levelAttrName = "";
	[Export] private String defaultVal = "NULL";
	
	public void loadContent(){
		GD.Print("I;m here");
		GD.Print(infoLabel.Text);
		String temp = infoLabel.Text.ToString();
		if(temp == "NULL"){
			optionButton.Select(0);
			
		} else {
			optionButton.Select(int.Parse(temp));
		}
		
	}
	
	public override void _Ready()
	{
		singleton = (Node)GetNode("/root/singleton");
		infoLabel = (Label)GetNode("VBoxContainer/infoLabel");
		
		optionButton = (OptionButton)GetNode("VBoxContainer/HBoxContainer/OptionButton");
		levelInd = int.Parse(singleton.Get("cur_level_ind").ToString());
	}

	private void _on_OptionButton_item_selected(int index)
	{
		singleton.Call("change_level_attr", levelAttrName, index.ToString(), levelInd);
		infoLabel.Text = index.ToString();
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

}







