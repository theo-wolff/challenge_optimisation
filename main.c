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
    for(int i = 0;i<mat.nb_ligne; i++)
    {
        for(int j = 0; j<mat.nb_colonne; j++)
        {
            if(mat.matrice[i][j]!=2)
            {
                int k=j;
                int p=i;
                while(k<mat.nb_colonne && mat.matrice[i][k]!=2)
                {
                    if(mat.matrice[i][k]==1)
                    {
                        mat.matrice[i][k]=4;
                        mat.matrice[i][j]=3;
                    }
                    k++;
                }
                
                while(p<mat.nb_ligne && mat.matrice[p][j]!=2 )
                {
                    if(mat.matrice[p][j]==1)
                    {
                        mat.matrice[p][j]=4;
                        mat.matrice[i][j]=3;
                    }
                    p++;
                }
                if(mat.matrice[i][j]==3)
                {
                    nb_gar++;
                    fprintf(res, "%d %d\n", i, j);
                }
            }
        }
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
        ALGO1(mat, &nb_gardiens, instance, res);
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
