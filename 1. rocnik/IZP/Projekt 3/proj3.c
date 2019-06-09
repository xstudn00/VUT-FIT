/**
 * Kostra programu pro 3. projekt IZP 2017/18
 * Ondrej Studnicka (xstudn00)
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>
#include <ctype.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

int premium_case;

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);

	c->size = 0;
	c->capacity = cap;
	//alokovani pameti pro c->obj (kapacita * velikost struktury obj_t)
	c->obj = malloc(c->capacity * sizeof(struct obj_t));

	//pokud se nepodari alokovat pamet, tak se kapacita rovna nule a vypisu hlaseni na stderr
	if (c->obj == NULL) {
		c->capacity = 0;
		fprintf(stderr, "Pamet nealokovana!\n");
	}

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	//odstraneni shluku
	free(c->obj);

	//inicializace na prazdny shluk
	c->capacity = 0;
	c->size = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
	// TUTO FUNKCI NEMENTE
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);

	if (c->capacity >= new_cap)
		return c;

	size_t size = sizeof(struct obj_t) * new_cap;

	void *arr = realloc(c->obj, size);
	if (arr == NULL)
		return NULL;

	c->obj = arr;
	c->capacity = new_cap;
	return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	//pokud se velikost rovna kapacite, tak rozsirim shluk a pridam objekt na konec shluku
	if (c->size >= c->capacity) {

		//pokud se pamet nepodarilo alokovat
		if (resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL) {
			fprintf(stderr, "Pamet nealokovana!\n");
		}

		//pridani objektu na konec shluku
		c->obj[c->size] = obj;
		c->size++;
	}

	//jinak pouze pridam objekt na konec shluku
	else {
		c->obj[c->size] = obj;
		c->size++;
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
	// TUTO FUNKCI NEMENTE
	const struct obj_t *o1 = a;
	const struct obj_t *o2 = b;
	if (o1->id < o2->id) return -1;
	if (o1->id > o2->id) return 1;
	return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
	qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c2 != NULL);

	//provadej append_cluster dokud je i mensi nez velikost shluku c2
	for (int i = 0; i < c2->size; i++) {
		append_cluster(c1, c2->obj[i]);
	}

	//usporadani shluku
	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);

	//odstraneni shluku na pozici idx
	clear_cluster(&carr[idx]);

	//snizeni poctu shluju o jedna
	narr--;

	//presouvani vsech shluku na index, ktery je o jedna ruzny nez jejich puvodni index
	for (int i = idx; i < narr ; i++) {
		carr[i] = carr[i + 1];
	}

	//vracim novy pocet shluku v poli
	return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2)
{

	assert(o1 != NULL);
	assert(o2 != NULL);

	//inicializace promenne
	float distance;

	//vzorec pro vypocet vzdalenosti (x1*x1 - 2x1*x2 +x2*x2)+(y1*y1 + 2*y1*y2 + y2*y2)
	distance = sqrtf((o1->x * o1->x - 2 * o1->x * o2->x + o2->x * o2->x) + (o1->y * o1->y - 2 * o1->y * o2->y + o2->y * o2->y));

	return distance;
}

/*
 Pocita vzdalenost dvou shluku.
*/

#define SIDE (1000+1)  //1000 je maximalni delka jedne strany mrizky, jednicka proto, aby byla vzdy splnena podminka nize ve funkci pri prvnim pruchodu

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	//inicializace promene
	float distance = 0;

	//--min
	if (premium_case == 2) {
		//inicializace promennych
		float current;

		//maximalni vzdalenost dvou shluku
		float final = sqrtf(SIDE * SIDE + SIDE * SIDE);

		for (int i = 0; i < c1->size; i++)
		{
			for (int j = 0; j < c2->size; j++)
			{
				//vzdalenost dvou objektu
				current = obj_distance(&c1->obj[i], &c2->obj[j]);
				//pokud je finalni vzdalenost vetsi nez prubezna (vzdy pri prvnim pruchodu) tak ulozim nove nalezenou vzdalenost
				if (final > current)
				{
					final = current;
				}
			}
		}
		distance = final;
	}

	//--max
	else if (premium_case == 3) {
		//inicializace promennych
		float current;

		//maximalni vzdalenost dvou shluku
		float final = 0.0;

		for (int i = 0; i < c1->size; i++)
		{
			for (int j = 0; j < c2->size; j++)
			{
				//vzdalenost dvou objektu
				current = obj_distance(&c1->obj[i], &c2->obj[j]);
				//pokud je finalni vzdalenost vetsi nez prubezna (vzdy pri prvnim pruchodu) tak ulozim nove nalezenou vzdalenost
				if (final < current)
				{
					final = current;
				}
			}
		}

		distance = final;
	}

	// -- avg
	else {
		int numberOfPasstroughs;
		float total;
		//nastaveni pocatecni velikosti promennych
		numberOfPasstroughs = 0;
		total = 0;
		//cykly pro pocitani vzdalenosti dvou shluku
		for (int i = 0; i < c1->size; i++) {
			for (int j = 0; j < c2->size; j++) {

				//vzorec vychazi z metody unweighted pair-group average; pocitam celkovou vzdalenost shluku
				total = total + obj_distance(&c1->obj[i], &c2->obj[j]);

				//pocet pruchodu
				numberOfPasstroughs++;
			}
		}

		//vychazi z unweighted pair-group average - prumer
		distance = total / numberOfPasstroughs;

	}

	//vracim vypoctenou vzdalenost
	return distance;

}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/


