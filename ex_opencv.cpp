#include <stdio.h> // Pour le printf()...

/* Les deux bibliothèques nécessaires d'opencv :
	- cv.h contient les structures et fonctions de manipulation d'images
	- highgui.h contient les fonctions d'affichage des images
*/
#include <cv.h>
#include <highgui.h>

//size image 141*141

int main(int argc, char *argv[])
{
    //Pointeur vers une image
    IplImage* img;
   
    //On récupère l'image
    img = cvLoadImage("./img/cercle.png");

    //On crée une fenêtre intitulée "Hello World",
    //La taille de cette fenêtre s'adapte à ce qu'elle contient.
    cvNamedWindow("Cercle", CV_WINDOW_AUTOSIZE);

    //On affiche l'image dans la fenetre
    cvShowImage("Cercle",img);

    //On attend que l'utilisateur appuie sur une touche
    cvWaitKey(0);

    //Destruction de la fenetre
    cvDestroyWindow("Cercle");

    //Libération de l'image
    cvReleaseImage(&img);

    return 0;
}   