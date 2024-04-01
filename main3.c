#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


typedef struct 
{
	int nb_ligne;
	int nb_colonne;
	int** matrice;
    int**cibles;
    int nb_cibles;
}Matrice;

typedef struct 
{
    int i;
    int j;
    int max_cible;

}Max;

void nb_lignes_fic(int *nb_lignes_fichier, int *nb_cibles, char *graph_fic);
int printRandoms(int lower, int upper);

Matrice lire_graphe_fichier(char* graph_fic)
{   
    int nb_ligne_total, nb_cibles;
    nb_lignes_fic(&nb_ligne_total,&nb_cibles, graph_fic);
    
	FILE* fp;
	fp=fopen(graph_fic,"r");
    Matrice mat;
    mat.nb_cibles = nb_cibles;
	if (fp!=NULL)

	{
		int nb_colonne, nb_ligne;
		fscanf(fp, "%*s");
        fscanf(fp,"%d",&nb_ligne);
        fscanf(fp, "%*s");
        fscanf(fp,"%d",&nb_colonne);

		mat.nb_colonne = nb_colonne;
        mat.nb_ligne=nb_ligne;
		mat.matrice = (int**) malloc(nb_ligne * sizeof(int*));
        mat.cibles=(int**)malloc(nb_cibles*sizeof(int*));


		for (int i = 0; i < nb_ligne; i++)
			mat.matrice[i] = (int*) malloc(nb_colonne * sizeof(int));
        for (int i=0; i<nb_cibles;i++)
            mat.cibles[i]=(int*)malloc(2*sizeof(int));

		/*Initialiser la matrice d'adjacence */
		for (int i = 0; i < nb_ligne; i++)
			for (int j = 0; j < nb_colonne; j++)
				mat.matrice[i][j] = 0;

		/*Instancier la matrice d'adjacence à partir d'un fichier */
        int j=0;
		for (int i = 0; i < nb_ligne_total; i++)
		{
			int u, v;
            char type[100];
            fscanf(fp,"%s", type);
			if (strcmp(type,"OBSTACLE")==0)
            {
                fscanf(fp, "%d%d", &u,&v);
                mat.matrice[u][v] = 2;
            }
            else
            {
                fscanf(fp, "%d %d", &u,&v);
                mat.matrice[u][v] = 1;
                mat.cibles[j][0]=u;
                mat.cibles[j][1]=v;
                j++;
            }
			    
		}
		fclose(fp);
	}

    return mat;
}


void nb_lignes_fic(int *nb_lignes_fichier, int *nb_cibles, char *graph_fic)
{
    FILE *fic = fopen(graph_fic, "r");
    char trash[100];
    int nb_ligne = 0, nb_cible = 0;
    fscanf(fic, "%*s%*d%*s%*d");
    while(fscanf(fic, "%s%*d%*d",trash)==1)
    {
   
        nb_ligne++;
        if(strcmp(trash,"CIBLE")==0)
        {
            nb_cible++;
        }
    }
    *nb_lignes_fichier=nb_ligne;
    *nb_cibles=nb_cible;

    fclose(fic);
}

void affichage(Matrice mat)
{
    for(int i= 0;i<mat.nb_ligne; i++)
    {
        for(int j=0; j<mat.nb_colonne; j++)
        {
            printf("%d ",mat.matrice[i][j]);
        }
        printf("\n");
    }
}


void affichage_cibles(Matrice mat)
{
     for(int i= 0;i<mat.nb_cibles; i++)
    {
        for(int j=0; j<2; j++)
        {
            printf("%d ",mat.cibles[i][j]);
        }
        printf("\n");
    }
}

