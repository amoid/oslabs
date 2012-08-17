#include "common.h"
#include "life.h"

void main(void)
{
	int row, col;
	Grid map;
	Grid newMap;
	
	Initialize(map);
	WriteMap(map);
	
	printf("This is the initial configuation you have chose.\n",
		"Press <Enter> to continue.\n");
		
	while(getcha() != '\n');
		
	do{
		for(row = 1; row <= MAXROW; row++){
			for(col = 1; col <= MAXCOL; col++){
				switch(NeighborCount(map, row, col)){
					case 0:
					case 1:
						newMap[row][col] = DEAD;
						break;
					case 2:
						newMap[row][col] = map[row][col];
						break;
					case 3:
						newMap[row][col] = ALIVE;
						break;
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
						newMap[row][col] = DEAD;
						break;					
				}	
			}	
		}	
		CopyMap(map,newMap);
		WriteMap(map);
		printf("Do you want to continue viewing the new generations\n");	
	}while(UserSayYes());
}