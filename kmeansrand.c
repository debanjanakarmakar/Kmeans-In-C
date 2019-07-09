#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<time.h>
#define MAX(a,b) (((a)>(b))?(a):(b))



int row,col,k,classlable,*lable;
float **p;

int *medoid,*cluster,*newmedoid;

void input()
{
	printf("Enter k:\n");
	scanf("%d",&k);	
	medoid=(int*)malloc(sizeof(int)*k);
	newmedoid=(int*)malloc(sizeof(int)*k);
}


void read()/*reads from file*/
{
	FILE *fp1=fopen("roar.txt","r");
	int i,j;
	if(fp1==NULL)
	{
		printf("error");
	}
	fscanf(fp1,"%d",&row);
	fscanf(fp1,"%d",&col);
//	fscanf(fp1,"%d",&classlable);
	printf("%d %d",row,col);
	/////////////////////////////////col reduce
	col-=1;
	classlable=2;
	p=(float**)malloc(sizeof(float*)*row);
	lable=(int *)malloc(sizeof(int)*row);

	for(i=0;i<row;i++)
	{
		p[i]=(float *)malloc(sizeof(float)*col);
	}
	float test;
	int test2;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			fscanf(fp1,"%f",&test);
			//printf("%f  ",test);
			p[i][j]=test;
		}
			fscanf(fp1,"%d",&test2);
			//printf(" \n%d ",test2);
			lable[i]=test2;
		
	}	
}

float distance(int x,int y)/*x=object row no.  ,  y=medoid i.e row no. in dataset that is selected*/
{
	float d=0.0;
	int i,j;
	for(i=0;i<col;i++)
	{
		d+=abs(p[x][i]-p[y][i]);	
	}
	return d;
}

void cluster_assign()
{
	int pos,in,j;
	float l=0.0;
	
	 for(in=0;in<row;in++)
	 {
	 	l=distance(in,medoid[0]);
		pos=0;
		
		for(j=1;j<k;j++)
		{
			if((distance(in,medoid[j]))<l)
			{
				l=distance(in,medoid[j]);
				pos=j;
			
			}
			
		}

		cluster[in]=pos;
		
	
 	}	

}

float cal_cost()
{
	float cost=0.0;
	int i,j;
	for(i=0;i<row;i++)
	{
		cost+=distance(i,cluster[i]);
	}
	return cost;
}

int notselected(int x)
{
	
	int i;
	for(i=0;i<k;i++)
	{
		if(medoid[i]==x)
			return 0;
			
	}
	return 1;
}

float swapfindcost()
{
	float cost=0.0,temp,l=0.0;
	int i,j,m;
	for(i=0;i<row;i++)
	{
		l=distance(i,newmedoid[0]);
		
		for(j=1;j<k;j++)
		{
			if(distance(i,newmedoid[j])<l)
				l=distance(i,newmedoid[j]);
		}
		cost+=l;
	}
	return cost;
}

void kmedoid()
{
		int i,j,l,m,n;
		float newcost=0.0;
		for(i=0;i<k;i++)
		{
			float cost1=cal_cost();
			for(j=0;j<row;j++)
			{
				if(notselected(j))
				{
				   for(m=0;m<k;m++)
				   {
				   		if(m==i)
				   			newmedoid[m]=j;
				   		else
				   			newmedoid[m]=medoid[m];
				   }
				   newcost=swapfindcost();
				   if(newcost<cost1)
				   {
						medoid[i]=j;
						cluster_assign();
				   }
				}
			}
		}
	
}




void randindex()
{
	int i,j;
	double rival=0;
	int a,b,c,d;
	a=0;
	b=0;
	c=0;
	d=0;
	for(i=0;i<row;i++)
	{
		for(j=0;j<row;j++)
		{
			if(lable[i]==lable[j])
			{
				if(cluster[i]==cluster[j])
					a++;
				else
					c++;
			}
			else
			{
				if(cluster[i]==cluster[j])
					d++;
				else
					b++;
					
			}
		}
	}
	printf("a,b,c,d=%d %d %d %d",a,b,c,d);
	rival=(double)(a+b)/(double)(a+b+c+d);
	printf("\n\n\nRand index=%lf",rival);
}

int main()
{
	read();
	input();
	int i,j;
	 srand(time(NULL));
	for(i=0;i<k;i++)
	{
		
		medoid[i]=rand()%(row-0)+0;

	}
	cluster=(int *)malloc(sizeof(int)*row);


	cluster_assign();
	
	kmedoid();
	for(i=0;i<row;i++)
	{
		printf("%d \t",cluster[i]);
	}	
	randindex();
	free(p);
	free(medoid);
	free(newmedoid);
	free(cluster);
	return 0;
	
}




