#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define sqr(x) x*x
#define MAX(a,b) (((a)>(b))?(a):(b))



double *high;
struct structure
{
	float a[18432];
	int cluster;	/*to store cluster no. to which the object belongs*/
}p[30];

struct structure centroid[136];


int k,i,j,row=30,col=18432,classlable=2;
double l,m[18432],mean;



int f[30];/*to store flag value of whether there is a change in centroid fields-0 for no change more than 0 for change need f[k]*/

void input()/*takes input of k*/
{
	printf("Enter k:\n");
	scanf("%d",&k);	
}


void initialize_centroid()/*sets the array storing result of comparison of old centroid and new centroid*/
{
	for(i=0;i<k;i++)
	{
		f[i]=1;
	}
}
double mean_difference(int n,int q)/*finds mean difference between two rows where n is the row and q is the centroid no.*/
{
	
	double d,mean=0;
	for(i=0;i<col;i++)
	{
		 d=(p[n].a[i]-centroid[q].a[i]);
		 mean+=sqr(d);
	}
	return(sqrt(mean));
	
}

double centroid_difference(int x)/*finds difference between two centroids*/
{
/*	printf("Finding centroid difference");*/

	double d,mean=0;
	for(i=0;i<col;i++)
	{
		 d=(centroid[x].a[i]-m[i]);
		 mean+=sqr(d);
	}
	printf("\nReturning mean\n");
	return(sqrt(mean));
}

void cluster_assign()/*assigns cluster to each row of dataset according to minimum distance*/
{
	int pos,in,l=0;
 	 static int g=0;
	 for(in=0;in<row;in++)
	 {
	 	l=mean_difference(in,0);
		pos=0;
		
		for(j=1;j<k;j++)
		{
			if((mean_difference(in,j))<l)
			{
				l=mean_difference(in,j);
				pos=j;
			}
		}
		p[in].cluster=pos;
	
 	}
	 
 	printf("\nCluster assign\n");
 	
}


void cluster_mean(int cl)/*finds mean/centroid of a cluster*/
{
	int c,x;
	printf("Cluster_mean running\n");
	for(x=0;x<col;x++)
	{
		mean=0.0;
		c=0;
		for(j=0;j<row;j++)
		{
			if(p[j].cluster!=cl)/*trying to match cluster no. of the present object with the given cluster no.*/
			{
				continue;	
			}
			mean+=p[j].a[x];
			c++;
		}
		mean/=c;
		m[x]=mean;	
	}
	printf("finding mean");	
}

int allnotzero()/*checks whether there is change in centroid values*/
{
	printf("all not zero check");
	for(i=0;i<k;i++)
	{
		if(f[i]!=0)
			return 1;
	}
	return 0;
}
void k_means()/*k_means algo*/
{
	int t,g=0;
	initialize_centroid();
	printf("Centroid initialized\n");
	while(allnotzero())
	{
		printf("while loop\n");
		
		for(t=0;t<k;t++)/*for each cluster*/
		{
			cluster_mean(t);/*finding new mean of t th cluster*/
			
			
			if(centroid_difference(t)<0.1)/*checking if there is change in the value of centroid */
				f[t]=0;/*if no change assigning flag for ith cluster to 0*/
				
			else /*if there is change assigning the flag for ith cluster to 1 and copying new array containing cluster mean to ith centroid */
				{ 
					f[t]=1;
					for(j=0;j<col;j++)
					{
						centroid[t].a[j]=m[j];
					}
				}
		
		}
		cluster_assign();/*assigning cluster to each row*/
	    printf("K means");
	}
	
}

void file_read()
{
	FILE *fp1=fopen("roar.txt","r");
	
	if(fp1==NULL)
	{
		printf("error");
	}
	fscanf(fp1,"%d",&row);
	fscanf(fp1,"%d",&col);
	fscanf(fp1,"%d",&classlable);
	printf("%d %d %d\n",row,col,classlable);
	col-=classlable;
	int i,j;
	float test;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			fscanf(fp1,"%f",&test);
		//	printf("%f",test);
			p[i].a[j]=test;
		}
	}		
	fclose(fp1);
}

//////////////Dunn indexing





double EDist(int x,int y)/*x=object row no.  ,  y=medoid i.e row no. in dataset that is selected*/
{
	double d=0.0;
	int i,j;
	for(i=0;i<col;i++)
	{
		d+=pow((p[x].a[i]-p[y].a[i]),2);	
	}
	return sqrt(d);
}



double maxDia_minInter()
{
	
	int i,j;
	double v,inter=EDist(0,1);
	v=inter;
	for(i=0;i<row;i++)
	{
		for(j=i+1;j<row;j++)
		{
			v=EDist(i,j);
			
			if(p[i].cluster==p[j].cluster)////same cluster so intra cluster
			{		
				
				if(v>high[p[i].cluster])
				{
					high[p[i].cluster]=v;		
				}	
			}
			else ///different cluster
			{
				if(inter>v)
				{	
					inter=v;
				}
			}
		}
	
	}
	return inter;
}
void dunn()
{
	
	int i;
	double di,inter,max;
	high=(double*)malloc(sizeof(double)*k);
	for(i=0;i<k;i++)
	{
		high[i]=0.0;
	}
	inter=maxDia_minInter();
     max=high[0];
	for(i=1;i<k;i++)
	{
		
		if(max<high[i])
			max=high[i];
	}
	
	
	for(i=0;i<k;i++)
	{
		printf("\nhigh{i}=%lf",high[i]);
	}
	printf("max %lf,min %lf",max,inter);

   di=inter/max;
	printf("\n\nThe dunn index is %lf",di);
	
}





int main()
{
	input();
	int temp;
	
	
	file_read();
	printf("\nRead from file\n");
	

	
    srand(time(NULL));
   
	for(i=0;i<k;i++)
	{
	   temp=rand()%(row-0)+0;
	   centroid[i].cluster=i;printf("%d",temp);
	   for(j=0;j<col;j++)
	   {
	   		centroid[i].a[j]=p[temp].a[j];
	   		
	   }
		
	}

	initialize_centroid();
    printf("\nInitial flag for centroids set\n");

	cluster_assign();
	printf("Initial clusters assigned\n");
	
	k_means();
	printf("k means performed\n");
	int h;
	printf("Row				Cluster\n\n");
	for(i=0;i<row;i++)
	{
		
		printf("%d				%d\n",i,p[i].cluster);
	}
	dunn();
	
	
	return 0;
}
