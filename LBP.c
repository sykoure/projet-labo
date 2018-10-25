#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define size 15
#define uniform 0

int test = 0;
int UniformPatterns[58] = {0,1,2,3,4,6,7,8,9,11,15,16,20,22,23,31,32,40,41,43,47,63,64,96,104,105,107,111,127,128,144,148,150,151,159,191,192,208,212,214,215,223,224,232,233,235,239,240,244,246,247,248,249,251,252,253,254,255};

//Declaration des fonctions
int **LBP(int** mat,int taillex,int tailley);
void AfficheMat(int** mat,int taillex,int tailley);
void RemplirMat(int** mat,int taillex,int tailley);
int **InitTab(int taillex,int tailley);
int TraitementLBP(int **mat,int i,int j,int taillex,int tailley);
void LibereMat(int **mat,int taillex);
void LibereMatFloat(float **mat,int taillex);
float **PourcentageTab(int **lbp,int taillex,int tailley);
float **PourcentageTabUniform(int** lbp,int taillex,int tailley);
void AfficheTab(float** mat,int taille);
float **InitTabFloat(int taillex,int tailley);
int AppartientTab(float valeur);
void AfficheTabUniform(float** mat,int taille);


//Fonction verifiant si un nombre est présent dans un tableau et renvoie l'indice dans le cas où c'est vrai
int AppartientTab(float valeur){
    int i;
    for(i = 0; i < 58;i++){
        if(valeur == UniformPatterns[i]){
            //printf("Valeur uniforme %.2f and index = %d\n",valeur,i);
            return i;
        }
        else if(valeur < UniformPatterns[i]){
            //printf("Valeur non uniforme %.2f and index = %d\n",valeur,i);
            return -1;
        }
    }
    return -1;
}

//Fonction faisant le traitement pour trouver le LBP d'une cellule
int TraitementLBP(int **mat,int i,int j,int taillex,int tailley){
    int ligne,colonne,compteur_l,compteur_c;
    int **matTemp = NULL;
    matTemp = InitTab(3,3);


    compteur_c = 0;
    compteur_l = 0;
    for(ligne = i-1;ligne<=i+1;ligne++){
        for(colonne = j-1;colonne<=j+1;colonne++){
            matTemp[compteur_l][compteur_c] = mat[ligne][colonne] - mat[i][j];

	    if(test < 5){		
	    printf("DIFFERENCE ENTRE %d et %d \n",mat[ligne][colonne],mat[i][j]);
		}
            //On transforme la matrice différence en matrice binaire
            if(matTemp[compteur_l][compteur_c]>= 0){
                matTemp[compteur_l][compteur_c] = 1;
            }
            else{
                matTemp[compteur_l][compteur_c] = 0;
            }

            //On incrémente les compteurs
            if(compteur_c ==2){
                compteur_c = 0;
                compteur_l++;
            }  
            else{
                compteur_c++;
            }
        }
    }
	if(test < 5){
		printf("binary matrice \n");
		AfficheMat(matTemp,3,3);
	}
    	

    //On s'occupe maintenant de passer la matrice binaire sous forme décimal
    int compteur_somme = 0;
    int somme = 0;
    for(ligne = 0;ligne < 3;ligne++){
        for(colonne = 0;colonne < 3;colonne++){
            if((ligne==1)&&(colonne==1)){}
            else{
                somme = somme + pow(2,compteur_somme)*matTemp[ligne][colonne];
		        if(test < 5){
			        printf("la somme est egal à %d\n",somme);
		        }
                compteur_somme++;
            }
        }
    }
    LibereMat(matTemp,3);
    return somme;
}

//Fonction permettant de créer la matrice Contenant les différents LBP
int **LBP(int** mat,int taillex,int tailley){
    int i,j;
    int **lbp = NULL;
    lbp = InitTab(taillex,tailley);
    for(i = 0;i < taillex;i++){
        for(j = 0;j < tailley;j++){
            if((i == 0)||(j == 0)||(i == taillex-1)||(j == tailley-1)){  
                lbp[i][j] = 165;
            }
            else{
                lbp[i][j] = TraitementLBP(mat,i,j,taillex,tailley);
		test++;
            }
        }
    }
    return lbp;
}

float **PourcentageTab(int** lbp,int taillex,int tailley){
	int i,j;
	int pacerelle=0;
	float **pourcentage = NULL;
	pourcentage = InitTabFloat(256,1);

	//On commence par remplir le tableau de 0
    	for(i = 0;i < 256;i++){
        	for(j = 0;j < 1;j++){
			pourcentage[i][j] = 0;
        	}
    	}		
	
	//On rajoute +1 pour chaque valeur
    	for(i = 0;i < taillex;i++){
        	for(j = 0;j < tailley;j++){
			pacerelle = lbp[i][j];
			pourcentage[pacerelle][0]++;
        	}
    	}

	//On fait le pourcentage
    	for(i = 0;i < 256;i++){
        	for(j = 0;j < 1;j++){
			    pourcentage[i][j] = (pourcentage[i][j] * 100) / (taillex*tailley);
        	}
    	}    	
	float somme = 0;
	for(i = 0;i < 256;i++){
        	for(j = 0;j < 1;j++){
			    somme = somme +pourcentage[i][j]; 
        	}
    	}
	printf("la somme est egale à %f\n",somme);
	return pourcentage;
}

