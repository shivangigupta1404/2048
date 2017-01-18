#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<iomanip.h>    //For setw
#include<graphics.h>
#include<time.h>       //For randomize
#include<stdlib.h>     //For random
#include<math.h>       //For ceil

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
	{ /* if(val==0) return default bkcolor */
	return 1;}
	tile(){}
	void ini(int left,int top,int right,int bottom)
	{
		this->left=left;
		this->top =top;
		this->right=right;
		this->bottom=bottom;
		this->value=0;
		valid_value=0;
		bkcolor=calColor();
		rectangle(left,top,right,bottom);
	}
	void copy(tile t)
	{
		this->value      =t.value;
		this->valid_value=t.valid_value;
		this->bkcolor    =t.bkcolor;
		//plot();
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
		bkcolor=calColor();
	}
	void addVal()
	{
		value*=2;
		bkcolor=calColor();
	}
	void plot()
	{
		int x=(left+right)/2;
		int y=(top+bottom)/2;
		if (value==0)
		    outtextxy(x,y,"hey");
		else
		{
		  char v[10];
		  itoa(value,v,10);
		  outtextxy(x,y,v);
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
	char up='u',down='s',left='a',right='d',undo='u',quit='q',choice;
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
	//Game start
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"c:/turboc3/bgi");
	int maxx=getmaxx();
	int maxy=getmaxy();
	setbkcolor(7);
	/* Border */
	int corners[]={0,0,maxx,0,maxx,maxy,0,maxy,0,0};
	drawpoly(5,corners);
	setlinestyle(0,1,3);
	setcolor(8);
	tile **arr=new tile*[n];
	for(i=0;i<n;++i)
		arr[i]=new tile[n];
	/*Forming grid */
	int dx=maxx/(n+4);
	int dy=maxy/(n+4);
	int start_x=2*dx;
	int start_y=2*dy;
	int tile_top,tile_left,tile_right,tile_bottom;
	//setviewport(start_x,start_y,start_x+n*dx,start_y+n*dy,1);
	for(i=0;i<n;++i)
	{
	    for(j=0;j<n;++j)
	    {
		tile_left=start_x+(dx*j);
		tile_right=tile_left+dx;
		tile_top=start_y+(dy*i);
		tile_bottom=tile_top+dy;
		arr[i][j].ini(tile_left,tile_top,tile_right,tile_bottom);
	    }
	}
	//Place two tiles of 2 in two random blocks
	int x1,x2,y1,y2;
	randomize();
	x1=random(n);
	y1=random(n);
	do
	{
		randomize();
		y2=random(n);
		x2=random(n);
	}while(x1==x2 && y1==y2);
	arr[x1][y1].validate(2);
	arr[x2][y2].validate(2);
	//Logic
	char input; int count,wall,pos,can_wall;
	input=getche();
	if(input==left){
		cout<<"left";
		 for(i=0;i<n;++i){
			//combining tiles
		    wall=-1;pos=-1;
		    for(j=0;j<n;++j)
		    {
			if(arr[i][j].valid_value)
			{
			     can_wall=wall;
			     for(k=j-1;k>wall;--k)
			     {
				if(arr[i][k].valid_value==0)
				{pos=k;can_wall=k-1;}
				else if(arr[i][k].value==arr[i][j].value)
				{pos=k;can_wall=k;break;}
				//If none of above two then its already at its position
			     }
			     /* Now the position is pos either double pos if not empty or replace if empty */
			     if(pos>=0 && pos!=j)
			     {
				if(arr[i][pos].valid_value==0)
					arr[i][pos].validate(arr[i][j].value);
				else
					arr[i][pos].addVal();
				arr[i][j].invalidate();
				wall=can_wall;
			     }
			}
		    }
		}
		clearviewport();
		show(arr,n);
		getch();
	}
	getch();
	closegraph();
	getch();
}