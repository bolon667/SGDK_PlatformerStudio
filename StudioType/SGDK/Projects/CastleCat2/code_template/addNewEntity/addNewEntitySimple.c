void addNew_EntitySimple(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &EntitySimple_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for EntitySimple...");
        return;
    }

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