float **PourcentageTabUniform(int** lbp,int taillex,int tailley){
	int i,j;
	float **pourcentage = NULL;
	pourcentage = InitTabFloat(59,1);

	//On commence par remplir le tableau de 0
    for(i = 0;i < 59;i++){
        for(j = 0;j < 1;j++){
		    pourcentage[i][j] = 0;
    	}    	}		
	int index;
	//On rajoute +1 pour chaque valeur si elle appartient au tableau
    for(i = 0;i < taillex;i++){
       	for(j = 0;j < tailley;j++){
		    index = AppartientTab(lbp[i][j]);
			if(index != -1){
               pourcentage[index][0]++;
            }
            else{
                pourcentage[58][0]++;
            }
		}
    }

	//On fait le pourcentage
    for(i = 0;i < 59;i++){
    	for(j = 0;j < 1;j++){
		    pourcentage[i][j] = (pourcentage[i][j] * 100) / (taillex*tailley);
        }
    }    	
	float somme = 0;
	for(i = 0;i < 59;i++){
        for(j = 0;j < 1;j++){
		    somme = somme +pourcentage[i][j]; 
        }
    }
	printf("la somme est egale à %f\n",somme);
	return pourcentage;
}

//Fonction affichant la matrice
void AfficheMat(int** mat,int taillex,int tailley){

    int i,j;
    for(i=0; i < taillex; i++){
        printf("\n ligne : %d\n",i);
        for(j = 0; j < tailley; j++){
            printf("[%d]",mat[i][j]);
        }
    }
    printf("\n");
}

//Fonction affichant un tableau
void AfficheTab(float** mat,int taille){

    int i;
    for(i = 0; i < taille; i++){
        printf("La valeur %d est présente à %.2f pourcent dans l'image\n",i,mat[i][0]);
    }
    printf("\n");
}

//Fonction affichant un tableau Uniform pattern
void AfficheTabUniform(float** mat,int taille){

    int i;
    for(i = 0; i < taille-1; i++){
        printf("La valeur %d est présente à %.2f pourcent dans l'image\n",UniformPatterns[i],mat[i][0]);
    }
    printf("Les patterns non-uniforms sont présents à %.2f pourcent dans l'image\n",mat[58][0]);
    printf("\n");
}


//Fonction permettant de remplir une matrice de nombre aléatoire entre 0 et 255
void RemplirMat(int** mat,int taillex,int tailley){

    int i,j;
    for(i=0; i < taillex; i++){
        for(j = 0; j < tailley; j++){
            mat[i][j] = rand()%256;
        }
    }
}

//Libere la matrice
void LibereMat(int **mat,int taillex){
    int i;
    for (i=0;i<taillex;i++){
        free(mat[i]);
        mat[i] = NULL ;
    }
    free(mat);
    mat = NULL;
}

//Libere la matrice
void LibereMatFloat(float **mat,int taillex){
    int i;
    for (i=0;i<taillex;i++){
        free(mat[i]);
        mat[i] = NULL ;
    }
    free(mat);
    mat = NULL;
}

//Fonction initialisant dynamiquement une matrice
int **InitTab(int taillex,int tailley){

    int i;
    int **matrice = NULL;

    //allocation d'un tableau de tableaux d'entiers
    matrice = malloc ( taillex * sizeof(int *) );    

    for ( i = 0 ; i < taillex ; i ++ )
    {
        //allocation d'un tableau de tableau 
        matrice[i] = malloc ( tailley * sizeof(int *) ); 
    }
    return matrice;
}

float **InitTabFloat(int taillex,int tailley){

    int i;
    float **matrice = NULL;

    //allocation d'un tableau de tableaux d'entiers
    matrice = malloc ( taillex * sizeof(float *) );    

    for ( i = 0 ; i < taillex ; i ++ )
    {
        //allocation d'un tableau de tableau 
        matrice[i] = malloc ( tailley * sizeof(float *) ); 
    }
    return matrice;
}


int main(int argv,char** tab){
    srand(time(NULL));
    int **matrice = NULL;
    int **lbp = NULL;
    float **pourcentage = NULL; 
    float **pourcentageUniform = NULL;   
    
    matrice = InitTab(size,size);
    lbp = InitTab(size,size);
    pourcentage = InitTabFloat(256,1);
    pourcentageUniform = InitTabFloat(59,1);

    RemplirMat(matrice,size,size);
    AfficheMat(matrice,size,size);
    lbp = LBP(matrice,size,size);
    AfficheMat(lbp,size,size);
    if(uniform == 0){
        pourcentage = PourcentageTab(lbp,size,size);
        AfficheTab(pourcentage,256);
    }
    else{
        pourcentageUniform = PourcentageTabUniform(lbp,size,size);
        AfficheTabUniform(pourcentageUniform,59);
    }

	LibereMatFloat(pourcentage,256);
	LibereMatFloat(pourcentageUniform,59);
	LibereMat(matrice,size);
	LibereMat(lbp,size);	

    return 0;
}



