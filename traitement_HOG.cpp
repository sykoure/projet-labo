#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <opencv/cv.h>
#include <time.h>
#include <math.h>
#include <opencv/highgui.h>

#define PI 3.14159265

//Nous utiliserons le filtre de Prewitt pour le moment
int GradiantFilter[3][3] = {
    {-1, 0, 1},
    {-1, 0, 1},
    {-1, 0, 1}};

int TGradiantFilter[3][3] = {
    {-1, -1, -1},
    {0, 0, 0},
    {1, 1, 1}};

//Structure du tableau contenant un angle et une direction
typedef struct tabpolaire tabpolaire;
struct tabpolaire
{
    int magnitude;
    int angle;
};

void LibereMat(int **mat, int taillex);
int **InitTab(int taillex, int tailley);
void AfficheMat(int **mat, int taillex, int tailley);
void RemplirMat(int **mat, int taillex, int tailley);
tabpolaire **InitTabPolaire(int taillex, int tailley);
tabpolaire **HOG(int **mat, int taillex, int tailley);
tabpolaire TraitementHOG(int **mat, int i, int j, int taillex, int tailley);
void AfficheMatGradiant(tabpolaire **tabpolaire, int taillex, int tailley);
void AfficheTab(float **mat, int taille);
float **InitTabFloat(int taillex, int tailley);
float **PourcentageTab(tabpolaire **hog, int taillex, int tailley);
uchar *cvPtr2D(const CvArr *arr, int idx0, int idx1, int *type);

