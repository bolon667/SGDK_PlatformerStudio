using Godot;
using System;
using System.IO;

public class InputDialog : Control
{
	private String workingDir;
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
		confirmButton.Connect("button_down", this, "makeNewProject");
		exitButton.Connect("button_down", this, "exitAction");
		workingDir = System.IO.Directory.GetCurrentDirectory();
		singleton = (Node)GetNode("/root/singleton");
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

		if (System.IO.Directory.Exists(ditrectoryPath))
		{
			infoLabel.Text = $"Project '{textEdit.Text}' already exists!";
			return;
		}
		String fromPathCodeTemplate = workingDir + "/StudioType/SGDK/Engines/platformer/code_template";
		String fromPathNewProject = workingDir + "/StudioType/SGDK/Engines/platformer/new_project";
		String fromPathIcon = workingDir + "/StudioType/SGDK/Engines/platformer/icon.png";

		CopyFilesRecursively(fromPathNewProject, toPathBuild);
		CopyFilesRecursively(fromPathCodeTemplate, toPathCodeTemplate);
		System.IO.File.Copy(fromPathIcon, toPathIcon, true);
		String dataJsonPath = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/data.json";
		singleton.Call("update_cur_project_path", dataJsonPath);
		singleton.Call("update_cur_project_folder_path", ditrectoryPath);
		singleton.Call("create_new_project");
		singleton.Call("save_project_last_paths");
		GetTree().ChangeScene("res://Scenes/mapEditorScene.tscn");
	}

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}
