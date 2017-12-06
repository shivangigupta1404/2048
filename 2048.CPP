#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<time.h>       //For randomize
#include<stdlib.h>     //For random,itoa

#define arrow_up 72
#define arrow_down 80
#define arrow_left 75
#define arrow_right 77
#define quit 27
#define BKCOLOR 0
#define GRIDBG 8
#define GRIDBORDER 15

//Stores empty locations
int *empty_slots;
int *backup_empty_slots;
int backup_empty_size;
int size=0;

void add_slot(int i,int j){
	empty_slots[size++]=i;
	empty_slots[size++]=j;
}

void remove_slot(int index,int j){
	for(int i=j+1;i<size;++i)
		empty_slots[i-2]=empty_slots[i];
	size=size-2;
}

class tile
{
	public:
	int left;
	int top;
	int right;
	int bottom;
	int value;
	int bkcolor;
	int valid_value;
	int calColor(){
		if(value==0)
			return GRIDBG;
		if(value%256==0)
			return 1;  //BLUE
		if(value%128==0)
			return 13; //LIGHTMAGENTA
		if(value%64==0)
			return 12; //LIGHTRED
		if(value%32==0)
			return 11; //LIGHTCYAN
		if(value%16==0)
			return 10; //LIGHTGREEN
		if(value%8==0)
			return 9;  //LIGHTBLUE
		if(value%4==0)
			return 2;  //GREEN
		else
			return 3;  //CYAN
	}
	tile(){}
	void ini(int left,int top,int right,int bottom){
		this->left=left;
		this->top =top;
		this->right=right;
		this->bottom=bottom;
		this->value=0;
		valid_value=0;
		bkcolor=GRIDBG;
		rectangle(left,top,right,bottom);
		int x=(left+right)/2;
		int y=(top+bottom)/2;
		setfillstyle(1,bkcolor);
		floodfill(x,y,GRIDBORDER);
	}
	void copy(tile t){
		this->value      =t.value;
		this->valid_value=t.valid_value;
		this->bkcolor    =t.bkcolor;
	}
	void validate(int value){
		this->value=value;
		valid_value=1;
		bkcolor=calColor();
		plot();
	}
	void invalidate(){
		value=0;
		valid_value=0;
		bkcolor=GRIDBG;
	}
	void addVal(){
		value*=2;
		bkcolor=calColor();
	}
	void plot(){
		rectangle(left,top,right,bottom);
		int x=(left+right)/2;
		int y=(top+bottom)/2;
		setfillstyle(1,bkcolor);
		floodfill(x,y,GRIDBORDER);
		if(valid_value==1)
		{
			char v[10];
			itoa(value,v,10);
			outtextxy(left+10,y-20,v);
		}
	}
};

void show(tile **arr,int n){
   for(int i=0;i<n;++i)
       for(int j=0;j<n;++j)
	   	arr[i][j].plot();
}

