
using System;
using System.IO;
using System.Diagnostics;
using Godot;
using Godot.Collections;

public class buidProject : Node
{
	// Declare member variables here. Examples:
	// private int a = 2;
	// private string b = "text";


	private String engineRootPath = "";
	private String fullEngineRootPath = "";
	private String fullEngineResPath = "";

	private String fullEmulatorPath = "";


	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		getEngineRootPath();
	}

	private void getEngineRootPath()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		engineRootPath = (String)singleton.Call("get_engine_root_path");
		String workingDir = System.IO.Directory.GetCurrentDirectory();
		fullEngineRootPath = workingDir + engineRootPath.Substring(1);
		fullEngineRootPath = fullEngineRootPath.Replace("\\", "/");

		fullEngineResPath = fullEngineRootPath + "/build/res";
		fullEmulatorPath = workingDir + "/Emulators/BlastEm/blastem.exe";
	}

	

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

	private void makeNewProjectFromTemplate()
	{
		String fromFolder = engineRootPath + "/project_template";
		String toFolder = engineRootPath + "/build";

		CopyFilesRecursively(fromFolder, toFolder);
		GD.Print("New SGDK project created");
	}

	public void buildProject()
	{
		
		makeNewProjectFromTemplate();
		copyRes();
		codeReplacer();
		genResCode();
		compileProject();
		runProject();
	}

	private void genResCode()
	{
		Process iStartProcess = new Process();
		iStartProcess.StartInfo.FileName = fullEngineResPath + "/sgdkrescodegen.exe";
		iStartProcess.StartInfo.WorkingDirectory = fullEngineResPath;
		iStartProcess.StartInfo.Arguments = "";
		//iStartProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
		iStartProcess.Start();
		iStartProcess.WaitForExit(120000);
	}
	private void copyRes()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		Dictionary levelDict = (Dictionary)singleton.Call("get_cur_level");
		String bgaPath = (String)levelDict["bgRelPath"];
		String levelName = (String)levelDict["identifier"];
		if (bgaPath.Length > 0)
		{
			String fileExtension = bgaPath.Substring(bgaPath.FindLast("."));

			String toPath = fullEngineResPath + "/gfx/" + levelName + "_bga" + fileExtension;
			GD.Print(toPath);
			System.IO.File.Copy(bgaPath, toPath, true);
		}
	}

	private void compileProject()
	{
		GD.Print("Compile project");

		Process iStartProcess = new Process();
		iStartProcess.StartInfo.FileName = fullEngineRootPath + @"\build\compile.bat";
		iStartProcess.StartInfo.WorkingDirectory = fullEngineRootPath + @"\build";
		iStartProcess.StartInfo.Arguments = ""; 
		//iStartProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
		iStartProcess.Start();
		iStartProcess.WaitForExit(120000); 
		GD.Print(fullEngineRootPath);
	}

	private void runProject()
	{
		GD.Print("Running project");
		Process iStartProcess = new Process();
		iStartProcess.StartInfo.FileName = fullEmulatorPath;
		iStartProcess.StartInfo.Arguments = fullEngineRootPath + "/build/out/rom.bin";
		//iStartProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
		iStartProcess.Start();
		
	}

	private void codeReplacer()
	{
		mapsC_CodeReplacer();
		mapsH_CodeReplacer();
		levelsC_CodeReplacer();
	}

	private void levelsC_CodeReplacer()
	{	
		String levelsC_path = engineRootPath + "/build/src/levels.c";
		String levelsCode = System.IO.File.ReadAllText(levelsC_path);
		String levelFullArr_code = genLevelFulArr_Code();
		levelsCode = levelsCode.Replace("//$levelFullArr$", levelFullArr_code);
		System.IO.File.WriteAllText(levelsC_path, levelsCode);
		GD.Print("levels.c code replaced");
		
	}

	private String localToResFolder(String path)
	{
		return path.Replace(fullEngineRootPath + "/build/res/", "");
	}

	private String pathToResName(String path)
	{
		String result = path;
		//Remove extension from path
		result = result.Substring(0, result.FindLast("."));
		result = result.Replace("/", "_");

		return result;
	}

	private String genLvlCode(int curLevel)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		Dictionary levelDict = (Dictionary)singleton.Call("get_cur_level");
		int cellSize = (int)singleton.Call("get_cell_size");
		Vector2 levelSizePx = (Vector2)singleton.Call("get_level_size");
		String levelSizePxText = "{" + levelSizePx.x.ToString() + ", " + levelSizePx.y.ToString() + "}";
		Vector2 levelSizeTiles = new Vector2();
		levelSizeTiles.x = (int)levelSizePx.x / cellSize;
		levelSizeTiles.y = (int)levelSizePx.y / cellSize;
		String levelSizeTilesText = "{" + levelSizeTiles.x.ToString() + ", " + levelSizeTiles.y.ToString() + "}";
		String bgaPath = (String)levelDict["bgRelPath"];
		String bgbPath = (String)levelDict["bgRelPath2"];

		String levelName = (String)levelDict["identifier"];

		Godot.Collections.Array startPos = (Godot.Collections.Array)singleton.Call("get_start_pos");
		String startPosText = "{" + startPos[0].ToString() + ", " + startPos[1].ToString() + "}";

		
		String bgaResName = levelName + "_bga";
		String bgbResName = levelName + "_bgb";
		String bgaMapResName;
		String bgaTilesetResName;
		String bgaPalResName;
		String bgbMapResName;
		String bgbTilesetResName;
		String bgbPalResName;
		if (bgaPath.Length > 0)
		{
			bgaMapResName = "&" + bgaResName + "_map";
			bgaTilesetResName = "&" + bgaResName + "_tileset";
			bgaPalResName = "&" + bgaResName + "_pal";
		}
		else
		{
			bgaMapResName = "NULL";
			bgaTilesetResName = "NULL";
			bgaPalResName = "NULL";
		}

		if (bgbPath.Length > 0)
		{
			bgbMapResName = "&" + bgbResName + "_map";
			bgbTilesetResName = "&" + bgbResName + "_tileset";
			bgbPalResName = "&" + bgbResName + "_pal";
		}
		else
		{
			bgbMapResName = "NULL";
			bgbTilesetResName = "NULL";
			bgbPalResName = "NULL";
		}

		String collMapName = "collisionMap";
		if (curLevel > 0)
		{
			collMapName = collMapName + curLevel.ToString();

		}

		String levelCode = $"{bgaMapResName}, {bgbMapResName}, {bgaTilesetResName}, {bgbTilesetResName}, {bgaPalResName}, {bgbPalResName}, {startPosText}, {collMapName}, {levelSizePxText}, {levelSizeTilesText}";
		return levelCode;
	}

	private String genLevelFulArr_Code()
	{
		int curLevel = 0;
		String levelCode = genLvlCode(curLevel);
		String entityAllCode = "0";
		String result = "const LevelFull const LevelFull_arr[] = {";
		result += "{";
		result += "{" + levelCode + "},{" + entityAllCode + "}}, ";
		/*
		{ { &level_map, NULL, &level_tileset, NULL, &level_palette, NULL, { 74, 300}, collisionMap, { 768, 768}, { 48, 48},},{ 0} },};";
		*/
		result += "};\n";
		GD.Print(result);
		return result;
	}

	//System.IO.File.WriteAllText(path, code);
	private void mapsC_CodeReplacer()
	{
		String mapsC_path = engineRootPath + "/build/src/maps.c";
		String mapsCode = System.IO.File.ReadAllText(mapsC_path);
		String collisionCode = genCollisionArrayCode(0);
		mapsCode = mapsCode.Replace("//$collisionMaps$", collisionCode);
		System.IO.File.WriteAllText(mapsC_path, mapsCode);
		GD.Print("maps.c code replaced");

	}

	private void mapsH_CodeReplacer()
	{
		String mapsH_path = engineRootPath + "/build/inc/maps.h";
		String mapsCode = System.IO.File.ReadAllText(mapsH_path);

		int curLevel = 0;

		String collisionDefCode = genCollisionDefinitionCode(curLevel);

		mapsCode = mapsCode.Replace("//$collisionMaps$", collisionDefCode);
		System.IO.File.WriteAllText(mapsH_path, mapsCode);
		GD.Print("maps.h code replaced");

	}

	private String genCollisionDefinitionCode(int levelNum)
	{

		TileMap tileMap = (TileMap)GetNode("/root/Control/TileMapEditorWindow/BGSprite/TileMap");
		Sprite bgSprite = (Sprite)GetNode("/root/Control/TileMapEditorWindow/BGSprite");

		Vector2 cellSize = tileMap.CellSize;

		Vector2 bgSpriteSize = bgSprite.Texture.GetSize();
		Vector2 bgSpriteSizeInCells = new Vector2(bgSpriteSize.x / cellSize.x, bgSpriteSize.y / cellSize.y);

		String levelNumText = "";
		if (levelNum > 0)
		{
			levelNumText = levelNum.ToString();

		}

		String resultCode = "extern const u8 const collisionMap" + levelNumText + "[" + bgSpriteSizeInCells.y + "][" + bgSpriteSizeInCells.x + "];\n";

		return resultCode;
	}

	private String genCollisionArrayCode(int levelNum)
	{
		
		TileMap tileMap = (TileMap)GetNode("/root/Control/TileMapEditorWindow/BGSprite/TileMap");
		Sprite bgSprite = (Sprite)GetNode("/root/Control/TileMapEditorWindow/BGSprite");

		Vector2 cellSize = tileMap.CellSize;

		Vector2 bgSpriteSize = bgSprite.Texture.GetSize();
		Vector2 bgSpriteSizeInCells = new Vector2(bgSpriteSize.x / cellSize.x, bgSpriteSize.y / cellSize.y);

		String levelNumText = "";
		if(levelNum > 0)
		{
			levelNumText = levelNum.ToString();

		}

		String resultCode = "const u8 const collisionMap" + levelNumText + "[" + bgSpriteSizeInCells.y + "][" + bgSpriteSizeInCells.x + "] = {\n";

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

		return resultCode;
	}


	//  // Called every frame. 'delta' is the elapsed time since the previous frame.
	//  public override void _Process(float delta)
	//  {
	//      
	//  }

}
