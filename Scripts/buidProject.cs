
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

	private void typesH_CodeReplacer()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef");

		String typesH_path = engineRootPath + "/build/inc/types.h";
		String typesCode = System.IO.File.ReadAllText(typesH_path);
		String mergedStructCode = genMergedStructCode(mergedFieldDef_arr);
		typesCode = typesCode.Replace("//$entityMergedStruct$", mergedStructCode);
		System.IO.File.WriteAllText(typesH_path, typesCode);
		GD.Print("types.h code replaced");
	}

	private String genMergedStructCode(Godot.Collections.Array mergedFieldDef_arr)
	{
		String result = "typedef struct {\n";
		//Predefined strct vars

		result += "  u16 entityType;\n";
		result += "  bool alive;\n";
		result += "  Vect2D_s16 posInt;\n";
		result += "  Vect2D_f32 pos;\n";
		result += "  Vect2D_f16 spd;\n";
		result += "  Vect2D_s16 size;\n";
		result += "  bool onScreen;\n";
		result += "  Sprite* spr;\n";

		foreach (Godot.Collections.Dictionary field in mergedFieldDef_arr)
		{

			String fieldName = (String)field["identifier"];
			String fieldType = (String)field["inCodeType"];
			result += $"  {fieldType} {fieldName};\n";

		}
		result += "} EntityMerged;\n";

		return result;
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

		//System.Threading.Thread.Sleep(3000);
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
		//Deleting "out/res" folder
		//System.IO.Directory.Delete(fullEngineRootPath + "/build/out/res", true);
		//Deleting "out/src" folder
		//System.IO.Directory.Delete(fullEngineRootPath + "/build/out/src", true);

		//Compiling
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
		genResCode();
		mapsC_CodeReplacer();
		mapsH_CodeReplacer();
		levelsC_CodeReplacer();
		typesH_CodeReplacer();
		
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

	private String genEntityMergedCode(int curLevel)
	{
		Node singleton = (Node)GetNode("/root/singleton");

		String result = "";;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);

		result += $"const EntityMerged const EntityMerged_arr_Level_{curLevel.ToString()}[] = ";

		//Opening EntityMerged_arr block
		result += "{";

		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{


			//Getting useful data about entity
			String entityName = (String)entityInst["__identifier"];
			int mergedId = (int)mergedIdsDict[entityName];
			Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];
			float width = (float)entityInst["width"];
			float height = (float)entityInst["height"];
			int[] spd = { 0, 0 };

			//Opening entityMerged block
			result += "{";

			//Applying to generated code

			/*****---> Struct reminder <---*****
			 
				result += "  u16 entityType;\n";
				result += "  bool alive;\n";
				result += "  Vect2D_s16 posInt;\n";
				result += "  Vect2D_f32 pos;\n";
				result += "  Vect2D_f16 spd;\n";
				result += "  Vect2D_s16 size;\n";
				result += "  bool onScreen;\n";
				result += "  Sprite* spr;\n";

			**********************************/

			result += $"{mergedId.ToString()},"; //mergedId
			result += $" TRUE, "; //alive 
			result += "{" + pos[0].ToString() + ", " + pos[1].ToString() + "}, "; //posInt
			result += "{FIX32(" + pos[0].ToString() + "), FIX32(" + pos[1].ToString() + ")}, "; //pos
			result += "{" + spd[0].ToString() + ", " + spd[1].ToString() + "}, "; //spd
			result += "{" + width.ToString() + ", " + height.ToString() + "}, "; //size
			result += "FALSE, "; //onScreen
			result += "NULL,"; //spr

			//Checking every field
			Godot.Collections.Array fieldInstances = (Godot.Collections.Array)entityInst["fieldInstances"];

			foreach (Godot.Collections.Dictionary field in fieldInstances)
			{
				//String inCodeType = (String)field["__inCodeType"];
				String value = (String)field["__value"];
				result += value + ", ";
			}

			//Closing entityMerged block
			result += "}, ";

		}
		//Closing EntityMerged_arr block
		result += "};\n";
		return result;
	}

	private String genTriggerCode(int curLevel)
	{
		Node singleton = (Node)GetNode("/root/singleton");

		String result = ""; ;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);

		result += $"const Trigger const Trigger_arr_Level_{curLevel.ToString()}[] = ";

		//Opening Trigger_arr block
		result += "{";

		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{
			//Getting useful data about entity
			String entityName = (String)entityInst["__identifier"];
			int mergedId = (int)mergedIdsDict[entityName];
			Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];
			float width = (float)entityInst["width"];
			float height = (float)entityInst["height"];
			int[] spd = { 0, 0 };

			//Opening Trigger block
			result += "{";

			//Applying to generated code

			/*****---> Struct reminder <---*****
			 
				typedef struct {
					bool alive;
					Vect2D_s16 firstPos;
					Vect2D_s16 lastPos;
					s8 trigger_type;
					s8 trigger_value;
					s16 triggerHp;
				} Trigger;

			**********************************/

			result += "TRUE, "; //alive
			result += "{0,0}, "; //firstPos
			result += "{0,0}, "; //lastPos
			result += "0, "; //triggerType
			result += "0, "; //triggerValue
			result += "1, "; //triggerHp

			//Closing Trigger block
			result += "}, ";

		}


		//Closing Trigger_arr block
		result += "};\n";
		return result;
	}

	private String genEntityAllCode(int curLevel)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		
		String result = "";
		result += $"const EntityAll const EntityAll_arr_Level_{curLevel}[] = ";
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);


		//Opening entityAll_arr block
		result += "{";

		//Entity count
		int entityAmount = (int)singleton.Call("get_entityInstanAmount_by_levelNum", curLevel);
		//Entity all
		result += entityAmount.ToString() + ", ";
		result += $"&EntityMerged_arr_Level_{curLevel.ToString()}, ";
		//Trigger
		result += entityAmount.ToString() + ", ";
		result += $"&Trigger_arr_Level_{curLevel.ToString()}, ";


		//Closing entityAll_arr block
		result += "};\n";

		return result;
	}


	private String genLevelFulArr_Code()
	{
		int curLevel = 0;
		String levelCode = genLvlCode(curLevel);
		String result = "";
		result += genEntityMergedCode(curLevel);
		result += genTriggerCode(curLevel);
		result += genEntityAllCode(curLevel);

		result += "const LevelFull const LevelFull_arr[] = {";
		result += "{";
		result += "{" + levelCode + "}," + $"&EntityAll_arr_Level_{curLevel.ToString()}" + "}, ";
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
