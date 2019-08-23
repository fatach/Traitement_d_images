//@@@@@@@@_______MODULE DE:TRAITEMEMT D'IMAGE______@@@@@@@@@@//

//____Promotion 23__________Master I______Annee:2019_________//

//_____________Student:Abdoul-Fatao OUEDRAOGO_______________//
 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
// Fonction de calcul du profil d'intensité
Mat printimg(Mat img, int vh, int valign)
{
    Point M, N;
    int lfc;
    Vec3b valpxdb;
    Vec3b valpxfn;
	// Profil d'intensité dans le cas d'une ligne horizontale
	if(vh==1)
	{
		lfc=img.cols;
	}
	else if(vh==2)
	{
		lfc=img.rows;
	}
		// Image du profil
    Mat crb_print(256,lfc,CV_8UC3,Scalar(0,0,0));
    for(int i=0;i< lfc-1;i++)
	{    //Recuperation des valeurs du pixel
	  if(vh==1)
	     {
                valpxdb=img.at<Vec3b>(valign, i);
                valpxfn=img.at<Vec3b>(valign,i+1);
	      }
	   else if(vh==2)
	      {
                valpxdb=img.at<Vec3b>(i, valign);
                valpxfn=img.at<Vec3b>(i+1,valign);
	      }

	  for(int j=0;j<3;j++)
	     {
		M.x=i;
		N.x=i+1;
		M.y=255-valpxdb.val[j];
		N.y= 255-valpxfn.val[j];

		// Profil de la ligne correspondant à la couleur Bleue
		if(j==0) line(crb_print,M,N,Scalar(255, 0, 0), 1, 8);

		// Profil de la ligne correspondant à la couleur Rouge
		if(j==1) line(crb_print,M,N, Scalar(0, 255, 0), 1, 8);

		// Profil de la ligne correspondant à la couleur Verte
		if(j==2) line(crb_print,M,N, Scalar(0, 0, 255), 1, 8);
	      }
	}

   return crb_print;
 }

// Fonction pour la détermination de l'histogramme d'une image
Mat histogrammeimg(Mat img)
{
	//Séparation de l'image source dans ses trois plans Bleu, Vert et Rouge
	vector<Mat> plnBVR; // Récuperation des cannaux des couleurs Bleu, vert et rouge
	split(img, plnBVR); //Division de l'image dans chaque canal de couleur

	//Etablissement du nombre de bacs
	int tailhisto=256;// de 0 à 255

    	//Définition de la plage de valeurs (0 à 255)
	float intrvl[]={0,256} ;//La limite supérieure (256) est exclusive
	const float* intrvlhisto={intrvl};

	//Création des matrices pour sauvegarder les histogrammes pour chaque niveau de couleur
	Mat histoB,histoV,histoR;

	//Uniformisation de la taille des bacs et effaçage de l'histogramme au début
	bool uni=true;
	bool acc=false;

	//Calcul de l'histogramme pour chaque niveau de couleur avec la fonction calcHist
	calcHist(&plnBVR[0],1,0,Mat(),histoB,1,&tailhisto,&intrvlhisto,uni,acc);
	calcHist(&plnBVR[1],1,0,Mat(),histoV,1,&tailhisto,&intrvlhisto,uni,acc);
	calcHist(&plnBVR[2],1,0,Mat(),histoR,1,&tailhisto,&intrvlhisto,uni,acc);
	
	//Dessin de l'histogramme pour cahque cannal de couleur
	int lrgrhisto = 512;//Définition de la largeur de l'histogramme
	int htrhisto = 400;//Définition de la hauteur de l'histogramme
	int bin_w = cvRound((double)lrgrhisto/tailhisto);
	int p=2; // Pas pour l'incrémentation
	Mat crbhistoimg(htrhisto, lrgrhisto, CV_8UC3, Scalar( 0,0,0));
	
	//Normalisation pour chaque niveau de couleur avec la fonction normalize
	normalize(histoB,histoB,0,crbhistoimg.rows, NORM_MINMAX, -1, Mat());
	normalize(histoV,histoV, 0, crbhistoimg.rows, NORM_MINMAX, -1, Mat());
	normalize(histoR,histoR, 0, crbhistoimg.rows,NORM_MINMAX,-1,Mat());

	
	for(int i =1;i<tailhisto; i++)
  	{
      		line(crbhistoimg,Point(bin_w*(i-1),htrhisto-cvRound(histoB.at<float>(i-1))),
      		Point(bin_w*(i),htrhisto-cvRound(histoB.at<float>(i))),Scalar(255,0,0),2,8,0);
      		line( crbhistoimg, Point(bin_w*(i-1),htrhisto-cvRound(histoV.at<float>(i-1))),
                Point(bin_w*(i), htrhisto-cvRound(histoV.at<float>(i))),
                Scalar(0,255, 0),2, 8, 0  );
      		line( crbhistoimg,Point( bin_w*(i-1),htrhisto-cvRound(histoR.at<float>(i-1))),
                Point( bin_w*(i), htrhisto-cvRound(histoR.at<float>(i))),
                Scalar(0,0,255),2,8,0);
         }
        return crbhistoimg;
}

