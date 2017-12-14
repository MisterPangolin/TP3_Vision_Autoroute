//
//  main.cpp
//  VSION-TP1
//
//  Created by Jean-Marie Normand on 04/02/2016.
//  Copyright © 2016 Jean-Marie Normand. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <opencv/cv.hpp>
#include <Vector>

// no idea why on the virtual box the keycode is completely wrong
#define ESC_KEY 27  // virtual : 1048603 // should be 27
#define Q_KEY 	113 // virtual : 1048689 // should be 113

using namespace std;
using namespace cv;

string videoName;
VideoCapture cap;
int nbFrames;
double fps = 30;
bool videoRead = false;
Mat im;
Mat imGray;
vector<Mat> Images;
vector<Mat> ImagesG;
Mat immoy; // Image moyenne en niveau de gris
int M = 200; // Nombre d'image pour le calcul de la moyenne

Mat ImageMoyenne(int M, vector<Mat> Video, int hauteur, int largeur ) {
	Mat ImageMoy = Mat::zeros(hauteur, largeur, CV_8UC1); // Création de l'image que l'on renverra
	int GreyValue; // Création de la valeur de niveau de Gris
	for (int i = 0; i < hauteur; i++) { // parcours des pixels en hauteur
		for (int j = 0; j < largeur; j++) { // parcour des pixels en largeur
			GreyValue = 0; // réinitialisation de la valeur de niveau de gris
			for (int k = 0; k < M; k++) { // parcour de chaque image
				GreyValue += Video[k].at<uchar>(i, j); // somme des valeurs de niveau de gris de chaques image
			}
			GreyValue = floor((float)GreyValue /  M); // calcul de la moyenne
			ImageMoy.at<uchar>(i, j) = GreyValue; // enregistrement de la valeur dans l'image avant le passage au pixel suivant
		}
	}
	return ImageMoy;
}

int main(int argc, const char * argv[]) {
   
	if(argc > 1) {
		videoName = argv[1];
		cout <<"video name: "<<videoName<<endl;
	}
	else {
		videoName = "video.avi.wm";
	}


	// Reading the image (and forcing it to grayscale)	   
	cap.open(videoName);
	fps = cap.get(CV_CAP_PROP_FPS); // recherche de l'information du nobmre d'images par secondes
	nbFrames = cap.get(CV_CAP_PROP_FRAME_COUNT); // recherche de l'information du nombre d'images totales
	float Time = nbFrames / fps; // calcul de la durée de la vidéo en secondes
	int Hauteur = cap.get(CV_CAP_PROP_FRAME_HEIGHT); // nombre de pixel de hauteur de la vidéo
	int Largeur = cap.get(CV_CAP_PROP_FRAME_WIDTH); // nombre de pixel de largeur de la vidéo
	//int Format = cap.get(CV_CAP_PROP_FORMAT); // Format RGB ou BGR ?


	// Affichage des résultats dans la console
	
	cout << "n :" << fps << endl;
	cout << "N :" << nbFrames << endl;
	cout << "t :" << Time << endl;
	cout << "H :" << Hauteur << endl;
	cout << "L :" << Largeur << endl;
	//cout << "Format :" << Format << endl;

   // Making sure the capture has opened successfully
	if(cap.isOpened()){
      cap.read(im);
	  Images.push_back(im.clone()); // enregistrement de chaque image dans un vesteur 

      // Turning im into grayscale and storing it in imGray
	  cvtColor(im, imGray, CV_BGR2GRAY); // passage en nuance de gris
	  ImagesG.push_back(imGray.clone()); // enregistrement des images en nuances de gris dans un autre vecteur
   }
	else {
      // capture opening has failed we cannot do anything :'(

		return 1;
	}
	

////////////////// ENREGISTREMENT DES IMAGES DANS LES VECTEURS ////////////////////   

	cout << endl << "Calcul de l'image moyenne..." << endl ;

	//Boucle de calcul 
   int key = 0;
   int Count = 0; // compteur d'images
   while ( (key != ESC_KEY) && (key!= Q_KEY) && (Count < nbFrames) ) {
      cap.read(im);
	  Images.push_back(im.clone()); // enregistrement des images dans un vecteur


      // Turning im into grayscale and storing it in imGray
	  cvtColor(im, imGray, CV_BGR2GRAY); // passage en nuances de Gris
	  ImagesG.push_back(imGray.clone()); // enregistrement du vecteurs d'images en nuances de gris
	  //cout << "Nombre de frames dans le vector ImagesG : " << ImagesG.size() << endl; // Debogage
      // Look for waitKey documentation
	 

	  if (Count == M) { // on ne calcul l'image moyenne qu'une fois le nombre d'image nécessaire atteint ... merci Captain O
		  immoy = ImageMoyenne(M, ImagesG, Hauteur, Largeur);
		  namedWindow("Image Moyenne");
		  imshow("Image Moyenne", immoy);
	  }
	Count += 1;
	}


   if ((key == ESC_KEY) && (key == Q_KEY)) {
	   cout << "Processus interrompu";
	   return EXIT_SUCCESS;
   }


   /////////////////////////////////// PROCESS //////////////////////////////////////////////











   ///////////////////////////////////// PLAY VIDEO //////////////////////////////////////////

   // Creating a window to display some images
   namedWindow("Original video");
   namedWindow("Gray video");

   Count = 0;
   // Waiting for the user to press ESCAPE before exiting the application	
   while ((key != ESC_KEY) && (key != Q_KEY) && (Count < nbFrames) ) {
	
	   imshow("Original video", Images[Count]);
	   imshow("Gray video", ImagesG[Count]);

	   key = waitKey( 1000/fps ); // video is XXfps
	   //key = waitKey(-3);
	   Count += 1;
   }
   

   // Destroying all OpenCV windows
	destroyAllWindows();
   
	return 0;
}
