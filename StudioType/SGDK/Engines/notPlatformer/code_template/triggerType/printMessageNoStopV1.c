if(!trigger->activated && trigger->prevActivated){
	//just deactivated
	VDP_setWindowVPos(FALSE, 0);
}

if(trigger->activated && !trigger->prevActivated){
	//just activated
	for(s16 i=0; i<curMessagePacks[trigger->val1].len; i++){
		printMessageNoStop(curMessagePacks[trigger->val1].messages[i].str, FALSE, 1);
	}
}
					