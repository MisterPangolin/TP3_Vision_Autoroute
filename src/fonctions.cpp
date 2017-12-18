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
	int compteur_route; // Ce compteur sera incrémenté lors du parcours d'images à chaque fois que le niveau du pixel a varié au delà de la plage définie
	const int val_max_route = M / 10; // Quand compteur_route atteint val_max_route, cela confirme que le pixel appartient à la route.

	for (int i = 0; i < hauteur; i++) {// parcours des pixels en hauteur
		for (int j = 0; j < largeur; j++) {// parcours des pixels en largeur
			compteur_route = 0;
			
			int k = 0;
			while (k < M && compteur_route < val_max_route){// parcours des images
				int max = imgMoy.at<uchar>(i, j) + 50; // variation minimale que l'on tolère pour définir la route
				int min = imgMoy.at<uchar>(i, j) - 50; // variation maxmale que l'on tolère pour définir la route
				int valeur = Video[k].at<uchar>(i, j); // valeur de nuance de gris de la photo
				if (valeur > max || valeur < min) { // si la valeur dépasse la variation définie aux lignes précédentes :

					compteur_route++; // on augmente le compteur
				}
				k++;
			}
			if (compteur_route < val_max_route) { // si le compteur n'a pas atteint la valeur définie :
				mask.at<uchar>(i, j) = 0; // alors le pixel appartient au décor...
			}
			else {
				mask.at<uchar>(i, j) = 255; // ... sinon c'est que le pixel appartient à la route.
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
		Mat zero = Mat::zeros(Video[i].size(), Video[i].type());
		zero.copyTo(Video[i], mask);
		Video[i] = zero;
	}

	return true;
}

bool extraction(vector<Mat> &Video, Mat mask) {
	return extraction(Video.size(), Video, mask);
}

void morpho(Mat img, int morphoType, int element_size){
	Mat kernel = getStructuringElement(MORPH_CROSS, Size(element_size, element_size));
	morphologyEx(img, img, morphoType, kernel);
}


void playVideo(int fps, vector<Mat> Video, vector<Mat> VideoGray = vector<Mat>()) {
	// Creating a window to display some images
	namedWindow("Original video");
	if (!VideoGray.empty()) {
		namedWindow("Gray video");
	}
	

	int Count = 0;
	int key = 0;
	int nbFrames = Video.size();
	// Waiting for the user to press ESCAPE before exiting the application	
	while ((key != ESC_KEY) && (key != Q_KEY) && (Count < nbFrames)) {

		imshow("Original video", Video[Count]);
		if (!VideoGray.empty()) {
			imshow("Gray video", VideoGray[Count]);
		}

		key = waitKey(1000 / fps); // video is XXfps
								   //key = waitKey(-3);
		Count += 1;
	}
}