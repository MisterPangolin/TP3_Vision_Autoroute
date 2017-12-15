#pragma once

#include <opencv/cv.hpp>
#include <Vector>

using namespace std;
using namespace cv;

////////// DECLARATION DES FONCTIONS /////////

Mat ImageMoyenne(int M, vector<Mat> Video, int hauteur, int largeur);
// Renvoie l'image moyenne des M premières images d'une video Video (sous forme de vecteur de Mat), de dimensions hauteur x largeur.

Mat mask_route(vector<Mat> Video, Mat imgMoy);
// Renvoie le mask

bool extraction(int M, vector<Mat> &Video, Mat mask);
// Cette fonction modifie la Video passée en paramètre pour extraire la partie non masquée par le mask.
// Renvoie true si tout se passe bien, false sinon (par exemple si le mask et la video ne sont pas de même dimension)
// M est le nombre d'images sur lequel le mask aura été appliqué (en partant de la première image)

bool extraction(vector<Mat> &Video, Mat mask);
// Surcharge de la fonction extraction :
// Sert à appliquer le mask sur toutes les images de
// la video (M = size(Video) )