#include "../hdr/my_function.h"

using namespace std;
using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::maps;
using namespace mrpt::obs;
using namespace mrpt::math;
using namespace mrpt::poses;


void ajustPoseWithICP(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, mrpt::maps::CSimplePointsMap *P_my_Map, CPose2D *My_Pose)	{
	//	fonctionne mais doit etre adaptee pour remplir la posture


	mrpt::slam::CICP ICP;

/*
	//	m1 & m2
	mrpt::maps::COccupancyGridMap2D m1 = *P_my_Occupancy_Grid;
	mrpt::maps::CSimplePointsMap m2 = *P_my_Map;
*/

	//	running time
	float runningTime;


	//	info
	mrpt::slam::CICP::TReturnInfo info;


	//	Options

	int ICP_method = (int) icpClassic;
	ICP.options.ICP_algorithm = (TICPAlgorithm)ICP_method;

	ICP.options.maxIterations			= 100;
	ICP.options.thresholdAng			= DEG2RAD(1.0f);
	ICP.options.thresholdDist			= 0.075f;
	ICP.options.ALFA					= 0.05f;
	ICP.options.smallestThresholdDist	= 0.005f;
	ICP.options.doRANSAC = false;

//	ICP.options.dumpToConsole();



	//	Pose initiale
	CPose2D		initialPose = *My_Pose;



	//	Allignement !
	CPosePDFPtr pdf = ICP.Align(
		P_my_Occupancy_Grid,
		P_my_Map,
		initialPose,
		&runningTime,
		(void*)&info);

int Afficher = 0;
if(Afficher)	{
	printf("ICP run in %.02fms, %d iterations (%.02fms/iter), %.01f%% goodness\n -> ",
			runningTime*1000,
			info.nIterations,
			runningTime*1000.0f/info.nIterations,
			info.goodness*100 );

	std::cerr << "Mean of estimation: " << pdf->getMeanVal() << std::endl << std::endl;
}
	CPosePDFGaussian  gPdf;
	gPdf.copyFrom(*pdf);

if(Afficher)	{
	std::cerr << "Covariance of estimation: " << std::endl << gPdf.cov << std::endl;

	std::cerr << " std(x): " << sqrt( gPdf.cov(0,0) ) << std::endl;
	std::cerr << " std(y): " << sqrt( gPdf.cov(1,1) ) << std::endl;
	std::cerr << " std(phi): " << RAD2DEG(sqrt( gPdf.cov(2,2) )) << " (deg)" << std::endl;

	std::cerr << "-> Saving transformed map to align as scan2_trans.txt" << std::endl;
}


	mrpt::maps::CSimplePointsMap P_my_Map_trans = *P_my_Map;
	P_my_Map_trans.changeCoordinatesReference( gPdf.mean );
	P_my_Map_trans.save2D_to_text_file("scan2_trans.txt");

if(Afficher)	{
	std::cerr << "-> Saving MATLAB script for drawing 2D ellipsoid as view_ellip.m" << std::endl;
}
	CMatrixFloat COV22 =  CMatrixFloat( CMatrixDouble( gPdf.cov ));
	COV22.setSize(2,2);
	CVectorFloat MEAN2D(2);
	MEAN2D[0] = gPdf.mean.x();
	MEAN2D[1] = gPdf.mean.y();


	//	Remplissage de la New_Pose
	mrpt::poses::CPose2D New_Pose = pdf -> getMeanVal();

	New_Pose.x(-New_Pose.x());
	New_Pose.y(-New_Pose.y());
	New_Pose.phi(-New_Pose.phi());

std::cerr << "    ICP::ajustement : (" << New_Pose.x() << ", " << New_Pose.y() << ", " << New_Pose.phi() << ")" << std::endl;

//	float new_X = My_Pose -> x() + New_Pose.x();
//	float new_Y = My_Pose -> y() + New_Pose.y();
//	float new_Phi = My_Pose -> phi() + New_Pose.phi();

	float new_X = New_Pose.x();
	float new_Y = New_Pose.y();
	float new_Phi = New_Pose.phi();

	My_Pose -> x(new_X);
	My_Pose -> y(new_Y);
	My_Pose -> phi(new_Phi);

//	(*My_Pose) = (*My_Pose) + New_Pose;

	return;
}






