
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
		String curEngineName = singleton.Call("get_cur_engine").ToString();
		//fullEngineRootPath = workingDir + engineRootPath.Substring(1);
		fullEngineRootPath = engineRootPath;

		fullEngineRootPath = fullEngineRootPath.Replace("\\", "/");

		fullEngineResPath = fullEngineRootPath + "/build/res";
		fullEmulatorPath = workingDir + "/Emulators/BlastEm/blastem.exe";

		fullTemplatePath = workingDir + $"/StudioType/SGDK/Engines/{curEngineName}";
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

	private void enumsH_CodeReplacer()
	{
		GD.Print("Replacing enums.h code");
		Node singleton = (Node)GetNode("/root/singleton");

		String enumsHPath = fullEngineRootPath + "/build/inc/enums.h";
		if(!System.IO.File.Exists(enumsHPath))
		{
			String fromPath = fullTemplatePath + "/build/inc/enums.h";
			System.IO.File.Copy(fromPath, enumsHPath, false);
		}
		String enumsHCode = System.IO.File.ReadAllText(enumsHPath);
		String enumCodePaste = "";
		enumCodePaste += genTriggerEnumCode();
		enumCodePaste += genCustomScriptsEnumCode();

		enumsHCode = enumsHCode.Replace("//$Enums$", enumCodePaste);
		System.IO.File.WriteAllText(enumsHPath, enumsHCode);
		
	}

	private String genTriggerEnumCode()
	{
		String triggerTypeTemplatesPath = fullEngineRootPath + "/code_template/triggerType/";
		String enumCode = "";
		enumCode += "typedef enum {\n";
		foreach (string newPath in System.IO.Directory.GetFiles(triggerTypeTemplatesPath, "*.*", SearchOption.AllDirectories))
		{

			String fileName = System.IO.Path.GetFileNameWithoutExtension(newPath);
			enumCode += "  TRIGGER_TYPE_" + fileName + ",\n";
		}
		enumCode += "} triggetType;\n\n";
		return enumCode;
	}

	private String genCustomScriptsEnumCode()
	{
		String enumCode = "typedef enum {\n";
		string scriptsFolderPath = engineRootPath + "/code_template/customScripts";
		String[] paths = System.IO.Directory.GetFiles(scriptsFolderPath);
		Array<String> fileNames = new Array<String>();
		enumCode += "  CUSTOM_SCRIPT_updatePlayer,\n";
		enumCode += "  CUSTOM_SCRIPT_updateCamera,\n";
		//Adding scripts code
		foreach (String path in paths)
		{
			String fileNameNoExt = System.IO.Path.GetFileName(path);
			fileNameNoExt = fileNameNoExt.Substring(0, fileNameNoExt.Find("."));
			enumCode += "  CUSTOM_SCRIPT_" + fileNameNoExt + ",\n";
		}
		enumCode += "} customScriptEnum;\n\n";
		return enumCode;
	}

	private void typesH_CodeReplacer()
	{
		GD.Print("typesH_CodeReplacer running...");
		Node singleton = (Node)GetNode("/root/singleton");
		int entityLoadMode = int.Parse(singleton.Call("get_entity_load_opt_mode").ToString());

		String typesH_path = engineRootPath + "/build/inc/types.h";
		String typesCode = System.IO.File.ReadAllText(typesH_path);
		//Adding merged struct for entity
		Godot.Collections.Array mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef", "entities");
		String mergedStructCode = genMergedStructCode_entity(mergedFieldDef_arr);
		typesCode = typesCode.Replace("//$entityMergedStruct$", mergedStructCode);
		//Adding merged struct for bullets
		mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef", "bulletEntities");
		mergedStructCode = genMergedStructCode_bullet(mergedFieldDef_arr);
		typesCode = typesCode.Replace("//$entityBulletMergedStruct$", mergedStructCode);
		//Addimg merged struct for variables on level
		Godot.Collections.Array mergedVars_arr = (Godot.Collections.Array)singleton.Call("get_merged_varInst_levels");
		String variableStructCode = genMergedStructCode_variable(mergedVars_arr);
		typesCode = typesCode.Replace("//$variableMergedStruct$", variableStructCode);

	
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

		//Changing entityMerged_arr to match entity load optimization

		String toReplace;
		switch (entityLoadMode)
		{
			case 0: //No opt
				toReplace = "u16 EntityMerged_size;\nEntityMerged* EntityMerged_arr;\n";
				typesCode = typesCode.Replace("//$EntityMergedArrStruct$", toReplace);
				break;
			case 1: //Chunk opt
				toReplace = "u16 EntityMergedChunk_size;\nEntityMergedChunk* EntityMergedChunk_arr;\n";
				typesCode = typesCode.Replace("//$EntityMergedArrStruct$", toReplace);
				break;
		}

		System.IO.File.WriteAllText(typesH_path, typesCode);
		GD.Print("types.h code replaced");
	}

	private void globalH_CodeReplacer()
	{
		GD.Print("globalH_CodeReplacer running...");
		Node singleton = (Node)GetNode("/root/singleton");
		bool testFuncOn = (bool)singleton.Call("get_testFunc_on");

		String globalH_path = engineRootPath + "/build/inc/global.h";
		String globalCode = System.IO.File.ReadAllText(globalH_path);
		//Adding merged struct for entity
		Godot.Collections.Array variables_arr = (Godot.Collections.Array)singleton.Call("get_global_variables");
		String variablesCode = genGlobalVariablesCode_h(variables_arr);

		
		

		globalCode = globalCode.Replace("//$globalVariables$", variablesCode);


		System.IO.File.WriteAllText(globalH_path, globalCode);
		GD.Print("global.h code replaced");
	}

	private void globalC_CodeReplacer()
	{
		GD.Print("globalC_CodeReplacer running...");
		Node singleton = (Node)GetNode("/root/singleton");
		bool testFuncOn = (bool)singleton.Call("get_testFunc_on");

		String globalC_path = engineRootPath + "/build/src/global.c";
		String globalCode = System.IO.File.ReadAllText(globalC_path);
		//Adding merged struct for entity
		Godot.Collections.Array variables_arr = (Godot.Collections.Array)singleton.Call("get_global_variables");
		String variablesCode = genGlobalVariablesCode_c(variables_arr);

		globalCode = globalCode.Replace("//$globalVariables$", variablesCode);
		
		System.IO.File.WriteAllText(globalC_path, globalCode);
		GD.Print("global.c code replaced");
	}

	private String genGlobalVariablesCode_c(Godot.Collections.Array variables_arr)
	{
		String result = "";
		foreach (Godot.Collections.Dictionary varInst in variables_arr)
		{
			String value = varInst["value"].ToString();
			result += varInst["type"].ToString() + " " + varInst["name"].ToString();

			if (value.Length > 0)
			{
				result += " = " + value;
			}
			result += ";\n";

		}
		return result;
	}

	private String genGlobalVariablesCode_h(Godot.Collections.Array variables_arr)
	{
		String result = "";
		foreach(Godot.Collections.Dictionary varInst in variables_arr)
		{
			result += "extern " + varInst["type"].ToString() + " " + varInst["name"].ToString() + ";\n";

		}
		return result;
	}

	private String genDefaultEntityConstCode_bullet()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		Dictionary mergedFieldDefs2 = (Dictionary)singleton.Call("get_merged_fieldDefs_v2", "bulletEntities");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");
		Godot.Collections.Array entityDefs = (Godot.Collections.Array)singleton.Call("get_entity_defs", "bulletEntities");
		//Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_entity_names");
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict", "bulletEntities");
		String entityType = "EntityBulletMerged";
		
		foreach (Dictionary entityDef in entityDefs)
		{
			Godot.Collections.Array triggerAABB = (Godot.Collections.Array)entityDef["triggerAABB"];
			String entityName = (String)entityDef["identifier"];
			//Add entity default const
			result += $"const {entityType} const {entityName}_default = {{";
			//Predefined values for const
			result += $"{mergedIdsDict[entityName]}, "; //entityType
			result += "FALSE, "; //alive
			result += "FALSE, "; //preDeath
			result += "{0,0}, "; //posInt
			result += "{0,0}, "; //pos
			result += "{0,0}, "; //spd
			result += "{0,0}, "; //size
			result += "FALSE, "; //onScreen
			result += "{" + triggerAABB[0] + ", " + triggerAABB[1] + ", " + triggerAABB[2] + ", " + triggerAABB[3] + "}, "; //aabb
			result += "NULL, "; //spr
			if (showTriggerRects)
			{
				result += "NULL, "; //debugSpr1
				result += "NULL, "; //debugSpr2
			}
			foreach (Godot.Collections.Dictionary field in (Godot.Collections.Array)mergedFieldDefs2[entityName])
			{
				if (!(bool)field["hasStruct"])
				{
					continue;
				}
				String fieldVal = (String)field["defaultValue"];
				result += $"{fieldVal}, ";
			}
			result += "};\n";
			//No trigger for bullet
			/*
			//Add entity_trigger default const
			if ((bool)entityDef["addTrigger"])
			{
				String triggerType = "0";
				if (entityDef.Contains("triggerTypeName"))
				{
					if (entityDef["triggerTypeName"].ToString().Length > 0)
					{
						triggerType = "TRIGGER_TYPE_" + entityDef["triggerTypeName"].ToString();
					}
				}
				Godot.Collections.Array aabb = (Godot.Collections.Array)entityDef["triggerAABB"];
				String triggerValue = entityDef["triggerValue"].ToString();
				String triggerValue2 = entityDef["triggerValue2"].ToString();
				String triggerValue3 = entityDef["triggerValue3"].ToString();

				result += $"const Trigger const {entityName}_defaultTrigger = {{";
				result += "FALSE, "; //alive
				result += "{0, 0}, "; //pos
				result += "{" + aabb[0].ToString() + ", " + aabb[1].ToString() + ", " + aabb[2].ToString() + ", " + aabb[3].ToString() + "}, "; //rect
				result += $"{triggerType}, "; //type
				result += $"{triggerValue}, "; //val1
				result += $"{triggerValue2}, "; //val2
				result += $"{triggerValue3}, "; //val3
				result += "1, "; //triggerHp


				result += "};\n";
			}
			*/


		}
		return result;
	}

	private String genDefaultEntityConstCode_entity()
	{
		//Godot.Collections.Array mergedFieldDef_arr
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		int slaveAmount = int.Parse(singleton.Call("get_slave_amount").ToString());
		Dictionary mergedFieldDefs2 = (Dictionary)singleton.Call("get_merged_fieldDefs_v2", "entities");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");
		Godot.Collections.Array entityDefs = (Godot.Collections.Array)singleton.Call("get_entity_defs", "entities");
		//Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_entity_names");
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict", "entities");
		String entityType = "EntityMerged";

		foreach (Dictionary entityDef in entityDefs)
		{
			String entityName = entityDef["identifier"].ToString();
			//Add entity default const
			result += $"const {entityType} const {entityName}_default = {{";
			//Predefined values for const
			result += $"{mergedIdsDict[entityName]}, "; //entityType
			result += $"0, ";//instId
			result += "FALSE, "; //alive
			result += "{0,0}, "; //posInt
			result += "{0,0}, "; //pos
			result += "{0,0}, "; //spd
			result += "{" + entityDef["width"].ToString() + ", " + entityDef["height"].ToString() + "}, "; //size
			result += "FALSE, "; //onScreen
			result += "NULL, "; //trigger
			result += "0, "; //triggerInd
			result += "NULL, "; //spr
			result += "FALSE, "; //activated

			//slaves_arr
			result += "{";
			for (int i=0; i< slaveAmount; i++)
			{
				result += "NULL, ";
			}
			result += "}, ";

			result += $"{slaveAmount.ToString()}, "; //slave_amount
			if (showTriggerRects)
			{
				result += "NULL, "; //debugSpr1
				result += "NULL, "; //debugSpr2
			}
			foreach (Godot.Collections.Dictionary field in (Godot.Collections.Array)mergedFieldDefs2[entityName])
			{
				if (!(bool)field["hasStruct"])
				{
					continue;
				}
				String fieldVal = field["defaultValue"].ToString();
				result += $"{fieldVal}, ";
			}
			result += "};\n";
			//Add entity_trigger default const
			if((bool)entityDef["addTrigger"])
			{
				String triggerType = "0";
				if (entityDef.Contains("triggerTypeName"))
				{
					if(entityDef["triggerTypeName"].ToString().Length > 0)
					{
						triggerType = "TRIGGER_TYPE_" + entityDef["triggerTypeName"].ToString();
					}
				}
				Godot.Collections.Array aabb = (Godot.Collections.Array)entityDef["triggerAABB"];
				String triggerValue = entityDef["triggerValue"].ToString();
				String triggerValue2 = entityDef["triggerValue2"].ToString();
				String triggerValue3 = entityDef["triggerValue3"].ToString();

				result += $"const Trigger const {entityName}_defaultTrigger = {{";
				result += "FALSE, "; //alive
				result += "{0, 0}, "; //pos
				result += "{" + aabb[0].ToString() + ", " + aabb[1].ToString() + ", " + aabb[2].ToString() + ", " + aabb[3].ToString() + "}, "; //rect
				result += $"{triggerType}, "; //type
				result += $"{triggerValue}, "; //val1
				result += $"{triggerValue2}, "; //val2
				result += $"{triggerValue3}, "; //val3
				result += "1, "; //triggerHp
				result += "FALSE, "; //activated
				result += "FALSE, "; //prevActivated
				result += "};\n";
			}


		}
		return result;
	}

	private String genMergedStructCode_variable(Godot.Collections.Array mergedArr)
	{
		Node singleton = (Node)GetNode("/root/singleton");

		String result = "typedef struct {\n";

		foreach (Godot.Collections.Dictionary field in mergedArr)
		{
			String fieldName = (String)field["name"];
			String fieldType = (String)field["type"];
			result += $"  {fieldType} {fieldName};\n";

		}


		result += "} LocalVariableMerged;\n";

		return result;
	}

	private String genMergedStructCode_bullet(Godot.Collections.Array mergedFieldDef_arr)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");


		String result = "typedef struct {\n";
		//Predefined strct vars

		result += "  u16 entityType;\n";
		result += "  bool alive;\n";
		result += "  bool preDeath;\n";
		result += "  Vect2D_s16 posInt;\n";
		result += "  Vect2D_f32 pos;\n";
		result += "  Vect2D_f16 spd;\n";
		result += "  Vect2D_s16 size;\n";
		result += "  bool onScreen;\n";
		result += "  AABB aabb;\n";
		result += "  Sprite* spr;\n";

		if (showTriggerRects)
		{
			result += "  Sprite* debugSpr1;\n";
			result += "  Sprite* debugSpr2;\n";
		}

		foreach (Godot.Collections.Dictionary field in mergedFieldDef_arr)
		{
			if (!(bool)field["hasStruct"])
			{
				continue;
			}
			String fieldName = (String)field["identifier"];
			String fieldType = (String)field["inCodeType"];
			result += $"  {fieldType} {fieldName};\n";

		}


		result += "} EntityBulletMerged;\n";

		return result;
	}

	private String genMergedStructCode_entity(Godot.Collections.Array mergedFieldDef_arr)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");

		int slaveAmount = int.Parse(singleton.Call("get_slave_amount").ToString());


		String result = "typedef struct {\n";
		//Predefined strct vars

		result += "  u16 entityType;\n";
		result += "  u16 instId;\n";
		result += "  bool alive;\n";
		result += "  Vect2D_s16 posInt;\n";
		result += "  Vect2D_f32 pos;\n";
		result += "  Vect2D_f16 spd;\n";
		result += "  Vect2D_s16 size;\n";
		result += "  bool onScreen;\n";
		result += "  Trigger* trigger;\n";
		result += "  u16 triggerInd;\n";
		result += "  Sprite* spr;\n";
		result += "  bool activated;\n";
		result += $"  struct EntityMerged *slaves_arr[{slaveAmount.ToString()}];\n";
		result += "  u16 slave_amount;\n";

		if (showTriggerRects)
		{
			result += "  Sprite* debugSpr1;\n";
			result += "  Sprite* debugSpr2;\n";
		}

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
		String fromFolder = fullTemplatePath + "/build";
		
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

			//No need for that, because i changed bgRelPath logic. Now it's no need to copy files, just use existent. Idk, why i didn't done that way in the first place. 
			//copyRes();
			
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
		fileReplceMod_opt();
	}

	private void fileReplceMod_opt()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		int entityLoadMode = int.Parse(singleton.Call("get_entity_load_opt_mode").ToString());
		String fromPath;
		String toPath;
		switch (entityLoadMode)
		{
			case 1:
				fromPath = engineRootPath + "/code_template/chunkOpt1/src/";
				toPath = engineRootPath + "/build/src/";
				CopyFilesRecursively(fromPath, toPath);
				fromPath = engineRootPath + "/code_template/chunkOpt1/inc/";
				toPath = engineRootPath + "/build/inc/";
				CopyFilesRecursively(fromPath, toPath);
				loadChunkRuntimeC_CodeReplacer();
				loadChunkFullC_CodeReplacer();
				break;
		}
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

		allocLevelC_CodeReplacer();

		updatePlayerC_CodeReplacer();
		playerInitC_CodeReplacer();
		loadLevelC_CodeReplacer();

		levelsC_CodeReplacer();
		typesH_CodeReplacer();

		globalH_CodeReplacer();
		globalC_CodeReplacer();

		enumsH_CodeReplacer();

		entityHandlerC_CodeReplacer();

		customScriptsC_CodeReplacer();
		customScriptsH_CodeReplacer();

		mainC_CodeReplacer();
	}

	private void mainC_CodeReplacer()
	{
		int curLevel;
		Node singleton = (Node)GetNode("/root/singleton");
		int entityLoadMode = int.Parse(singleton.Call("get_entity_load_opt_mode").ToString());
		curLevel = (int)singleton.Call("get_run_level_ind");
		String mainC_path = engineRootPath + "/build/src/main.c";
		String mainCode = System.IO.File.ReadAllText(mainC_path);
		String finalCode = mainCode.Replace("$curLevel$", curLevel.ToString());
		if(entityLoadMode != 0)
		{
			finalCode = finalCode.Replace("//$chunkLoadFunc$", "loadChunkRuntime();");
		}
		System.IO.File.WriteAllText(mainC_path, finalCode);
		GD.Print("main.c code replaced");
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
		result += "updatePlayer, ";
		result += "updateCamera, ";
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
		entityHandlerCode = entityHandlerCode.Replace("//$entityDefaultConsts$", genDefaultEntityConstCode_entity());
		entityHandlerCode = entityHandlerCode.Replace("//$entityBulletDefaultConsts$", genDefaultEntityConstCode_bullet()); ;
		entityHandlerCode = entityHandlerCode.Replace("//$showEntityFuncs$", genShowEntityCodeAll_entity());
		entityHandlerCode = entityHandlerCode.Replace("//$showEntityBulletFuncs$", genShowEntityCodeAll_bullet());
		entityHandlerCode = entityHandlerCode.Replace("//$addNewEntityFuncs$", genAddNewEntityCodeAll_entity());
		entityHandlerCode = entityHandlerCode.Replace("//$addNewEntityBulletFuncs$", genAddNewEntityCodeAll_bullet());
		entityHandlerCode = entityHandlerCode.Replace("//$triggerTypeFuncs$", genTriggerTypeCodeAll());
		
		System.IO.File.WriteAllText(entityHandlerC_path, entityHandlerCode);
		GD.Print("entityHandler.c code replaced");
	}

	private String genTriggerTypeCodeAll()
	{
		GD.Print("genTriggerTypeCodeAll");
		String result = "";
		String funcCode = "";
		String arrCode = "";
		Node singleton = (Node)GetNode("/root/singleton");

		String sourcePath = fullEngineRootPath + "/code_template/triggerType/";
		//Putting this funcs in arr
		arrCode += "void(* triggerTypeFuncArr[])(Trigger*, AABB*) = {";
		foreach (string newPath in System.IO.Directory.GetFiles(sourcePath, "*.*", SearchOption.AllDirectories))
		{
			
			String fileName = System.IO.Path.GetFileNameWithoutExtension(newPath);
			arrCode += "triggerTypeFunc_" + fileName + ", ";

			funcCode += "void triggerTypeFunc_" + fileName + "(Trigger* trigger, AABB* triggerBounds) {\n";
			funcCode += System.IO.File.ReadAllText(newPath) + "\n";
			funcCode += "}\n";
		}
		arrCode += "};\n";
		result = funcCode + arrCode;
		return result;
	}

	private String genAddNewEntityCodeAll_bullet()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array entityDefs = (Godot.Collections.Array)singleton.Call("get_entity_defs", "bulletEntities");
		//Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_entity_names");
		//Generating funcs
		foreach (Dictionary entityDef in entityDefs)
		{
			String entityName = (String)entityDef["identifier"];
			bool entityAddTrigger = (bool)entityDef["addTrigger"];
			//GD.Print("HI ", entityName, " THERE ", entityAddTrigger);
			String defaultEntityCodePath = engineRootPath + "/code_template/addNewEntityBullet/addNewDefault.c";
			String defaultAddEntityTriggerPath = engineRootPath + "/code_template/addNewEntityBullet/addNewDefaultTrigger.c";
			String checkPath = engineRootPath + "/code_template/addNewEntityBullet/addNew" + entityName + ".c";
			if (System.IO.File.Exists(checkPath))
			{
				String insertData = System.IO.File.ReadAllText(checkPath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityBulletMerged");

				result += insertData + "\n";
			}
			else
			{
				String insertData;

				if (entityAddTrigger)
				{
					insertData = System.IO.File.ReadAllText(defaultAddEntityTriggerPath);
				} else
				{
					insertData = System.IO.File.ReadAllText(defaultEntityCodePath);
				}
		
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityBulletMerged");

				System.IO.File.WriteAllText(checkPath, insertData);
				result += insertData + "\n";
			}
		}

		return result;
	}



	private String genAddNewEntityCodeAll_entity()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Array entityDefs = (Godot.Collections.Array)singleton.Call("get_entity_defs", "entities");
		//Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_entity_names");
		//Generating funcs
		foreach (Dictionary entityDef in entityDefs)
		{
			String entityName = (String)entityDef["identifier"];
			bool entityAddTrigger = (bool)entityDef["addTrigger"];
			//GD.Print("HI ", entityName, " THERE ", entityAddTrigger);
			String defaultEntityCodePath = engineRootPath + "/code_template/addNewEntity/addNewDefault.c";
			String defaultAddEntityTriggerPath = engineRootPath + "/code_template/addNewEntity/addNewDefaultTrigger.c";
			String checkPath = engineRootPath + "/code_template/addNewEntity/addNew" + entityName + ".c";
			if (System.IO.File.Exists(checkPath))
			{
				String insertData = System.IO.File.ReadAllText(checkPath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				
				result += insertData + "\n";
			}
			else
			{
				String insertData;

				if (entityAddTrigger)
				{
					insertData = System.IO.File.ReadAllText(defaultAddEntityTriggerPath);
				}
				else
				{
					insertData = System.IO.File.ReadAllText(defaultEntityCodePath);
				}

				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				
				System.IO.File.WriteAllText(checkPath, insertData);
				result += insertData + "\n";
			}
		}

		return result;
	}

	private String convertSpritePathToSpriteName(String spritePath)
	{
		String result = "";
		String rootFolderFilter = "build/res/sprites/";
		result = spritePath.Substring(spritePath.FindLast(rootFolderFilter) + rootFolderFilter.Length);
		result = result.Split(".")[0];
		result = result.Split("-")[0];
		result = result.Replace("/", "_");

		return result;
	}


	private String genShowEntityCodeAll_entity()
	{
		GD.Print("In genShowEntityCodeAll_entity...");
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");
		Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_def_entityCollection_names", "entities");
		//Generating funcs
		foreach(Godot.Collections.Dictionary entityDict in entityNames)
		{
			String entityName = entityDict["name"].ToString();
			int defId = int.Parse(entityDict["defId"].ToString());
			Godot.Collections.Dictionary entityDef = (Godot.Collections.Dictionary)singleton.Call("get_entityDef_by_defId", defId);
			String defaultEntityCodePath = engineRootPath + "/code_template/showEntity/showDefault.c";
			String checkPath = engineRootPath + "/code_template/showEntity/show" + entityName + ".c";
			if (System.IO.File.Exists(checkPath))
			{
				String insertData = System.IO.File.ReadAllText(checkPath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				String addSpriteCode = "";
				String releaseSpriteCode = "";
				//Add slaves code
				Godot.Collections.Array slaves = (Godot.Collections.Array)(Godot.Collections.Array)entityDef["subordinates"];
				if(slaves.Count > 0)
				{
					releaseSpriteCode += "for(u16 curSlaveInd=0; curSlaveInd<entity->slave_amount; curSlaveInd++){\n" +
							"curSlave = entity->slaves_arr[curSlaveInd];\n" +
							"curSlave->trigger->alive = FALSE;\n" +
							"}\n";

					int curSlaveInd = 0;
					addSpriteCode += "if(!entity->activated) {\n";
					//addSpriteCode += "EntityMerged* curSlave;\n";
					foreach (Godot.Collections.Dictionary slaveDict in slaves)
					{
						//Moving slave, err, subordinate, to his position
						Godot.Collections.Array slavePos = (Godot.Collections.Array)slaveDict["px"];
						String x_pos = slavePos[0].ToString();
						String y_pos = slavePos[1].ToString();

						int slaveDefId = int.Parse(slaveDict["defId"].ToString());

						Godot.Collections.Dictionary slaveEntityDef = (Godot.Collections.Dictionary)singleton.Call("get_entityDef_by_defId", slaveDefId);
						String slaveEntityName = slaveEntityDef["identifier"].ToString();
						//addSpriteCode += $"curSlave = entity->slaves_arr[{curSlaveInd.ToString()}];\n";
						addSpriteCode += $"entity->slaves_arr[{curSlaveInd.ToString()}] = addNew_{slaveEntityName}((Vect2D_s16){{{x_pos}+entity->posInt.x, {y_pos}+entity->posInt.y}}, (Vect2D_f16){{0, 0}});\n";

						

						//addSpriteCode += $"curSlave->posInt = (Vect2D_s16){{{x_pos}+entity->posInt.x, {y_pos}+entity->posInt.y}};\n";
						//addSpriteCode += $"curSlave->pos = (Vect2D_f32){{FIX32({x_pos}+entity->posInt.x), FIX32({y_pos}+entity->posInt.y)}};\n";

						/*
						String spritePath = slaveDict["__spritePath"].ToString();
						if (spritePath.Length > 0)
						{
							String spriteName = "&spr_" + convertSpritePathToSpriteName(spritePath);
							//Adding sprite
							addSpriteCode += $"curSlave->spr = SPR_addSprite({spriteName}, {x_pos}+posX_OnCam, {y_pos}+posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 11, FALSE, FALSE));\n";
						}
						*/




						curSlaveInd++;
					}
					addSpriteCode += "entity->activated = TRUE;\n";
					addSpriteCode += "}\n";
				}
					
				//addSpriteCode += 
				if (showTriggerRects)
				{
					releaseSpriteCode += "if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);\n" +
						"if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);\n";
					
					addSpriteCode += "entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));\n" +
						"entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));\n";
					
					String moveSpriteCode = "if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);\n" +
						"if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);\n";
					insertData = insertData.Replace("//$showTriggerRects_moveSprite$", moveSpriteCode);
				}
				insertData = insertData.Replace("//$showTriggerRects_releaseSprite$", releaseSpriteCode);
				insertData = insertData.Replace("//$showTriggerRects_addSprite$", addSpriteCode);
				result += insertData + "\n";
			} else
			{
				if (!System.IO.File.Exists(defaultEntityCodePath))
				{
					String fromPath = fullTemplatePath + "/code_template/showEntity/showDefault.c";
					GD.Print(fromPath);
					GD.Print("test122222");
					System.IO.File.Copy(fromPath, defaultEntityCodePath, false);
				}
				String insertData = System.IO.File.ReadAllText(defaultEntityCodePath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityMerged");
				System.IO.File.WriteAllText(checkPath, insertData);
				result += insertData + "\n";
			}
		}

		//Putting this funcs in arr
		result += "void(* showEntityFuncArr[])(EntityMerged*) = {";
		foreach (Godot.Collections.Dictionary entityDict in entityNames)
		{
			String entityName = entityDict["name"].ToString();
			result += "show" + entityName + ", ";
		}
		result += "};\n";
		return result;
	}

	private String genShowEntityCodeAll_bullet()
	{
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");
		Godot.Collections.Array entityNames = (Godot.Collections.Array)singleton.Call("get_entity_names", "bulletEntities");
		//Generating funcs
		foreach (String entityName in entityNames)
		{
			String defaultEntityCodePath = engineRootPath + "/code_template/showEntityBullet/showDefault.c";
			String checkPath = engineRootPath + "/code_template/showEntityBullet/show" + entityName + ".c";
			if (System.IO.File.Exists(checkPath))
			{
				String insertData = System.IO.File.ReadAllText(checkPath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityBulletMerged");
				if (showTriggerRects)
				{
					String releaseSpriteCode = "if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);\n" +
						"if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);\n";
					insertData = insertData.Replace("//$showTriggerRects_releaseSprite$", releaseSpriteCode);
					String addSpriteCode = "entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));\n" +
						"entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));\n";
					insertData = insertData.Replace("//$showTriggerRects_addSprite$", addSpriteCode);
					String moveSpriteCode = "if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->posInt.x-cameraPosition.x)+entity->aabb.min.x, (entity->posInt.y-cameraPosition.y)+entity->aabb.min.y);\n" +
						"if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->posInt.x-cameraPosition.x)+entity->aabb.max.x-8, (entity->posInt.y-cameraPosition.y)+entity->aabb.max.y-8);\n";
					insertData = insertData.Replace("//$showTriggerRects_moveSprite$", moveSpriteCode);

				}
				result += insertData + "\n";
			}
			else
			{
				if (!System.IO.File.Exists(defaultEntityCodePath))
				{
					String fromPath = fullTemplatePath + "/code_template/showEntityBullet/showDefault.c";
					System.IO.File.Copy(fromPath, defaultEntityCodePath, false);
				}
				String insertData = System.IO.File.ReadAllText(defaultEntityCodePath);
				insertData = insertData.Replace("$entityName$", entityName);
				insertData = insertData.Replace("$entityType$", "EntityBulletMerged");
				System.IO.File.WriteAllText(checkPath, insertData);
				result += insertData + "\n";
			}
		}

		//Putting this funcs in arr
		result += "void(* showEntityBulletFuncArr[])(EntityBulletMerged*) = {";
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

		float chunkSizeX = float.Parse(singleton.Call("get_chunkSizeX").ToString());
		float chunkSizeY = float.Parse(singleton.Call("get_chunkSizeY").ToString());
		double levelSizeChunksX = Math.Ceiling(levelSizePx.x / chunkSizeX);
		double levelSizeChunksY = Math.Ceiling(levelSizePx.y / chunkSizeY);
		String levelSizeChunksText = "{" + levelSizeChunksX.ToString() + ", " + levelSizeChunksY.ToString() + "}";

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
		String musicSizeof = "0";
		String beforeLevelScriptName = "NULL";
		String everyFrameScriptName = "emptyScript";
		String afterLevelScriptName = "NULL";
		String pal0Name = "NULL";
		String pal1Name = "NULL";
		String pal2Name = "NULL";
		String pal3Name = "NULL";

		String levelMode = "0"; //0 = level; 1 = scene
		String controlScript = "CUSTOM_SCRIPT_updatePlayer";
		String updateCameraScript = "CUSTOM_SCRIPT_updateCamera";

		String forePalIndex = "0";
		String backPalIndex = "0";

		String musicMode = "0";
		String pcmChannel = "0";
		String musicLoop = "0";
		String freshMusicStart = "1";


		if (levelDict.Contains("freshMusicStart"))
		{
			String temp = levelDict["freshMusicStart"].ToString();
			if (temp.Length > 0)
			{
				freshMusicStart = temp;
			}
		}

		if (levelDict.Contains("musicLoop"))
		{
			String temp = levelDict["musicLoop"].ToString();
			if (temp.Length > 0)
			{
				musicLoop = temp;
			}
		}

		if (levelDict.Contains("pcmChannel"))
		{
			String temp = levelDict["pcmChannel"].ToString();
			if (temp.Length > 0)
			{
				pcmChannel = temp;
			}
		}

		if (levelDict.Contains("musicMode"))
		{
			String temp = levelDict["musicMode"].ToString();
			if (temp.Length > 0)
			{
				musicMode = temp;
			}
		}

		if (levelDict.Contains("forePalIndex"))
		{
			String temp = levelDict["forePalIndex"].ToString();
			if (temp.Length > 0)
			{
				forePalIndex = temp;
			}
		}

		if (levelDict.Contains("backPalIndex"))
		{
			String temp = levelDict["backPalIndex"].ToString();
			if (temp.Length > 0)
			{
				backPalIndex = temp;
			}
		}

		if (levelDict.Contains("levelMode"))
		{
			String temp = levelDict["levelMode"].ToString();
			if (temp.Length > 0)
			{
				levelMode = temp;
			}
		}

		if(int.Parse(levelMode) > 0)
		{
			if (levelDict.Contains("controlScript"))
			{
				String temp = levelDict["controlScript"].ToString();
				if (temp.Length > 0)
				{
					controlScript = "CUSTOM_SCRIPT_" + temp;
				}
			}
			if (levelDict.Contains("updateCameraScript"))
			{
				String temp = levelDict["updateCameraScript"].ToString();
				if (temp.Length > 0)
				{
					updateCameraScript = "CUSTOM_SCRIPT_" + temp;
				}
			}
		}
		

		if (levelDict.Contains("pal0SpriteName"))
		{
			String temp = (String)levelDict["pal0SpriteName"];
			if (temp.Length > 0)
			{
				if(temp != "NULL")
				{
					pal0Name = $"&pal_{temp}";
				}
				
			}
		}

		if (levelDict.Contains("pal1SpriteName"))
		{
			String temp = (String)levelDict["pal1SpriteName"];
			if (temp.Length > 0)
			{
				if (temp != "NULL")
				{
					pal1Name = $"&pal_{temp}";
				}
			}
		}

		if (levelDict.Contains("pal2SpriteName"))
		{
			String temp = (String)levelDict["pal2SpriteName"];
			if (temp.Length > 0)
			{
				if (temp != "NULL")
				{
					pal2Name = $"&pal_{temp}";
				}
			}
		}

		if (levelDict.Contains("pal3SpriteName"))
		{
			String temp = (String)levelDict["pal3SpriteName"];
			if (temp.Length > 0)
			{
				if (temp != "NULL")
				{
					pal3Name = $"&pal_{temp}";
				}
			}
		}

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
			if (!(temp == "NULL" || temp.Length == 0))
			{
				if (temp.Find("-") != -1)
				{
					temp = temp.Substring(0, temp.Find("-"));
				}
				musicName = "&mus_" + temp;
				musicSizeof = "sizeof(" + "mus_" + temp + ")";
			}
		}

		if (bgaPath.Length > 0)
		{
			String fileName = bgaPath.Substring(bgaPath.FindLast("/")+1);
			fileName = fileName.Substring(0, fileName.LastIndexOf("."));
			switch (bgaMode)
			{
				case 0: //map mode
					bgaMapResName = "&map_" + fileName;
					bgaTilesetResName = "&tileset_" + fileName;
					bgaPalResName = "&pal_" + fileName;
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
					bgbMapResName = "&map_" + fileName;
					bgbTilesetResName = "&tileset_" + fileName;
					bgbPalResName = "&pal_" + fileName;
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
			$" {bgaImageName}, {bgbImageName}, {posArrText}, {posAmountText}, {collMapName}, {levelSizePxText}, {levelSizeTilesText}, {levelSizeChunksText}, {musicName}, {musicSizeof}, {beforeLevelScriptName}, {everyFrameScriptName}," +
			$" {afterLevelScriptName}, {pal0Name}, {pal1Name}, {pal2Name}, {pal3Name}, {levelMode}, {controlScript}, {updateCameraScript}, {forePalIndex}, {backPalIndex}, {musicMode}, {pcmChannel}, {musicLoop}," +
			$" {freshMusicStart}}};\n";
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

	private Godot.Collections.Array mergeGodotArrays(Godot.Collections.Array arr1, Godot.Collections.Array arr2)
	{
		Godot.Collections.Array resultArray = new Godot.Collections.Array();
		foreach (var val in arr1)
		{
			resultArray.Add(val);

		}
		foreach (var val in arr2)
		{
			resultArray.Add(val);

		}
		return resultArray;
	}

	private String genLocalVariableMergedCode(int curLevel)
	{
		GD.Print("Gen LocalVariableMerged Code...");
		String result = "";
		result += $"const LocalVariableMerged const LocalVariable_arr_Level_{curLevel.ToString()}[] = ";
		Node singleton = (Node)GetNode("/root/singleton");
		Godot.Collections.Dictionary level = (Godot.Collections.Dictionary)singleton.Call("get_level", curLevel);
		Godot.Collections.Array mergedTemplate = (Godot.Collections.Array)singleton.Call("get_merged_varInst_levels");
		if(mergedTemplate == null)
		{
			mergedTemplate = new Godot.Collections.Array();

		}
		GD.Print(mergedTemplate);
		
		result += "{";
		foreach (Godot.Collections.Dictionary varInstTemplate in mergedTemplate)
		{
			String value = "0";
			//finding value of this fieldName, why? Because in struct order is nessesary
			GD.Print(321);
			foreach (Godot.Collections.Dictionary varInst in (Godot.Collections.Array)level["localVars"])
			{
				GD.Print(123);
				//if names of varInst and varInstTemplate are equal
				if ((String)varInst["name"] == (String)varInstTemplate["name"])
				{
					//Then value is found
					String tempValue = varInst["value"].ToString();
					if (tempValue.Length > 0)
					{
						value = tempValue.ToString();
					}
					
					break;
				}

			}
			//In most caces entity doesn't have every possible field, so, we are using default value
			result += value + ", ";
		}
		result += "};\n";


		return result;
	}
	private String genTriggerCode_chunkOpt1(int curLevel, ref System.Collections.Generic.Dictionary<int, int> entityIdEqualsTriggerInd_dict)
	{
		GD.Print("Gen TriggerCode_chunkOpt1 for level ", curLevel);

		Node singleton = (Node)GetNode("/root/singleton");
		//GD.Print(0);
		String result = ""; ;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");

		//GD.Print(1);

		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		//Adding gates
		Godot.Collections.Array gateInstances = (Godot.Collections.Array)singleton.Call("get_gateInstances_by_levelNum", curLevel);
		//Merging them
		entityInstances = mergeGodotArrays(entityInstances, gateInstances);
		

		result += $"const Trigger const Trigger_arr_Level_{curLevel.ToString()}[] = ";

		//Opening Trigger_arr block
		result += "{";


		System.Collections.Generic.Dictionary<int, String> triggerLinesDict = new System.Collections.Generic.Dictionary<int, string>();

		int triggerAmount = 0;
		foreach (Godot.Collections.Dictionary entityInst in entityInstances)
		{
			String oneTriggerLine = "";
			//GD.Print(2);
			//Getting useful data about entity
			if (!entityInst.Contains("__identifier"))
			{
				continue;
			}
			int instId = 0;
			instId = (int)int.Parse(entityInst["instId"].ToString());
			
			if (!entityInst.Contains("addTrigger") || (bool)entityInst["addTrigger"] == false)
			{
				continue;
			}
			triggerAmount++;
			String entityName = (String)entityInst["__identifier"];

			//GD.Print(entityName);
			//GD.Print(3);

			Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];

			Godot.Collections.Array triggerAABB = new Godot.Collections.Array();

			if (entityInst.Contains("triggerAABB"))
			{
				triggerAABB = (Godot.Collections.Array)entityInst["triggerAABB"];
			}
			else
			{
				triggerAABB.Add(0);
				triggerAABB.Add(0);
				triggerAABB.Add(0);
				triggerAABB.Add(0);

			}
			//GD.Print(4);
			String triggerType = "0";
			if (entityInst.Contains("triggerTypeName"))
			{
				if (entityInst["triggerTypeName"].ToString().Length > 0)
				{
					triggerType = "TRIGGER_TYPE_" + entityInst["triggerTypeName"].ToString();
				}
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
			oneTriggerLine += "{";

			//Applying to generated code

			/*****---> Struct reminder <---*****
			 
				typedef struct {
					bool alive;
					Vect2D_s16 firstPos;
					Vect2D_s16 lastPos;
					s8 trigger_type;
					s8 trigger_value;
					s16 triggerHp;
					bool activated;
					bool prevActivated;
				} Trigger;

			**********************************/

			oneTriggerLine += "TRUE, "; //alive
			oneTriggerLine += "{" + $"{pos[0]}, {pos[1]}" + "}, "; //pos
			oneTriggerLine += "{" + $"{triggerAABB[0]}, {triggerAABB[1]}, {triggerAABB[2]}, {triggerAABB[3]}" + "}, "; //triggerRect
			oneTriggerLine += $"{triggerType}, "; //triggerType
			oneTriggerLine += $"{triggerValue}, "; //triggerValue
			oneTriggerLine += $"{triggerValue2}, "; //triggerValue2
			oneTriggerLine += $"{triggerValue3}, "; //triggerValue3
			oneTriggerLine += "1, "; //triggerHp
			oneTriggerLine += "FALSE, "; //activated
			oneTriggerLine += "FALSE, "; //prevActivated

			//Closing Trigger block
			oneTriggerLine += "}, ";

			if (entityIdEqualsTriggerInd_dict.ContainsKey(instId))
			{
				triggerLinesDict[entityIdEqualsTriggerInd_dict[instId]] = oneTriggerLine;
			}
		}

		for(int i=0; i< triggerAmount; i++)
		{
			result += triggerLinesDict[i];
		}

		//Closing Trigger_arr block
		result += "};\n";
		return result;
	}

	private String genEntityMergedCode_chunkOpt1(int curLevel, ref System.Collections.Generic.Dictionary<int, int> entityIdEqualsTriggerInd_dict)
	{
		GD.Print("Gen EntityMerged Code: chunkOpt1...");
		String result = "";
		Node singleton = (Node)GetNode("/root/singleton");
		int slaveAmount = int.Parse(singleton.Call("get_slave_amount").ToString());
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");
		Godot.Collections.Array mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef", "entities");
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict", "entities");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		//Adding gates
		Godot.Collections.Array gateInstances = (Godot.Collections.Array)singleton.Call("get_gateInstances_by_levelNum", curLevel);
		//Merging them
		entityInstances = mergeGodotArrays(entityInstances, gateInstances);
		//Get level size
		Vector2 levelSize = (Vector2)singleton.Call("get_level_size", curLevel);
		String chunksCode = "";
		String chunksCodeAll = $"const EntityMergedChunk const EntityMergedChunk_arr_Level_{curLevel.ToString()}[] = {{";

		int chunkSizeX = int.Parse(singleton.Call("get_chunkSizeX").ToString());
		int chunkSizeY = int.Parse(singleton.Call("get_chunkSizeX").ToString());

		int firstPosX = 0;
		int firstPosY = 0;

		

		int lastTriggerInd = 0;
		int curTriggerInd = 0;
		int lastPosX = chunkSizeX;
		int lastPosY = chunkSizeY;

		GD.Print("levelSize");

		GD.Print(levelSize.x);
		GD.Print(levelSize.y);

		while (firstPosY <= levelSize.y+1)
		{
			while (firstPosX <= levelSize.x+1)
			{
				
				String arrName = $"EntityMerged_arr_ChunkOpt1_Level_{curLevel.ToString()}_chunk_{firstPosX.ToString()}_{firstPosY.ToString()}";
				//EntityMerged_arr open
				String tempChunkCode = $"const EntityMerged const {arrName}[] = {{";
				//chunksCode += $"const EntityMerged const {arrName}[] = {{";

				int entityAmountInChunk = 0;
				foreach (Godot.Collections.Dictionary entityInst in entityInstances)
				{
					//Getting useful data about entity
					if (!entityInst.Contains("__identifier"))
					{
						continue;
					}
					Godot.Collections.Array pos = (Godot.Collections.Array)entityInst["px"];
					int width = 0;
					int height = 0;
					width = (int)int.Parse(entityInst["width"].ToString());
					//Studio crashes if you don't do proper conversion.
					height = (int)int.Parse(entityInst["height"].ToString());

					int entityFirstPosX = int.Parse(pos[0].ToString());
					int entityFirstPosY = int.Parse(pos[1].ToString());

					int entityLastPosX = entityFirstPosX + width;
					int entityLastPosY = entityFirstPosY + height;

					//Checking all 4 corners of entity, to make sure, that entity copied in all intercepted chunks.
					bool leftUpPointCheck = !((entityFirstPosX >= firstPosX) && (entityFirstPosX <= lastPosX)) || !((entityFirstPosY >= firstPosY) && (entityFirstPosY <= lastPosY));
					bool rightUpPointCheck = !((entityLastPosX >= firstPosX) && (entityLastPosX <= lastPosX)) || !((entityFirstPosY >= firstPosY) && (entityFirstPosY <= lastPosY));
					bool leftDownPointCheck = !((entityFirstPosX >= firstPosX) && (entityFirstPosX <= lastPosX)) || !((entityLastPosY >= firstPosY) && (entityLastPosY <= lastPosY));
					bool rightDownPointCheck = !((entityLastPosX >= firstPosX) && (entityLastPosX <= lastPosX)) || !((entityLastPosY >= firstPosY) && (entityLastPosY <= lastPosY));

					//If entity outside chunk bounds
					if (leftUpPointCheck && rightUpPointCheck && leftDownPointCheck && rightDownPointCheck)
					{
						//Then, skipping this entity
						continue;
					}
					entityAmountInChunk++;
					bool addTrigger = false;
					String entityName = (String)entityInst["__identifier"];
					int mergedId = (int)mergedIdsDict[entityName];

					int instId = 0;
					instId = (int)int.Parse(entityInst["instId"].ToString());
					

					int[] spd = { 0, 0 };

					//Opening entityMerged block
					tempChunkCode += "{";

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
						  bool activated;
						  struct EntityMerged *slaves_arr[10];
						  u16 slave_amount;
						} EntityMerged;

					**********************************/

					tempChunkCode += $"{mergedId.ToString()},"; //mergedId
					tempChunkCode += $"{instId.ToString()},"; //instId
					tempChunkCode += $" TRUE, "; //alive 
					tempChunkCode += "{" + pos[0].ToString() + ", " + pos[1].ToString() + "}, "; //posInt
					tempChunkCode += "{FIX32(" + pos[0].ToString() + "), FIX32(" + pos[1].ToString() + ")}, "; //pos
					tempChunkCode += "{" + spd[0].ToString() + ", " + spd[1].ToString() + "}, "; //spd
					tempChunkCode += "{" + width.ToString() + ", " + height.ToString() + "}, "; //size
					tempChunkCode += "FALSE, "; //onScreen

					if (!entityInst.Contains("addTrigger") || (bool)entityInst["addTrigger"] == false)
					{
						tempChunkCode += $"NULL,"; //trigger = NULL
					}
					else
					{
						if (entityIdEqualsTriggerInd_dict.ContainsKey(instId))
						{
							curTriggerInd = entityIdEqualsTriggerInd_dict[instId];
						} else
						{
							curTriggerInd = lastTriggerInd;
							entityIdEqualsTriggerInd_dict[instId] = curTriggerInd;
							lastTriggerInd++;
						}
						//addTrigger = true;
						tempChunkCode += $"&Trigger_arr_Level_{curLevel}[{curTriggerInd}],"; //trigger

					}
					tempChunkCode += $"{curTriggerInd},"; //triggerInd small this is for unoptimized entity load algorithm
					tempChunkCode += "NULL,"; //spr
					tempChunkCode += "FALSE, "; // activated
					//slaves_arr
					tempChunkCode += "{";
					for (int i = 0; i < slaveAmount; i++)
					{
						tempChunkCode += "NULL, ";
					}
					tempChunkCode += "}, ";

					int defId = int.Parse(entityInst["defId"].ToString());
					Godot.Collections.Dictionary entityDef = (Godot.Collections.Dictionary)singleton.Call("get_entityDef_by_defId", defId);
					Godot.Collections.Array slaves = (Godot.Collections.Array)entityDef["subordinates"];
					tempChunkCode += $"{slaves.Count.ToString()}, "; //slave_amount

					if (showTriggerRects)
					{
						tempChunkCode += "NULL, "; //debugSpr1
						tempChunkCode += "NULL, "; //debugSpr2
					}

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
							if ((String)fieldInst["__identifier"] == (String)fieldDef["identifier"])
							{
								//GD.Print(fieldInst["__identifier"]);
								//Then value is found
								value = fieldInst["__value"].ToString();
								break;
							}

						}
						//In most caces entity doesn't have every possible field, so, we are using default value

						if (value.Length == 0)
						{
							value = fieldDef["defaultValue"].ToString();
						}
						tempChunkCode += value + ", ";


					}
					//if (addTrigger)
					//{
					//	curTriggerInd++;
					//}

					//Closing entityMerged block
					tempChunkCode += "}, ";
				}
				if(entityAmountInChunk == 0)
				{
					arrName = "NULL";
				} else
				{
					arrName = "&" + arrName;

				}
				chunksCodeAll += $"{{{entityAmountInChunk.ToString()}, {arrName}}}, ";
				tempChunkCode += "};\n";
				lastPosX += chunkSizeX;
				firstPosX += chunkSizeX;

				if (entityAmountInChunk == 0)
				{
					//If chunk is empty, then, no need to waste rom storage.
					tempChunkCode = "";
				} else
				{
					//If chunk have at least 1 entity, then, saving chunks code
					chunksCode += tempChunkCode;

				}
			}
			firstPosX = 0;
			lastPosX = chunkSizeX;
			lastPosY += chunkSizeY;
			firstPosY += chunkSizeY;
		}
		chunksCodeAll += "};\n";

		result += chunksCode;
		result += chunksCodeAll;
		return result;
	}
	private String genEntityMergedCode(int curLevel)
	{
		GD.Print("Gen EntityMerged Code...");
		Node singleton = (Node)GetNode("/root/singleton");
		int slaveAmount = int.Parse(singleton.Call("get_slave_amount").ToString());
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");
		Godot.Collections.Array mergedFieldDef_arr = (Godot.Collections.Array)singleton.Call("get_merged_fieldDef", "entities");
		//GD.Print(1);
		String result = "";;
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict", "entities");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);
		//Adding gates
		Godot.Collections.Array gateInstances = (Godot.Collections.Array)singleton.Call("get_gateInstances_by_levelNum", curLevel);
		//Merging them
		entityInstances = mergeGodotArrays(entityInstances, gateInstances);
		//entityInstances.Add(gateInstances);

		//Getting gate entityInstance
		//Godot.Collections.Dictionary gateInst_t = (Godot.Collections.Dictionary)singleton.Call("get_entityInstance_t_defId", 0);
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

			int instId = 0;
			instId = (int)int.Parse(entityInst["instId"].ToString());

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
				  bool activated;
				  struct EntityMerged *slaves_arr[10];
				  u16 slave_amount;
				} EntityMerged;

			**********************************/

			result += $"{mergedId.ToString()},"; //mergedId
			result += $"{instId.ToString()},"; //instId
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
			result += "FALSE, "; //activated
			//slaves_arr
			result += "{";
			for (int i = 0; i < slaveAmount; i++)
			{
				result += "NULL, ";
			}
			result += "}, ";

			int defId = int.Parse(entityInst["defId"].ToString());
			Godot.Collections.Dictionary entityDef = (Godot.Collections.Dictionary)singleton.Call("get_entityDef_by_defId", defId);
			Godot.Collections.Array slaves = (Godot.Collections.Array)entityDef["subordinates"];
			result += $"{slaves.Count.ToString()}, "; //slave_amount

			if (showTriggerRects)
			{
				result += "NULL, "; //debugSpr1
				result += "NULL, "; //debugSpr2
			}

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
		//Adding gates
		Godot.Collections.Array gateInstances = (Godot.Collections.Array)singleton.Call("get_gateInstances_by_levelNum", curLevel);
		//Merging them
		entityInstances = mergeGodotArrays(entityInstances, gateInstances);

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
			String triggerType = "0";
			if (entityInst.Contains("triggerTypeName"))
			{
				if (entityInst["triggerTypeName"].ToString().Length > 0)
				{
					triggerType = "TRIGGER_TYPE_" + entityInst["triggerTypeName"].ToString();
				}
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
					bool activated;
					bool prevActivated;
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
			result += "FALSE, "; //activated
			result += "FALSE, "; //prevActivated

			//Closing Trigger block
			result += "}, ";
		}

		//Closing Trigger_arr block
		result += "};\n";
		return result;
	}

	private String genPreloadSprDefCode(int curLevel)
	{
		GD.Print("Gen genPreloadSprDefCode code...");
		Node singleton = (Node)GetNode("/root/singleton");
		String result = "";
		Godot.Collections.Array sprPreloadOptArr = (Godot.Collections.Array)singleton.Call("get_sprOpt1_arr", curLevel);
		String sprPreloadOptArrStr = "{";

		foreach (String sprDefName in sprPreloadOptArr)
		{
			GD.Print(sprDefName);
			sprPreloadOptArrStr += sprDefName + ", ";

		}
		sprPreloadOptArrStr += "}";
		if(sprPreloadOptArr.Count == 0)
		{
			result += $"const PreloadSprDefs const PreloadSprDef_arr_Level_{curLevel.ToString()} = {{NULL, 0}};\n";
		} 
		else
		{
			result += $"const SpriteDefinition** const SprDef_PreloadSprDef_arr_Level_{curLevel.ToString()}[] = " + sprPreloadOptArrStr + ";\n";
			result += $"const PreloadSprDefs const PreloadSprDef_arr_Level_{curLevel.ToString()} = {{&SprDef_PreloadSprDef_arr_Level_{curLevel.ToString()}, {sprPreloadOptArr.Count.ToString()}}};\n";
		}
		
		

		return result;
	}

	private String genEntityAllCode(int curLevel)
	{
		Node singleton = (Node)GetNode("/root/singleton");
		int entityLoadMode = int.Parse(singleton.Call("get_entity_load_opt_mode").ToString());


		String result = "";
		result += $"const EntityAll const EntityAll_arr_Level_{curLevel}[] = ";
		//Getting "entity_name: mergedId" pairs
		Dictionary mergedIdsDict = (Dictionary)singleton.Call("get_entityMeged_ids_dict");
		//Getting entityInstances in curLevel
		Godot.Collections.Array entityInstances = (Godot.Collections.Array)singleton.Call("get_entityInstances_by_levelNum", curLevel);


		//Opening entityAll_arr block
		result += "{";

		//Bullet count
		String bulletAmount = singleton.Call("get_bullet_amount").ToString();
		//AdditionalSlots count		
		String additionalSlots = singleton.Call("get_addEntityMergedSlots").ToString();
		//Entity count
		int entityAmount = (int)singleton.Call("get_entityInstanAmount_by_levelNum", curLevel);
		//Trigger count
		int triggerAmount = (int)singleton.Call("get_TriggerAmount_by_levelNum", curLevel);
		//Gen code
		//Additional slots
		result += additionalSlots + ", ";

		//Bullet
		result += bulletAmount + ", ";
		result += "NULL, ";
		//Entity all
		switch (entityLoadMode)
		{
			case 0: // load entity: no optmization
				result += entityAmount.ToString() + ", ";
				result += $"&EntityMerged_arr_Level_{curLevel.ToString()}, ";
				break;
			case 1: // load entity: chunk optimization
				result += entityAmount.ToString() + ", ";
				result += $"&EntityMergedChunk_arr_Level_{curLevel.ToString()}, ";
				break;
		}
		
		//Trigger
		result += triggerAmount.ToString() + ", ";
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

		int entityLoadMode = int.Parse(singleton.Call("get_entity_load_opt_mode").ToString());
		
		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			result += genPositionsCode(curLevel);
			
			switch (entityLoadMode)
			{
				case 0:
					result += genTriggerCode(curLevel);
					result += genEntityMergedCode(curLevel);
					break;
				case 1:
					System.Collections.Generic.Dictionary<int, int> entityIdEqualsTriggerInd_dict = new System.Collections.Generic.Dictionary<int, int>();
					String temp = "";
					temp += genEntityMergedCode_chunkOpt1(curLevel, ref entityIdEqualsTriggerInd_dict);

					result += genTriggerCode_chunkOpt1(curLevel, ref entityIdEqualsTriggerInd_dict);
					result += temp;
					break;
			}
			
			result += genLocalVariableMergedCode(curLevel);

			result += genEntityAllCode(curLevel);
			
			result += genMessagePacksCode(curLevel);

			result += genLvlCode(curLevel);

			result += genPreloadSprDefCode(curLevel);

		}
		//return result;
		result += "const LevelFull const LevelFull_arr[] = {";
		for (int curLevel = 0; curLevel < amountOfLevel; curLevel++)
		{
			String curLevelStr = curLevel.ToString();
			result += "{" + $"&lvl_Level_{curLevelStr}, &EntityAll_arr_Level_{curLevelStr}, &MessagePack_Level_{curLevelStr}, &LocalVariable_arr_Level_{curLevelStr},  &PreloadSprDef_arr_Level_{curLevelStr}, NULL," + "}, ";
		}
		result += "};\n";

		return result;

	}

	private void loadChunkRuntimeC_CodeReplacer()
	{
		GD.Print("loadChunkRuntime.c code replacing...");
		Node singleton = (Node)GetNode("/root/singleton");
		String loadChunkRuntime_path = engineRootPath + "/build/src/loadChunkRuntime.c";
		String loadChunkRuntimeCode = System.IO.File.ReadAllText(loadChunkRuntime_path);

		String chunkSizeX = singleton.Call("get_chunkSizeX").ToString();
		String chunkSizeY = singleton.Call("get_chunkSizeY").ToString();

		loadChunkRuntimeCode = loadChunkRuntimeCode.Replace("$chunkSizeX$", chunkSizeX);
		loadChunkRuntimeCode = loadChunkRuntimeCode.Replace("$chunkSizeY$", chunkSizeY);

		System.IO.File.WriteAllText(loadChunkRuntime_path, loadChunkRuntimeCode);
		GD.Print("loadChunkRuntime.c code replaced");
	}

	private void loadChunkFullC_CodeReplacer()
	{
		GD.Print("loadChunkFull.c code replacing...");
		Node singleton = (Node)GetNode("/root/singleton");
		String loadChunkFull_path = engineRootPath + "/build/src/loadChunkFull.c";
		String loadChunkFullCode = System.IO.File.ReadAllText(loadChunkFull_path);

		String chunkSizeX = singleton.Call("get_chunkSizeX").ToString();
		String chunkSizeY = singleton.Call("get_chunkSizeY").ToString();

		loadChunkFullCode = loadChunkFullCode.Replace("$chunkSizeX$", chunkSizeX);
		loadChunkFullCode = loadChunkFullCode.Replace("$chunkSizeY$", chunkSizeY);

		System.IO.File.WriteAllText(loadChunkFull_path, loadChunkFullCode);
		GD.Print("loadChunkFull.c code replaced");
	}

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

	private void playerInitC_CodeReplacer()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");

		String codePath = engineRootPath + "/build/src/playerInit.c";
		String codeText = System.IO.File.ReadAllText(codePath);

		if (showTriggerRects)
		{
			String replaceText = "playerBody.debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, levelStartPos.x, levelStartPos.y, TILE_ATTR(PAL3, 11, FALSE, FALSE));\n" +
			"playerBody.debugSpr2 = SPR_addSprite(&spr_debugRightBottom, levelStartPos.x, levelStartPos.y, TILE_ATTR(PAL3, 11, FALSE, FALSE));\n";
			codeText = codeText.Replace("//$showTriggerRects$", replaceText);

		}

		System.IO.File.WriteAllText(codePath, codeText);
		GD.Print("playerInit.c code replaced");
	}

	private void loadLevelC_CodeReplacer()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		int entityLoadMode = int.Parse(singleton.Call("get_entity_load_opt_mode").ToString());

		String codePath = engineRootPath + "/build/src/loadLevel.c";
		String codeText = System.IO.File.ReadAllText(codePath);

		switch (entityLoadMode)
		{
			case 1: //Chunk optimization
				codeText = codeText.Replace("//$loadScreenChunks$", "loadChunkFull();");
				break;
		}
		System.IO.File.WriteAllText(codePath, codeText);
		GD.Print("loadLevel.c code replaced");
	}

	private void allocLevelC_CodeReplacer()
	{
		Node singleton = (Node)GetNode("/root/singleton");

		String codePath = engineRootPath + "/build/src/allocLevel.c";
		String codeText = System.IO.File.ReadAllText(codePath);

		if (true)
		{
			String replaceText = "SPR_setPosition(playerBody.debugSpr1, playerBody.position.x+playerBody.aabb.min.x, playerBody.position.y+playerBody.aabb.min.y);\n" +
			"SPR_setPosition(playerBody.debugSpr2, playerBody.position.x+playerBody.aabb.max.x-8, playerBody.position.y+playerBody.aabb.max.y-8);\n";
			codeText = codeText.Replace("//$spriteTileAlloc$", replaceText);

		}

		System.IO.File.WriteAllText(codePath, codeText);
		GD.Print("allocLevel.c code replaced");
	}
	private void updatePlayerC_CodeReplacer()
	{
		Node singleton = (Node)GetNode("/root/singleton");
		bool showTriggerRects = (bool)singleton.Call("get_show_trigger_rects");

		String codePath = engineRootPath + "/build/src/updatePlayer.c";
		String codeText = System.IO.File.ReadAllText(codePath);

		if (showTriggerRects)
		{
			String replaceText = "SPR_setPosition(playerBody.debugSpr1, playerBody.position.x+playerBody.aabb.min.x, playerBody.position.y+playerBody.aabb.min.y);\n" +
			"SPR_setPosition(playerBody.debugSpr2, playerBody.position.x+playerBody.aabb.max.x-8, playerBody.position.y+playerBody.aabb.max.y-8);\n";
			codeText = codeText.Replace("//$showTriggerRects$", replaceText);
			
		}

		System.IO.File.WriteAllText(codePath, codeText);
		GD.Print("updatePlayer.c code replaced");
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
