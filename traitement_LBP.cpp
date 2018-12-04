#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <opencv/cv.h>
#include <time.h>
#include <math.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <fstream>

#define uniform 1

using namespace std;
int test = 0;
int UniformPatterns[58] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 11, 15, 16, 20, 22, 23, 31, 32, 40, 41, 43, 47, 63, 64, 96, 104, 105, 107, 111, 127, 128, 144, 148, 150, 151, 159, 191, 192, 208, 212, 214, 215, 223, 224, 232, 233, 235, 239, 240, 244, 246, 247, 248, 249, 251, 252, 253, 254, 255};

//Declaration des fonctions
int **LBP(int **mat, int taillex, int tailley);
void AfficheMat(int **mat, int taillex, int tailley);
int **InitTab(int taillex, int tailley);
int TraitementLBP(int **mat, int i, int j, int taillex, int tailley);
void LibereMat(int **mat, int taillex);
void LibereMatFloat(float **mat, int taillex);
float **PourcentageTab(int **lbp, int taillex, int tailley);
float **PourcentageTabUniform(int **lbp, int taillex, int tailley);
void AfficheTab(float **mat, int taille);
float **InitTabFloat(int taillex, int tailley);
int AppartientTab(float valeur);
void AfficheTabUniform(float **mat, int taille);
uchar *cvPtr2D(const CvArr *arr, int idx0, int idx1, int *type);

/**
 * Inverse une image "en place"
 */
void remplir(IplImage *img, int **matrice)
{
    int x, y;
    uchar *p;
    assert(img->depth == IPL_DEPTH_8U && img->nChannels == 1);

    for (y = 0; y < img->height; ++y)
    {
        for (x = 0; x < img->width; ++x)
        {
            // récupération d'un pointeur sur le pixel de coordonnées (x,y)
            p = cvPtr2D(img, y, x, NULL);
            matrice[x][y] = *p;
        }
    }
}

//Remplit l'image avec un tableau traité
void renvoyerImg(IplImage *img, int **matrice)
{
    int x, y;
    uchar *p;
    assert(img->depth == IPL_DEPTH_8U && img->nChannels == 1);

    for (y = 0; y < img->height; ++y)
    {
        for (x = 0; x < img->width; ++x)
        {
            // récupération d'un pointeur sur le pixel de coordonnées (x,y)
            p = cvPtr2D(img, y, x, NULL);
            *p = matrice[x][y];
        }
    }
}

//Fonction verifiant si un nombre est présent dans un tableau et renvoie l'indice dans le cas où c'est vrai
int AppartientTab(float valeur)
{
    int i;
    for (i = 0; i < 58; i++)
    {
        if (valeur == UniformPatterns[i])
        {
            //printf("Valeur uniforme %.2f and index = %d\n",valeur,i);
            return i;
        }
        else if (valeur < UniformPatterns[i])
        {
            //printf("Valeur non uniforme %.2f and index = %d\n",valeur,i);
            return -1;
        }
    }
    return -1;
}

//Fonction faisant le traitement pour trouver le LBP d'une cellule
int TraitementLBP(int **mat, int i, int j, int taillex, int tailley)
{
    int ligne, colonne, compteur_l, compteur_c;
    int **matTemp = NULL;
    matTemp = InitTab(3, 3);

    compteur_c = 0;
    compteur_l = 0;
    for (ligne = i - 1; ligne <= i + 1; ligne++)
    {
        for (colonne = j - 1; colonne <= j + 1; colonne++)
        {
            matTemp[compteur_l][compteur_c] = mat[ligne][colonne] - mat[i][j];

            if (test < 5)
            {
                printf("DIFFERENCE ENTRE %d et %d \n", mat[ligne][colonne], mat[i][j]);
            }
            //On transforme la matrice différence en matrice binaire
            if (matTemp[compteur_l][compteur_c] >= 0)
            {
                matTemp[compteur_l][compteur_c] = 1;
            }
            else
            {
                matTemp[compteur_l][compteur_c] = 0;
            }

            //On incrémente les compteurs
            if (compteur_c == 2)
            {
                compteur_c = 0;
                compteur_l++;
            }
            else
            {
                compteur_c++;
            }
        }
    }
    if (test < 5)
    {
        printf("binary matrice \n");
        AfficheMat(matTemp, 3, 3);
    }

    //On s'occupe maintenant de passer la matrice binaire sous forme décimal
    int compteur_somme = 0;
    int somme = 0;
    for (ligne = 0; ligne < 3; ligne++)
    {
        for (colonne = 0; colonne < 3; colonne++)
        {
            if ((ligne == 1) && (colonne == 1))
            {
            }
            else
            {
                somme = somme + pow(2, compteur_somme) * matTemp[ligne][colonne];
                if (test < 5)
                {
                    printf("la somme est egal à %d\n", somme);
                }
                compteur_somme++;
            }
        }
    }
    LibereMat(matTemp, 3);
    return somme;
}

