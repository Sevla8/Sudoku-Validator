Compilation :

gcc -pthread main.c

Execution :

./a.out test.txt

Description :

*)Le programme lit un fichier contenant une seule grille de sudoku.
*)Les différents types d'erreurs sont gérés.
*)Si la grille n'est pas correct, le message d'erreur correspondant est affiché sur la sortie d'erreur standard et le programme se termine.
*)Si la grille est correct, 3*9 threads sont lancés : 1 par ligne, 1 par colonne et 1 par block.
*)Au premier chiffre absent d'une ligne/colonne/sous-grille, le thread se termine. On ne cherche pas à lister tout les chiffres manquants, seulement 1 par ligne/colonne/sous-grille.
*)Les ligne/colonne/sous-grille sont numéroté à partir de 0.
*)Les sous-grilles sont numérotés de gauche à droite, de haut en bas.

