


#define J_SIZE 				80
#define JLPOSX 				0
#define JPOSY 				240 - J_SIZE
#define JRPOSX 				320 - J_SIZE

#define JLNUMPOSX 		JLPOSX+J_SIZE+1
#define JRNUMPOSX 		JRPOSX-8*8-1
#define JNUMPOSY			JPOSY + J_SIZE - 16*2

#define J_MOUSESIZE 	3
#define J_MOUSESCALE 	(J_SIZE/2-J_MOUSESIZE-1)/9
#define JLMOUSESPOSX 	JLPOSX+J_SIZE/2
#define JRMOUSESPOSX 	JRPOSX+J_SIZE/2
#define JMOUSESPOSY 	JPOSY+J_SIZE/2

#define ButtonSIZE		J_SIZE/3/2

#define ButtonAPOSX		JLPOSX + J_SIZE + ButtonSIZE*3
#define ButtonBPOSX		JRPOSX-ButtonSIZE*3
#define ButtonPOSY		JPOSY + ButtonSIZE*3



void Test_Window (void);
