$entityType$* addNew_$entityName$(Vect2D_s32 posInt, Vect2D_f16 spd){
    s16 foundTriggerInd = -1;
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(curEntityAll->Trigger_arr[i].isFree == TRUE){
            //We can't change here, because, what if we have available trigger but not have available EntityMerged, correct, everything will break
            KDebug_Alert("Success for Trigger!");
            foundTriggerInd = i;
            break;
        }
    }

    // s16 testAmount = 0;
    // for(u16 i=0; i<curEntityAll->Trigger_size; i++){
    //     if(curEntityAll->Trigger_arr[i].isFree == TRUE){
    //         testAmount++;
    //     }
    // }

    // KLog_S1("available triggers:", testAmount);

    if(foundTriggerInd == -1) {
        KDebug_Alert("Fail for Trigger and for $entityName$...");
        return NULL;
    }

    for(u16 i=0; i<curEntityAll->$entityType$_size; i++){
        if(!curEntityAll->$entityType$_arr[i].alive){
            //Changing trigger
            KLog_S1("CCCCC:", foundTriggerInd);
            memcpy(&curEntityAll->Trigger_arr[foundTriggerInd], &$entityName$_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[foundTriggerInd].pos = posInt;
            curEntityAll->Trigger_arr[foundTriggerInd].alive = TRUE;
            curEntityAll->Trigger_arr[foundTriggerInd].isFree = FALSE;
            // curEntityAll->Trigger_arr[foundTriggerInd].isFree = FALSE;

            //Copying template of default values
            memcpy(&curEntityAll->$entityType$_arr[i], &$entityName$_default, sizeof($entityType$));
            //Changing pos
            curEntityAll->$entityType$_arr[i].posInt = posInt;
            curEntityAll->$entityType$_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            //Changing spd
            curEntityAll->$entityType$_arr[i].spd = spd;
            //Changing making entity ALIVE
            curEntityAll->$entityType$_arr[i].alive = TRUE;
            //Adding trigger to entity
            curEntityAll->$entityType$_arr[i].trigger = &curEntityAll->Trigger_arr[foundTriggerInd];
            KDebug_Alert("Success for $entityName$!");

            curEntityAll->curEntityMerged_aliveAmount++;
            return &curEntityAll->$entityType$_arr[i];
        }
    }
    KDebug_Alert("Fail for $entityName$...");
    return NULL;
}

    
    