void addNew_$entityName$(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //$addNewTrigger$

    //find first free index in arr
    KDebug_Alert("new $entityName$");
    for(u16 i=0; i<curEntityAll->$entityType$_size; i++){
        if(!curEntityAll->$entityType$_arr[i].alive){
            memcpy(&curEntityAll->$entityType$_arr[i], &$entityName$_default, sizeof($entityType$));
            curEntityAll->$entityType$_arr[i].posInt = posInt;
            curEntityAll->$entityType$_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->$entityType$_arr[i].spd = spd;
            curEntityAll->$entityType$_arr[i].alive = TRUE;
            curEntityAll->$entityType$_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for $entityName$!");
            return;
        }
    }
    KDebug_Alert("Fail for $entityName$...");
    
}