void tracerligne(Mat image, int choix, int valign)
{
	//Déclaration de deux points pour permettre de tracer la droite passant par les deux points
	Point point1, point2;
	point1.x = 0;
	point1.y = valign;
	point2.x = image.cols;
	point2.y = valign;
       	//Tracé de la ligne sur l'image
	line(image, point1, point2, Scalar(0, 0, 255), 1, 8);

	//Affichage du résultat
	imshow("Image originale avec ligne tracee",image);

	// Enregistrement dans un fichier image
	if(!imwrite("TraceeDeLaLigne.png",image))
	cout<<"Erreur lors de l'enregistrement de l'image"<<endl;
}
//Courbe du profil d'intensité de la ligne 
void printime(Mat image, int choix, int valign)
{	//Représentation des points en 2D
	Point P1,P2;
    	//lfc permet de déterminer la largeur de la fenetre de la courbe du profil d'intensité
	int lfc;

	//La Structuration des pixels au début
   	Vec3b ValeurPicxelDebut;

	//LaStructuration des pixels à la fin 
   	Vec3b ValeurPicxelFin; 

	lfc=image.cols;// On prend toute la colonne de l'image
	
	// Image de la courbe du profil d'intensité de la ligne tracée
	Mat crb_print (256,lfc,CV_8UC3,Scalar(0,0,0));

	for(int p=0;p< lfc-1;p++)
	 {    
	   //Recupération des valeurs du pixel
           ValeurPicxelDebut=image.at<Vec3b>(valign, p);
           ValeurPicxelFin=image.at<Vec3b>(valign,p+1);
           for(int q=0; q<3;q++)
	     {
	       P1.x = p;
	       P2.x = p+1;
	       P1.y = 255 - ValeurPicxelDebut.val[q];
	       P2.y = 255 - ValeurPicxelFin.val[q];

               //Profil de la ligne pour la couleur Bleue
		if(q==0) line(crb_print,P1,P2,Scalar(255,0,0),1,8);

	       //Profil de la ligne pour la couleur Rouge
		if(q==1) line(crb_print,P1,P2,Scalar(0,255,0),1,8);

		//Profil de la ligne pour la couleur Verte
		if(q==2) line(crb_print,P1,P2,Scalar(0,0,255),1,8);
		}
	  }
	//Pour l'affichage automatique de l'image de la courbe
	imshow("Courbe du profil d'Intensite",crb_print);
	//Enregistrement del'image .png pour compresser sans modification des valeurs des pixel
	if(!imwrite("CourbeProfilIntensite.png",crb_print))
        cout<<"Erreur lors de la sauvegarde du fichier\n"<<endl;
}

//Fonction pour le traçage du trait du profil d'intensité sur l'image
Mat tracelign(Mat img, int vh, int valign)
{
	Point debut, fin;

	if (vh==1)
	{
		debut.x = 0;
		debut.y = valign;
		fin.x = img.cols;
		fin.y = valign;
	}
	else if(vh==2)
	{
		debut.x = valign;
		debut.y = 0;
		fin.x = valign;
		fin.y = img.rows;
	}

	//Tracé de la ligne sur l'image
	line(img,debut,fin,Scalar(0, 0, 255),1,8);

    return img;
}

