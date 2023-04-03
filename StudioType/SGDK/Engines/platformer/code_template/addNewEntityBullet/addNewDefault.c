$entityType$* addNew_$entityName$(Vect2D_s16 posInt, Vect2D_f16 spd){
    KDebug_Alert("new $entityName$");
    for(u16 i=0; i<curEntityAll->$entityType$_size; i++){
        if(!curEntityAll->$entityType$_arr[i].alive){
            //Copying template of default values
            memcpy(&curEntityAll->$entityType$_arr[i], &$entityName$_default, sizeof($entityType$));
            //Changing pos
            curEntityAll->$entityType$_arr[i].posInt = posInt;
            curEntityAll->$entityType$_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            //Changing spd
            curEntityAll->$entityType$_arr[i].spd = spd;
            //Changing making entity ALIVE
            curEntityAll->$entityType$_arr[i].alive = TRUE;
            KDebug_Alert("Success for $entityName$!");
            return NULL;
        }
    }
    KDebug_Alert("Fail for $entityName$...");
    return NULL;
}
