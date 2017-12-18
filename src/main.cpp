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

#include "fonctions.h"

// no idea why on the virtual box the keycode is completely wrong
#define ESC_KEY 27  // virtual : 1048603 // should be 27
#define Q_KEY 	113 // virtual : 1048689 // should be 113
#define O_KEY	111
#define N_KEY	110

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
Mat masque; // masque de la route
int M = 200; // Nombre d'image pour le calcul de la moyenne

int key = 0;


int main(int argc, const char * argv[]) {

	namedWindow("Original video");
   
	if(argc > 1) {
		videoName = argv[1];
		cout <<"video name: "<<videoName<<endl;
	}
	else {
		videoName = "video.avi";
	}

	// Reading the image (and forcing it to grayscale)	   
	cap.open(videoName);
	fps = cap.get(CV_CAP_PROP_FPS); // recherche de l'information du nobmre d'images par secondes
	nbFrames = cap.get(CV_CAP_PROP_FRAME_COUNT); // recherche de l'information du nombre d'images totales
	float Time = nbFrames / fps; // calcul de la durée de la vidéo en secondes
	int Hauteur = cap.get(CV_CAP_PROP_FRAME_HEIGHT); // nombre de pixel de hauteur de la vidéo
	int Largeur = cap.get(CV_CAP_PROP_FRAME_WIDTH); // nombre de pixel de largeur de la vidéo

	// Affichage des résultats dans la console
	
	cout << "n :" << fps << endl;
	cout << "N :" << nbFrames << endl;
	cout << "t :" << Time << endl;
	cout << "H :" << Hauteur << endl;
	cout << "L :" << Largeur << endl;
	//cout << "Format :" << Format << endl;

   // Making sure the capture has opened successfully
	if(!cap.isOpened()){
        // capture opening has failed we cannot do anything :'(
		cout << "Erreur au chargement de la vidéo." << endl;
		return EXIT_FAILURE ;
	}
	
////////////////////// I/O //////////////////////////
	cout << endl << "Charger une image moyenne deja calculee ? (O/N)" << endl;
	key = waitKey();
	if (key == O_KEY) {
		cout << "Indiquer le chemin et le nom de l'image :" << endl;
		string chemin;
		cin >> chemin;
		immoy = imread(chemin, CV_LOAD_IMAGE_GRAYSCALE);
		namedWindow("Image Moyenne");
		imshow("Image Moyenne", immoy);
	}


	cout << endl << "Charger un masque deja calcule ? (O/N)" << endl;
	key = waitKey();
	if (key == O_KEY) {
		cout << "Indiquer le chemin et le nom de l'image :" << endl;
		string chemin;
		cin >> chemin;
		immoy = imread(chemin, CV_LOAD_IMAGE_GRAYSCALE);
		namedWindow("masque");
		imshow("masque", masque);
	}


////////////////// ENREGISTREMENT DES IMAGES DANS LES VECTEURS ////////////////////   

	cout << endl << "Calcul de l'image moyenne..." << endl ;

	//Boucle de lecture et enregistrement de la vidéo et calcul de l'image moyenne 
   int Count = 0; // compteur d'images
   while ( (key != ESC_KEY) && (key!= Q_KEY) && (Count < nbFrames) ) {

      cap.read(im);
	  Images.push_back(im.clone()); // enregistrement des images dans un vecteur
	  
      // Turning im into grayscale and storing it in imGray
	  cvtColor(im, imGray, CV_BGR2GRAY); // passage en nuances de Gris
	  ImagesG.push_back(imGray.clone()); // enregistrement du vecteurs d'images en nuances de gris

	  if (Count == M && immoy.empty()) { // on ne calcule l'image moyenne qu'une fois le nombre d'image nécessaire atteint ... merci Captain O
		  immoy = ImageMoyenne(M, ImagesG, Hauteur, Largeur);
		  namedWindow("Image Moyenne");
		  imshow("Image Moyenne", immoy);
	  }
	Count += 1;
	}


   if ((key == ESC_KEY) || (key == Q_KEY)) {
	   cout << "Processus interrompu";
	   return EXIT_SUCCESS;
   }


   /////////////////////////////////// PROCESS //////////////////////////////////////////////
   if (masque.empty()) {
	   cout << "calcul du masque en cours ..." << endl;
	   masque = mask_route(M, ImagesG, immoy, Hauteur, Largeur);
	   cout << "calcul du masque terminé ..." << endl;

	   namedWindow("masque");
	   imshow("masque", masque);
   }

   cout << endl << "Appliquer un traitement au masque ? (O/N)" << endl;
   key = waitKey(0);

   if (key == O_KEY) {
	   cout << endl << "Traitement du masque..." << endl;
	   morpho(masque, MORPH_CLOSE, 7);
	   morpho(masque, MORPH_OPEN, 9);
	   cout << endl << "Traitement termine." << endl;
	   imshow("masque", masque);
   }


   




   ///////////////////////////////////// PLAY VIDEO //////////////////////////////////////////

   // Creating a window to display some images
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

   /////////////   I/O   /////////////

	cout << endl << "Enregistrer l'image moyenne calculée ? (O/N)" << endl;
	key = waitKey();
	if (key == O_KEY) {
		cout << "Indiquer le chemin et le nom de l'image :" << endl;
		string chemin;
		cin >> chemin;
		imwrite(chemin, immoy);
	}

	cout << endl << "Enregistrer le masque calculé ? (O/N)" << endl;
	key = waitKey();
	if (key == O_KEY) {
		cout << "Indiquer le chemin et le nom de l'image :" << endl;
		string chemin;
		cin >> chemin;
		imwrite(chemin, masque);
	}
   
	cout << endl << "Appuyer sur une touche pour terminer l'application..." << endl;
	waitKey();



	// Destroying all OpenCV windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}
