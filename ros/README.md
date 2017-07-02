MOD : 19/12/2016 - 12:05
	              _____   ____   _____  
	             |  __ \ / __ \ / ____| 
	 _   _ ______| |__) | |  | | (___   
	| | | |______|  _  /| |  | |\___ \  
	| |_| |      | | \ \| |__| |____) | 
	| ._,_|      |_|  \_\\____/|_____/  
	| |                                 
	|_|                                 


INTRODUCTION
------------------------
L'équipe : Aurélien BEC et Quentin HUET (ROB4)

Le but de ce projet est de concevoir et implémenter un middleware pour la robotique.
Pour cela, un executable MASTER est necessaire qui devra etre capable de lire un fichier de configuration et de lancer d'autres executables pour lancer la simualtion demandée.
Nous avons donc implementer ce MASTER ainsi que les autres NODES necessaires.


INSTALLATION
------------------------
Le projet peut être compilé via la commande 'make'

Néammoins, plusieurs autres cibles sont présentes dans le makefile :
'make zip'		: Exporte le prohet en .zip 
'make fclean'	: Supprime toute trace de fichiers reconstructibles
'make re'		: Reconstruit entierement le projet



LE PROJET
------------------------

LES STRUCTURES
--------------
Plusieurs structres sont utilisées dans le projet :

La structure principale Config contient : (util_config.h)
	- un pointeur sur liste chainée de Node
	- un pointeur sur liste chainée de Fifo
	- une chaine de carateres 'path' qui contient le chemin d'environnement

La structure Node contient : (util_node.h)
	- une chaine de caracteres 'name' qui contient le nom du node
	- une chaine de caracteres 'bin' qui contient la commande de l'executable
	- une chaine de caracteres 'name_file' qui contient le nom de l'executable ( = bin - './')
	- une chaine de caracteres 'args' qui contient les arguments supplémentaires
	- un pointeur sur liste chainée de Port listen
	- un pointeur sur liste chainée de Port publish

La structure Fifo contient : (util_fifo.h)
	- une chaine de caracteres 'name' qui contient le nom du fifo
	- un entier 'nb_publish' qui contient le nombre de publicateurs
	- un entier 'nb_listen' qui contient le nombre d'ecouteurs

La strcture Port contient : (util_fifo.h)
	- une chaine de caracteres 'nom_du_port' qui contient le nom du port
	- une chaine de caracteres 'nom_du_fifo' qui contient le nom du fifo

La structure Command_Robot contient : (util_simu.h)
	- une chaine de caracteres 'chaine' qui contient la commande de vitesse
	- un entier 'port' qui contient le numero du port sur lequel publier la commande
	- un entier 'stop' qui determine l'arret du programme
	- un float 'vr' qui contient la vitesse de la roue droite
	- un float 'vl' qui contient la vitesse de la roue gauche
	- un mutex qui bloque l'utilisation multiple


LES FONCTIONS
--------------
Plusieurs fonction sont utilisées dans le projet :
Elles sont reparties dans des headers selon la regle suivante :
	Une fonction commencant par XXXX_foo_bar() sera dans le fichier util_XXXX.h

Config* config_new()
	- créée une nouvelle Config
Node* config_lecture( const char*)
	- lit un fichier de config et renvoie la liste des nodes lus
void config_verif_exe( Config*)
	- vérifie la présence des executables
void config_verif_fifo(Config*)
	- vérifie la création des fifos
int config_nb_node(Config*)
	- vérifie le nombre d'entrées sorties des fifos
void config_add_fifo(Config*, Port*, const char)
	- ajoute un fifo à la liste des fifos de Config
void config_free(Config*)
	- free la structure Config
Node* node_new()
	- créée un nouveau Node
void node_add_name( Node*, const char*)
	- ajoute un nom au node
void node_add_bin( Node*, const char*)
	- ajoute un binaire au node
void node_add_port(Node *, const char*, const char)
	- ajoute un Port au node dont le type est passé en argument
void node_add_args( Node*, const char*)
	- ajoute une lsite d'arguments supplémentaires au node
void node_print( Node*)
	- affiche les champs des Nodes de la liste chainée
void node_exec( Node*, const char*)
	- execute le Node
void node_free( Node*)
	- libère la liste chainée des nodes
Port* port_new()
	- créée un nouveau port
void port_print( Port*, const char*)
	- affiche les champs du port de la liste chainée
void port_free( Port*)
	- libère la liste chainée des ports
Fifo* fifo_new(const char*)
	- créée un nouveau fifo
int fifo_is_present(Fifo*, const char*)
	- indique si un fifo est deja présent dans la liste
void fifo_add_port(Fifo*, const char*, const char)
	- ajoute un port au fifo
void fifo_free(Fifo*)
	- libère la liste chainée des fifos
void fifo_creer(Fifo*)
	- créée un nouveau fifo dans le dossier


LES NODES
--------------
MASTER 
	- Lit un fichier de config, le verifie et lance les nodes necessaires
NODE_ECHO
	- ecrit dans un fifo une chaine donnée
NODE_LOGGER
	- ecrit dans un fichier ce qui est lu dans un fifo
NODE_SIMU
	- Execute la simulation du robot
NODE_TELE
	- Ecrit dans un fifo es commanes des vitesses
NODE_IA
	- Deplace le robot de maniere autonome
