/*
* Projekt c. 1
* Vytvoril: Ondrej Studnicka (xstudn00)
* VUT FIT BIT 2017-2020
*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>


//prevod na velka pismena
char *naVelke(char *slovo){							
	unsigned char *noveSlovo = (unsigned char *)slovo;

	while (*noveSlovo){
		*noveSlovo = toupper(*noveSlovo);
		noveSlovo++;
	}
	return slovo;
}	


//zjistuju jestli uzivatel vlozil pouze pismena
int jePismeno(char* porovnavanyVyraz){

	//projdi vsechny znaky porovnavaneho retezce(argumentu)
    for(int i = 0, len = strlen(porovnavanyVyraz); i < len; i++){

    	//pokud vlozeny znak neni pismeno a zaroven neni mezera a zaroven neni pomlcka (pro pripad frydek-mistek) a zaroven neni cislo (praha 2)tak vrat jednicku -> chyba
        if(isalpha(porovnavanyVyraz[i]) == 0 && porovnavanyVyraz[i] !=' ' && porovnavanyVyraz[i] !='-' && isdigit(porovnavanyVyraz[i]) == 0){
            return 1;
        }
    }
    return 0;
}




//vypisovani nalezenych vysledku
void nalezenyVysledek(int pocetShod, char* nalezeneMesto, int* moznaPismena, int vypsano){
	//nebyla nalezena zadna shoda
	if(pocetShod==0){
		if(vypsano!=1){
			printf("Not found\n");
		}
	}

	//bylo nalezeno prave jedno mesto
	if(pocetShod==1){
		if(vypsano!=1){
			printf("Found: %s\n", nalezeneMesto);
		}
	}

	//bylo nalezeno vice shodnych mest, tisknu mozna pismena
	if(pocetShod>1){
		printf("Enable: ");
		for(int pismeno=0; pismeno < 91; pismeno++){
			if(moznaPismena[pismeno]>0){
			printf("%c", pismeno);
			}
		}
	printf("\n");
	}
}



int main(int argc, char* argv[]){
	//pomoci poceShod budu pocitat pocet nalezenych adres
	int pocetShod=0; 

	//pocet velkych pismen v abecede, defaultne vsechno nastaveno na nula
	int moznaPismena[91]={0}; 

	//vstupni mesta ze souboru
	char nacteneMesto[101];

	//shodna mesta	
	char nalezeneMesto[101];

	//do promenne porovnavanyVyraz kopiruju argument	 
	char porovnavanyVyraz[101];

	//delka argumentu
	int delkaArgumentu;

	//kontrola jestli uz byl retezec vypan
	int vypsano = 0;

	/*
	pokud existuje pouze jeden argument (nazev souboru), tak pokracuj v teto vetvi
	prvni znak je zaroven konec slova
	delka vlozeneho argumentu je 0
	*/

	if(argc==1){
		porovnavanyVyraz[0]='\0';
		delkaArgumentu = 0;
	}

	/*
	pokud jsou zadany vice nez dva argumenty (nazev souboru a hledane mesto), tak pokracuj tady
	vystrazna hlaska, mnoho argumentu, s tim neumim pracovat
	vystup na stderr
	*/
	else if(argc>2){
		fprintf(stderr, "Vlozil jste mnoho argumentu!\n");
		return 1;
	}

	/*
	pokud je uzivatelem zadan jeden argument, tak pokracuj tady
	zjisti jestli zadany argument je pismeno
	jestli ano, tak zkopiruj argument zadany uzivatelem do promenne porovnavanyVyraz, lepe se s tim pracuje
	spocitam delku argumentu

	jestli ne tak vypis ze je potreba zadavat jenom pismena a ukonci program
	*/
	else{
		if(jePismeno(argv[1])==0){		 
			strcpy(porovnavanyVyraz, argv[1]); 
			delkaArgumentu = strlen(porovnavanyVyraz);
		}
		else{
			fprintf(stderr, "Zadal jste neocekavane znaky (napr. tecku, procento, ...)!\n");
			return 1;
		} 
	}

	/*
	prohledavam soubor a nacitam znaky do promenne a dokud nedojdu na konec souboru
	zjistuji jestli se nactena mesta skladaji pouze z pismen (nebo cisel nebo pomlcky nebo mezery)
	prevadim vsechna pismena z nactenych mest na velke pismena, aby byl program case-insensitive
	prevadim vsechna pismena ze zedaneho argumentu(respektive promenne porovnavanyVyraz) na velka, a byl program case-insensitive
	podminka, funkce strncmp - slovo vyhledavane uzivatelem, nactene slovo ze seznamu adres, pocet porovnavanych znaku; pokud se znaky
	sobe rovnaji podminka plati a pokracuje dale do vetve jinak se vraci cyklus na zacetek

	ukladam nalezene slovo
	pokud je nalezene mesto do pismene shodne se zadanym argumentem tak ho vytisknu, nastavim vypsano na hodnotu jedna abych
	pak netiskl Found: 2x

	pocitam kolik slov jsem nalezl
	inkrementuju moznaPismena -diky tomuhle jsem schopny usporadat pismena podle abecedy, zvyraznuju nabizene pismeno

	pokud soubor obsahuje nepodporovane znaky tak konec
	*/
	

	while(scanf("%100[^\n]%*c", nacteneMesto) != EOF){		

			if(jePismeno(nacteneMesto)==0){	
				naVelke(nacteneMesto); 
				naVelke(porovnavanyVyraz);

				if((strncmp(porovnavanyVyraz, nacteneMesto, delkaArgumentu)) == 0){
					strcpy(nalezeneMesto,nacteneMesto); 

					if(strcmp(porovnavanyVyraz,nalezeneMesto)==0) {
						printf("Found: %s\n", nalezeneMesto);
						vypsano=1;
					}
					
					pocetShod++; 
					moznaPismena[nacteneMesto[delkaArgumentu]+0]++; 
				}
			}

			else{
				fprintf(stderr, "Soubor s adresami obsahuje neocekavane znaky (napr. tecku, procento, ...)!\n");
				return 1;
			}
	}



	
	nalezenyVysledek(pocetShod, nalezeneMesto, moznaPismena, vypsano);

	
	return 0;
} 