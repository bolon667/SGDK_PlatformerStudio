using Godot;
using System;

public class MessagePacksMenu : Control
{
	private Node singleton;
	private VBoxContainer settingsContainer;
	private Label curLevelLabel;
	public int level_ind;
	public override void _Ready()
	{
		singleton = (Node)GetNode("/root/singleton");
		settingsContainer = (VBoxContainer)GetNode("CanvasLayer/VBoxContainer/ScrollContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer");
		curLevelLabel = (Label)GetNode("CanvasLayer/VBoxContainer/HBoxContainer2/curLevelLabel");
		curLevelLabel.Text = "Level_" + level_ind.ToString();
		loadSettings();

	}

	private void loadSettings()
	{
		Godot.Collections.Array children = (Godot.Collections.Array)settingsContainer.GetChildren();
		foreach(Node child in children)
		{
			String levelAttrName = (String)child.Get("levelAttrName");
			String attrVal = (String)singleton.Call("get_level_attr", levelAttrName, level_ind);
			Label infoLabel = (Label)child.GetNode("VBoxContainer/infoLabel");
			if(attrVal.Length == 0)
			{
				infoLabel.Text = "NULL";
			} else
			{
				infoLabel.Text = attrVal;
			}
		}

	}
	private void _on_ExitBtn_pressed()
	{
		QueueFree();
	}
}



