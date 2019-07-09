#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define sqr(x) x*x
#define MAX(a,b) (((a)>(b))?(a):(b))

float dist[136],count[136];


struct structure
{
	float a[12600];
	int cluster;	/*to store cluster no. to which the object belongs*/
}p[136];



struct structure centroid[50];


int k,i,j,row=136,col=12600,classlable=2;
double l,m[12600],mean;



int f[100];/*to store flag value of whether there is a change in centroid fields-0 for no change more than 0 for change need f[k]*/

void input()//takes input of k
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
	FILE *fp1=fopen("prostate.txt","r");
	
	if(fp1==NULL)
	{
		printf("error");
	}
	fscanf(fp1,"%d",&row);
	fscanf(fp1,"%d",&col);
	fscanf(fp1,"%d",&classlable);
	
	
	
	printf("%d %d \n",row,col);
	//col-=1;
	int i,j;
	float test;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			fscanf(fp1,"%f",&test);
		
			p[i].a[j]=test;
		}
		fscanf(fp1,"%d",&test);
	}		
	fclose(fp1);
}




float intra(int r)
{
	int i,j,clusterno=p[r].cluster,c=0,in=0;
	float s=0.0,sum=0.0;
	for(i=0;i<row;i++)
	{
		if(p[r].cluster==clusterno)
		{
			c++;
			s=0;
			for(j=0;j<col;j++)
			{
				s+=(p[r].a[j]-p[i].a[j]);
			}
			
			sum+=s;
		}
	}
	return fabsf(sum/c);

}

double inter(int r)
{
	int i,j;
	double s=0.0;
	for(i=0;i<k;i++)
	{
		dist[i]=0;
		count[i]=0;
	}
	count[p[r].cluster]=1;
	for(i=0;i<row;i++)
	{
		if(p[i].cluster==p[r].cluster)
			continue;
		for(j=0;j<col;j++)
			dist[p[i].cluster]+=fabs(p[i].a[j]-p[r].a[j]);
		count[p[i].cluster]++;
	}
	double min;
	min=HUGE_VAL;
	
	
	int index=0;
	for(i=0;i<k;i++)
	{
		if(p[r].cluster==i)
			continue;
		dist[i]=dist[i]/count[i];
	
		if(min>dist[i])
		{
			min=dist[i];
			index=i;
		}
	}
	
	return min;

}

void silhoutte()
{
	int i;
	double s=0.0,sh,ai=0,bi=0,m,d;
	for(i=0;i<row;i++)
	{
		ai=inter(i);  
		bi=intra(i);
		d=ai-bi;
		printf("d=%lf ",d);
		m=MAX(ai,bi);
		printf("max=%lf ",m);
		sh=d/m;
		printf("sh=%lf \n",sh);
		s+=sh;
	}
	s/=row;
	printf("%lf",s);
}




int main()
{
	input();
	int temp,a;
	
	
	file_read();
	printf("\nRead from file\n");
	

//	for(a=2;i<8;i++)
//	{
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
/*	for(i=0;i<row;i++)
	{
		
		printf("%d				%d\n",i,p[i].cluster);
	}*/
	printf("For k=%d",a);
	silhoutte();
	printf("\n");
	
	
	
	return 0;
}