// Fonction linéaire à trois points de transformation du contraste
Mat fctline3pts(Mat img,Point pt1, Point pt2, Point pt3)
{
    Mat img3pts(img.rows, img.cols, CV_8UC3, Scalar( 0,0,0));
    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            for(int c = 0; c < 3; c++)
            {
		int val = img.at<Vec3b>(i,j)[c];
		if(val>=0 && val <= pt1.x) val = (int) pt1.y/pt1.x * val;
    		else if(pt1.x < val && val <= pt2.x) val =(int) ((pt2.y - pt1.y)/(pt2.x - pt1.x)) * (val - pt1.x) + pt1.y;
    		else if(pt2.x < val && val <= pt3.x) val =(int) ((pt3.y - pt2.y)/(pt3.x - pt2.x)) * (val - pt2.x) + pt2.y;
    		else if(pt3.x < val && val <= 255) val =(int) ((255 - pt3.y)/(255 - pt3.x)) * (val - pt3.x) + pt3.y;
		img3pts.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(val);
            }
        }
    }

    //Image contrastee par la fonction linéaire à trois points
    imshow("Image contrastee par fl3pts",img3pts);
    if(!imwrite("Image contrastee par fl3pts.png",img3pts))
    cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(5000);//Affichage de l'image pendant 5s
    destroyAllWindows();//Fermeture de la fenêtre 

    return img3pts;
}

// Procedure pour représenter la courbe de la fonction linéaire à trois points
void crbfctline3pts(Point pt1, Point pt2, Point pt3){

	// Declaration de l'image de la courbe
	Mat crb3pts(256,256,CV_8UC3,Scalar(0,0,0));
	// Déclaration et initialisation des points de début et de fin
	Point debut, fin;
	debut.x = 0 ;
	debut.y = 255 ;
	fin.x = 255;
	fin.y = 0;
	pt1.y = 255 - pt1.y;
	pt2.y = 255 - pt2.y;
	pt3.y = 255 - pt3.y;

	// Tracé des lignes de la courbe
	line(crb3pts, debut, pt1, Scalar(255, 0, 0), 1, 8);
	line(crb3pts, pt1, pt2, Scalar(255, 0, 0), 1, 8);
	line(crb3pts, pt2, pt3, Scalar(255, 0, 0), 1, 8);
	line(crb3pts, pt3, fin, Scalar(255, 0, 0), 1, 8);

	// Affichage de la courbe de la fonction
        imshow("Fonction lineaire a trois points",crb3pts);

       // Enregistrement de l'histogramme orginal dans un fichier image
       if(!imwrite("FonctionLineaire3pts.png", crb3pts))
     cout<<"Enregistrement echoué"<<endl;
}

// Fonction non linéaire pour modifier le contraste suivant la correction de gamma
Mat corgam(Mat img,float g)
{
    Mat imgcorgam(img.rows, img.cols, CV_8UC3, Scalar( 0,0,0));
    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j< img.cols;j++)
        {
            for(int c=0;c<3;c++)
            {
                int val = img.at<Vec3b>(i,j)[c];
                if(val < 0) val = 0;
                else if(val > 255) val =255;
                else if(0 <= val && val <= 255)
                val = pow((val/255.0),g)*255.0;
               imgcorgam.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(val);
            }
        }
    }

   //Image contrastee par gamma
    imshow("Image contrastee par gamma",imgcorgam);
    if(!imwrite("ImageContrasteeParGamma.png",imgcorgam))
    cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(5000);//Affichage de l'image pendant 5s
    destroyAllWindows();//Fermeture de la fenêtre

   return imgcorgam;
}

// Procedure pour la représentation de la courbe de gamma
void crbcorgam(float g){

	Mat imgcrbgam(256, 256, CV_8UC3, Scalar( 0,0,0));
	Point pt1, pt2;
	for (float i =0.0;i<256.0;i++)
	{
	  pt1.x = i;
	  pt2.x = i+1;
	  pt1.y = 255.0 - (pow((pt1.x/255.0),g)*255.0);
	  pt2.y = 255.0 - (pow((pt2.x/255.0),g)*255.0);
	  if(pt1.y < 0.0)  pt1.y = 0.0;
	  if(pt1.y> 255.0) pt1.y = 255.0;
	  if(pt2.y < 0.0)  pt2.y = 0.0;
	  if(pt2.y> 255.0) pt2.y = 255.0;
	  line(imgcrbgam,pt1,pt2, Scalar(255,0,0),1,8);
	}
	 // Affichage et enregistrement de la courbe de la fonction
        imshow("Fonction gamma",imgcrbgam);
        if(!imwrite("FonctionGamma.png",imgcrbgam))
        cout<<"Erreur lors de l'enregistrement"<<endl;
        waitKey(5000);//Affichage de l'image pendant 5s
        destroyAllWindows();//Fermeture de la fenêtre 
}

