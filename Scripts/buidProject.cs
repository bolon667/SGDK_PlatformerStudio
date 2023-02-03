
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

	private String fullTemplatePath = "";

	private String fullEmulatorPath = "";


	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		getEngineRootPath();
	}

	private void getEngineRootPath()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		engineRootPath = (String)singleton.Call("get_project_folder_path");
		String workingDir = System.IO.Directory.GetCurrentDirectory();
		//fullEngineRootPath = workingDir + engineRootPath.Substring(1);
		fullEngineRootPath = engineRootPath;

		fullEngineRootPath = fullEngineRootPath.Replace("\\", "/");

		fullEngineResPath = fullEngineRootPath + "/build/res";
		fullEmulatorPath = workingDir + "/Emulators/BlastEm/blastem.exe";

		fullTemplatePath = workingDir + "/StudioType/SGDK/Engines/platformer/build";
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

		int cellSize = (int)singleton.Call("get_cell_size");
		int shiftVal = 3;
		if (cellSize == 16)
		{
			shiftVal = 4;
		}
		String definesCode = "";
		definesCode += "#define TILE_SHIFT_LEFT_VALUE " + shiftVal.ToString() + "\n";
		definesCode += "#define CELL_SIZE " + cellSize.ToString() + "\n";
		definesCode += "#define CELL_SIZE_DIV_2 " + (cellSize/2).ToString() + "\n";

		typesCode = typesCode.Replace("//$definesCode$", definesCode);

		
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
		result += "  Trigger* trigger;\n";
		result += "  u16 triggerInd;\n";
		result += "  Sprite* spr;\n";


		foreach (Godot.Collections.Dictionary field in mergedFieldDef_arr)
		{
			if(!(bool)field["hasStruct"])
			{
				continue;
			}
			String fieldName = (String)field["identifier"];
			String fieldType = (String)field["inCodeType"];
			result += $"  {fieldType} {fieldName};\n";

		}
		result += "} EntityMerged;\n";

		return result;
	}

	private void makeNewProjectFromTemplate()
	{
		//String fromFolder = engineRootPath + "/project_template";
		String fromFolder = fullTemplatePath;
		
		String toFolder = engineRootPath + "/build";

		CopyFilesRecursively(fromFolder, toFolder);
		GD.Print("New SGDK project created");
	}

	public void buildProject()
	{
		try
		{
			makeNewProjectFromTemplate();

			copyRes();


			codeReplacer();
			//return;
			//System.Threading.Thread.Sleep(3000);
			compileProject();
			runProject();
		} catch(Exception ex)
		{
			GD.Print(ex);
		}
		
	}

	private void genResCode()
	{
		GD.Print("Gen res code");
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
		int levelCount = (int)singleton.Call("get_level_count");
		int curLevel = 0;
		
		while(curLevel < levelCount)
		{
			Dictionary levelDict = (Dictionary)singleton.Call("get_level", curLevel);
			String bgaPath = (String)levelDict["bgRelPath"];
			String bgbPath = (String)levelDict["bgRelPath2"];
			String levelName = (String)levelDict["identifier"];
			if (bgaPath.Length > 0)
			{
				String fileExtension = bgaPath.Substring(bgaPath.FindLast("."));

				String toPath = fullEngineResPath + $"/gfx/Level_{curLevel.ToString()}_bga" + fileExtension;
				//GD.Print(toPath);
				System.IO.File.Copy(bgaPath, toPath, true);
			}
			if (bgbPath.Length > 0)
			{
				String fileExtension = bgbPath.Substring(bgbPath.FindLast("."));

				String toPath = fullEngineResPath + $"/gfx/Level_{curLevel.ToString()}_bgb" + fileExtension;
				System.IO.File.Copy(bgbPath, toPath, true);
			}
			curLevel++;
		}
		
		GD.Print("bgA and bgB copied");
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
		GD.Print(fullEngineRootPath + "/build/out/rom.bin");
		iStartProcess.StartInfo.Arguments = '"' + fullEngineRootPath + "/build/out/rom.bin\"";
		//iStartProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
		iStartProcess.Start();
		
	}

	private void codeReplacer()
	{
		genResCode();
		
		mapsC_CodeReplacer();
		mapsH_CodeReplacer();
		playerC_CodeReplacer();

		levelsC_CodeReplacer();
		typesH_CodeReplacer();

		entityHandlerC_CodeReplacer();

	}

	private void entityHandlerC_CodeReplacer()
	{
		String entityHandlerC_path = engineRootPath + "/build/src/entityHandler.c";
		String entityHandlerCode = System.IO.File.ReadAllText(entityHandlerC_path);
		String finalCode = entityHandlerCode.Replace("//$showEntityFuncs$", genShowEntityCodeAll());
		System.IO.File.WriteAllText(entityHandlerC_path, finalCode);
		GD.Print("entityHandler.c code replaced");
	}

	private String genShowEntityCodeAll()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_entity_names");
		//Generating funcs
		foreach(String entityName in entityNames)
		{
			String checkPath = engineRootPath + "/code_template/showEntity/show" + entityName + ".c";
			if (System.IO.File.Exists(checkPath))
			{
				String insertData = System.IO.File.ReadAllText(checkPath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				result += insertData + "\n";
			}
		}

		//Putting this funcs in arr
		result += "void(* showEntityFuncArr[])(EntityMerged*) = {";
		foreach (String entityName in entityNames)
		{
			result += "show" + entityName + ", ";
		}
		result += "};\n";
		return result;
	}

	private void levelsC_CodeReplacer()
	{
		GD.Print("levels.c code replacing...");
		String levelsC_path = engineRootPath + "/build/src/levels.c";
		String levelsCode = System.IO.File.ReadAllText(levelsC_path);
		String levelFullArr_code = genLevelFulArr_Code();
		levelsCode = levelsCode.Replace("//$levelFullArr$", levelFullArr_code);
		//loadLevel replace
		String loadLevelPath = engineRootPath + "/code_template/loadLevel.c";
		String loadLevelCode = System.IO.File.ReadAllText(loadLevelPath);
		levelsCode = levelsCode.Replace("//$loadLevel$", loadLevelCode);
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
		Dictionary levelDict = (Dictionary)singleton.Call("get_level", curLevel);
		int cellSize = (int)singleton.Call("get_cell_size");
		Vector2 levelSizePx = (Vector2)singleton.Call("get_level_size", curLevel);
		String levelSizePxText = "{" + levelSizePx.x.ToString() + ", " + levelSizePx.y.ToString() + "}";
		Vector2 levelSizeTiles = new Vector2();
		levelSizeTiles.x = (int)levelSizePx.x / cellSize;
		levelSizeTiles.y = (int)levelSizePx.y / cellSize;
		String levelSizeTilesText = "{" + levelSizeTiles.x.ToString() + ", " + levelSizeTiles.y.ToString() + "}";
		String bgaPath = (String)levelDict["bgRelPath"];
		String bgbPath = (String)levelDict["bgRelPath2"];

		String levelName = (String)levelDict["identifier"];

		GD.Print(9);
		String posArrText = $"&Position_arr_Level_{curLevel.ToString()}";
		GD.Print(9.5);
		//(int)int.Parse(entityInst["width"].ToString());

		String posAmountText = singleton.Call("get_positionInstancesAmount_by_levelNum", curLevel).ToString();
		GD.Print(10);
		
		String bgaResName = $"Level_{curLevel.ToString()}_bga";
		String bgbResName = $"Level_{curLevel.ToString()}_bgb";
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

		String levelCode = $"const Level const lvl_Level_{curLevel.ToString()} = {{{bgaMapResName}, {bgbMapResName}, {bgaTilesetResName}, {bgbTilesetResName}, {bgaPalResName}, {bgbPalResName}, {posArrText}, {posAmountText}, {collMapName}, {levelSizePxText}, {levelSizeTilesText}}};\n";
		return levelCode;
	}

	private String genPositionsCode(int curLevel)
	{
		GD.Print($"Gen Position code for level {curLevel}");
		Node singleton = (Node)GetNode("/root/singleton");
		String result = ""; ;
		Godot.Collections.Array positionInstances = (Godot.Collections.Array)singleton.Call("get_positionInstances_by_levelNum", curLevel);
		result += $"const Vect2D_s16 const Position_arr_Level_{curLevel.ToString()}[] = ";

		//Opening Position_arr block
		result += "{";
		foreach (Godot.Collections.Dictionary positionInst in positionInstances)
		{
			Godot.Collections.Array px = (Godot.Collections.Array)positionInst["px"];
			//Opening Position block
			result += "{";
			result += px[0] + ", ";
			result += px[1];

			//Closing Position block
			result += "}, ";
		}
		//Closing Position_arr block
		result += "};\n";
		return result;
	}

	private String genEntityMergedCode(int curLevel)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef");
		GD.Print(1);
		String result = "";;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		GD.Print(2);
		result += $"const EntityMerged const EntityMerged_arr_Level_{curLevel.ToString()}[] = ";

		//Opening EntityMerged_arr block
		result += "{";
		int curEntityInd = 0;
		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{
			//Getting useful data about entity
			String entityName = (String)entityInst["__identifier"];
			int mergedId = (int)mergedIdsDict[entityName];
			Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];
			int width = 0;
			int height = 0;
			GD.Print(0.4);
			width = (int)int.Parse(entityInst["width"].ToString());
			GD.Print(0.5);
			//Studio crashes if you don't do proper conversion.
			height = (int)int.Parse(entityInst["height"].ToString());

			int[] spd = { 0, 0 };

			//Opening entityMerged block
			result += "{";

			//Applying to generated code

			/*****---> Struct reminder <---*****
			 
				typedef struct {
				  u16 entityType;
				  bool alive;
				  Vect2D_s16 posInt;
				  Vect2D_f32 pos;
				  Vect2D_f16 spd;
				  Vect2D_s16 size;
				  bool onScreen;
				  Trigger* trigger;
				  u16 triggerInd;
				  Sprite* spr;
				} EntityMerged;

			**********************************/

			result += $"{mergedId.ToString()},"; //mergedId
			result += $" TRUE, "; //alive 
			result += "{" + pos[0].ToString() + ", " + pos[1].ToString() + "}, "; //posInt
			result += "{FIX32(" + pos[0].ToString() + "), FIX32(" + pos[1].ToString() + ")}, "; //pos
			result += "{" + spd[0].ToString() + ", " + spd[1].ToString() + "}, "; //spd
			result += "{" + width.ToString() + ", " + height.ToString() + "}, "; //size
			result += "FALSE, "; //onScreen
			result += $"&Trigger_arr_Level_{curLevel}[{curEntityInd}],"; //trigger
			result += $"{curEntityInd},"; //triggerInd = curEntityInd, since all entity have trigger, which is not good for preformance reasons
			result += "NULL,"; //spr

			//Checking every field
			Godot.Collections.Array fieldInstances = (Godot.Collections.Array)entityInst["fieldInstances"];


			foreach (Godot.Collections.Dictionary fieldDef in mergedFieldDef_arr)
			{
				//String inCodeType = (String)field["__inCodeType"];
				if (!(bool)fieldDef["hasStruct"])
				{
					continue;
				}
				
				//finding value of this fieldName, why? Because in struct order is nessesary
				var value = "";
				foreach (Godot.Collections.Dictionary fieldInst in fieldInstances)
				{
					//if names of fieldDef and fieldInstance are equal
					if((String)fieldInst["__identifier"] == (String)fieldDef["identifier"])
					{
						GD.Print(fieldInst["__identifier"]);
						//Then value is found
						value = fieldInst["__value"].ToString();
						break;
					}

				}
				//In most caces entity doesn't have every possible field, so, we are using default value
				
				if(value.Length == 0)
				{
					value = fieldDef["defaultValue"].ToString();
				}
				result += value + ", ";
				
				
			}

			curEntityInd++;
			//Closing entityMerged block
			result += "}, ";
		}
		//Closing EntityMerged_arr block
		result += "};\n";
		return result;
	}

	private String genTriggerCode(int curLevel)
	{
		GD.Print("curLevel: ", curLevel);
		
		Node singleton = (Node)GetNode("/root/singleton");
		GD.Print(0);
		String result = ""; ;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");

		GD.Print(1);
		
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		
		result += $"const Trigger const Trigger_arr_Level_{curLevel.ToString()}[] = ";

		//Opening Trigger_arr block
		result += "{";

		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{
			GD.Print(2);
			//Getting useful data about entity
			if (!entityInst.Contains("__identifier"))
			{
				continue;
			}
			String entityName = (String)entityInst["__identifier"];

			GD.Print(entityName);
			GD.Print(3);

			Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];
			
			Godot.Collections.Array triggerAABB = new Godot.Collections.Array();
			
			if (entityInst.Contains("triggerAABB"))
			{
				triggerAABB = (Godot.Collections.Array)entityInst["triggerAABB"];
			} else
			{
				triggerAABB.Add(0);
				triggerAABB.Add(0);
				triggerAABB.Add(0);
				triggerAABB.Add(0);

			}
			
			float triggerType = 0;
			if (entityInst.Contains("triggerType")) {
				triggerType = (float)entityInst["triggerType"];

			}
			
			int triggerValue = 0;
			int triggerValue2 = 0;
			int triggerValue3 = 0;
			if (entityInst.Contains("triggerValue"))
			{
				triggerValue = (int)int.Parse(entityInst["triggerValue"].ToString());
			}
		
			if (entityInst.Contains("triggerValue2"))
			{
				triggerValue2 = (int)int.Parse(entityInst["triggerValue2"].ToString());
			}
		
			if (entityInst.Contains("triggerValue3"))
			{
				triggerValue3 = (int)int.Parse(entityInst["triggerValue3"].ToString());
			}
			
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
			result += "{" + $"{pos[0]}, {pos[1]}" + "}, "; //pos
			result += "{" + $"{triggerAABB[0]}, {triggerAABB[1]}, {triggerAABB[2]}, {triggerAABB[3]}" + "}, "; //triggerRect
			result += $"{triggerType}, "; //triggerType
			result += $"{triggerValue}, "; //triggerValue
			result += $"{triggerValue2}, "; //triggerValue2
			result += $"{triggerValue3}, "; //triggerValue3
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

		//Bullet count
		int bulletAmount = 10;
		//Entity count
		int entityAmount = (int)singleton.Call("get_entityInstanAmount_by_levelNum", curLevel);
		//Bullet
		result += bulletAmount.ToString() + ", ";
		result += "NULL, ";
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
		GD.Print("Gen LevelFullArr_code");
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		int amountOfLevel = (int)singleton.Call("get_level_count");


		
		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			result += genPositionsCode(curLevel);
			
			result += genTriggerCode(curLevel);
			
			result += genEntityMergedCode(curLevel);
			
			result += genEntityAllCode(curLevel);
			
			result += genLvlCode(curLevel);
			
		}
		//return result;
		result += "const LevelFull const LevelFull_arr[] = {";
		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			result += "{" + $"&lvl_Level_{curLevel.ToString()}, &EntityAll_arr_Level_{curLevel.ToString()}" + "}, ";
		}
		result += "};\n";

		return result;

	}

	//System.IO.File.WriteAllText(path, code);
	private void mapsC_CodeReplacer()
	{
		GD.Print("maps.c code replacing...");
		Node singleton = (Node)GetNode("/root/singleton");
		int amountOfLevel = (int)singleton.Call("get_level_count");
		String mapsC_path = engineRootPath + "/build/src/maps.c";
		String mapsCode = System.IO.File.ReadAllText(mapsC_path);
		String collisionCode = "";
		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			collisionCode += genCollisionArrayCode(curLevel);
			//break;
		}
		mapsCode = mapsCode.Replace("//$collisionMaps$", collisionCode);
		System.IO.File.WriteAllText(mapsC_path, mapsCode);
		GD.Print("maps.c code replaced");

	}

	private void mapsH_CodeReplacer()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		int amountOfLevel = (int)singleton.Call("get_level_count");

		String mapsH_path = engineRootPath + "/build/inc/maps.h";
		String mapsCode = System.IO.File.ReadAllText(mapsH_path);

		String collisionDefCode = "";

		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			collisionDefCode += genCollisionDefinitionCode(curLevel);
		}

		mapsCode = mapsCode.Replace("//$collisionMaps$", collisionDefCode);
		System.IO.File.WriteAllText(mapsH_path, mapsCode);
		GD.Print("maps.h code replaced");

	}

	private void playerC_CodeReplacer()
	{
		String playerCReplace_path = engineRootPath + "/code_template/src/player.c";
		if (System.IO.File.Exists(playerCReplace_path))
		{
			String playerCode_path = engineRootPath + "/build/src/player.c";
			String playerCReplaceCode = System.IO.File.ReadAllText(playerCReplace_path);
			System.IO.File.WriteAllText(playerCode_path, playerCReplaceCode);
		}

		//String playerCode = System.IO.File.ReadAllText(playerCode_path);
		//playerCode = playerCode.Replace(@"//$updateAnimations$", updateAnimationsCode);




		
		GD.Print("player.c code replaced");
	}

	private String genCollisionDefinitionCode(int levelNum)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array collisionMap = (Godot.Collections.Array)singleton.Call("get_collisionMapForLevel", levelNum);

		int cellSize = (int)singleton.Call("get_cell_size");

		//New level requires level size pxWei pxHei
		Vector2 levelSize = (Vector2)singleton.Call("get_level_size", levelNum);

		Vector2 bgSpriteSizeInCells = new Vector2(levelSize.x / cellSize, levelSize.y / cellSize);

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
		GD.Print($"Collision code replacing for level {levelNum}");
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array collisionMap = (Godot.Collections.Array)singleton.Call("get_collisionMapForLevel", levelNum);

		
		int cellSize = (int)singleton.Call("get_cell_size");

		
		//New level requires level size pxWei pxHei
		Vector2 levelSize = (Vector2)singleton.Call("get_level_size", levelNum);
		
		Vector2 bgSpriteSizeInCells = new Vector2(levelSize.x / cellSize, levelSize.y / cellSize);

		
		String levelNumText = "";
		if(levelNum > 0)
		{
			levelNumText = levelNum.ToString();

		}

		
		String resultCode = "const u8 const collisionMap" + levelNumText + "[" + bgSpriteSizeInCells.y + "][" + bgSpriteSizeInCells.x + "] = {\n";

		if(collisionMap.Count == 0)
		{
			GD.Print("Collision map is null, making empty collision map");
			collisionMap = new Godot.Collections.Array();
			//If you have no collision map, then, make empty collision map
			float size = bgSpriteSizeInCells.x * bgSpriteSizeInCells.y;
			for (int i=0; i< size; i++)
			{
				collisionMap.Add(0);
			}
		}

		int curX = 0;
		int curY = 0;
		while (curY < (int)bgSpriteSizeInCells.y)
		{
			resultCode += "{";
			while (curX < (int)bgSpriteSizeInCells.x)
			{
				resultCode += (collisionMap[curX+(curY* (int)bgSpriteSizeInCells.x)]).ToString() + ", ";
				curX++;
			}
			
			curX = 0;
			curY++;
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