void main()
{
	clrscr();
	//Key Configurations
	char undo='u';
	int n=4,i,j,k;
	empty_slots=new int[2*n];
	backup_empty_slots=new int[2*n];

	//Game window appears
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"c:/turboc3/bgi");
	setbkcolor(BKCOLOR);
	settextstyle(1,0,3);
	/* Border */
	int maxx=getmaxx();
	int maxy=getmaxy();
	int corners[]={0,0,maxx,0,maxx,maxy,0,maxy,0,0};
	drawpoly(5,corners);

	//Store values in grid
	tile **arr=new tile*[n];
	tile **backup=new tile*[n];
	for(i=0;i<n;++i){
		arr[i]=new tile[n];
		//Store values in previous state
		backup[i]=new tile[n];
	}

	/*initialising grid */
	setlinestyle(0,1,3);
	setcolor(GRIDBORDER);
	int dx=maxx/(n+4);
	int dy=maxy/(n+4);
	int start_x=2*dx;
	int start_y=2*dy;
	int tile_top,tile_left,tile_right,tile_bottom;
	outtextxy(start_x+10,start_y-50,"Use 'u' for undo");
	setviewport(start_x,start_y,start_x+n*dx,start_y+n*dy,1);
	for(i=0;i<n;++i){
		tile_top=(dy*i);
		tile_bottom=tile_top+dy;
	    for(j=0;j<n;++j){
			tile_left=(dx*j);
			tile_right=tile_left+dx;
			arr[i][j].ini(tile_left,tile_top,tile_right,tile_bottom);
			backup[i][j].ini(tile_left,tile_top,tile_right,tile_bottom);
			add_slot(i,j);
	    }
	}

	//Place two tiles of 2 in two random blocks
	int x,y,rand_index;
	for(i=0;i<2;++i){
		randomize();
		rand_index=random(size/2);
		x=2*rand_index;
		y=x+1;
		arr[empty_slots[x]][empty_slots[y]].validate(2);
		remove_slot(x,y);
	}

	//Logic
	char input;
	int wall,pos,can_wall,random_i,random_j,random_value;
	input=getche();
	do
	{
		if(input==quit)
			exit(0);
		else
		if(input==undo){
			//Restore backup
			for(i=0;i<n;++i)
				for(j=0;j<n;++j)
					arr[i][j].copy(backup[i][j]);
			for(i=0;i<backup_empty_size;++i)
				empty_slots[i]=backup_empty_slots[i];
			size=backup_empty_size;
		}
		else{
			//If only some valid move
			//backup empty slots and grid status
			if(input==arrow_left || input==arrow_right || input==arrow_up || input==arrow_down){
				for(i=0;i<n;++i)
					for(j=0;j<n;++j)
						backup[i][j].copy(arr[i][j]);

				for(i=0;i<size;++i)
					backup_empty_slots[i]=empty_slots[i];
				backup_empty_size=size;
			}

			if(input==arrow_left)
			{
				for(i=0;i<n;++i)
				{
					//combining tiles
				    wall=-1;
				    pos=-1;
				    for(j=1;j<n;++j)
				    {
						if(arr[i][j].valid_value)
						{
						    can_wall=wall;
						    for(k=j-1;k>wall;--k)
						    {
								if(arr[i][k].valid_value==0)
								{
									pos=k;
									can_wall=k-1;
								}
								else
								{
									//It hits a block
									if(arr[i][k].value==arr[i][j].value)
									{
										pos=k;
										can_wall=k;

									}
									break;
								}
								//If none of above two then its already at its position
						    }
						    if(pos>=0 && pos!=j)
						    {
								if(arr[i][pos].valid_value==0)
									arr[i][pos].copy(arr[i][j]);
								else
									arr[i][pos].addVal();
								arr[i][j].invalidate();
								wall=can_wall;
						    }
						}
				    }
				}
			}
			else
			if(input==arrow_right)
			{
				for(i=0;i<n;++i)
				{
					//combining tiles
				    wall=n;
				    pos=n;
				    for(j=n-1;j>=0;--j)
				    {
						if(arr[i][j].valid_value)
						{
						    can_wall=wall;
						    for(k=j+1;k<wall;k++)
						    {
								if(arr[i][k].valid_value==0)
								{
									pos=k;
									can_wall=k+1;
								}
								else  //it hits a block
								{
									if(arr[i][k].value==arr[i][j].value)
									{
										pos=k;
										can_wall=k;
									}
									break;
								}
								//If none of above two then its already at its position
						    }
						    if(pos<n && pos!=j)
						    {
								if(arr[i][pos].valid_value==0)
									arr[i][pos].copy(arr[i][j]);
								else
									arr[i][pos].addVal();
								arr[i][j].invalidate();
								wall=can_wall;
						    }
						}
				    }
				}
			}
			else
			if(input==arrow_up)
			{
				for(j=0;j<n;++j)
				{
					//combining tiles
				    wall=-1;
				    pos=-1;
				    for(i=0;i<n;++i)
				    {
						if(arr[i][j].valid_value)
						{
						    can_wall=wall;
						    for(k=i-1;k>wall;--k)
						    {
								if(arr[k][j].valid_value==0)
								{
									pos=k;
									can_wall=k-1;
								}
								else
								{
									if(arr[i][j].value==arr[k][j].value)
									{
										pos=k;
										can_wall=k;
									}
									break;
								}
								//If none of above two then its already at its position
						    }
						    if(pos>=0 && pos!=i)
						    {
								if(arr[pos][j].valid_value==0)
									arr[pos][j].copy(arr[i][j]);
								else
									arr[pos][j].addVal();
								arr[i][j].invalidate();
								wall=can_wall;
						    }
						}
				    }
				}
			}
			else
			if(input==arrow_down)
			{
				//logic for bottom
				for(j=0;j<n;++j)
				{
					//combining tiles
				    wall=n;
				    pos=n;
				    for(i=n-1;i>=0;--i)
				    {
						if(arr[i][j].valid_value)
						{
						    can_wall=wall;
						    for(k=i+1;k<wall;k++)
						    {
								if(arr[k][j].valid_value==0)
								{
									pos=k;
									can_wall=k+1;
								}
								else
								{
									if(arr[i][j].value==arr[k][j].value)
									{
										pos=k;
										can_wall=k;
									}
									break;
								}
						    }
						    if(pos<n && pos!=i)
						    {
								if(arr[pos][j].valid_value==0)
									arr[pos][j].copy(arr[i][j]);
								else
									arr[pos][j].addVal();
								arr[i][j].invalidate();
								wall=can_wall;
						    }
						}
				    }
				}
			}
		}
		clearviewport();
		show(arr,n);
		//Generate a new random tile
		if(input==arrow_left || input==arrow_right || input==arrow_up || input==arrow_down)
		{
			size=0;
			for(i=0;i<n;++i)
				for(j=0;j<n;++j)
					if(arr[i][j].valid_value==0)
						add_slot(i,j);

			randomize();
			rand_index=random(size/2);
			x=2*rand_index;
			y=x+1;
			randomize();
			random_value=random(5);
			if(random_value!=4)
				random_value=2;
			arr[empty_slots[x]][empty_slots[y]].validate(random_value);
			remove_slot(x,y);
		}
		input=getch();
	}
	while(size!=0);
	clearviewport();
	int score=0;
	for(i=0;i<n;++i)
		for(j=0;j<n;++j)
			score+=arr[i][j].value;
	char *s;
	itoa(score,s,10);
	outtextxy(10,10,"Game Over!!");
	outtextxy(20,40,"Your Score");
	outtextxy(50,80,s);
	getch();
	getch();
	closegraph();
	getch();
}