void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
	assert(narr > 0);

	//inicializace promennych
	float current;
	float final;

	//maximalni vzdalenost; pythagorova veta
	final = sqrtf(SIDE * SIDE + SIDE * SIDE);

	for (int i = 0; i < narr; i++) {
		for (int j = 0; j < narr; j++) {

			//vynecham stejne shluky
			if (i != j) {

				//prubezna vzdalenost
				current = cluster_distance(&carr[i], &carr[j]);

				//pokud je prubezna vzdalenost mensi jak finalni (plati vzdy pri prvni pruchudu)
				if (current < final) {
					//ukladani nove nejkratsi vzdalenosti
					final = current;
					//ukladani nalezenych shluku
					*c1 = i;
					*c2 = j;
				}
			}
		}
	}
}



/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
	for (int i = 0; i < c->size; i++)
	{
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);

	//inicializace promennych
	FILE *file;
	int fileClusters;
	int id;
	float x, y;
	int numberOfLines = 0;

	//otevrit soubor (pokud se nepodari tak konec)
	if ((file = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Soubor neotevren!\n");
		return -1;
	}

	//nacteni postu shluku v souboru (pokud se nepodari tak konec)
	if (fscanf(file, "count=%d", &fileClusters) != 1) {
		fprintf(stderr, "Spatny format souboru!\n");

		//nepodari se zavrit soubor
		if (fclose(file) == EOF) {
			fprintf(stderr, "Soubor nezavren!\n");
		}
		return -1;
	}

	//pokud je v souboru count mensi nebo rovno nule tak konec
	if (fileClusters <= 0) {
		fprintf(stderr, "Count se nesmi rovnat (nebo byt mensi) nule!\n");
		if (fclose(file) == EOF) {
			fprintf(stderr, "Soubor nezavren!\n");
		}
		return -1;
	}

	//alokace pameti
	*arr = malloc(sizeof(struct cluster_t) * fileClusters);

	// pokud se alokace pameti nezdari, tak konec
	if (*arr == NULL) {
		fprintf(stderr, "Pamet nealokovana!\n");
		if (fclose(file) == EOF) {
			fprintf(stderr, "Soubor nezavren!\n");
		}
		return -1;
	}


	for (int i = 0; i < fileClusters; i++) {

		//inicializace shluku
		init_cluster(&(*arr)[i], 1);
		//velikost shluku
		(*arr)[i].size = 1;
		//pocet nactenych radku
		numberOfLines++;

		// pokud se alokace pameti pro objekt nepodarila, tak konec
		if ((*arr)[i].obj == NULL) {

			fprintf(stderr, "Pamet nealokovana!\n");
			if (fclose(file) == EOF) {
				fprintf(stderr, "Soubor nezavren!\n");
			}
			return -1;
		}

		//nacteni id a souradnic shluku, pokud je neco spatne, tak cistim nactenou pamet a konec
		if (fscanf(file, "%d %f %f", &id, &x, &y) != 3) {
			fprintf(stderr, "Nelze nacist souradnice!\n");

			for (int i = 0; i < numberOfLines; i++) {
				clear_cluster(&(*arr)[i]);
			}

			free(*arr);

			if (fclose(file) == EOF)
				fprintf(stderr, "Soubor nezavren!\n");
			return -1;

		}
		else {

			//pokud jsou souradnice mimo zvolenou mez (0 - 1000), nebo nejsou cela cisla, tak cistim pamet a konec
			if (x < 0 || y < 0 || x > 1000 || y > 1000 || x - roundf(x) != 0 || y - roundf(y) != 0) {
				fprintf(stderr, "Neplatne souradnice objektu!\n");

				for (int i = 0; i < numberOfLines; i++) {
					clear_cluster(&(*arr)[i]);
				}

				free(*arr);

				if (fclose(file) == EOF)
					fprintf(stderr, "Soubor nezavren!\n");
				return -1;
			}

			//pokud jsou vsechny podminky splenny, tak ulozim nactene hndnoty
			else {

				//ukladam ID
				(*arr)[i].obj->id = id;
				//ukladam X souradnici
				(*arr)[i].obj->x = x;
				//ukladam Y souradnici
				(*arr)[i].obj->y = y;
			}

		}
	}

	if (fclose(file) == EOF) { //uzavreni souboru s overenim

		fprintf(stderr, "Soubor nezavren!\n");
		return -1;
	}

	return numberOfLines;   // vraci pocet nactenych radku


}
/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
	printf("Clusters:\n");
	for (int i = 0; i < narr; i++)
	{
		printf("cluster %d: ", i);
		print_cluster(&carr[i]);
	}
}