void TestICP(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, mrpt::maps::CSimplePointsMap *P_my_Map)	{
	//	fonctionne mais doit etre adaptee pour remplir la posture


	mrpt::slam::CICP ICP;

	//	m1 & m2
	mrpt::maps::COccupancyGridMap2D m1 = *P_my_Occupancy_Grid;
	mrpt::maps::CSimplePointsMap m2 = *P_my_Map;


	//	running time
	float runningTime;


	//	info
	mrpt::slam::CICP::TReturnInfo info;


	//	Options

	int ICP_method = (int) icpClassic;
	ICP.options.ICP_algorithm = (TICPAlgorithm)ICP_method;

	ICP.options.maxIterations			= 100;
	ICP.options.thresholdAng			= DEG2RAD(10.0f);
	ICP.options.thresholdDist			= 0.75f;
	ICP.options.ALFA					= 0.5f;
	ICP.options.smallestThresholdDist	= 0.05f;
	ICP.options.doRANSAC = false;

	ICP.options.dumpToConsole();



	//	Pose initiale
	CPose2D		initialPose(0.8f,0.0f,(float)DEG2RAD(0.0f));



	//	Allignement !
	CPosePDFPtr pdf = ICP.Align(
		&m1,
		&m2,
		initialPose,
		&runningTime,
		(void*)&info);



	printf("ICP run in %.02fms, %d iterations (%.02fms/iter), %.01f%% goodness\n -> ",
			runningTime*1000,
			info.nIterations,
			runningTime*1000.0f/info.nIterations,
			info.goodness*100 );

	std::cerr << "Mean of estimation: " << pdf->getMeanVal() << std::endl << std::endl;

	CPosePDFGaussian  gPdf;
	gPdf.copyFrom(*pdf);

	std::cerr << "Covariance of estimation: " << std::endl << gPdf.cov << std::endl;

	std::cerr << " std(x): " << sqrt( gPdf.cov(0,0) ) << std::endl;
	std::cerr << " std(y): " << sqrt( gPdf.cov(1,1) ) << std::endl;
	std::cerr << " std(phi): " << RAD2DEG(sqrt( gPdf.cov(2,2) )) << " (deg)" << std::endl;

	std::cerr << "-> Saving transformed map to align as scan2_trans.txt" << std::endl;
	mrpt::maps::CSimplePointsMap m2_trans = m2;
	m2_trans.changeCoordinatesReference( gPdf.mean );
	m2_trans.save2D_to_text_file("scan2_trans.txt");

	std::cerr << "-> Saving MATLAB script for drawing 2D ellipsoid as view_ellip.m" << std::endl;
	CMatrixFloat COV22 =  CMatrixFloat( CMatrixDouble( gPdf.cov ));
	COV22.setSize(2,2);
	CVectorFloat MEAN2D(2);
	MEAN2D[0] = gPdf.mean.x();
	MEAN2D[1] = gPdf.mean.y();

	return;
}







//	Observation stuff

	//	A MODIFIER PAR AURELIEN
int read_Data(float *P_Data_Tab, char *P_Validity_Tab, Data_lidar* data_lidar)	{

	int i, nVal = -1;

	do
	{
		data_lidar->mtx->lock();
		nVal = data_lidar->data[0][0];

		if( nVal > 0)
		{
			//cerr << "slam received " << nVal << " data from packet " << data_lidar->data[0][1] << endl;
			for( i = 0; i < nVal; i++)
			{
				P_Data_Tab[i] = data_lidar->data[i+1][1] / 500;
			//	500 EST ARBITRAIRE ! C EST UNE FACTEUR DE CONVERSION D UNITE (unite de distance captee par le lidar -> unite de distance dans l occupancy grid
				
				if(P_Data_Tab[i] == 0)	{
					//	Data invalide
					P_Validity_Tab[i] = 0;
				}
				else	{
					//	Data valide
					P_Validity_Tab[i] = 1;
				}
			}
			//usleep(5000);
		}
		data_lidar->mtx->unlock();
	} while (nVal < 1);

	return nVal;
		//	On retourne le nombre de donnees (qui peut varier d une mesure a l autre)
}


void createMyVector(float *tab, std::vector<float> *vec, int Nombre_De_Distances)	{
		//	Cree et renvoie un vector contenant les mesures

	int i;
	for(i=0; i < Nombre_De_Distances; i++)	{
		(*vec)[i] = tab[i];
	}
	return;
}

void createMyVectorChar(char *tab, std::vector<char> *vec, int Nombre_De_Distances)	{
	int i;
	for(i=0; i < Nombre_De_Distances; i++)	{
		(*vec)[i] = tab[i];
	}

	return;
}


