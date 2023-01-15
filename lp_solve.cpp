 /*	## Membres du groupe:
	- Abdoulaye Djibril DIALLO  22112788 -> Lecture du fichier et formules pour les coordonée xij dans row[]
	- Mamady DJIGUINE           22110369 -> Interprétation de la modelisaiton
	- Elhadj Alseiny DIALLO     22011830 -> implémentation resolution
	- Mamadou Alpha DIALLO      22107614 -> représentation sous format png avec l'outils dot

	voir les reponses aux questions et solution dans le fichier readMe
*/
#include <lpsolve/lp_lib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

void vider_row(REAL row[], int taille){
	for (int i = 0 ; i<taille ; i++){
		row[i] = 0.0;
	}
}

/*tab est un tableau a deux dimension
M et N et l'indexe de deux ville dans le tableau
ainsi cette fonction retuourne la distance euclidienne entre les deux;
*/
float dist_euc (int** tab, int M,int N){
	int A = (tab[N][0]-tab[M][0]);
	int B = (tab[N][1]-tab[M][1]);
	return sqrt(A*A + B*B);
}

int main (int argc, char* argv[])
{
	int ** tabs = NULL;
	std::string ligne;
	std::ifstream fichier("a280.tsp");
	if (!fichier.is_open()) {
		std::cout<< "Impossible d'ouvrir le fichier" << std::endl;
		exit(EXIT_FAILURE);
	}
	int nbVille;
	int infoVille = 0;
	int indiceVille=0,ord,absc;
	while(getline(fichier, ligne)) {
		std::string label;
		std::istringstream iss (ligne);
		iss >> label;
		//si c'est une information sur une ville on charge les informations
		//sur un tableau a deux dimension le numéro de la colonne correspond à la infoVille
		// et les deux valeurs de la colonne les coordonées x y (tableau[ville3][x],tableau[ville3][y])
		if (infoVille  && label != "EOF") {
			iss >> absc >> ord;
			tabs[indiceVille][0] = absc;
			tabs[indiceVille][1] = ord;
			indiceVille++;
			}
			//si le label est d'une dimension on recupère le nombre de ville
			//et on alloue dynamiquement un tableau contenant les informations de ville
			else if (label =="DIMENSION:") {
				iss>>nbVille;
				tabs = new int*[nbVille];
				for(int i=0; i<nbVille; i++) {
					tabs[i] = new int[2];
					}
				//pendant la lecture du nom de colonne on passe en lecture d'information de ville
				//donc infoVille TRUE
				}else if (label=="NODE_COORD_SECTION"){
					infoVille=1;
				}
	}
	fichier.close();

	/* le nombre de variables est nbVille*(nbVille-1)+nbVille-1; car pour chaque ville i il va falloir
		x(i-j) telque : j allant de 1 à nbVille-1 pour ne pas rajouter une variable supplémentaire qui restrai toujours a 0
		est xij / (i=j) donc nbVille*(nbVille-1) ajouter à cela il ya Ui pour i allant de 2 à nbVille donc plus nbVille-1
		ici aussi pour eviter u1 qui est unitule on lexclue.

		on tiendra comptra de l'exclusion de ces variables dans l'interpretation de row[]
	*/
	//nombre de contraintes à 0
	int nbConstraint = 0;
	int nbVariable = nbVille*(nbVille-1)+nbVille-1;
	cout<<nbVariable;
	lprec *lp;
	lp = make_lp(nbConstraint, nbVariable);
	REAL row[nbVariable + 1];
	vider_row(row, nbVariable+1);

	/*on a supposé dans la suite que, dans le tableau row , les nbTotalVilles valeur sont les combinaisons
		possibles x1j (de la ville i=1 avec les autres villes j) dans l'ordre croissant  après x2j -- xnj jusqu'à n(xnn)
		et en excluant (i=j)
		puis suivront les (Ui) i allant de 2 à n; donc u2 est le nbTotalVille variable après les xij
	*/
	//on définie les variables xij en tant que boolean
	for (int i = 1; i < nbVille*(nbVille-1)+1; i++) {
		set_binary(lp, i, TRUE);
	}

	/*l'indexation de la variable xij pour deux villes se fait avec la formule suivante
		si i>j :indice = (i-1)*nbVille +j-(i-1);
			   (i-1)*nbVille pour trouver le modulo , indexer la premier valeur de X(i-j)et on ajoute j pour atteindre le
			   bon emplacement mais vu que a chaque i depasser on retire une variable xij/ (i=j) qui es jugé unitile donc -(i-1)
		si i<j :indice = (i-1)*nbVille +j-(i-1)-1;
			en plus de la formule de base on a depasser xij /i=j pour la valeur actuelle donc qu'il faut reculer l'indexe d'un case
	*/
	//nbTotalVilles contrainte
	for (int i = 1; i < nbVille+1; i++) {
		for (int j = 1; j < nbVille+1; j++) {
			if(i!=j){
				int indice = (i-1)*nbVille +j-(i-1);
				if(i<j){
					indice -=1;
				}
				row[indice] = 1;
			}
		}
		add_constraint(lp, row, EQ, 1.0);
		vider_row(row, nbVariable+1);
	}

	//-deuxieme contrainte
	for (int j = 1; j < nbVille+1; j++) {
			for (int  i= 1; i < nbVille+1; i++) {
				if(i!=j){
					int indice = (i-1)*nbVille +j-(i-1);
					if(i<j){
						indice -=1;
					}
					row[indice] = 1;
			}
		}
		add_constraint(lp, row, EQ, 1.0);
		vider_row(row, nbVariable+1);
	}

	//contrainte sur les ordres de passage sur i
	//posU indexe de la fin des contrainte boolean xij
	int posU=nbVille*(nbVille-1)-1;
	for (int i = 2; i < nbVille+1; i++) {
		for (int  j= 2; j < nbVille+1; j++) {
			if(i!=j){
				row[i+posU]=1;
				row[j+posU]=-1;
				int indice = (i-1)*nbVille +j-(i-1);
				if(i<j){
					indice -=1;
				}
				row[indice]=nbVille;
				add_constraint(lp, row, LE, nbVille-1);
				vider_row(row, nbVariable+1);
			}
		}
	}

	//contrainte 4 1<ui<n-1
	for (int i = 2; i < nbVille+1; i++) {
		row[i+posU]=1;
		add_constraint(lp, row, LE, nbVille-1);
		add_constraint(lp, row, GE, 1);
		vider_row(row, nbVariable+1);
	}

	//--fonction objective
	for (int j = 1; j < nbVille+1; j++) {
		for (int  i= 1; i < nbVille+1; i++) {
			if(i!=j){
				int indice = (i-1)*nbVille +j-(i-1);
				if(i<j){
					indice -=1;
				}
				row[indice] = dist_euc(tabs,i-1,j-1);
			}
		}
	}
	//résolution et création du contenue du fichier.dot pour eviter le double parcour
	set_obj_fn(lp, row);
	int ret = solve(lp);
	if (ret==0)
	{
		//resultat de la fonction objectif
		cout << "Value = " << get_objective(lp) << endl;
		get_variables(lp, row);
		//---dot
		string filename("tmp.txt");
		fstream file_out;

		file_out.open("fichier.dot", std::ios_base::out);
		if (!file_out.is_open()) {
			cout << "failed to open file" << filename << '\n';
		}else{
			//création des variables dans fichier.dot
			file_out <<"graph {"<< endl;
			for (int  i= 1; i < nbVille+1; i++) {
				file_out << i<<";" << endl;
			}
			file_out << endl<< endl;
		}

			for (int  i= 1; i < nbVille+1; i++) {
				for (int j = 1; j < nbVille+1; j++) {
					if(i!=j){
						int indice = (i-1)*nbVille +j-(i-1);
						if(i<j){
							indice -=1;
						}
						if(row[indice-1]==1){
							cout <<"Ville " << i <<" vers Ville "<<j<< endl;
							file_out << i <<" -- "<<j<<";"<< endl;
						}

					}
				}
			}
			file_out << "}" << endl;
			file_out.close();
	}

}
