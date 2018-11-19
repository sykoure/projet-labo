#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define size 10
#define PI 3.14159265

//Nous utiliserons le filtre de Prewitt pour le moment
int GradiantFilter[3][3] = {
                            {-1,0,1},
                            {-1,0,1},
                            {-1,0,1}
                            };

int TGradiantFilter[3][3] = {
                            {-1,-1,-1},
                            {0,0,0},
                            {1,1,1}
                            };

                            

//Structure du tableau contenant un angle et une direction
typedef struct tabpolaire tabpolaire;
struct tabpolaire{
	int magnitude;
	int angle;
};

void LibereMat(int **mat,int taillex);
int** InitTab(int taillex,int tailley);
void AfficheMat(int** mat,int taillex,int tailley);
void RemplirMat(int** mat,int taillex,int tailley);
tabpolaire** InitTabPolaire(int taillex,int tailley);
tabpolaire **HOG(int** mat,int taillex,int tailley);
tabpolaire TraitementHOG(int **mat,int i,int j,int taillex,int tailley);
void AfficheMatGradiant(tabpolaire** tabpolaire,int taillex,int tailley);
void AfficheTab(float** mat,int taille);
float **InitTabFloat(int taillex,int tailley);
float **PourcentageTab(tabpolaire **hog,int taillex,int tailley);


//Fonction permettant de faire le calcul de gradient
tabpolaire** HOG(int** mat,int taillex,int tailley){
    int i,j;
    tabpolaire** tabpolaire;
    tabpolaire = InitTabPolaire(taillex,tailley);

    for(i = 0;i < taillex;i++){
        for(j = 0;j < tailley;j++){
            if((i == 0)||(j == 0)||(i == taillex-1)||(j == tailley-1)){  
                tabpolaire[i][j].angle = 0;
                tabpolaire[i][j].magnitude = 0;
            }
            else{
                tabpolaire[i][j] = TraitementHOG(mat,i,j,taillex,tailley);
            }
        }
    }
    return tabpolaire;
}

//Fonction permettant de faire la partie calcule de la fonction HOG()
tabpolaire TraitementHOG(int **mat,int i,int j,int taillex,int tailley){
    int ligne,colonne,compteur_l,compteur_c;
    double val = 180.0 / PI;

    int gradientX = 0;
    int GradientY = 0;
    int magnitude = 0;
    double angle = 0;

    compteur_c = 0;
    compteur_l = 0;

    for(ligne = i-1;ligne<=i+1;ligne++){
        for(colonne = j-1;colonne<=j+1;colonne++){

            //application du filtre
            gradientX = gradientX + GradiantFilter[compteur_l][compteur_c] * mat[ligne][colonne];
            GradientY = GradientY + TGradiantFilter[compteur_l][compteur_c] * mat[ligne][colonne];

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

    magnitude = sqrt(gradientX*gradientX + GradientY*GradientY);
    angle = atan2(GradientY,gradientX)*val;
    angle = (angle < 0)? angle+360 : angle;
    tabpolaire tabpolaire;
    tabpolaire.magnitude = magnitude;
    tabpolaire.angle = (int)angle;

    printf("[(%f,%d)]\n",angle,magnitude);

    return tabpolaire;
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

//Fonction initialisant dynamiquement une matrice contenant les valeurs des angles polaires
tabpolaire **InitTabPolaire(int taillex,int tailley){

    int i;
  	tabpolaire **matrice = NULL;

    //allocation d'un tabpolaire
    matrice = (tabpolaire**)malloc ( taillex * sizeof(tabpolaire *) );    

    for ( i = 0 ; i < taillex ; i ++ )
    {
        //allocation d'un tableau de tabpolaire 
        matrice[i] = (tabpolaire*) malloc ( tailley * sizeof(tabpolaire) ); 
		matrice[i]->magnitude = 0;
		matrice[i]->angle = 0;
    }
    return matrice;
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

float **PourcentageTab(tabpolaire** hog,int taillex,int tailley){
	int i,j;
	int pacerelle=0;
	float **pourcentage = NULL;
	pourcentage = InitTabFloat(360,1);

	//On commence par remplir le tableau de 0
    	for(i = 0;i < 360;i++){
        	for(j = 0;j < 1;j++){
			pourcentage[i][j] = 0;
        	}
    	}		
	
	//On rajoute +1 pour chaque valeur
    	for(i = 0;i < taillex;i++){
        	for(j = 0;j < tailley;j++){
			pacerelle = hog[i][j].angle;
			pourcentage[pacerelle][0]++;
        	}
    	}

	//On fait le pourcentage
    	for(i = 0;i < 360;i++){
        	for(j = 0;j < 1;j++){
			    pourcentage[i][j] = (pourcentage[i][j] * 100) / (taillex*tailley);
        	}
    	}    	
	float somme = 0;
	for(i = 0;i < 360;i++){
        	for(j = 0;j < 1;j++){
			    somme = somme +pourcentage[i][j]; 
        	}
    	}
	printf("la somme est egale à %f\n",somme);
	return pourcentage;
}

//Fonction affichant la matrice
void AfficheMatGradiant(tabpolaire** mat,int taillex,int tailley){

    int i,j;
    for(i=0; i < taillex; i++){
        printf("\n ligne : %d\n",i);
        for(j = 0; j < tailley; j++){
            printf("[(%d,%d)]",mat[i][j].angle,mat[i][j].magnitude);
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

//Fonction main
int main(int argv,char** argc){
	srand(time(NULL));
	int **matrice = NULL;
	tabpolaire **hog = NULL;
    float** pourcentage = NULL;
    int **cercle = NULL;
    
    cercle = InitTab(10,10);
	hog = InitTabPolaire(size,size);
	matrice = InitTab(size,size);
    pourcentage = InitTabFloat(360,1);

    cercle[0][3] = 255;
    cercle[0][4] = 255;
    cercle[0][5] = 255;
    cercle[0][6] = 255;
    cercle[1][2] = 255;
    cercle[1][3] = 255;
    cercle[1][6] = 255;
    cercle[1][7] = 255;
    cercle[2][1] = 255;
    cercle[2][2] = 255;
    cercle[2][7] = 255;
    cercle[2][8] = 255;
    cercle[3][0] = 255;
    cercle[3][1] = 255;
    cercle[3][8] = 255;
    cercle[3][9] = 255;
    cercle[4][0] = 255;
    cercle[4][9] = 255;
    cercle[5][0] = 255;
    cercle[5][1] = 255;
    cercle[5][8] = 255;
    cercle[5][9] = 255;
    cercle[6][1] = 255;
    cercle[6][2] = 255;
    cercle[6][7] = 255;
    cercle[6][8] = 255;
    cercle[7][2] = 255;
    cercle[7][3] = 255;
    cercle[7][6] = 255;
    cercle[7][7] = 255;
    cercle[8][3] = 255;
    cercle[8][4] = 255;
    cercle[8][5] = 255;
    cercle[8][6] = 255;

	RemplirMat(cercle,size,size);
	AfficheMat(cercle,size,size);

	hog = HOG(cercle,size,size);
    AfficheMatGradiant(hog,size,size);
    pourcentage = PourcentageTab(hog,size,size);
    AfficheTab(pourcentage,256);
	
	

	return 0;
}