//Fonction permettant de créer la matrice Contenant les différents LBP
int **LBP(int **mat, int taillex, int tailley)
{
    int i, j;
    int **lbp = NULL;
    lbp = InitTab(taillex, tailley);
    for (i = 0; i < taillex; i++)
    {
        for (j = 0; j < tailley; j++)
        {
            if ((i == 0) || (j == 0) || (i == taillex - 1) || (j == tailley - 1))
            {
                lbp[i][j] = 165;
            }
            else
            {
                lbp[i][j] = TraitementLBP(mat, i, j, taillex, tailley);
                test++;
            }
        }
    }
    return lbp;
}

// Fonction permettant de renvoyer un tableau de pourcentage pour les valeurs non-uniformes
float **PourcentageTab(int **lbp, int taillex, int tailley)
{
    int i, j;
    int pacerelle = 0;
    float **pourcentage = NULL;
    pourcentage = InitTabFloat(256, 1);

    //On commence par remplir le tableau de 0
    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 1; j++)
        {
            pourcentage[i][j] = 0;
        }
    }

    //On rajoute +1 pour chaque valeur
    for (i = 0; i < taillex; i++)
    {
        for (j = 0; j < tailley; j++)
        {
            pacerelle = lbp[i][j];
            pourcentage[pacerelle][0]++;
        }
    }

    //On fait le pourcentage
    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 1; j++)
        {
            pourcentage[i][j] = (pourcentage[i][j] * 100) / (taillex * tailley);
        }
    }
    float somme = 0;
    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 1; j++)
        {
            somme = somme + pourcentage[i][j];
        }
    }
    printf("la somme est egale à %f\n", somme);
    return pourcentage;
}

// Fonction permettant de renvoyer un tableau de pourcentage pour les valeurs uniformes
float **PourcentageTabUniform(int **lbp, int taillex, int tailley)
{
    int i, j;
    float **pourcentage = NULL;
    pourcentage = InitTabFloat(59, 1);

    //On commence par remplir le tableau de 0
    for (i = 0; i < 59; i++)
    {
        for (j = 0; j < 1; j++)
        {
            pourcentage[i][j] = 0;
        }
    }
    int index;
    //On rajoute +1 pour chaque valeur si elle appartient au tableau
    for (i = 0; i < taillex; i++)
    {
        for (j = 0; j < tailley; j++)
        {
            index = AppartientTab(lbp[i][j]);
            if (index != -1)
            {
                pourcentage[index][0]++;
            }
            else
            {
                pourcentage[58][0]++;
            }
        }
    }

    //On fait le pourcentage
    for (i = 0; i < 59; i++)
    {
        for (j = 0; j < 1; j++)
        {
            pourcentage[i][j] = (pourcentage[i][j] * 100) / (taillex * tailley);
        }
    }
    float somme = 0;
    for (i = 0; i < 59; i++)
    {
        for (j = 0; j < 1; j++)
        {
            somme = somme + pourcentage[i][j];
        }
    }
    printf("la somme est egale à %f\n", somme);
    return pourcentage;
}

//Fonction affichant la matrice
void AfficheMat(int **mat, int taillex, int tailley)
{

    int i, j;
    for (i = 0; i < taillex; i++)
    {
        printf("\n ligne : %d\n", i);
        for (j = 0; j < tailley; j++)
        {
            printf("[%d]", mat[i][j]);
        }
    }
    printf("\n");
}

//Fonction affichant un tableau
void AfficheTab(float **mat, int taille)
{

    int i;
    for (i = 0; i < taille; i++)
    {
        printf("La valeur %d est présente à %.2f pourcent dans l'image\n", i, mat[i][0]);
    }
    printf("\n");
}

//Fonction affichant un tableau Uniform pattern
void AfficheTabUniform(float **mat, int taille)
{

    int i;
    for (i = 0; i < taille - 1; i++)
    {
        printf("La valeur %d est présente à %.2f pourcent dans l'image\n", UniformPatterns[i], mat[i][0]);
    }
    printf("Les patterns non-uniforms sont présents à %.2f pourcent dans l'image\n", mat[58][0]);
    printf("\n");
}

//Libere la matrice
void LibereMat(int **mat, int taillex)
{
    int i;
    for (i = 0; i < taillex; i++)
    {
        free(mat[i]);
        mat[i] = NULL;
    }
    free(mat);
    mat = NULL;
}