//	A MODIFIER PAR AURELIEN
void setPose(mrpt::poses::CPose2D *P_My_Pose, Odom* odom)	{

	//	Pour determiner la "Pose", sera remplacer par l odometrie + localisation dans l OccupancyGrid
/*	float Vitesse_Robot_X = -1;
	float Vitesse_Robot_Y = -1;

	float Norme_2 = sqrt(Vitesse_Robot_X *Vitesse_Robot_X + Vitesse_Robot_Y * Vitesse_Robot_Y);

	Vitesse_Robot_X /= Norme_2;
	Vitesse_Robot_Y /= Norme_2;
		//	Normalisation

	float Celerity = 0.1;


	float new_X = P_My_Pose -> x() + Vitesse_Robot_X * Celerity;
	float new_Y = P_My_Pose -> y() + Vitesse_Robot_Y * Celerity;
	float new_Phi = P_My_Pose -> phi();*/

	odom->mtx->lock();
	if( odom->check)
	{
		if( !(odom->x == -1 && odom->y == -1 && odom->angle == -1))
		{
			//cerr << "slam : x: " << odom.x << " y: " << odom.y << " a: " << odom.angle << endl;

			P_My_Pose -> phi(odom->angle);
			P_My_Pose -> x(odom->x);
			P_My_Pose -> y(odom->y);
			odom->check = false;
		}
	}
	odom->mtx->unlock();
//	My_Pose.setFromValues(Vitesse_Robot_X * Celerity * Measure_Number, Vitesse_Robot_Y * Celerity * Measure_Number, 0);

	return;
}








void updateOccupancyGrid(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, Odom* odom, Data_lidar* data_lidar, int Measure_Number)	{

	//	Creation d une "Observation"
	mrpt::obs::CObservation2DRangeScan my_Observation;
   		my_Observation.aperture = 6.283;	//	Angle d observation = 2 * PI
		my_Observation.rightToLeft = false;	//	Eventuellement a changer

	//	"Creation" de l'occupancy grid map : Elle est declaree dans le main, nous indiquons que c est elle que nous modifions
	mrpt::maps::COccupancyGridMap2D my_Occupancy_Grid = *P_my_Occupancy_Grid;

	P_my_Occupancy_Grid -> saveAsBitmapFile("Occupancy_Grid_EMPTY.png");
		//	Saving my_Occupancy_Grid while empty


		//	SensorPose
		mrpt::poses::CPose2D My_Pose;	//	Nous utilisons le constructeur par default, les autres posent des problemes que je n explique pas
			My_Pose.x(0);
			My_Pose.y(0);
			My_Pose.phi(0);


		//	Lecture des donnees
		float *Observation_Data_Read = (float *) malloc(OBSERVATION_SIZE_MAX * sizeof(float));
		char *Validation_Data_Read = (char *) malloc(OBSERVATION_SIZE_MAX * sizeof(char));
			//	Ces variables vont contenir les donnees lues

		//	Lecture des donnees & recuperation du nombre de donnees
		int Nombre_De_Distances = read_Data(Observation_Data_Read, Validation_Data_Read, data_lidar);
		//cerr << "update received " << Nombre_De_Distances << " data" << endl;

		//	Remplissage de la "Observation" avec les mesure recuperees
		std::vector<char> Observation_Valid(Nombre_De_Distances);
		std::vector<float> Observation_Data(Nombre_De_Distances);

		createMyVectorChar(Validation_Data_Read, &Observation_Valid, Nombre_De_Distances);
		createMyVector(Observation_Data_Read, &Observation_Data, Nombre_De_Distances);
			//	Resolution de problemes de compatibilite : la "Observation" necessite des std::vector<type>, pas des tableaux

		my_Observation.resizeScan(Nombre_De_Distances);
			//	On indique la taille des vectors
		my_Observation.scan = Observation_Data;

		my_Observation.validRange = Observation_Valid;
			//	Les mesures sont inserees dans la "Observation"

		mrpt::maps::CSimplePointsMap m1;
		m1.insertObservation(&my_Observation);

	//	KA BOOOOOOOOM !

		setPose(&My_Pose, odom);
		//cerr << "fin odom " << My_Pose.x() << " " << My_Pose.y() << " " << My_Pose.phi() << endl;
//		std::cerr << "Apres odometrie : My_Pose = (" << My_Pose.x() << ", " << My_Pose.y() << ", " << My_Pose.phi() << ")" << std::endl;

//		std::cerr << "Avant ICP : My_Pose = (" << My_Pose.x() << ", " << My_Pose.y() << ", " << My_Pose.phi() << ")" << std::endl;
//		ajustPoseWithICP(P_my_Occupancy_Grid, &m1, &My_Pose);
//		std::cerr << "Apres ICP : My_Pose = (" << My_Pose.x() << ", " << My_Pose.y() << ", " << My_Pose.phi() << ")" << std::endl;


		mrpt::poses::CPose3D My_Pose_3D;
		My_Pose_3D.setFromValues(My_Pose.x(), My_Pose.y(), 0, My_Pose.phi(), 0, 0);

		//std::cerr << "My_Pose_3D = " << My_Pose_3D << std::endl;

		my_Observation.setSensorPose(My_Pose_3D);

			//	Nous inserons la "Pose" dans la "Observation" : Nous indiquons a l observation ou elle a ete prise
		//std::cerr << "my_Observation.sensorPose : " << my_Observation.sensorPose << std::endl;

		my_Occupancy_Grid.insertObservation(&my_Observation, NULL);
			//	Insertion de la "Observation" dans la "Occupancy grid" a la "Pose" indiquee


		saveOccupancyGrid(&my_Occupancy_Grid, Measure_Number);

		//	Fin du traitement de la mesure	
		free(Observation_Data_Read);
		free(Validation_Data_Read);

	//	Traitement de la mesure suivante
	

	return;
}


