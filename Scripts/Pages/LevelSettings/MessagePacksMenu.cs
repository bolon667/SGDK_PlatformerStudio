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
		try {
		singleton = (Node)GetNode("/root/singleton");
		settingsContainer = (VBoxContainer)GetNode("CanvasLayer/VBoxContainer/ScrollContainer/SettingsContainer");
		curLevelLabel = (Label)GetNode("CanvasLayer/VBoxContainer/HBoxContainer2/curLevelLabel");
		curLevelLabel.Text = "Level_" + level_ind.ToString();
		loadSettings();
		} catch (Exception ex){
			GD.Print(ex);
		}
	}

	private void loadSettings()
	{
		Godot.Collections.Array children = (Godot.Collections.Array)settingsContainer.GetChildren();
		foreach(Node child in children)
		{
			if (child.HasMethod("get_container"))
			{
				VBoxContainer groupContainer = (VBoxContainer)child.Call("get_container");
				Godot.Collections.Array propertyContainer_arr = (Godot.Collections.Array)groupContainer.GetChildren();
				foreach (Node propertyContainer in propertyContainer_arr)
				{
					String levelAttrName = (String)propertyContainer.Get("levelAttrName");
					String attrVal = (String)singleton.Call("get_level_attr", levelAttrName, level_ind);
					Label infoLabel = (Label)propertyContainer.GetNode("VBoxContainer/infoLabel");
					if (attrVal.Length == 0)
					{
						infoLabel.Text = "NULL";
					}
					else
					{
						infoLabel.Text = attrVal;
					}
				}
			} else
			{
				String levelAttrName = (String)child.Get("levelAttrName");
				String attrVal = singleton.Call("get_level_attr", levelAttrName, level_ind).ToString();
				if (child.HasNode("VBoxContainer/infoLabel"))
				{
					Label infoLabel = (Label)child.GetNode("VBoxContainer/infoLabel");
					if (attrVal.Length == 0)
					{
						infoLabel.Text = "NULL";
					}
					else
					{
						infoLabel.Text = attrVal;
					}
				} else if (child.HasNode("VBoxContainer/HBoxContainer/OptionButton"))
				{
					OptionButton optionButton = (OptionButton)child.GetNode("VBoxContainer/HBoxContainer/OptionButton");
					optionButton.Select(int.Parse(attrVal));
				}

			}
			
		}

	}
	private void _on_ExitBtn_pressed()
	{
		QueueFree();
	}
}



