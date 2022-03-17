// =========================================================================
//	Popen.c
//	J. Menu - http://cui.unige.ch/DI/cours/SystInformatiques
//
//	$Id$
// =========================================================================


// -------------------------------------------------------------------------
// Popen
// -------------------------------------------------------------------------

#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

int main ()
	{
if (0) {
	printf (
		"--> Emploi de 'popen ()' pour faire executer un 'sort -n -r'\n"
		"    et recuperer la sortie standard de ce dernier\n\n" );

	FILE		* flotDeSortie;
	int			i;
	const int	LIMITE = 5;

	if ((flotDeSortie = popen ("sort -n -r", "w")) < 0)
		printf ("Probleme pour creer le flot de sortie par 'popen ()'");

	// on ecrit sur ce flot
	for (i = 1; i <= LIMITE; i ++)
		fprintf (flotDeSortie, "%2d : %s\n", i, "Hello world !");
	fflush (flotDeSortie);

	// on ferme le flot
	return pclose (flotDeSortie);
}

else {
	printf (
		"--> Emploi de 'popen ()' pour faire executer un 'ls -sal'\n"
		"    et recuperer la sortie standard de ce dernier\n\n" );

	FILE		* flotDEntree;
	int			i;
	const int	LIMITE = 5;

	if ((flotDEntree = popen ("ls -sal", "r")) < 0)
		printf ("Probleme pour creer le flot d'entree par 'popen ()'");

	// on lit sur ce flot
	for (i = 1; i <= LIMITE; i ++)
		printf ("%s", readline (NULL));
}
	}  // main
