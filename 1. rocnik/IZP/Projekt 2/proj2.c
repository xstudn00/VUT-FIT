/*
* Projekt c. 2
* Vytvoril: Ondrej Studnicka (xstudn00)
* VUT FIT BIT 2017-2020
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

//vypis funkce napoveda
void napoveda(){
		printf("Jak pouzivat program:\n"
		"--help - prikaz pro vytisknuti napovedy\n\n"
		"--tan  \t- prikaz vytiskne na obrazovku hodnoty funkce tangens\n"
		"\t- je treba zadat velikost uhlu (radiany) v intervalu (0 ; 1.4>\n"
		"\t- je treba zadat pocet iteraci v rozmezi <1 ; 13>\n"
		"\t- vysledek se zobrazi v nasledujicim poradi:\n"
		"\t- pocet iteraci, vysledek tan z knihovny math.h, vysledek dle Taylorova polynomu, odchylka Taylorova polynomu, vysledek zlomku, odchylka zlomku\n\n"
		"-c      - prikaz vypocte vzdalenost (pripadne vysku) pozorovaneho objektu pri zadani vysky pozorovani\n"
		"\t- prikaz se zadava v nasledujicim formatu -c X -m A [B]\n"
		"\t- X znaci vysku pozorovatele v intervalu (0 ; 100>m, A znaci hloubkovy uhel v intervalu (0 ; 1.4> radianu, B znaci vyskovy uhel v intervalu (0 ; 1.4> radianu (je nepovinne)\n"
		"\t- program vytiskne prvni vzdalenost, potom vysku\n\n"
		"-m      - prikaz vypocte vzdalenost (pripadne vysku) pozorovaneho objektu pri pevne stanovene vysce pozorovani 1.5 metru\n"
		"\t- prikaz se zadava v nasledujicim formatu -m A [B]\n"
		"\t-  A znaci hloubkovy uhel v intervalu (0 ; 1.4> radianu, B znaci vyskovy uhel v intervalu (0 ; 1.4> radianu (je nepovinne)\n"
		"\t- program vytiskne prvni vzdalenost, potom vysku\n"
		"Vytvoril: Ondrej Studnicka\n");
}

//funkce pro vypocet Taylorova polynomu
double taylor_tan(double uhel, unsigned int pocatek){
	//citatel z taylorovy rady
	double citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};

	//jmenovatel z taylorovy rady
	double jmenovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
	//inicializace promennych pro pozdejsi vypocet
	double soucetRady = 0, uhelUhel;

	//vypocet druhe mocniny uhlu
	uhelUhel=uhel*uhel;

	//cyklus pro vypocet taylorova polynomu
	for(unsigned int n = 0; n < pocatek; n++){

		//soucet jednotlivych zlomku z taylorovy rady
		soucetRady += uhel * citatel[n] / jmenovatel[n];

		//umocneni uhlu
		uhel *= uhelUhel;
	}

	//vracim soucet rady
	return soucetRady;
}

//funkce pro vypocet zretezenoho zlomku
double cfrac_tan(double uhel, unsigned int pocatek){
	//inicializace promennych pro pozdejsi vypocet	
	double zlomek = 0.0;

	//citatel je roven druhe mocnine zadaneho uhlu
	double citatel = uhel*uhel;

	//cyklus pro vypocet zretezenych zlomku
	for(int n = pocatek-1; n >= 1; n--){
		//pri kazdem pruchodu cyklem se pricte dvojka
		int celeCislo = (n+1)*2-1;
		//rovnice pro vypocet zretezeneho zlomku
		zlomek = citatel / (celeCislo - zlomek);
	}	

	//vracim vysledek zretezeneho zlomku
	return uhel/(1.0 - zlomek);
}


//vypis hodnot funkce tangens v porovnani s mnou dopocitanymi hodnotami
void vypisTangens(int prvniIterace, int posledniIterace, double uhel){
	//cyklus pro dopocitani pozadovanych hodnot
	for(int pocatek = prvniIterace; pocatek <= posledniIterace; pocatek++){
		//inicializace promennych
		double vysledekTaylor, vysledekZlomek;

		//pocitam pomoci funkce taylor_tan
		vysledekTaylor = taylor_tan(uhel, pocatek);
		//pocitam pomoci funkce cfrac_tan
		vysledekZlomek = cfrac_tan(uhel, pocatek);
		//vypis dopocitanych hodnot z jednotlivych iteraci
		printf("%d %e %e %e %e %e\n", pocatek, tan(uhel), vysledekTaylor, fabs((tan(uhel)-vysledekTaylor)), vysledekZlomek, fabs((tan(uhel)-vysledekZlomek)));
	}

}

//vypocet vzdalenosti pomoci vlastni funkce
double vypocetVzdalenosti(double vyskaPozorovatele, double uhelAlfa){
	//pocet iteraci pro pozadovanou presnost
	int pocetIteraci = 10;
	//inicializace promenne
	double vzdalenost;
	//vzorec pro vypocet vzdalenosti predmetu od pozorovatele
	vzdalenost = vyskaPozorovatele / cfrac_tan(uhelAlfa, pocetIteraci);
	//vracim vzdalenost
	return vzdalenost;
}

//vypocet vysky pomoci vlastni funkce
double vypocetVysky(double vyskaPozorovatele, double uhelBeta, double vzdalenost){
	//pocet iteraci pro pozadovanou presnost
	int pocetIteraci = 10;
	//inicializace promenne
	double vyska;
	//vzorec pro vypocet vysky predmetu
	vyska=cfrac_tan(uhelBeta, pocetIteraci) * vzdalenost + vyskaPozorovatele;
	//vracim vysku
	return vyska;
}

//funkce pro zjisteni jestli uzivatel zadal jenom cisla(nebo tecku nebo minus)
int jsiCislo(char* slovo){
	//inicializace promennych
	int delka, flagTecka = 0, flagMinus=0, flagE=0;
	//spocitam delku zadaneho argumentu
	delka = strlen(slovo);
	//cyklem prochazim jednotlive znaky argumentu a kontroluji jestli jsou to cisla
	for(int i=0; i<delka; i++){
		//pokud to nejsou cisla nebo to neni tecka nebo to neni minus, tak vracim 1 -> chyba
		if(isdigit(slovo[i])==0 && slovo[i] != '.' && slovo[i] != '-' && slovo[i] != 'e'){				
			return 1;
		}
		//jinak kontroluji tecky a minuska jestli jsou zadane opravdu jenom jednou
		else{
			if(slovo[i]=='.'){
				flagTecka++;
			}

			if(slovo[i]=='-'){
				flagMinus++;
			}

			if(slovo[i]=='e'){
				flagE++;
			}
		}

	}
	//pokud je v argumentu tecka vic nez jednou tak vracim 1 -> chyba
	if(flagTecka >1){
		return 1;
	}

	if(flagE >1){
		return 1;
	}
	//pokud je v argumentu minus vic nez jednou tak vracim 1 -> chyba
	if(flagMinus >1){
		return 1;
	}
	//pokud je vse v poradku tak vracim nulu
	return 0;
}

//hlavni funkce main
int main(int argc, char* argv[]){
	//kontroluji jestli uzivatel vubec neco zadal, pokud ne, tak davam k dispozici napovedu
	if(!argv[1]){
		fprintf(stderr, "Nevolal jsi zadnou funkci! (Zavolej: --help, --tan, -c, -m)\n");
		return 1;
	}
	//pokud zadal, tak pokracuju tady
	else{
		//inicializace promennych
		int prvniIterace, posledniIterace;
		char zvolenaMoznost[7]; // sedm protoze --help (6) + 1 (\0)

		//kopiruju prvni argument do promenne pro snazsi praci
		strcpy(zvolenaMoznost, argv[1]);

		//pokud uzivatel zavolal funkci --help, tak mu zobrazim navod na pouzivani programu
		if(strcmp(zvolenaMoznost, "--help")==0){
			napoveda();
		}

		//pokud uzivatel zavolal funkci --tan tak pokracuji tady
		else if(strcmp(zvolenaMoznost, "--tan")==0){
			//uzivatel musi zadat presne pet argumentu, jinak je funkce neproveditelna
			if(argc!=5){
				fprintf(stderr, "Spatny pocet argumentu! Zadej 4 argumenty.\n");
				return 1;
			}


			else{
			double uhel;

				//kontroluju jestli uzivatel zadal cisla
				if(jsiCislo(argv[2])==0 && jsiCislo(argv[3])==0 && jsiCislo(argv[4])==0){
					//prevod argumentu na cisla
					uhel =  atof(argv[2]);
					prvniIterace = atoi(argv[3]);
					posledniIterace = atoi(argv[4]);

					//kontroluji jestli zadane argumenty splnuji vsechny pozadavky
					if(posledniIterace>=prvniIterace &&  posledniIterace<14 && prvniIterace>0){
						//vypisuji hodnoty tangens
						vypisTangens(prvniIterace, posledniIterace, uhel);
					}
					//pokud uzivatel nesplnil vsechny pozadavky, dostava informaci, jak spravne spustit program
					else{
						fprintf(stderr, "Prvni iterace musi byt mensi nez posledni, maximalni pocet iteraci je 13,\n minimalni pocet iteraci je jedna, uhel musi byt v intervalu (0;1.4>\n");
						return 1;
					}
				}				
				else{
					//upozorneni ze uzivatel nezadal pouze cisla
					fprintf(stderr, "Nektery za zadanych argumentu neni cislo, nebo nema pozadovany format!\n");
					return 1;
				}
			}
		}

		//uzivatel zvolil funkci pro mereni s nastavenim vlastni vysky
		else if(strcmp(zvolenaMoznost, "-c")==0 && strcmp(argv[3], "-m")==0){
			
			//pokud uzivatel nezadal minimalni pocet argumentu, tak je upozornen
			if(argc<5){
			fprintf(stderr, "Spatny pocet argumentu! Zadej 4 a vice argumentu.\n");
			return 1;
			}

			else{
				//kontroluji jestli uzivatel zadal cisla
				if(jsiCislo(argv[2])==0 && jsiCislo(argv[4])==0){
					//inicializace promennych
					double vyskaPozorovatele, uhelAlfa, vzdalenost;

					//prevod argumentu na cisla
					vyskaPozorovatele = atof(argv[2]);
					uhelAlfa = atof(argv[4]);	

					//kontrola jestli zadane argumenty splnuji vsechny pozadavky 
					if(vyskaPozorovatele > 0 && vyskaPozorovatele<= 100 && uhelAlfa > 0 && uhelAlfa<=1.4){	
						//volam funkci pro vypocet vzdalenosti a tisknu vysledek
						vzdalenost = vypocetVzdalenosti(vyskaPozorovatele, uhelAlfa);
						printf("%.10e\n", vzdalenost);

						//kontroluji jestli uzivatel chce spocitat i vysku predmetu -> jestli existuje dalsi argument
						if(argv[5]){
							if(jsiCislo(argv[5])==0){
								//inicializace promenne
								double uhelBeta, vyska;
								
								//prevod argumentu na cislo
								uhelBeta = atof(argv[5]);

								//kontrola jestli zadany argument splnuje vsechny pozadavky
								if(uhelBeta > 0 && uhelBeta<=1.4){

									//volam funkci pro vypocet vysky a tisknu vysledek
									vyska = vypocetVysky(vyskaPozorovatele, uhelBeta, vzdalenost);
									printf("%.10e\n", vyska);
								}

								//uzivatel zadal uhel, ktery neni v pozadovanem intervalu
								else{
									fprintf(stderr, "Uhel beta je v intervalu (0;1.4>!\n");
									return 1;
								}
							}

							//uzivatel nezadal cislo 
							else{
								fprintf(stderr, "Uhel beta neni cislo nebo nema pozadovany format!\n");
								return 1;
							}
						}
					}

					//vyska pozorovatele/uhel neni v pozadovanem intervalu
					else{
						fprintf(stderr, "Vyska pozorovatele je v intervalu (0;100>, uhel alfa je v intervalu (0;1.4>!\n");
						return 1;
					}
				}

				//uzivatel nezadal cislo
				else{
					fprintf(stderr, "Vyska pozorovatele/uhel alfa neni cislo nebo nema pozadovany format!\n");
					return 1;
				}
			}
		}

		//uzivatel zvolil funkci pro mereni vzdalenosti s pevne nastavenou vyskou
		else if(strcmp(zvolenaMoznost, "-m")==0){

			//kontrola jestli uzivatel zadal pozadovany pocet argumentu
			if(argc<3){
				fprintf(stderr, "Spatny pocet argumentu! Zadej 2 a vice argumentu.\n");
				return 1;
			}
			else{
				if(jsiCislo(argv[2])==0){
					//inicializace promennych
					double vyskaPozorovatele = 1.5, uhelAlfa, vzdalenost, vyska;

					//prevod argumentu na cislo
					uhelAlfa = atof(argv[2]);

					//kontrola jestli zadany argument splnuje pozadavky
					if(uhelAlfa > 0 && uhelAlfa<=1.4){

						//vypocet a tisk vypoctene vzdalenosti
						vzdalenost = vypocetVzdalenosti(vyskaPozorovatele, uhelAlfa);
						printf("%.10e\n", vzdalenost);

						//kontrola jestli chce uzivatel taky vypocitat vysku predmetu
						if(argv[3]){
							//kontrola jestli uzivatel vlozil cislo
							if(jsiCislo(argv[3])==0){
								//inicializace promenne
								double uhelBeta;

								//prevod argumentu na cislo
								uhelBeta = atof(argv[3]);

								//kontrola jestli argument splnuje pozadavky
								if(uhelBeta > 0 && uhelBeta<=1.4){
									//vypocet vysky predmetu a tisk vypocitane vysky
									vyska = vypocetVysky(vyskaPozorovatele, uhelBeta, vzdalenost);
									printf("%.10e\n", vyska);
								}

								//upozorneni ze uzivatel nezadal uhel v pozadovanem intervalu
								else{
									fprintf(stderr, "Uhel beta je v intervalu (0;1.4>!\n");
									return 1;
								}
							}

							//upozorneni ze uzivatel nezadal cislo
							else{
							fprintf(stderr, "Uhel beta neni cislo nebo nema pozadovany format!\n");
							return 1;
							}
						}
					}

					//upozorneni ze uzivatel nezadal uhel v pozadovanem intervalu
					else{
						fprintf(stderr, "Uhel alfa je v intervalu (0;1.4>!\n");
						return 1;
					}
				}

				//upozorneni ze uzivatel nezadal cislo
				else{
					fprintf(stderr, "Uhel alfa neni cislo nebo nema pozadovany format!\n");
					return 1;
				}
			}
		}

		//upozorneni ze uzivatel spatne zavoal funkci
		else{
			printf("Spatne zvolena moznost\n");
			return 1;
		}
	}
	return 0;
}