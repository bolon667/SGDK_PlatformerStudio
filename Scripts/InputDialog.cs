using Godot;
using System;
using System.IO;

public class InputDialog : Control
{
	private String workingDir;
	private String engineName = "platformer";
	OptionButton curEngineOption;
	TextEdit textEdit;
	Button confirmButton;
	Button exitButton;
	Node singleton;
	Label infoLabel;
	private static void CopyFilesRecursively(string sourcePath, string targetPath)
	{
		//Now Create all of the directories
		foreach (string dirPath in System.IO.Directory.GetDirectories(sourcePath, "*", SearchOption.AllDirectories))
		{
			System.IO.Directory.CreateDirectory(dirPath.Replace(sourcePath, targetPath));
		}

		//Copy all the files & Replaces any files with the same name
		foreach (string newPath in System.IO.Directory.GetFiles(sourcePath, "*.*", SearchOption.AllDirectories))
		{
			System.IO.File.Copy(newPath, newPath.Replace(sourcePath, targetPath), true);
		}
	}

	public override void _Ready()
	{
		textEdit = (TextEdit)GetNode("ColorRect/VBoxContainer2/VBoxContainer/TextEdit");
		confirmButton = (Button)GetNode("ColorRect/VBoxContainer2/VBoxContainer/Button");
		exitButton = (Button)GetNode("ColorRect/VBoxContainer2/HBoxContainer/ExitBtn");
		infoLabel = (Label)GetNode("ColorRect/VBoxContainer2/VBoxContainer/InfoLabel");
		curEngineOption = (OptionButton)GetNode("ColorRect/VBoxContainer2/VBoxContainer/curEngineOption");
		confirmButton.Connect("button_down", this, "makeNewProject");
		exitButton.Connect("button_down", this, "exitAction");
		workingDir = System.IO.Directory.GetCurrentDirectory();
		singleton = (Node)GetNode("/root/singleton");

		clearOptionButton();
		findPossibleEngines();
	}

	public void exitAction()
	{
		QueueFree();
	}

	public void makeNewProject()
	{
		String ditrectoryPath = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text;
		String toPathBuild = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/build";
		String toPathCodeTemplate = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/code_template";
		String toPathIcon = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/icon.png";
		String toFastInfo = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/fast_info.json";


		if (System.IO.Directory.Exists(ditrectoryPath))
		{
			infoLabel.Text = $"Project '{textEdit.Text}' already exists!";
			return;
		}
		String fromPathCodeTemplate = workingDir + $"/StudioType/SGDK/Engines/{engineName}/code_template";
		String fromPathNewProject = workingDir + $"/StudioType/SGDK/Engines/{engineName}/new_project";
		String fromPathIcon = workingDir + $"/StudioType/SGDK/Engines/{engineName}/icon.png";
		String fromFastInfo = workingDir + $"/StudioType/SGDK/Engines/{engineName}/fast_info.json";

		CopyFilesRecursively(fromPathNewProject, toPathBuild);
		CopyFilesRecursively(fromPathCodeTemplate, toPathCodeTemplate);
		System.IO.File.Copy(fromPathIcon, toPathIcon, true);
		System.IO.File.Copy(fromFastInfo, toFastInfo, true);
		String dataJsonPath = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/data.json";
		singleton.Call("update_cur_project_path", dataJsonPath);
		singleton.Call("update_cur_project_folder_path", ditrectoryPath);
		singleton.Call("create_new_project");
		singleton.Call("change_cur_engine", curEngineOption.GetItemText(curEngineOption.Selected));
		singleton.Call("save_project_last_paths");
		GetTree().ChangeScene("res://Scenes/mapEditorScene.tscn");
	}

	private void clearOptionButton()
	{
		int itemCount = curEngineOption.GetItemCount();
		for(int i=0; i<itemCount; i++)
		{
			curEngineOption.RemoveItem(i);
		}

	}

	private void findPossibleEngines()
	{
		String checkPath = workingDir + "/StudioType/SGDK/Engines/";
		int cutNum = checkPath.Length;
		bool firstSelect = false;
		foreach (string dirPath in System.IO.Directory.GetDirectories(checkPath, "*", SearchOption.TopDirectoryOnly))
		{
			String tempEngineName = dirPath.Substring(cutNum);
			String iconPath = checkPath + tempEngineName + "/engine_icon.png";
			Texture tex = ResourceLoader.Load(iconPath) as Texture;
			curEngineOption.AddIconItem(tex, tempEngineName);
			if (!firstSelect)
			{
				firstSelect = true;
				curEngineOption.Select(0);

			}
		}
	}
	
	private void _on_curEngineOption_item_selected(int index)
	{
		engineName = curEngineOption.GetItemText(index);
	}

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}



