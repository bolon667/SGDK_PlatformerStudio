using Godot;
using System;

public class MessagePacksMenu : Control
{
	private Node singleton;
	private VBoxContainer settingsContainer;

	public override void _Ready()
	{
		singleton = (Node)GetNode("/root/singleton");
		settingsContainer = (VBoxContainer)GetNode("CanvasLayer/VBoxContainer/HBoxContainer/VBoxChooseEntity/ScrollContainer/SettingsContainer");

		loadSettings();

	}

	private void loadSettings()
	{
		Godot.Collections.Array children = (Godot.Collections.Array)settingsContainer.GetChildren();
		foreach(Node child in children)
		{
			String levelAttrName = (String)child.Get("levelAttrName");
			String attrVal = (String)singleton.Call("get_level_attr", levelAttrName);
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



