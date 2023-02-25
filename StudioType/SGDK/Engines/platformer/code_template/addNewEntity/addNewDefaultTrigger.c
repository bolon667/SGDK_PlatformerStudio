//find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &$entityName$_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for $entityName$...");
        return;
    }