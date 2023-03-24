void addNew_EntitySimple(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //$addNewTrigger$

    //find first free index in arr
    KDebug_Alert("new EntitySimple");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &EntitySimple_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for EntitySimple!");
            return;
        }
    }
    KDebug_Alert("Fail for EntitySimple...");
    
}
