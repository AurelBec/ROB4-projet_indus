#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream>

#include <vector>
#include <cmath> 
#include "../../roomba/hdr/Roomba.h"


//Renvoie une distance
double calculDist(double x1, double y1, double x2, double y2){
	return (sqrt((x1-x2)*(x1-x2)+((y1-y2)*(y1-y2))));
}

using namespace std;

int main(int argc, char* argv[])
{
	int port_posture_out, port_localisation_in, port_path_in;

	int i(1);
	while (i < argc)
	{
		if(string(argv[i]) == "--listen")
		{
			if(string(argv[i + 1]) == "localisation")
			{
				port_localisation_in = open(argv[i+2], O_RDONLY);
				if( port_localisation_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "chemin")
			{
				port_path_in = open(argv[i+2], O_RDONLY);
				if( port_path_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		if(string(argv[i]) == "--publish")
		{
			if(string(argv[i + 1]) == "postures")
			{
				port_posture_out = open(argv[i+2], O_WRONLY);
				if( port_posture_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		i++;
	}

	cerr << "Starting node suivi" << endl;
//
//
//					Entre ici...

/*





	//Recupere la posture du robot depuis l'odométrie
	double x,y,angle;
	
	//Distance entre la position du robot et le point du chemin le plus proche
	double dist;

	//Angle le robot et le point 
	double theta;
	
	//indice indiquant à quel point du chemin on en est
	int ind=0;
	
	//On définit eps une distance de 5 cm
	int eps=0.05;
	
	//Valeur du Kp à régler
	int Kp=3;

	//Vitesse à envoyer aux moteurs
	double vitesse=0;

	//Erreur: distance entre la position du robot et celle du point à atteindre
	double err=0;

	//	Recupereation de la trajectoire
	std::deque<TPoint2D> thePath = getPath();	//	TODO


		//	Parcours de tous les points, une fois qu'on a reçu le chemin
	for (std::deque<TPoint2D>::const_iterator it = thePath.begin(); it != thePath.end(); ++it){

		//On récupère les bons x,y,angle
		updatePos();		//	TODO

		//on récupère la distance entre le point du chemin et le robot		
		dist = calculDist(gridmap.x2idx(it -> x), gridmap.y2idx(it -> y), x, y);

		//On recupère l'angle entre le point à atteindre et le robot
		//atan2(y,x)
		double alpha;
		alpha = atan2(gridmap.y2idx(it -> y) - y, gridmap.x2idx(it -> x) - x);

		//On tourne le robot de manière à l'orienter vers le point à rejoindre
		theta = alpha - angle;
		printf("Angle envoyé %lf\n",theta);

		//On tourne le robot
		//Il faudra vérifier l'orientation
		turn(theta);

		//	Pour savoir quand on se rapproche, quand on l eloigne
		float Previous_Distance = Dist;

		//On va jusqu'au point suivant du chemin grace à un correcteur proportionnel
		while((dist > eps) && (Previous_Distance > Dist)){
			//	Tan qu on est loin && Tan qu on se rapproche

			Previous_Distance = Dist;
			err = dist;
			vitesse = Kp * err;
			//Ca va continuer à envoyer cette vitesse pour toujours? Vérifier ? Faire une boucle de temps non bloquante
			driveDirect(vitesse, vitesse);
			updatePos();
			dist = calculDist(gridmap.x2idx(it -> x),gridmap.y2idx(it -> y), x, y);
		}
	}

*/

//
//					...et là
//







	cerr << "Ending node suivi" << endl;
	close(port_posture_out);
	close(port_path_in);
	close(port_localisation_in);

	return 0;
}
