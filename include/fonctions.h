#pragma once

#include <opencv/cv.hpp>
#include <Vector>

// no idea why on the virtual box the keycode is completely wrong
#define ESC_KEY 27  // virtual : 1048603 // should be 27
#define Q_KEY 	113 // virtual : 1048689 // should be 113
#define O_KEY	111
#define N_KEY	110

using namespace std;
using namespace cv;

////////// DECLARATION DES FONCTIONS /////////

Mat ImageMoyenne(int M, vector<Mat> Video, int hauteur, int largeur);
// Renvoie l'image moyenne des M premi�res images d'une video Video (sous forme de vecteur de Mat), de dimensions hauteur x largeur.

Mat mask_route(int M, vector<Mat> Video, Mat imgMoy, int hauteur, int largeur);
// Renvoie le mask


bool extraction(int M, vector<Mat> &Video, Mat mask);
// Cette fonction modifie la Video pass�e en param�tre pour extraire la partie non masqu�e par le mask.
// Renvoie true si tout se passe bien, false sinon (par exemple si le mask et la video ne sont pas de m�me dimension)
// M est le nombre d'images sur lequel le mask aura �t� appliqu� (en partant de la premi�re image)

bool extraction(vector<Mat> &Video, Mat mask);
// Surcharge de la fonction extraction :
// Sert � appliquer le mask sur toutes les images de
// la video (M = size(Video) )

void morpho(Mat img, int morphoType, int element_size);
// Applique une morphologie au mask

void playVideo(int fps, vector<Mat> Video, vector<Mat> VideoGray);
// Lit la vid�o Video � un taux de fps images par secondes,
// et lit la vid�o VideoGray si elle est fournie