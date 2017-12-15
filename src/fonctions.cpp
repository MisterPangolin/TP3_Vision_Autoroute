#include "fonctions.h"

//////////////// DEFINITION DES FONCTIONS ////////////////

Mat ImageMoyenne(int M, vector<Mat> Video, int hauteur, int largeur) {
	Mat ImageMoy = Mat::zeros(hauteur, largeur, CV_8UC1); // Création de l'image que l'on renverra
	int GreyValue; // Création de la valeur de niveau de Gris
	for (int i = 0; i < hauteur; i++) { // parcours des pixels en hauteur
		for (int j = 0; j < largeur; j++) { // parcour des pixels en largeur
			GreyValue = 0; // réinitialisation de la valeur de niveau de gris
			for (int k = 0; k < M; k++) { // parcour de chaque image
				GreyValue += Video[k].at<uchar>(i, j); // somme des valeurs de niveau de gris de chaques image
			}
			GreyValue = floor((float)GreyValue / M); // calcul de la moyenne
			ImageMoy.at<uchar>(i, j) = GreyValue; // enregistrement de la valeur dans l'image avant le passage au pixel suivant
		}
	}
	return ImageMoy;
}

bool extraction(int M, vector<Mat> &Video, Mat mask) {
	
	if (mask.size != Video[0].size) {
		return false;
	}

	for (int i = 0; i < M; i++) {
		Video[i].copyTo(Video[i], mask);
	}

	return true;
}

bool extraction(vector<Mat> &Video, Mat mask) {
	return extraction(Video.size, Video, mask);
}
