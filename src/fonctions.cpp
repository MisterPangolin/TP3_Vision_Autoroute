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


Mat mask_route(int M, vector<Mat> Video, Mat imgMoy, int hauteur, int largeur) {
	Mat mask = Mat(hauteur, largeur, CV_8UC1);
	for (int i = 0; i < hauteur; i++) {// parcours des pixels en hauteur
		for (int j = 0; j < largeur; j++) {// parcour des pixels en largeur
			bool Noir = 0;// booléen servant a définir si un pixel appartiens au décor ou non  
			for (int k = 0; k < M; k++) {// parcours de chaques images
				int max = imgMoy.at<uchar>(i, j) + 10; // variation minimale que l'on tolère pour définir la route
				int min = imgMoy.at<uchar>(i, j) - 10; // variation maxmale que l'on tolère pour définir la route
				int valeur = Video[k].at<uchar>(i, j); // valeur de nuance de gris de la photo
				if (valeur < max && valeur > min) {

					Noir = 1;
				}
				if (Noir == 1) {
					mask.at<uchar>(i, j) = 0;
				}
				else {
					mask.at<uchar>(i, j) = 255;
				}
				Noir = 0;
			}
		}
	}
	return mask;
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
	return extraction(Video.size(), Video, mask);
}