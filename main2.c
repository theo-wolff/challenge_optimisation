#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct 
{
	int nb_ligne;
	int nb_colonne;
	int** matrice;
    int**cibles;
    int nb_cibles;
}Matrice;


void nb_lignes_fic(int *nb_lignes_fichier, int *nb_cibles, char *graph_fic);

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

void ALGO1(Matrice mat, int *nb_gardiens, int n, char *fichier)
{
    FILE *res = fopen(fichier, "w");
    fprintf(res, "EQUIPE asuwant\n");
    fprintf(res, "INSTANCE %d\n", n);

    int nb_gar=0;
    int max_cible=0;
    int indice_i=0;
    int indice_j=0;
    int nb_cibles = 0;

 

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
                        }
            
                        k++;
                       
                    }
                    while(k2>=0 && mat.matrice[i][k2]!=2)
                    {
                    
                        if(mat.matrice[i][k2]==1)
                        {
                            nb_cibles++;
                            
                        }
                       
                        k2--;
                    }
                    
                    while(p<mat.nb_ligne && mat.matrice[p][j]!=2)
                    {
                        if(mat.matrice[p][j]==1)
                        {
                            nb_cibles++;
                         
                        }
                       
                        p++;
                     
                    }
                    while(p2>=0 && mat.matrice[p2][j]!=2)
                    {
                       
                        if(mat.matrice[p2][j]==1)
                        {
                            nb_cibles++;
                  
                        }
                    
                        p2--;
                    }
                  
                    if(mat.matrice[i][j]==1)
                    {
                        nb_cibles-=3;
                    }
                
                    if(nb_cibles>=max_cible)
                    {
                        max_cible=nb_cibles;
                        indice_i=i;
                        indice_j=j;

                    }
                }
            }
        }



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
        mat.nb_cibles-=max_cible;
    }
    *nb_gardiens=nb_gar;
}

void main()
{
    int nb_gardiens, instance=1;
    char gr[30] = "gr1.txt";
    char res[30] = "res_1.txt";
   
    for(int i =1; i<10; i++)
    {
        Matrice mat = lire_graphe_fichier(gr);

        affichage(mat);
        ALGO1(mat, &nb_gardiens, instance, res);
        printf("\n");
        affichage(mat);

        gr[2]++;
        res[4]++;
        instance++;
    }

   char gr1[30]="gr10.txt";
   char res1[30] = "res_10.txt";
   
    for(int i =1; i<8; i++)
    {
        Matrice mat = lire_graphe_fichier(gr1);
        ALGO1(mat, &nb_gardiens, instance, res1);
        gr1[3]++;
        res1[5]++;
        instance++;
    }
}
