
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

	private String workingDir;


	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		getEngineRootPath();
	}

	private void getEngineRootPath()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		engineRootPath = (String)singleton.Call("get_project_folder_path");
		workingDir = System.IO.Directory.GetCurrentDirectory();
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
		GD.Print($"Got cell_size {cellSize}");
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
		String outFolder = toFolder + "/out";
		//Delete out folder
		//System.IO.Directory.Delete(outFolder, true);

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
			applyMods();
			//return;
			//System.Threading.Thread.Sleep(3000);
			compileProject();
			runProject();
		} catch(Exception ex)
		{
			GD.Print(ex);
		}
		
	}

	private void applyMods()
	{
		fileReplceMod();

	}

	private void fileReplceMod()
	{
		String fromPath;
		String toPath;
		fromPath = engineRootPath + "/code_template/src/";
		toPath  = engineRootPath + "/build/src/";
		CopyFilesRecursively(fromPath, toPath);
		fromPath = engineRootPath + "/code_template/inc/";
		toPath = engineRootPath + "/build/inc/";
		CopyFilesRecursively(fromPath, toPath);
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
			int bgaMode = 0;
			if (levelDict.Contains("bgaMode"))
			{
				bgaMode = (int)int.Parse(levelDict["bgaMode"].ToString());
			}
			String bgbPath = (String)levelDict["bgRelPath2"];
			int bgbMode = 0;
			if (levelDict.Contains("bgbMode"))
			{
				bgbMode = (int)int.Parse(levelDict["bgbMode"].ToString());
			}
			String levelName = (String)levelDict["identifier"];
			if (bgaPath.Length > 0)
			{
				String fileName = bgaPath.Substring(bgaPath.FindLast("/"));
				String fileExtension = bgaPath.Substring(bgaPath.FindLast("."));
				String toPath;
				switch (bgaMode)
				{
					case 0: //map mode
						toPath = fullEngineResPath + $"/gfx/{fileName}";
						//GD.Print(toPath);
						System.IO.File.Copy(bgaPath, toPath, true);
						break;
					case 1: //image mode
						toPath = fullEngineResPath + $"/images/{fileName}";
						//GD.Print(toPath);
						System.IO.File.Copy(bgaPath, toPath, true);
						break;
				}
			}
			if (bgbPath.Length > 0)
			{
				String fileName = bgbPath.Substring(bgbPath.FindLast("/"));
				String fileExtension = bgbPath.Substring(bgbPath.FindLast("."));
				String toPath;
				switch (bgbMode)
				{
					case 0: //map mode
						toPath = fullEngineResPath + $"/gfx/{fileName}";
						//GD.Print(toPath);
						System.IO.File.Copy(bgbPath, toPath, true);
						break;
					case 1: //image mode
						toPath = fullEngineResPath + $"/images/{fileName}";
						//GD.Print(toPath);
						System.IO.File.Copy(bgbPath, toPath, true);
						break;
				}
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

		customScriptsC_CodeReplacer();
		customScriptsH_CodeReplacer();
	}

	private void customScriptsC_CodeReplacer()
	{
		String customScriptsC_path = engineRootPath + "/build/src/customScripts.c";
		String customScriptsCode = System.IO.File.ReadAllText(customScriptsC_path);
		String finalCode = customScriptsCode.Replace("//$customScripts$", genCustomScriptsCodeC());
		System.IO.File.WriteAllText(customScriptsC_path, finalCode);
		GD.Print("customScripts.c code replaced");
	}

	private void customScriptsH_CodeReplacer()
	{
		String customScriptsH_path = engineRootPath + "/build/inc/customScripts.h";
		String customScriptsCode = System.IO.File.ReadAllText(customScriptsH_path);
		String finalCode = customScriptsCode.Replace("//$customScripts$", genCustomScriptsCodeH());
		System.IO.File.WriteAllText(customScriptsH_path, finalCode);
		GD.Print("customScripts.h code replaced");
	}

	private String genCustomScriptsCodeC()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		//find scripts in use.
		string scriptsFolderPath = engineRootPath + "/code_template/customScripts";
		String[] paths = System.IO.Directory.GetFiles(scriptsFolderPath);
		Array<String> fileNames = new Array<String>();
		//Adding scripts code
		foreach (String path in paths)
		{
			String curScriptText = System.IO.File.ReadAllText(path);
			result += curScriptText + "\n";
			String fileNameNoExt = System.IO.Path.GetFileName(path);
			fileNameNoExt = fileNameNoExt.Substring(0, fileNameNoExt.Find("."));
			fileNames.Add(fileNameNoExt);
		}
		//Add default empty func
		result += "void emptyScript(){};\n";
		//Creating arr of funcs, to be able to use them, when collision with trigger occurs
		result += "void (* customScriptArr[])(void) = {";
		foreach (String funcName in fileNames)
		{
			result += funcName + ", ";
		}
		result += "};\n";
		return result;
	}

	private String genCustomScriptsCodeH()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		//find scripts in use.
		string scriptsFolderPath = engineRootPath + "/code_template/customScripts";
		String[] paths = System.IO.Directory.GetFiles(scriptsFolderPath);
		//Creating externs for funcs (scripts)
		foreach (String path in paths)
		{
			String fileNameNoExt = System.IO.Path.GetFileName(path);
			fileNameNoExt = fileNameNoExt.Substring(0, fileNameNoExt.Find("."));
			result += $"extern void {fileNameNoExt}();\n";
		}
		result += "extern void emptyScript();\n";
		//Creating arr of funcs, to be able to use them, when collision with trigger occurs
		result += "extern void (* customScriptArr[])(void);\n";
		return result;
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
			String defaultEntityCodePath = engineRootPath + "/code_template/showEntity/showDefault.c";
			String checkPath = engineRootPath + "/code_template/showEntity/show" + entityName + ".c";
			if (System.IO.File.Exists(checkPath))
			{
				String insertData = System.IO.File.ReadAllText(checkPath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				result += insertData + "\n";
			} else
			{
				String insertData = System.IO.File.ReadAllText(defaultEntityCodePath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				System.IO.File.WriteAllText(checkPath, insertData);
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
		GD.Print($"Gen lvl code for level {curLevel}");
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

		


		int bgaMode = 0;
		int bgbMode = 0;
		if (levelDict.Contains("bgaMode"))
		{
			bgaMode = (int)int.Parse(levelDict["bgaMode"].ToString());
		}
		if (levelDict.Contains("bgbMode"))
		{
			bgbMode = (int)int.Parse(levelDict["bgbMode"].ToString());
		}

		String levelName = (String)levelDict["identifier"];

		//GD.Print(9);
		String posArrText = $"&Position_arr_Level_{curLevel.ToString()}";
		//GD.Print(9.5);
		//(int)int.Parse(entityInst["width"].ToString());

		String posAmountText = singleton.Call("get_positionInstancesAmount_by_levelNum", curLevel).ToString();
		//GD.Print(10);

		String bgaMapResName = "NULL";
		String bgaTilesetResName = "NULL";
		String bgaPalResName = "NULL";
		String bgaImageName = "NULL";
		String bgbMapResName = "NULL";
		String bgbTilesetResName = "NULL";
		String bgbPalResName = "NULL";
		String bgbImageName = "NULL";
		String musicName = "NULL";
		String beforeLevelScriptName = "NULL";
		String everyFrameScriptName = "emptyScript";
		String afterLevelScriptName = "NULL";

		if (levelDict.Contains("beforeLevelScript"))
		{
			String temp = (String)levelDict["beforeLevelScript"];
			if (temp.Length > 0)
			{
				beforeLevelScriptName = temp;
			}
		}

		if (levelDict.Contains("everyFrameScript"))
		{
			String temp = (String)levelDict["everyFrameScript"];
			if (temp.Length > 0)
			{
				everyFrameScriptName = temp;
			}
		}

		if (levelDict.Contains("afterLevelScript"))
		{
			String temp = (String)levelDict["afterLevelScript"];
			if (temp.Length > 0)
			{
				afterLevelScriptName = temp;
			}
		}

		if (levelDict.Contains("musicName"))
		{
			String temp = (String)levelDict["musicName"];
			if (temp.Length > 0)
			{
				musicName = "&mus_" + temp;
			}
		}

		if (bgaPath.Length > 0)
		{
			String fileName = bgaPath.Substring(bgaPath.FindLast("/")+1);
			fileName = fileName.Substring(0, fileName.LastIndexOf("."));
			switch (bgaMode)
			{
				case 0: //map mode
					bgaMapResName = "&" + fileName + "_map";
					bgaTilesetResName = "&" + fileName + "_tileset";
					bgaPalResName = "&" + fileName + "_pal";
					break;
				case 1: //image mode
					bgaImageName = "&img_" + fileName;
					break;
			}		
		}

		if (bgbPath.Length > 0)
		{
			String fileName = bgbPath.Substring(bgbPath.FindLast("/")+1);
			fileName = fileName.Substring(0, fileName.LastIndexOf("."));
			switch (bgbMode)
			{
				case 0: //map mode
					bgbMapResName = "&" + fileName + "_map";
					bgbTilesetResName = "&" + fileName + "_tileset";
					bgbPalResName = "&" + fileName + "_pal";
					break;
				case 1: //image mode
					bgbImageName = "&img_" + fileName;
					break;
			}
		}


		String collMapName = "collisionMap";
		if (curLevel > 0)
		{
			collMapName = collMapName + curLevel.ToString();

		}



		String levelCode = $"const Level const lvl_Level_{curLevel.ToString()} = {{{bgaMapResName}, {bgbMapResName}, {bgaTilesetResName}, {bgbTilesetResName}, {bgaPalResName}, {bgbPalResName}," +
			$" {bgaImageName}, {bgbImageName}, {posArrText}, {posAmountText}, {collMapName}, {levelSizePxText}, {levelSizeTilesText}, {musicName}, {beforeLevelScriptName}, {everyFrameScriptName}," +
			$" {afterLevelScriptName}}};\n";
		return levelCode;
	}

	private String genMessagePacksCode(int curLevel)
	{
		String result = "";
		GD.Print($"Gen MessagePacks code for level {curLevel}");
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array messagePacks = (Godot.Collections.Array)singleton.Call("get_messagePacks_by_levelNum", curLevel);
		int amountOfMessages = 0;
		int curMsgPack = 0;
		if (messagePacks == null)
		{
			result += $"const Message const msgArr_Level_{curLevel}_msgPack_{curMsgPack}[] = {{}};\n";
			result += $"const MessagePack const MessagePack_Level_{curLevel}[] = {{}};\n";
			return result;
		}
		
		String messagePackCode = $"const MessagePack const MessagePack_Level_{curLevel}[] = {{";
		foreach (Godot.Collections.Dictionary messagePack in messagePacks)
		{
			result += $"const Message const msgArr_Level_{curLevel}_msgPack_{curMsgPack}[] = {{";
			amountOfMessages = 0;
			foreach (String message in (Godot.Collections.Array)messagePack["messages"])
			{
				result += $"{{\"{message}\", {message.Length}}}, ";
				amountOfMessages++;
			}
			result += "};\n";
			String messagePackName = (String)messagePack["name"];
			messagePackCode += $"{{msgArr_Level_{curLevel}_msgPack_{curMsgPack}, {amountOfMessages}, \"{messagePackName}\"}}, ";
			curMsgPack++;
		}
		messagePackCode += "};\n";
		result += messagePackCode;
		return result;
	}

	private String genPositionsCode(int curLevel)
	{
		GD.Print($"Gen Position code for level {curLevel}");
		Node singleton = (Node)GetNode("/root/singleton");
		String result = "";
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
		GD.Print($"Created Position code for level {curLevel}");
		return result;
	}

	private String genEntityMergedCode(int curLevel)
	{
		GD.Print("Gen EntityMerged Code...");
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef");
		//GD.Print(1);
		String result = "";;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		//GD.Print(2);
		result += $"const EntityMerged const EntityMerged_arr_Level_{curLevel.ToString()}[] = ";

		//Opening EntityMerged_arr block
		result += "{";
		int curTriggerInd = 0;
		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{
			//Getting useful data about entity
			if (!entityInst.Contains("__identifier"))
			{
				continue;
			}
			bool addTrigger = false;
			String entityName = (String)entityInst["__identifier"];
			int mergedId = (int)mergedIdsDict[entityName];
			Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];
			int width = 0;
			int height = 0;
		//	GD.Print(0.4);
			width = (int)int.Parse(entityInst["width"].ToString());
		//	GD.Print(0.5);
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


			if (!entityInst.Contains("addTrigger") || (bool)entityInst["addTrigger"] == false)
			{
				result += $"NULL,"; //trigger = NULL
			} else
			{
				addTrigger = true;
				result += $"&Trigger_arr_Level_{curLevel}[{curTriggerInd}],"; //trigger
			}
			result += $"{curTriggerInd},"; //triggerInd = curEntityInd, since all entity have trigger, which is not good for preformance reasons
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
						//GD.Print(fieldInst["__identifier"]);
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
			if (addTrigger)
			{
				curTriggerInd++;
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
		GD.Print("Gen TriggerCode for level ", curLevel);
		
		Node singleton = (Node)GetNode("/root/singleton");
		//GD.Print(0);
		String result = ""; ;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");

		//GD.Print(1);
		
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		
		result += $"const Trigger const Trigger_arr_Level_{curLevel.ToString()}[] = ";

		//Opening Trigger_arr block
		result += "{";

		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{
			//GD.Print(2);
			//Getting useful data about entity
			if (!entityInst.Contains("__identifier"))
			{
				continue;
			}
			if(!entityInst.Contains("addTrigger") || (bool)entityInst["addTrigger"] == false)
			{
				continue;
			}
			String entityName = (String)entityInst["__identifier"];

			//GD.Print(entityName);
			//GD.Print(3);

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
			//GD.Print(4);
			int triggerType = 0;
			if (entityInst.Contains("triggerType")) {
				
				triggerType = (int)int.Parse(entityInst["triggerType"].ToString());

			}
			//GD.Print(4.5);
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
			//GD.Print(5);

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
			
			result += genMessagePacksCode(curLevel);

			result += genLvlCode(curLevel);

		}
		//return result;
		result += "const LevelFull const LevelFull_arr[] = {";
		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			result += "{" + $"&lvl_Level_{curLevel.ToString()}, &EntityAll_arr_Level_{curLevel.ToString()}, &MessagePack_Level_{curLevel.ToString()}" + "}, ";
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
		//GD.Print($"Collision code replacing for level {levelNum}");
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
