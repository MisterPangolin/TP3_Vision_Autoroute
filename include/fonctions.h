#pragma once

#include <opencv/cv.hpp>
#include <Vector>

using namespace std;
using namespace cv;

////////// DECLARATION DES FONCTIONS /////////

Mat ImageMoyenne(int M, vector<Mat> Video, int hauteur, int largeur);
// Renvoie l'image moyenne des M premières images d'une video Video (sous forme de vecteur de Mat), de dimensions hauteur x largeur.

Mat mask_route(int M, vector<Mat> Video, Mat imgMoy, int hauteur, int largeur);
// Renvoie le mask