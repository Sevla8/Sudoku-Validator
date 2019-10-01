<h1>Sudoku-Validator</h1><br>

<p>Sudoku validator using multithreading</p><br>

<h2>Compilation</h2><br>

<strong>$ make</strong><br>

<h2>Execution</h2><br>

<strong>$ ./sudoku < [sudoku_grid]</strong><br>
Où <em>[sudoku_grid]</em> représente un fichier contenant une grille de sudoku.
Le fichier <em>grid.txt</em> en est un exemple.
À la place d'utiliser un fichier pour renseigner l'entrée, on peut également la renseigner manuellement :<br>
<strong>$ ./sudoku<br>
[chiffre]...</strong><br>
Où <em>[chiffre]...</em> représente  89 chiffres séparés par un caractere contenu dans <em>$IFS</em><br>
Il est conseiller d'utiliser un fichier pour renseigner l'entrée du programme.