/**
 * Donne l'image en noire et blanc à la matrice
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
void renvoyerImg(IplImage *img, tabpolaire **matrice)
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
            *p = (int)matrice[x][y].angle*0.71;
        }
    }
}

//Fonction permettant de faire le calcul de gradient
tabpolaire **HOG(int **mat, int taillex, int tailley)
{
    int i, j;
    tabpolaire **tabpolaire;
    tabpolaire = InitTabPolaire(taillex, tailley);

    for (i = 0; i < taillex; i++)
    {
        for (j = 0; j < tailley; j++)
        {
            if ((i == 0) || (j == 0) || (i == taillex - 1) || (j == tailley - 1))
            {
                tabpolaire[i][j].angle = 0;
                tabpolaire[i][j].magnitude = 0;
            }
            else
            {
                tabpolaire[i][j] = TraitementHOG(mat, i, j, taillex, tailley);
            }
        }
    }
    return tabpolaire;
}

//Fonction permettant de faire la partie calcule de la fonction HOG()
tabpolaire TraitementHOG(int **mat, int i, int j, int taillex, int tailley)
{
    int ligne, colonne, compteur_l, compteur_c;
    double val = 180.0 / PI;

    int gradientX = 0;
    int GradientY = 0;
    int magnitude = 0;
    double angle = 0;

    compteur_c = 0;
    compteur_l = 0;

    for (ligne = i - 1; ligne <= i + 1; ligne++)
    {
        for (colonne = j - 1; colonne <= j + 1; colonne++)
        {

            //application du filtre
            gradientX = gradientX + GradiantFilter[compteur_l][compteur_c] * mat[ligne][colonne];
            GradientY = GradientY + TGradiantFilter[compteur_l][compteur_c] * mat[ligne][colonne];

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

    magnitude = sqrt(gradientX * gradientX + GradientY * GradientY);
    angle = atan2(GradientY, gradientX) * val;
    angle = (angle < 0) ? angle + 360 : angle;
    tabpolaire tabpolaire;
    tabpolaire.magnitude = magnitude;
    tabpolaire.angle = (int)angle;

    printf("[(%f,%d)]\n", angle, magnitude);

    return tabpolaire;
}

//Fonction permettant de remplir une matrice de nombre aléatoire entre 0 et 255
void RemplirMat(int **mat, int taillex, int tailley)
{

    int i, j;
    for (i = 0; i < taillex; i++)
    {
        for (j = 0; j < tailley; j++)
        {
            mat[i][j] = rand() % 256;
        }
    }
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

//Fonction initialisant dynamiquement une matrice
int **InitTab(int taillex, int tailley)
{

    int i;
    int **matrice = NULL;

    //allocation d'un tableau de tableaux d'entiers
    matrice = (int**)malloc(taillex * sizeof(int *));

    for (i = 0; i < taillex; i++)
    {
        //allocation d'un tableau de tableau
        matrice[i] = (int*)malloc(tailley * sizeof(int ));
    }
    return matrice;
}

//Fonction initialisant dynamiquement une matrice contenant les valeurs des angles polaires
tabpolaire **InitTabPolaire(int taillex, int tailley)
{

    int i;
    tabpolaire **matrice = NULL;

    //allocation d'un tabpolaire
    matrice = (tabpolaire **)malloc(taillex * sizeof(tabpolaire *));

    for (i = 0; i < taillex; i++)
    {
        //allocation d'un tableau de tabpolaire
        matrice[i] = (tabpolaire *)malloc(tailley * sizeof(tabpolaire));
        matrice[i]->magnitude = 0;
        matrice[i]->angle = 0;
    }
    return matrice;
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

float **PourcentageTab(tabpolaire **hog, int taillex, int tailley)
{
    int i, j;
    int pacerelle = 0;
    float **pourcentage = NULL;
    pourcentage = InitTabFloat(360, 1);

    //On commence par remplir le tableau de 0
    for (i = 0; i < 360; i++)
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
            pacerelle = hog[i][j].angle;
            pourcentage[pacerelle][0]++;
        }
    }

    //On fait le pourcentage
    for (i = 0; i < 360; i++)
    {
        for (j = 0; j < 1; j++)
        {
            pourcentage[i][j] = (pourcentage[i][j] * 100) / (taillex * tailley);
        }
    }
    float somme = 0;
    for (i = 0; i < 360; i++)
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
void AfficheMatGradiant(tabpolaire **mat, int taillex, int tailley)
{

    int i, j;
    for (i = 0; i < taillex; i++)
    {
        printf("\n ligne : %d\n", i);
        for (j = 0; j < tailley; j++)
        {
            printf("[(%d,%d)]", mat[i][j].angle, mat[i][j].magnitude);
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

float **InitTabFloat(int taillex, int tailley)
{

    int i;
    float **matrice = NULL;

    //allocation d'un tableau de tableaux d'entiers
    matrice = (float**)malloc(taillex * sizeof(float *));

    for (i = 0; i < taillex; i++)
    {
        //allocation d'un tableau de tableau
        matrice[i] = (float*)malloc(tailley * sizeof(float ));
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
    IplImage *img = NULL;
    const char *src_path = NULL;
    const char *dst_path = NULL;
    const char *window_title = "Image de base";
    const char *window_title2 = "Image traitée";

    int sizex, sizey;

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
        fprintf(stderr, "couldn't open image file in Grayscale format: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    sizey = img->height;
    sizex = img->width;

    cvNamedWindow(window_title, CV_WINDOW_AUTOSIZE);
    cvShowImage(window_title, img);

    // Partie traitement
    int **matrice = NULL;
    tabpolaire **hog = NULL;
    float **pourcentage = NULL;

	hog = InitTabPolaire(sizex,sizey);
	matrice = InitTab(sizex,sizey);
    pourcentage = InitTabFloat(360,1);

    remplir(img, matrice);
    //AfficheMat(matrice,size,size);

	AfficheMat(matrice,sizex,sizey);

	hog = HOG(matrice,sizex,sizey);
    AfficheMatGradiant(hog,sizex,sizey);
    pourcentage = PourcentageTab(hog,sizex,sizey);
    
    renvoyerImg(img, hog);
    
    cvNamedWindow(window_title2, CV_WINDOW_AUTOSIZE);
    cvShowImage(window_title2, img);
    cvWaitKey(0);
    cvDestroyAllWindows();

    if (dst_path && !cvSaveImage(dst_path, img, NULL))
    {
        fprintf(stderr, "couldn't write image to file: %s\n", dst_path);
    }

    // Libère l'image
    cvReleaseImage(&img);


    return EXIT_SUCCESS;
}