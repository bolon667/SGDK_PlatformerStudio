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
		String toPath = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text;
		if (System.IO.Directory.Exists(toPath))
		{
			infoLabel.Text = $"Project '{textEdit.Text}' already exists!";
			return;
		}
		String fromPath = workingDir + "/StudioType/SGDK/Engines/platformer";
		
		String dataJsonPath = workingDir + "/StudioType/SGDK/Projects/" + textEdit.Text + "/data.json";
		CopyFilesRecursively(fromPath, toPath);
		singleton.Call("update_cur_project_path", dataJsonPath);
		singleton.Call("update_cur_project_folder_path", toPath);
		singleton.Call("save_project");
		singleton.Call("save_project_last_paths");
		GetTree().ChangeScene("res://Scenes/mapEditorScene.tscn");
	}

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}