//kontrola jestl je zadany argument cislo
int isNumber(char* word) {
	//inicializace promennych
	int lenght;
	//spocitam delku zadaneho argumentu
	lenght = strlen(word);
	//cyklem prochazim jednotlive znaky argumentu a kontroluji jestli jsou to cisla
	for (int i = 0; i < lenght; i++) {
		//pokud to nejsou cisla, tak vracim 1 -> chyba
		if (isdigit(word[i]) == 0) {
			return 1;
		}
	}
	//pokud je vse v poradku tak vracim nulu
	return 0;
}

int main(int argc, char *argv[])
{
	// pole shluku
	struct cluster_t *clusters;
	//pocet cilovych shluku
	int clustersWantedByUser;
	//pocet nactenych shluku ze souboru
	int clustersInFile;

	//musim zadat minimalne dva argumenty, maximalne ctyri (premium_case)
	if (argc >= 2 && argc <= 4) {

		// kdyz neni zadan argument N, priradi se mu hodnota 1
		if (argc == 2) {
			clustersWantedByUser = 1;
		}

		else if (argc == 3)
		{
			//pokud je zadany argument cele nenulove cislo, tak ok
			if (isNumber(argv[2]) == 0) {
				clustersWantedByUser = atoi(argv[2]);
				//pokud je vsak nula tak konec
				if (clustersWantedByUser == 0) {
					fprintf(stderr, "Argument se nesmi rovnat nule.\n");
					return 1;
				}
			}
			else {
				fprintf(stderr, "Uzivatelem zadany pocet shluku neni v poradku.\n");
				return 1;
			}
		}

		//premium case
		else if (argc == 4)
		{
			//pokud je zadany argument cele nenulove cislo, tak ok
			if (isNumber(argv[2]) == 0) {
				clustersWantedByUser = atoi(argv[2]);
				//pokud je vsak nula tak konec
				if (clustersWantedByUser == 0) {
					fprintf(stderr, "Argument se nesmi rovnat nule.\n");
					return 1;
				}

				//pokud uzivatel zvoli metodu --avg, tak premium_case je jedna
				if (strcmp("--avg", argv[3]) == 0) {
					premium_case = 1;
				}

				//pokud uzivatel zvoli metodu --min, tak premium_case je dva
				else if (strcmp("--min", argv[3]) == 0) {
					premium_case = 2;
				}

				//pokud uzivatel zvoli metodu --max, tak premium_case je tri
				else if (strcmp("--max", argv[3]) == 0) {
					premium_case = 3;
				}
				//pokud zada cokoliv jineho tak je to spatne a program konci
				else {
					fprintf(stderr, "Posledni argument neni v poradku.\n");
					return 1;
				}

			}

			//uzivatel zadal napr. 10.33
			else {
				fprintf(stderr, "Uzivatelem zadany pocet shluku neni v poradku.\n");
				return 1;
			}
		}
	}
	// neplatny pocet argumentu -> chybove hlaseni
	else {
		fprintf(stderr, "Spusteni je mozne pouze v nasledujici podobe: ./proj3 soubor [N]\n");
		return 1;
	}

	//nacteni shluku ze souboru
	clustersInFile = load_clusters(argv[1], &clusters);

	//pokud je pocet nactenych shluku mensi jak jedna tak se nepodarilo nacist shluky a koncim
	if (clustersInFile < 0) {
		fprintf(stderr, "Nepodarilo se nacist shluky\n");
		return 1;
	}

	// pocet cilovych shluku nesmi byt vetsi nez pocet shluku v souboru, pokud neni podminka splnena, tak cistim pamet a koncim
	if (clustersWantedByUser > clustersInFile)
	{
		fprintf(stderr, "Pozadujete vic shluku nez je mozno.\n");
		for (int i = 0; i < clustersInFile; i++) {
			clear_cluster(&clusters[i]);
		}

		free(clusters);
		return 1;
	}


	//inicializace pomocnych promennych
	int firstCluster, secondCluster;

	//dokud je pocet shluku v souboru vetsi nez pocet shluku pozadovanych uzivatelem tak provadim nasledujici
	while (clustersInFile > clustersWantedByUser) {

		//hledam souseda
		find_neighbours(clusters, clustersInFile, &firstCluster, &secondCluster);

		//spojuji shluky
		merge_clusters(&clusters[firstCluster], &clusters[secondCluster]);

		//odstranuji slucovany
		clustersInFile = remove_cluster(clusters, clustersInFile, secondCluster);
	}

	//tisk nalezenych shluku
	print_clusters(clusters, clustersInFile);

	//cisteni pameti
	for (int i = 0; i < clustersInFile; i++) {
		clear_cluster(&clusters[i]);
	}

	free(clusters);

	return 0;
}