#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<iomanip.h>    //For setw
#include<graphics.h>
#include<time.h>       //For randomize
#include<stdlib.h>     //For random

int BKCOLOR=0;
int GRIDBG=8;
int GRIDBORDER=15;

//Stores empty locations
int empty_slots[32];
int backup_empty_slots[32];
int backup_empty_size;
int size=0;

void add_slot(int i,int j)
{
	empty_slots[size++]=i;
	empty_slots[size++]=j;
}

void remove(int index)
{
	for(int i=index+1;i<size;++i)
		empty_slots[i-1]=empty_slots[i];
	size--;
}

void remove_slot(int i,int j)
{
	if(i<j)
	{
		remove(i);remove(j-1);
	}
	else
	if(j<i)
	{
		remove(j);remove(i-1);
	}
	else
		remove(i);
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
	int calColor()
	{
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
	void ini(int left,int top,int right,int bottom)
	{
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
	void copy(tile t)
	{
		this->value      =t.value;
		this->valid_value=t.valid_value;
		this->bkcolor    =t.bkcolor;
	}
	void validate(int value)
	{
		this->value=value;
		valid_value=1;
		bkcolor=calColor();
		plot();
	}
	void invalidate()
	{
		value=0;
		valid_value=0;
		bkcolor=GRIDBG;
	}
	void addVal()
	{
		value*=2;
		bkcolor=calColor();
	}
	void plot()
	{
		rectangle(left,top,right,bottom);
		int x=(left+right)/2;
		int y=(top+bottom)/2;
		setfillstyle(1,bkcolor);
		floodfill(x,y,GRIDBORDER);
		if(valid_value==1)
		{
			char v[10];
			itoa(value,v,10);
			outtextxy(x,y-20,v);
		}
	}
	void display()
	{
		cout<<"\n"<<"left="<<left<<"\tright="<<right<<"\ttop="<<top<<"\tbottom="<<bottom;
	}
};

void show(tile **arr,int n)
{
   for(int i=0;i<n;++i)
       for(int j=0;j<n;++j)
	   arr[i][j].plot();
}

void main()
{
	clrscr();
	//Key Configurations
	char up='w',down='s',left='a',right='d',undo='u',quit='q',choice;
	int n=4,i,j,k;
	do
	{
		gotoxy(17,5);
		cout<<"KEYS CONFIGURATIONS"<<"\n";
		cout<<setw(36)<<"  -------------------\n";
		cout<<setw(17)<<"|"<<setw(8)<<"UP"   <<setw(6)<<"  |   ";
		up=getche();cout<<setw(4)<<"|\n";
		cout<<setw(17)<<"|"<<setw(8)<<"DOWN" <<setw(6)<<"  |   ";
		down=getche();cout<<setw(4)<<"|\n";
		cout<<setw(17)<<"|"<<setw(8)<<"LEFT" <<setw(6)<<"  |   ";
		left=getche();cout<<setw(4)<<"|\n";
		cout<<setw(17)<<"|"<<setw(8)<<"RIGHT"<<setw(6)<<"  |   ";
		right=getche();cout<<setw(4)<<"|\n";
		cout<<setw(17)<<"|"<<setw(8)<<"UNDO"<<setw(6)<<"  |   ";
		undo=getche();cout<<setw(4)<<"|\n";
		cout<<setw(17)<<"|"<<setw(8)<<"QUIT"<<setw(6)<<"  |   ";
		quit=getche();cout<<setw(4)<<"|\n";
		cout<<setw(36)<<"  -------------------\n";
		gotoxy(15,14);
		cout<<"Are you sure?(y/n) : ";
		cin>>choice;
	}
	while(choice!='y'&& choice!='Y');
	gotoxy(15,15);
	cout<<"Press any key to start";
	getch();

	//Game window appears
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"c:/turboc3/bgi");
	setbkcolor(BKCOLOR);
	settextstyle(1,0,3);

	cout<<"\n   KEYS";
	cout<<"\n  ----------";
	cout<<"\n  QUIT  : "<<quit;
	cout<<"\n  UNDO  : "<<undo;
	cout<<"\n  UP    : "<<up;
	cout<<"\n  DOWN  : "<<down;
	cout<<"\n  LEfT  : "<<left;
	cout<<"\n  RIGHT : "<<right;
	/* Border */
	int maxx=getmaxx();
	int maxy=getmaxy();
	int corners[]={0,0,maxx,0,maxx,maxy,0,maxy,0,0};
	drawpoly(5,corners);

	//Store values in grid
	tile **arr=new tile*[n];
	for(i=0;i<n;++i)
		arr[i]=new tile[n];

	//Store values in previous state
	tile **backup=new tile*[n];
	for(i=0;i<n;++i)
		backup[i]=new tile[n];

	/*initialising grid */
	setlinestyle(0,1,3);
	setcolor(GRIDBORDER);
	int dx=maxx/(n+4);
	int dy=maxy/(n+4);
	int start_x=2*dx;
	int start_y=2*dy;
	int tile_top,tile_left,tile_right,tile_bottom;
	setviewport(start_x,start_y,start_x+n*dx,start_y+n*dy,1);
	for(i=0;i<n;++i)
	{
	    for(j=0;j<n;++j)
	    {
			tile_left=(dx*j);
			tile_right=tile_left+dx;
			tile_top=(dy*i);
			tile_bottom=tile_top+dy;
			arr[i][j].ini(tile_left,tile_top,tile_right,tile_bottom);
			backup[i][j].ini(tile_left,tile_top,tile_right,tile_bottom);
			add_slot(i,j);
	    }
	}

	//Place two tiles of 2 in two random blocks
	int x,y,rand_index;
	for(i=0;i<2;++i)
	{
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
			break;
		else
		if(input==undo)  //Restore backup
		{
			for(i=0;i<n;++i)
				for(j=0;j<n;++j)
					arr[i][j].copy(backup[i][j]);
			for(i=0;i<backup_empty_size;++i)
				empty_slots[i]=backup_empty_slots[i];
			size=backup_empty_size;

		}
		else //If only some valid move 
		{
			//backup empty slots and grid status
			if(input==left || input==right || input==up || input==down)
			{
				for(i=0;i<n;++i)
					for(j=0;j<n;++j)
						backup[i][j].copy(arr[i][j]);
				
				for(i=0;i<size;++i)
					backup_empty_slots[i]=empty_slots[i];
				backup_empty_size=size;
			}

			if(input==left)
			{
				for(i=0;i<n;++i)
				{
					//combining tiles
				    wall=-1;
				    pos=-1;
				    for(j=0;j<n;++j)
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
								else   //It hits a block
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
			if(input==right)
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
			if(input==up)
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
			if(input==down)
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
		if(input==left || input==right || input==up || input==down)
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
	while(input!=quit);
	closegraph();
}