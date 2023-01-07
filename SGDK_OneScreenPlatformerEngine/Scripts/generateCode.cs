using Godot;
using System;
using System.IO;

public class generateCode : Button
{
	// Declare member variables here. Examples:
	// private int a = 2;
	// private string b = "text";

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		FileDialog saveFileDialog = (FileDialog)GetNode("/root/Control/CanvasLayer/SaveFile");
		saveFileDialog.Connect("file_selected", this, "_OnSaveDialog");
	}

	private void _OnSaveDialog(String path)
	{
		TileMap tileMap = (TileMap)GetNode("/root/Control/TileMapEditorWindow/TileMap");
		Sprite bgSprite = (Sprite)GetNode("/root/Control/TileMapEditorWindow/BGSprite");

		Vector2 cellSize = tileMap.CellSize;

		Vector2 bgSpriteSize = bgSprite.Texture.GetSize();
		Vector2 bgSpriteSizeInCells = new Vector2(bgSpriteSize.x / cellSize.x, bgSpriteSize.y / cellSize.y);

		String resultCode = "const u8 const collArr[][] = {\n";

		for (int curY = 0; curY < (int)bgSpriteSizeInCells.y; curY++)
		{
			resultCode += "{";
			for (int curX = 0; curX < (int)bgSpriteSizeInCells.x; curX++)
			{
				resultCode += (tileMap.GetCell(curX, curY) + 1).ToString() + ", ";
			}
			resultCode += "}, \n";
		}
		resultCode += "};\n";

		//String workingDir = System.IO.Directory.GetCurrentDirectory();
		GD.Print(path);
		if (path[path.Length-1] == '/')
		{
			System.IO.File.WriteAllText(path+"/generatedCode.txt", resultCode);
		} else
		{
			System.IO.File.WriteAllText(path, resultCode);
		}
		
	}
	public void test()
	{
		FileDialog saveFileDialog = (FileDialog)GetNode("/root/Control/CanvasLayer/SaveFile");
		saveFileDialog.PopupCentered();

	}
}