// Fonction linéaire avec saturation (min et max)
Mat fctlinsat(Mat img, Point Min, Point Max)
{
    Mat imgsat(img.rows, img.cols, CV_8UC3, Scalar( 0,0,0));
    for(int i = 0; i < img.rows; i++)
     {
        for(int j = 0; j < img.cols; j++)
        {
            for(int c = 0; c < 3; c++)
            {
		int val = img.at<Vec3b>(i,j)[c];
		if(val>=0 && val <= Min.x) val = 0;
    		else if(Min.x < val && val <= Max.x)
                val =(int) (255/(Max.x - Min.x)) * (val - Min.x);
    		else if(Max.x < val && val <= 255) val = 255;
                imgsat.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(val);
            }
        }
    }

    //Image contrastee par la saturation
    imshow("Image contrastee par saturation",imgsat);
    if(!imwrite("Image contrastee par saturation.png",imgsat))
    cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(5000);//Affichage de l'image pendant 5s
    destroyAllWindows();
    return imgsat;
}

// Procedure pour la représentation de la fonction linéaire de saturation
void crbfctlinsat(Point Min, Point Max)
{
        // Declaration de l'image de la courbe
	Mat crbfctlinsat(256,256,CV_8UC3,Scalar(0,0,0));
   	// Déclaration et initialisation des points de début et de fin
	Point debut, fin;
	debut.x = 0 ;
	debut.y = 255 ;
	fin.x = 255;
	fin.y = 0;
	Min.y = 255 - Min.y;
	Max.y = 255 - Max.y;

	// Tracé des lignes de la courbe
	line(crbfctlinsat, debut, Min, Scalar(255, 0, 0), 1, 8);
	line(crbfctlinsat, Min, Max, Scalar(255, 0, 0), 1, 8);
	line(crbfctlinsat, Max, fin, Scalar(255, 0, 0), 1, 8);

	// Affichage de la courbe de la fonction
    	imshow( "Fonction lineaire avec saturation",crbfctlinsat);

    	// Enregistrement de l'histogramme orginal dans un fichier image
	if(!imwrite("fonction_saturation.png", crbfctlinsat))
	cout<<"Erreur lors de l'enregistrement"<<endl;
	waitKey(5000);//Affichage de l'image pendant 5s
	 destroyAllWindows();
}

// **********************************PROGRAMME PRINCIPAL**********************

