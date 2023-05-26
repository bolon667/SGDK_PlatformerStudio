#include "../inc/levels.h"
void deallocLevel(){
    //Deallocate prev entityData to avoid memory leak
	MEM_free(curLocalVariables);
	MEM_free(curEntityAll->EntityBulletMerged_arr);
	MEM_free(curEntityAll->EntityMerged_arr);
	MEM_free(curEntityAll->Trigger_arr);
	MEM_free(curEntityAll);

    MEM_free(bga);
	MEM_free(bgb);

	MEM_pack();
}