void saveOccupancyGrid(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, int Measure_Number)
{
	char File_Name[258];
	File_Name[0] = '\0';
	strcat(File_Name, "lidar_output/Occupancy_Grid_");

	char My_Number[128];

	//	Ajout de '0's pour que les fichiers apparaissent dans le bon ordre : '008', '009', '010'... au lieu de '8', '9', '10'...
	char Zero[50];
	sprintf(Zero, "%d", 0);
	if(Measure_Number < 100)	{
		if(Measure_Number < 10)	{
			strcat(File_Name, Zero);
		}
		strcat(File_Name, Zero);
	}

	sprintf(My_Number, "%d", Measure_Number);
	strcat(File_Name, My_Number);

	strcat(File_Name, ".png");
		//	File_Name contient desormais le nom du fichier ou la "OccupancyGrid" va etre sauvegardee

//		printf("%s\n", File_Name);
		//	Affichage du nom du fichier sauvegarde

	P_my_Occupancy_Grid->saveAsBitmapFile(File_Name);
}







void addOdometry(mrpt::poses::CPose2D *My_Pose)	{
//std::cerr << "Debut odometry        My_Pose : " << My_Pose -> x() << "  " << My_Pose -> y() << "  " << My_Pose -> phi() << std::endl;
	//	Remplissage de la Pose


	//	Pour determiner la "Pose", sera remplacer par l odometrie + localisation dans l OccupancyGrid
	float Vitesse_Robot_X = -1;
	float Vitesse_Robot_Y = -1;

	float Norme_2 = sqrt(Vitesse_Robot_X *Vitesse_Robot_X + Vitesse_Robot_Y * Vitesse_Robot_Y);

	Vitesse_Robot_X /= Norme_2;
	Vitesse_Robot_Y /= Norme_2;
		//	Normalisation

	float Celerity = 0.1;

	mrpt::poses::CPose2D Pose_To_Add;
		Pose_To_Add.x(Vitesse_Robot_X * Celerity);
		Pose_To_Add.y(Vitesse_Robot_Y * Celerity);
		Pose_To_Add.phi(0);

//std::cerr << "    Odometry::ajustement : " << Pose_To_Add.x() << ", " << Pose_To_Add.y() << ", " << Pose_To_Add.phi() << ")" << std::endl;
//std::cerr << "    Pose_To_Add = (" << Pose_To_Add.x() << ", " << Pose_To_Add.y() << ", " << Pose_To_Add.phi() << ")" << std::endl;

float new_X = My_Pose -> x() + Pose_To_Add.x();
//new_X = 666;
float new_Y = My_Pose -> y() + Pose_To_Add.y();
//new_Y = 667;
float new_Phi = My_Pose -> phi() + Pose_To_Add.phi();
//new_Phi = 668;

//std::cerr << "new_X = " << new_X << "      " << "new_Y = " << new_Y << "      " << "new_Phi = " << new_Phi << std::endl;

	My_Pose -> x(new_X);
	My_Pose -> y(new_Y);
	My_Pose -> phi(new_Phi);

//	(*My_Pose) = (*My_Pose) + Pose_To_Add;
//std::cerr << "        My_Pose : " << My_Pose -> x() << "  " << My_Pose -> y() << "  " << My_Pose -> phi() << std::endl;
//std::cerr << " TEST : My_Pose.x() = " << My_Pose -> x() << "My_Pose.y() = " << My_Pose -> y() < std::endl;

	return;
}