int ALGO1(Matrice mat, int n, char *fichier)
{
    FILE *res = fopen(fichier, "w");
    fprintf(res, "EQUIPE asuwant\n");
    fprintf(res, "INSTANCE %d\n", n);
    Max Maximum[10000];


    int nb_gar=0;
    int max_cible=0;
    int indice_i=0;
    int indice_j=0;
    int nb_cibles = 0;
    int indice = 0; 
 

    while(mat.nb_cibles!=0)
    {
        max_cible=0;
        indice_i=0;
        indice_j=0;

        for(int i = 0;i<mat.nb_ligne; i++)
        {
            for(int j = 0; j<mat.nb_colonne; j++)
            {
                if(mat.matrice[i][j]!=2)
                {
                    nb_cibles = 0;
              
                  
                    int k=j;
                    int k2=j;
                    int p=i;
                    int p2=i;//pour faire varier dans l'autre sens
                    while(k<mat.nb_colonne && mat.matrice[i][k]!=2)
                    {
                        if(mat.matrice[i][k]==1)
                        {
                            nb_cibles++;
                           // printf("A %d %d\n", i, k);
                            
                        }
            
                        k++;
                       
                    }
                    while(k2>=0 && mat.matrice[i][k2]!=2)
                    {
                    
                        if(mat.matrice[i][k2]==1)
                        {
                            nb_cibles++;
                            //printf("B %d %d\n", i, k2);
                        }
                       
                        k2--;
                    }
                    
                    while(p<mat.nb_ligne && mat.matrice[p][j]!=2)
                    {
                        if(mat.matrice[p][j]==1)
                        {
                            nb_cibles++;
                         //   printf("C %d %d\n", p, j);
                        }
                       
                        p++;
                     
                    }
                    while(p2>=0 && mat.matrice[p2][j]!=2)
                    {
                       
                        if(mat.matrice[p2][j]==1)
                        {
                            nb_cibles++;
                      //      printf("D %d %d\n", p2, j);
                        }
                    
                        p2--;
                    }
                   // printf("%d %d\n", i,j);
                    if(mat.matrice[i][j]==1)
                    {
                        nb_cibles-=3;
                    }
                    //printf("%d\n",nb_cibles);

                    if(nb_cibles==max_cible)
                    {
                        Maximum[indice].i=i;
                        Maximum[indice].j=j;
                        Maximum[indice].max_cible=nb_cibles;
                        max_cible=nb_cibles;
                        indice++;
                    }
                    if(nb_cibles>max_cible)
                    {
                        for(int i=0;i<100;i++)
                        {
                            Maximum[i].i=0;
                            Maximum[i].j=0;
                            Maximum[i].max_cible=0;

                        }
                        indice = 0;
                        Maximum[indice].i=i;
                        Maximum[indice].j=j;
                        Maximum[indice].max_cible=nb_cibles;
                        max_cible=nb_cibles;
                        indice++;
                    }
                    

                }
            }
        }

        int rd = printRandoms(0, indice-1);
      

        max_cible=Maximum[rd].max_cible;
        indice_i=Maximum[rd].i;
        indice_j=Maximum[rd].j;




        //printf("B %d %d %d\n",indice_i, indice_j, max_cible );
        int k=indice_j;
        int k2=indice_j;
        int p=indice_i;
        int p2=indice_i;//pour faire varier dans l'autre sens
        while(k<mat.nb_colonne && mat.matrice[indice_i][k]!=2)
        {
            if(mat.matrice[indice_i][k]==1)
            {
                mat.matrice[indice_i][k]=4;
                mat.matrice[indice_i][indice_j]=3;
            }
         
            k++;
          
        }
        
        while(k2>=0 && mat.matrice[indice_i][k2]!=2)
        {
            if(mat.matrice[indice_i][k2]==1)
            {
                mat.matrice[indice_i][k2]=4;
                mat.matrice[indice_i][indice_j]=3;
            }
         
            k2--;
        }
        while(p<mat.nb_ligne && mat.matrice[p][indice_j]!=2)
        {
            if(mat.matrice[p][indice_j]==1)
            {
                mat.matrice[p][indice_j]=4;
                mat.matrice[indice_i][indice_j]=3;
            }
           
            p++;
          
        }
        while(p2>=0 && mat.matrice[p2][indice_j]!=2)
        {
            
            if(mat.matrice[p2][indice_j]==1)
            {
                mat.matrice[p2][indice_j]=4;
                mat.matrice[indice_i][indice_j]=3;
            }
        
            p2--;
        }
        if(mat.matrice[indice_i][indice_j]==3)
        {
            nb_gar++;
            fprintf(res, "%d %d\n", indice_i, indice_j);
        }
        //printf("%d",max_cible);
        mat.nb_cibles-=max_cible;
       // printf("%d\n",mat.nb_cibles);
    }
    fclose(res);
    return nb_gar;
}

int printRandoms(int lower, int upper)
{
{
    // Initialiser la graine pour la génération des nombres aléatoires
    srand(time(NULL));

    // Générer un nombre aléatoire entre min et max
    int randomNum = (rand() % (upper - lower + 1)) + lower;
    

    return randomNum;
}
}

void main()
{
    int nb_gardiens, instance=15;
    char gr[30] = "gr15.txt";
    char res[30] = "res_15.txt";
    int cpt=0;
 
    // for(int i =1; i<10; i++)
    // {
    Matrice mat = lire_graphe_fichier(gr);
    nb_gardiens = ALGO1(mat, instance, res);

        while(nb_gardiens>140)
        {
         usleep(1000);
         Matrice mat = lire_graphe_fichier(gr);
        
        nb_gardiens = ALGO1(mat, instance, res);
        printf("%d\n", nb_gardiens);
        cpt++;
        

        }
       printf("%d",cpt);

//         gr[2]++;
//         res[4]++;
//         instance++;
//     }

//    char gr1[30]="gr10.txt";
//    char res1[30] = "res_10.txt";
   
//     for(int i =1; i<8; i++)
//     {
//         Matrice mat = lire_graphe_fichier(gr1);
//         ALGO1(mat, &nb_gardiens, instance, res1);
//         gr1[3]++;
//         res1[5]++;
//         instance++;
//     }
}
