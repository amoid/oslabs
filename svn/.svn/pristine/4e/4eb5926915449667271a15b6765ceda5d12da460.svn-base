#include <stdio.h>

static int cur_disk = 0;

void 
move(int disk_count, int start, int finish, int temp)
{
	if(disk_count == 0)return;
		
	move(disk_count - 1, start, temp, finish);
	printf("Move the %d disk from %d to %d\n\n",disk_count, start, finish);
	move(disk_count - 1, temp, finish, start);
	return;	
}


int 
fn(int n)
{
	if(n <= 1) return n;
		
	if(n%2 == 0) return (n + fn(n-1)/2);
		
	if(n%2) return ((fn((n+1)/2)) + (fn((n-1)/2)));	
}


int 
main(void)
{
	int disk_count = 5;
	
	printf("start\n");
	move(disk_count, 1, 3, 2);
	
	printf("fn(100) = %d\n",fn(100));
	
	return 0;	
}