int main(int argc, char** argv)
{
//Declaration des variables globales
	int choix=0;
	int choixh=0;
	char nomimg[100];
	Mat imgorig;
	Mat imaamlr;
	Mat courbe;
	int vh;
	int valign;
	int choixp=0;
	Mat img_org; //Déclaration de la variable pour determiner la matrice de l'image originale
	int test=0;
	if(argc!=1)
	{
		cout << "Erreur de syntaxe. Merci de saisir seulement le nom du programme : ./Amelioration_const"<< endl;
	}
	else
	{	do{
		cout << "-------------------------------------------------------------------------------- "<<endl;
                cout << "                                                                                 "<<endl;
		cout << "                                                                                 "<<endl;
		cout << "------------------------------TRAITEMENT D'IMAGES------------------------------- "<<endl;
                cout << "---------Entrer le numero de votre choix suivant l'operation a effectuer---------"<<endl;
                cout << "                                                                                 "<<endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
                cout << "                                                                                 "<<endl;
		cout << "    1. Profil d'intensité des pixels d'une ligne d’une image"<<endl;
		cout << "    2. Modification des constrastes d'une image             "<<endl;
		cout << "    3. Quitter                                             "<<endl;
		cout << "--- Veuillez entrez votre choix suivant le numero d'ordre:  ";
		cin >> choixp;
		cout << "Entez le chemin d'acces de votre image:"<<endl;
		cout << "$ ";
		cin  >> nomimg;
		imgorig=imread(nomimg,1); //Chargement de l'image originale
                if(!imgorig.data)
		   {
			cout << "Image invalide. Merci fournir une image valide \n"<<endl;
		   }

		else
                   {
		switch (choixp){
		case 1:{cout << "                      "<<endl;
			cout << "                      "<<endl;
			cout << "       4. Histogramme de l'image originale                     "<<endl;
			cout << "       5. Courbe du profil d'intensité    "<<endl;
			cout << "       6. Quitter                          "<<endl;
			cout << " Veuillez entrez votre choix suivant le numero d'ordre:   ";
			cin >> choixh;
		
			switch(choixh){
			case 5:{
			cout << "      Traçage de la courbe du profil d'intensité          "<<endl;
		        cout << "Donner la valeur (numéro) de la ligne choisie"<<endl;
					cin >> valign;
					if(imgorig.rows<valign) {
            				   cout<<"Veuillez entrer une valeur de la ligne inférieure à "<<imgorig.rows<<endl;
                                           test =1;
                                        }
                                        else if(test==0){
                                          // Traçage de la courbe du profil d'intensité
                                          printime(imgorig,choix,valign);

                                          // Traçage de la ligne sur l'image
                                              tracerligne(imgorig,choix,valign);
                                          waitKey(0);
                                          cvDestroyAllWindows();
                                          return 0;
                                        }
                                       else{
            				  cout<<"Erreur de syntaxe. Merci de vérifier les données saisies\n"<<endl; // Erreur de syntaxe
            				  return 0; 
				       }
			break;}
			case 4:
				{
			cout << "      Traçage de l'histogramme          "<<endl;
			Mat histoimgcont=histogrammeimg(imgorig);
			//TraceHisto(nomimg);
			namedWindow("Histogramme de l'image",CV_WINDOW_AUTOSIZE);
                        imshow("Histogramme de l'image",histoimgcont);
                        if(!imwrite("HistoImgOrigl.png",histoimgcont))
                        cout<<"Erreur lors de l'enregistrement"<<endl;
                        waitKey(8000);//Affichage de l'image pendant 8s
                        destroyAllWindows();//Fermeture de la fenêtre 
			break;}
			case 6:{return 0; break;}
			}
                                    break;}
		case 2: {
                    do{
			cout << "                                            "<<endl;
			cout << "              ------------------            "<<endl;
                        cout << "       Modification de constraste d'une image"<<endl;
			cout << " +++++++++++++___________________++++++++++++"<<endl;

                        cout << "    1. Avec la fonction linéaire à trois points"<<endl;
                        cout << "    2. Avec la fonction linéaire avec saturation (min et max)"<<endl;
                        cout << "    3. Avec la correction gamma(fonction non linéaire)"<<endl;
                        cout << "    4. Avec l'égalisation de l'histogramme "<<endl;
                        cout << "    5. Voulez vous quitter le programme?"<<endl;
                        cout << "    Veuillez faire un choix: $ ";
                        cin  >> choix;

                        if((choix!=1)&&(choix!=2)&&(choix!=3)&&(choix!=4)&&(choix!=5))
                        {
                            cout << "Veuillez choisir un numero valide"<<endl;
                        }
                        else{
                            if(choix==5)
                            {
                                cout << " ^^^^^---FIN---^^^^"<< endl;
                                return 0;
                            }
                            else
                                {

				cout << "Traçage du profil d'intensité: Taper '1' pour une ligne horizontale ou '2' pour une ligne verticale" << endl;
			        cin >> vh;
				cout << "Entrer la valeur(nombre) de la ligne à tracer"<<endl;
				cin >> valign;
				switch (choix)
				   {
					case 1: // Amélioration avec la fonction linéaire à trois points
                                	 {
					    Point pt1, pt2, pt3;
					    // Collecte des informations spécifiques à la fonction linéaire
					    cout << "-- Amélioration du contraste avec fonction linéaire à trois (03) points --"<<endl;
					    cout << " -- Veuillez entrer les coordonnées des trois points --"<<endl;
                                            cout << "Abscisse du point p1 :";
					    cin  >> pt1.x;
					    cout << "Ordonnée du point p1 :";
					    cin  >> pt1.y;
					    cout << "Abscisse du point p2 :";
					    cin  >> pt2.x;
					    cout << "Ordonnée du point p2 :";
					    cin  >> pt2.y;
 					    cout << "Abscisse du point p3:";
					    cin  >> pt3.x;
					    cout << "Ordonnée du point p3:";
					    cin  >> pt3.y;

					    // Traitement de l'image selon les données receuillies
					    imaamlr=fctline3pts(imgorig,pt1,pt2,pt3);
					    crbfctline3pts(pt1,pt2,pt3);

				     break;
				 }

				case 2: // Modification linaire avec saturation
				  {
					Point Min, Max;
					// Collecte des informations spécifiques à la fonction linéaires
					cout << "______Modification avec la fonction linéaire avec saturation____"<<endl;
					cout << "______Veuillez entrer les abscisses des deux points (Min et Max)____"<<endl;
					cout << "Abscisse Min $ ";
					cin  >> Min.x;
					Min.y = 0;
					cout << "Abscisse Max $ ";
					cin  >> Max.x;
					Max.y = 255;

					// Traitement de l'image avec les points Min et Max
					imaamlr=fctlinsat(imgorig,Min,Max);

					//Courbe de la fonction gamma
					crbfctlinsat(Min, Max);
                                break;
				}

				case 3: // Amélioration avec la correction gamma: fonction non linéaire
				  {
				        float g;
					// Collecte des informations spécifiques à la fonction linéaires

					cout << "___Amélioration du contraste avec le fonction gamma(fonction non linéaire)__"<<endl;
					cout << " Veuillez entrer la valeur de gamma qui est comprise entre 0 et 1"<<endl;
					cin  >>g;

					// Traitement de l'image selon la valeur de gamma
					imaamlr=corgam(imgorig,g);

					//Courbe de la fonction gamma
					crbcorgam(g);

				break;
				}
				case 4: // Amélioration avec l'égalisation de l'histogramme
				 {
                                	cout << "L'égalisation de l'histogramme est la solution par defaut pour améliorer le contraste d'image 						à niveau de gris\n"<<endl;
                               		cout << "De plus, elle ne necessite pas plusieurs parametres, juste l'image source!!!\n"<<endl;

                                	const char* fenetre_sourc = "Image Source";
                                	const char* fenetre_egal = "Image egalisee";

		                        // Convertion en niveaux de gris de l'image
		                        Mat imgng;
		                        cvtColor(imgorig,imgng,COLOR_BGR2GRAY);

		                        // Application de l'égalisation de l'histogramme avec la fonction 'equalizeHist'
		                        Mat imgegal;

		                        equalizeHist(imgng,imgegal);

		                        imaamlr==imgegal;

		                        // Affichage des résultats des deux resultats

		                        namedWindow( fenetre_sourc, WINDOW_AUTOSIZE );
		                        namedWindow( fenetre_egal, WINDOW_AUTOSIZE );

		                        imshow(fenetre_sourc,imgorig);
		                        imshow(fenetre_egal,imgegal);

		                        if(!imwrite("ImgEgalisee.png",imgegal))
		                        cout<<"Enregistrement echoué"<<endl;

				break;
			     }
                        }
			Mat printimgori=printimg(imgorig,vh,valign);

                        imshow("Profil d'intensite de l'image originale",printimgori);
                        if(!imwrite("profilIntenImgOrigl.png",printimgori))
                        cout<<"Enregistrement echoué"<<endl;
                        
			//Image avec la ligne tracee

			Mat traceimgori=tracelign(imgorig,vh,valign);
                        imshow("Image originale avec trait",traceimgori);
                        if(!imwrite("LgnTraImaOrigl.png",traceimgori))
                        cout<<"Enregistrement echoué"<<endl;
                        
                        /*/ Affichage et enregistrement des images issues de de l'image contrastée /*/

			//Profil d'intensité
			Mat printimgcont=printimg(imaamlr,vh,valign);
			imshow("Profil d'intensite de l'image contrastee",printimgcont);
                        if(!imwrite("profilIntenImgContras.png",printimgcont))
                        cout<<"Enregistrement echoué"<<endl;
                        
			// Histogramme
			Mat histoimgcont=histogrammeimg(imaamlr);
			namedWindow("Histogramme image contrastee",CV_WINDOW_AUTOSIZE);
                        imshow("Histogramme image contrastée",histoimgcont);
                        if(!imwrite("HistoImgContras.png",histoimgcont))
                        cout<<"Enregistrement echoué"<<endl;
                       
			//Image avec le trait
			Mat traceimgcont=tracelign(imaamlr,vh,valign);
                        imshow("Image contrastee avec la ligne tracée",traceimgcont);
                        if(!imwrite("LgnImgContras.png",traceimgcont))
                        cout<<"Enregistrement echoué"<<endl;
                        // Quitter le programme

			return 0;
		  }
		 }
		}while(choix!=5);
	    }break;
         }
      }
    }while(choixp!=3);
   }
}