//Libere la matrice
void LibereMatFloat(float **mat, int taillex)
{
    int i;
    for (i = 0; i < taillex; i++)
    {
        free(mat[i]);
        mat[i] = NULL;
    }
    free(mat);
    mat = NULL;
}

//Fonction initialisant dynamiquement une matrice
int **InitTab(int taillex, int tailley)
{

    int i;
    int **matrice = NULL;

    //allocation d'un tableau de tableaux d'entiers
    matrice = (int **)malloc(taillex * sizeof(int *));

    for (i = 0; i < taillex; i++)
    {
        //allocation d'un tableau de tableau
        matrice[i] = (int *)malloc(tailley * sizeof(int));
    }
    return matrice;
}

float **InitTabFloat(int taillex, int tailley)
{

    int i;
    float **matrice = NULL;

    //allocation d'un tableau de tableaux d'entiers
    matrice = (float **)malloc(taillex * sizeof(float *));

    for (i = 0; i < taillex; i++)
    {
        //allocation d'un tableau de tableau
        matrice[i] = (float *)malloc(tailley * sizeof(float));
    }
    return matrice;
}

/**
 * Ce programme prend deux arguments dont un optionnel:
 * IMAGE:     l'image à inverser
 * SAVE_PATH: (optionnel) l'image dans laquelle sauvegarder le résultat
 */
int main(int argc, char *argv[])
{
    // Initialise les variables pour la partie OpenCV
    IplImage *img = NULL;
    const char *src_path = NULL;
    const char *dst_path = NULL;
    const char *window_title = "Image de base";
    const char *window_title2 = "Image traitée";
    ofstream fichier("test.csv", ios::out | ios::ate);  // ouverture en écriture avec effacement du fichier

    // Initialise les variables pour la partie traitement LBP
    int sizex, sizey;
    int **matrice = NULL;
    int **lbp = NULL;
    float **pourcentage = NULL;
    float **pourcentageUniform = NULL;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s IMAGE [SAVE_PATH]\n", argv[0]);
        return EXIT_FAILURE;
    }

    src_path = argv[1];

    // optionnel: sauvegarde du résultat
    // si un second chemin est passé au programme
    if (argc > 2)
        dst_path = argv[2];

    if (!(img = cvLoadImage(src_path, CV_LOAD_IMAGE_GRAYSCALE)))
    {
        fprintf(stderr, "couldn't open image file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    cvNamedWindow(window_title, CV_WINDOW_AUTOSIZE);
    cvShowImage(window_title, img);

    sizey = img->height;
    sizex = img->width;

    // Partie traitement
    matrice = InitTab(sizex, sizey);
    lbp = InitTab(sizex, sizey);
    pourcentage = InitTabFloat(256, 1);
    pourcentageUniform = InitTabFloat(59, 1);

    remplir(img, matrice);

    lbp = LBP(matrice, sizex, sizey);
    AfficheMat(lbp, sizex, sizey);
    if (uniform == 0)
    {
        pourcentage = PourcentageTab(lbp, sizex, sizey);
        AfficheTab(pourcentage, 256);
    }
    else
    {
        pourcentageUniform = PourcentageTabUniform(lbp, sizex, sizey);
        AfficheTabUniform(pourcentageUniform, 59);
    }

    //Permet de sauvegarder les valeurs dans un fichier (afin de creer un histogramme)
    int i;
    for (i = 0; i < 59; i++)
    {
        printf("La valeur %d est présente à %.2f pourcent dans l'image\n", i, pourcentageUniform[i][0]); 
        if(fichier)
        {
                float mat = pourcentageUniform[i][0];
                fichier << mat << ",";
 
        }
        else{
                cerr << "Impossible d'ouvrir le fichier !" << endl;
        }
 
        
    }

    renvoyerImg(img, lbp);

    cvNamedWindow(window_title2, CV_WINDOW_AUTOSIZE);
    cvShowImage(window_title2, img);
    cvWaitKey(0);
    cvDestroyAllWindows();

    // Dans le cas où l'image n'a pas pu être sauvegardée
    if (dst_path && !cvSaveImage(dst_path, img, NULL))
    {
        fprintf(stderr, "couldn't write image to file: %s\n", dst_path);
    }

    // Libère l'image
    cvReleaseImage(&img);

    // Libère les tableaux
    LibereMatFloat(pourcentage, 256);
    LibereMatFloat(pourcentageUniform, 59);
    LibereMat(matrice, sizex);
    LibereMat(lbp, sizex);

    
    return EXIT_SUCCESS;
}