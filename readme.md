## Membres du groupe:
- Abdoulaye Djibril DIALLO  22112788 -> Lecture du fichier et formules pour les coordonnées xij dans row[]
- Mamady DJIGUINE           22110369 -> Interprétation de la Modélisation	
- Elhadj Alseiny DIALLO     22011830 -> implémentation resolution
- Mamadou Alpha DIALLO      22107614 -> représentation sous format png avec l'outils dot

## Modélisation :
1. le sens des variables Xij:
       Xij est un boolean qui représente si oui ou non une ville i est connectée avec une ville J 
2. le sens des variables Ui:
       Ui représente l'ordre de passage sur une ville, Ui=k signifie que la ville i a eté raccordée en k-ieme position
3. les quatre contraintes utilisées:
    Première contrainte: pour une ville i il existe un et un seul  cable reliant cette ville à une autre ville j (de i vers j) /Xij =1 
    Deuxième contrainte: pour une ville j il existe un et un seul  cable reliant une ville i a lui même (de i vers j) /Xij =1
    Troisième contrainte : soit deux villes i j telles que Ui < Uj
        si Xij = 1, cela veut que la ville i est raccordée avec celle j .De meme il faut que la différence de leur ordre de passage dans 
        le raccordement soit plus petite ou égale -1
        si Xij = 0, cela veut dire que la ville i n'est pas raccordée avec la ville j, et il faut tout de même que la différence de leur 
        ordre de raccordement soit plus petite que (n-1) qui est :le nombre de raccordement minimal pour relier n villes
    Quatrième contrainte : il faut que l'ordre de passage de chaque ville i soit compris entre 1 et n-1.
        le nombre de connections qu'il a fallu pour l'atteindre doit etre plus petit que le nombre câble nécessaire 
        pour raccorder n villes en fil (anneau) c'est à dire n-1 qui est le minimum possible. minimum 1 sinon ça serait dépourvue de sens.

4. la fonction objective avec le sens d’optimisation:
        quel est le cout minimal nécéssaire pour raccorder n villes en fibre optique 
        la somme des distances entre les villes directement connectées soit minimale
    Sens d'optimisation : minimisation


## RESULTAT D'OPTIMISATION 

- Optimal solution       174.891958237

ville 1 vers 2
ville 2 vers 3
ville 3 vers 4
ville 4 vers 9
ville 9 vers 10
ville 10 vers 8
ville 8 vers 7
ville 7 vers 6
ville 6 vers 5
ville 5 vers 1
