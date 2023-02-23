void addNew_$entityName$(Vect2D_s16 posInt, Vect2D_f16 spd){
    //find first free index in arr
    for(u16 i=0; i<curEntityAll->EntityMerged_arr->$entityName$_size; i++){
        if(curEntityAll->EntityMerged_arr->$entityName$_arr[i].alive == FALSE){
            memcpy(curEntityAll->$entityName$_arr[i], $entityName$_default, sizeof($entityName$))
            curEntityAll->$entityName$_arr[i].posInt = posInt;
            curEntityAll->$entityName$_arr[i].spd = spd;
            return;
        }
    }
}
