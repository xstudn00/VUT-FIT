------------------------------------------------
-- IDS projekt
-- Ond�ej Studni�ka, V�clav Trampe�ka
-- 25. 4. 2019
------------------------------------------------

------------------------------------------------
--      DROP TABLES, PROCEDURES AND VIEW
------------------------------------------------
DROP TABLE multikino CASCADE CONSTRAINTS;
DROP TABLE zamestnanec CASCADE CONSTRAINTS;
DROP TABLE film CASCADE CONSTRAINTS;
DROP TABLE sal CASCADE CONSTRAINTS;
DROP TABLE promitani CASCADE CONSTRAINTS;
DROP TABLE zakaznik CASCADE CONSTRAINTS;
DROP TABLE rezervace CASCADE CONSTRAINTS;
DROP TABLE listek CASCADE CONSTRAINTS;

DROP PROCEDURE vekove_skupiny_zakazniku_procentualne;
DROP PROCEDURE celkova_trzba_jednotlivych_filmu;

--DROP MATERIALIZED VIEW multikino_view;

------------------------------------------------
--              CREATE TABLES
------------------------------------------------
CREATE TABLE multikino
(
    id_kino NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    mesto VARCHAR2(50) NOT NULL,
    ulice VARCHAR2(100) NOT NULL,
    cislo_popisne NUMBER(4),
    postovni_smerovaci_cislo NUMBER NOT NULL CHECK(REGEXP_LIKE(postovni_smerovaci_cislo,'^[0-9]{5}$'))    
);

CREATE TABLE zamestnanec
(           
    rodne_cislo NUMBER NOT NULL CHECK(REGEXP_LIKE(rodne_cislo,'^([0-9]{2})(01|02|03|04|05|06|07|08|09|10|11|12|51|52|53|54|55|56|57|58|59|60|61|62)(([0]{1}[1-9]{1})|([1-2]{1}[0-9]{1})|([3]{1}[0-1]{1}))([0-9]{3,4})$')) PRIMARY KEY,
    jmeno VARCHAR2(50) NOT NULL,
    prostredni_jmeno VARCHAR2(50) NOT NULL,
    prijmeni VARCHAR2(70) NOT NULL,
    datum_narozeni DATE NOT NULL,
    mesto VARCHAR2(50) NOT NULL,
    ulice VARCHAR2(100) NOT NULL,
    cislo_popisne NUMBER(4),
    postovni_smerovaci_cislo NUMBER NOT NULL CHECK(REGEXP_LIKE(postovni_smerovaci_cislo,'^[0-9]{5}$')),
    login VARCHAR2(20) NOT NULL,
    heslo VARCHAR2(255) NOT NULL,    
    postaveni VARCHAR(1) CHECK (postaveni='v' or postaveni=NULL),
    id_kino NUMBER NOT NULL,
    CONSTRAINT zamestnanec_FK_multikino FOREIGN KEY (id_kino) REFERENCES multikino(id_kino)
);

CREATE TABLE film
(
    id_film NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    nazev VARCHAR2(50) NOT NULL,
    zanr VARCHAR2(50) NOT NULL,
    delka NUMBER(3) NOT NULL,
    jazyk VARCHAR2(15) NOT NULL,
    puvod VARCHAR2(3) NOT NULL,
    rezie VARCHAR2(100) NOT NULL,
    obsazeni VARCHAR2(2048) NOT NULL,
    popis LONG NOT NULL,
    pristupny VARCHAR2(3) NOT NULL CHECK (pristupny='Ano' or pristupny='Ne')    
);

CREATE TABLE sal
(
    id_sal NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    kapacita NUMBER NOT NULL,
    id_kino NUMBER NOT NULL,
    CONSTRAINT sal_FK_multikino FOREIGN KEY (id_kino) REFERENCES multikino(id_kino)
);

CREATE TABLE promitani
(   
    id_promitani NUMBER NOT NULL PRIMARY KEY,
    --id_promitani NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY, -> zmena kvuli pozadovanemu triggeru
    datum DATE NOT NULL,
    cas TIMESTAMP NOT NULL,
    id_film NUMBER NOT NULL,
    id_sal NUMBER NOT NULL,
    CONSTRAINT promitani_FK_film FOREIGN KEY (id_film) REFERENCES film(id_film),
    CONSTRAINT promitani_FK_sal FOREIGN KEY (id_sal) REFERENCES sal(id_sal)
);

CREATE TABLE zakaznik
(
    id_zakaznik NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    jmeno VARCHAR2(50),
    prostredni_jmeno VARCHAR2(50),
    prijmeni VARCHAR2(70),
    telefon VARCHAR2(15) CHECK(REGEXP_LIKE(telefon,'^\+(420|421)[0-9]{9}$')),
    email VARCHAR2(320) CHECK(REGEXP_LIKE(email,'^\w.+@\w.+(\.)(cz|sk|com)$')),
    vekova_skupina VARCHAR2(15) NOT NULL CHECK (vekova_skupina='D�t�' or vekova_skupina='Student' or vekova_skupina='Dosp�l�' or vekova_skupina='D�chodce' or vekova_skupina='ZTP'),
    rodne_cislo NUMBER,
    CONSTRAINT zakaznik_FK_zamestnanec FOREIGN KEY (rodne_cislo) REFERENCES zamestnanec(rodne_cislo)
);


CREATE TABLE rezervace
(
    id_rezervace NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    datum_rezervace DATE NOT NULL,
    aktivni NUMBER NOT NULL CHECK (aktivni = 0 or aktivni = 1),
    id_zakaznik NUMBER NOT NULL,
    id_promitani NUMBER NOT NULL,
    CONSTRAINT rezervace_FK_zakaznik FOREIGN KEY (id_zakaznik) REFERENCES zakaznik(id_zakaznik),
    CONSTRAINT rezervace_FK_promitani FOREIGN KEY (id_promitani) REFERENCES promitani(id_promitani)
);

CREATE TABLE listek
(
    id_listek NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    datum_prodeje DATE NOT NULL,
    rada NUMBER NOT NULL,
    sedadlo NUMBER NOT NULL,
    cena NUMBER NOT NULL,
    rodne_cislo NUMBER NOT NULL,
    id_promitani NUMBER NOT NULL,
    id_zakaznik NUMBER NOT NULL,
    id_rezervace NUMBER,
    CONSTRAINT listek_FK_zamestnanec FOREIGN KEY (rodne_cislo) REFERENCES zamestnanec(rodne_cislo),
    CONSTRAINT listek_FK_promitani FOREIGN KEY (id_promitani) REFERENCES promitani(id_promitani),
    CONSTRAINT listek_FK_zakaznik FOREIGN KEY (id_zakaznik) REFERENCES zakaznik(id_zakaznik),
    CONSTRAINT listek_FK_rezervace FOREIGN KEY (id_rezervace) REFERENCES rezervace(id_rezervace)
);

------------------------------------------
--- projekt 4
------------------------------------------

------------------------------------------
--              TRIGGERS
------------------------------------------

-- trigger na automaticke inkrementovani id_promitani v tabule promitani

DROP SEQUENCE id_promitani_seq;

CREATE SEQUENCE id_promitani_seq START WITH 1 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER promitani_trigger
 BEFORE INSERT ON promitani 
 FOR EACH ROW
BEGIN
  SELECT id_promitani_seq.nextval INTO :NEW.id_promitani FROM DUAL;
END;
/

-- trigger na zji�t�n� jestli je s�l ji� obsazen
-- pokud je s�l obsazen v dobu, kdy chce u�ivatel napl�novat dal��, tak dojde k vyvol�n� vyj�mky

CREATE OR REPLACE TRIGGER obsazenost_salu
BEFORE INSERT ON promitani
FOR EACH ROW
DECLARE je_promitano INTEGER;
BEGIN SELECT NVL((
            SELECT COUNT(*)
            FROM promitani p1 JOIN film ON (film.id_film = p1.id_film)
            WHERE p1.id_sal = :NEW.id_sal 
            AND (
            :NEW.cas BETWEEN p1.cas AND (p1.cas+numToDSInterval( film.delka, 'MINUTE' ) )
            )
            ), 0)
            INTO je_promitano FROM DUAL;
                     
IF je_promitano > 0 THEN
    RAISE_APPLICATION_ERROR(-20100, 'S�l je v tuto dobu ji� vyu�it.');
END IF;
END;
/

-----------------------------------
-- PROCEDURES
-----------------------------------

-- povolen� v�pis�
SET serveroutput ON;


-- procedura na v�pis percentu�ln�ho zastoupen� r�zn�ch v�kov�ch skupin z�kazn�k� v datab�zi

CREATE OR REPLACE PROCEDURE vekove_skupiny_zakazniku_procentualne AS
  CURSOR zakaznici IS 
    SELECT * FROM zakaznik 
    WHERE vekova_skupina IS NOT NULL;

  zkznk zakaznik%ROWTYPE;
  celkem NUMBER := 0;
  ZTP NUMBER := 0;
  duchodci NUMBER := 0;
  dospeli NUMBER := 0;
  studenti NUMBER := 0;
  deti NUMBER := 0;

  BEGIN
    OPEN zakaznici;
    LOOP
      FETCH zakaznici INTO zkznk;
      EXIT WHEN zakaznici%NOTFOUND;
      
      celkem := celkem+1;

      IF zkznk.vekova_skupina = 'ZTP' THEN
        ZTP := ZTP+1;
      ELSIF zkznk.vekova_skupina = 'D�chodce' THEN
        duchodci := duchodci+1;
      ELSIF zkznk.vekova_skupina = 'Dosp�l�' THEN
        dospeli := dospeli+1;
      ELSIF zkznk.vekova_skupina = 'Student' THEN
        studenti := studenti+1;
      ELSIF zkznk.vekova_skupina = 'D�t�' THEN
        deti := deti+1;
      END IF;
    END LOOP;  

    IF celkem = 0 THEN
      RAISE NO_DATA_FOUND;
    END IF;
    dbms_output.put_line('Procentu�ln� rozd�len� z�kazn�k� podle v�kov�ch skupin:');
    dbms_output.put_line('Deti      : '|| ROUND((deti/celkem)*100,1) ||'%');
    dbms_output.put_line('Studenti  : '|| ROUND((studenti/celkem)*100,1) ||'%');
    dbms_output.put_line('Dospeli   : '|| ROUND((dospeli/celkem)*100,1) ||'%');
    dbms_output.put_line('Duchodci  : '|| ROUND((duchodci/celkem)*100,1) ||'%');
    dbms_output.put_line('ZTP       : '|| ROUND((ZTP/celkem)*100,1) ||'%');
    CLOSE zakaznici;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20101, 'Nebyli nalezeni ��dn� z�kazn�ci.');
    WHEN OTHERS THEN
      RAISE_APPLICATION_ERROR(-20102, 'Chyba, co se nestane.');
  END;
/

-- procedura na zjisteni celkov�ch v�d�lk� jednotliv�ch film� 

CREATE OR REPLACE PROCEDURE celkova_trzba_jednotlivych_filmu AS
    film_id NUMBER;
    promitani_id NUMBER;

  CURSOR cursor_film IS 
    SELECT * FROM film;
  flm film%ROWTYPE;

  CURSOR cursor_promitani IS 
    SELECT * FROM promitani
    WHERE promitani.id_film = film_id;
  prmtn promitani%ROWTYPE;

  CURSOR cursor_listek IS 
    SELECT * FROM listek
    WHERE listek.id_promitani = promitani_id;
  lstk listek%ROWTYPE;

  trzba NUMBER;
  
  BEGIN
    dbms_output.put_line('Celkov� tr�by jednotliv�ch film�:');
    OPEN cursor_film;
    LOOP
      FETCH cursor_film INTO flm;
      EXIT WHEN cursor_film%NOTFOUND;

      trzba := 0;
      film_id := flm.id_film;

      OPEN cursor_promitani;
      LOOP
        FETCH cursor_promitani INTO prmtn;
        EXIT WHEN cursor_promitani%NOTFOUND;

        promitani_id := prmtn.id_promitani;
        
        OPEN cursor_listek;
        LOOP
          FETCH cursor_listek INTO lstk;
          EXIT WHEN cursor_listek%NOTFOUND;

          trzba := trzba+lstk.cena;
          
        END LOOP;
        CLOSE cursor_listek;

      END LOOP;
      CLOSE cursor_promitani;
      dbms_output.put_line(flm.nazev || ':  '|| trzba || 'Kc');
    END LOOP;  

    CLOSE cursor_film;
  END;
/

------------------------------------------------
--           INSERT INTO TABLES
------------------------------------------------

INSERT INTO multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Plze�', 'N�dra�n�', 20, 30100);
INSERT INTO multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Praha', 'Jana Nepomuck�ho', 361, 10000);
INSERT INTO multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Ostrava', 'Hlavn�', 1024, 70030);

INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, postaveni, id_kino) VALUES (6851034606, 'Alex', 'Petr', 'Novotn�', date '1968-01-03', 'Plze�', 'Po�tovn�', 321, 30100, 'novotny', '12345', 'v', 1);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, id_kino) VALUES (9710110321, 'Jakub', 'Jan', 'Slov�k', date '1997-10-11', 'Plze�', 'Sportovn�', 25, 30100, 'slovak', '67890', 1);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, postaveni, id_kino) VALUES (8351293112, 'Alexandra', 'Lucie', 'Proch�zkov�', date '1983-01-29', 'Praha', 'V Kout�', 1021, 10000, 'prochazkova', '13579', 'v', 2);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, id_kino) VALUES (6906302589, 'V�clav', 'David', 'Mach��ek', date '1969-06-30', 'Plze�', 'Okru�n�', 137, 10000, 'machacek', '24680', 2);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, postaveni, id_kino) VALUES (9210240546, 'Stanislav', 'Petr', 'Koutn�', date '1992-10-24', 'Ostrava', 'B��sk�', 42, 70030, 'koutny', '14702', 'v', 3);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, id_kino) VALUES (8358218564, 'Aneta', 'Anna', 'Byst�ick�', date '1983-08-21', 'Ostrava', 'Opavsk�', 256, 70030, 'bystricka', '35671', 3);

INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Nedotknuteln�', 'Komedi�ln� drama', 113, '�esk�', 'FRA', 'Olivier Nakache, Eric Toledano', 'Fran�ois Cluzet, Omar Sy, Anne Le Ny, Audrey Fleurot, Jos�phine de Meaux, Clotilde Mollet, Alba Ga?a Kraghede Bellugi, Cyril Mendy, Salimata Kamate, Gr�goire Oestermann, Dominique Daguier, Fran�ois Caron, Christian Ameri, Thomas Soliv�res, Doroth�e Briere, �milie Caen, Sylvain Lazard, Jean-Fran�ois Cayrey, Fran�ois Bureloup', 'Ochrnut� a bohat� aristokrat Philippe si za sv�ho nov�ho opatrovn�ka vybere Drisse, �iveln�ho mlad�ka z p�edm�st�, kter�ho pr�v� propustili z v�zen�. Jin�mi slovy - najde si na tuto pr�ci tu nejm�n� vhodnou osobu. Poda�� se jim v�ak propojit nemo�n�: Vivaldiho a popul�rn� hudbu, seri�znost a �ovi�ln� vt�pky, luxusn� obleky a tepl�ky. Bl�zniv�m, z�bavn�m, siln�m, neo�ek�van�m a hlavn� �nedotknuteln�m�, p�esn� takov�m se stane jejich p��telstv� Komedie s dramatickou z�pletkou o tom, �e ani od krku po prsty u nohou nepohybliv� �lov�k odk�zan� na pomoc druh�ch, nemus� ztratit smysl �ivota. A o tom, �e i nejm�n� pravd�podobn� spojen� melancholick�ho multimilion��e a extrovertn�ho recidivisty m��e humorn� zap�sobit na div�ka a m��e se z n�j st�t kasovn� trh�k.', 'Ano');
INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Fantastick� zv��ata: Grindelwaldovy zlo�iny', 'Dobrodru�n�', 134, 'anglick�', 'UK', 'David Yates', 'Eddie Redmayne, Katherine Waterston, Dan Fogler, Johnny Depp, Jude Law, Alison Sudol, Ezra Miller, Zo� Kravitz, Claudia Kim, Carmen Ejogo, Jessica Williams, �lafur Darri �lafsson, Callum Turner, Victoria Yeates, Poppy Corby-Tuech, Brontis Jodorowsky, William Nadylam, Fiona Glascott, Derek Riddell, Kevin Guthrie, David Sakurai', 'Na konci prvn�ho filmu KOKUSA (Kouzeln�ck� kongres USA) zajme s pomoc� Mloka Scamandera (Eddie Redmayne) mocn�ho �ernokn�n�ka Gellerta Grindelwalda (Johnny Depp). Grindelwald ale splnil svou hrozbu, �e ute�e z vazby, a za�al shroma��ovat sv� stoupence, z nich� v�t�ina v�bec nic netu�ila o jeho skute�n�ch c�lech: p�ipravit vl�du �istokrevn�ch �arod�j� nad v�emi nemagick�mi bytostmi. Ve snaze zma�it Grindelwaldovy pl�ny, Albus Brumb�l (Jude Law) z�sk� sv�ho b�val�ho studenta Mloka Scamandera, kter� souhlas� s t�m, �e pom��e, ani� by si uv�domoval, jak�mu nebezpe�� se t�m vystavuje. Bitevn� linie jsou vyty�eny a ve st�le v�ce znesv��en�m sv�t� �arod�j� je podrobena zkou�ce l�ska a loajalita, dokonce i mezi nejv�rn�j��mi p��teli a rodinou.', 'Ano');
INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Ml�en� jeh��tek', 'Filmov� drama', 138, '�esk�', 'USA', 'Jonathan Demme', 'Jodie Foster, Anthony Hopkins, Scott Glenn, Anthony Heald, Ted Levine, Frankie Faison, Kasi Lemmons, Brooke Smith, Obba Babatund�, Diane Baker, Roger Corman, Charles Napier, Cynthia Ettinger, Chris Isaak, Daniel von Bargen, Ted Monte, George A. Romero, Tracey Walter, Stuart Rudin, Brent Hinkley, John W. Iwanonkiw, John Hall', 'Clarice Starlingov�, nadan� studentka akademie FBI, je p�id�lena agentu Crawfordovi vy�et�uj�c�mu p��pad masov�ho vraha Buffalo Billa, kter� sv� ob�ti stahuje z k��e. Clarice nav�t�v� v baltimorsk� v�ze�sk� nemocnici b�val�ho vynikaj�c�ho psychiatra Hannibala Lectera, odsouzen�ho na do�ivot� za s�rii brut�ln�ch vra�d a kanibalismus, kter� by o vrahovi mohl n�co v�d�t. Odsouzen� psychiatr Clarice z�rove� d�s� a fascinuje. Kdy� je masov�m vrahem unesena dcera sen�torky Martinov�, nab�dne Clarice Lecterovi p�ijateln�j�� v�ze�sk� podm�nky v�m�nou za informaci, vedouc� k dopaden� Buffalo Billa...', 'Ne');

INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Forrest Gump', 'Filmov� drama', 144, '�esk�', 'USA', 'Robert Zemeckis', 'Tom Hanks, Robin Wright, Gary Sinise, Mykelti Williamson, Sally Field, Haley Joel Osment, Peter Dobson, Siobhan Fallon Hogan, Hanna Hall, Brett Rice, Kirk Ward, Kurt Russell, Stephen Bridgewater, Michael Jace, Michael McFall, Joe Alaskey, Sam Anderson, Jacqueline Lovell, Mary Ellen Trainor, Vanessa Roth, Bryan Hanna, Jason McGuire', 'Zemeckis�v film je brilantn�m shrnut�m dosavadn�ch re�is�rov�ch poznatk� o mo�nostech "comicsov�ho" vypr�v�n�. Form�ln� i obsahov� nejm�n� konven�n� z jeho sn�mk� p�esv�d�uje komedi�ln�mi gagy i nal�hav� patetick�m t�nem. Jeho hrdinou je pros���ek Forrest Gump, typick� oby�ejn� mu�, kter� od d�tstv� d�lal, co se mu �eklo. Do �ivota si tak odnesl n�kolik ponau�en� sv� pe�liv� maminky a osv�d�en� pravidlo, je� se mu hod� mnohokr�t v nejr�zn�j��ch situac�ch: Kdy� se dostane� do probl�m�, ut�kej. Forrest prout�k� �kolou, jako hr�� americk�ho fotbalu i univerzitou, potom peklem vietnamsk� v�lky a zoufalstv�m nad mat�inou smrt�. V�dycky je toti� n�kdo nebo n�co, co po n�m skute�n� �i obrazn� "h�z� kameny". Nakonec v�ak Forrest pozn�v�, �e jsou i jin� �e�en� situac� ne� �t�k. Sv�j �ivot spojuje s kamar�dkou ze �koly Jenny, kter� pro n�j z�stane prov�dycky jedinou l�skou, s p��telem z vojny, �ernochem Bubbou, kter� d� sm�r jeho �vah�m o lovu krevet a s poru��kem Taylorem, jemu� ve Vietnamu zachr�n� �ivot. Forrestova �ivotn� pou� od 50. do 80. let je koncipovan� jako tot�ln� "bezelstn�" vypr�v�n� hrdiny, neschopn�ho obecn�j��ho hodnocen� situace. Forrest jako sportovec i jako v�le�n� hrdina se setk�v� se slavn�mi lidmi, kter� nakonec v�dycky n�kdo zast�el� (J. F. Kennedy, J. Lennon). Tak� jeho bli�n� um�raj�, ale on s�m z�st�v�. Empiricky se dopracov�v� od impulsivn�ho �t�ku p�ed �ivotem k �vah�m o lidsk�m osudu a o Bohu. Soukrom� hrdin�v osud z�rove� postihuje t�icet let pov�le�n�ch americk�ch d�jin.', 'Ano');


INSERT INTO sal(kapacita, id_kino) VALUES (186, 1);
INSERT INTO sal(kapacita, id_kino) VALUES (204, 1);
INSERT INTO sal(kapacita, id_kino) VALUES (298, 2);
INSERT INTO sal(kapacita, id_kino) VALUES (156, 2);
INSERT INTO sal(kapacita, id_kino) VALUES (128, 3);
INSERT INTO sal(kapacita, id_kino) VALUES (256, 3);
INSERT INTO sal(kapacita, id_kino) VALUES (50, 3);


INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-25', timestamp '2019-03-25 15:30:00.00 +01:00', 1, 1);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-26', timestamp '2019-03-26 15:30:00.00 +01:00', 1, 2);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-27', timestamp '2019-03-27 15:30:00.00 +01:00', 1, 3);

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-28', timestamp '2019-03-28 19:30:00.00 +01:00', 2, 1);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-29', timestamp '2019-03-29 19:30:00.00 +01:00', 2, 2);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-30', timestamp '2019-03-30 19:30:00.00 +01:00', 2, 3);

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-03-31', timestamp '2019-03-31 15:30:00.00 +01:00', 3, 1);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-01', timestamp '2019-04-01 15:30:00.00 +01:00', 3, 2);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-02', timestamp '2019-04-02 15:30:00.00 +01:00', 3, 3);

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-03', timestamp '2019-04-03 15:30:00.00 +01:00', 4, 1);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-04', timestamp '2019-04-04 15:30:00.00 +01:00', 4, 2);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-05', timestamp '2019-04-05 15:30:00.00 +01:00', 4, 3);

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-06', timestamp '2019-04-06 19:30:00.00 +01:00', 5, 1);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-07', timestamp '2019-04-07 19:30:00.00 +01:00', 5, 2);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-08', timestamp '2019-04-08 19:30:00.00 +01:00', 5, 3);

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-09', timestamp '2019-04-09 20:30:00.00 +01:00', 6, 1);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-10', timestamp '2019-04-10 20:30:00.00 +01:00', 6, 2);
INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-11', timestamp '2019-04-11 20:30:00.00 +01:00', 6, 3);


INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Jan', 'Maxmili�n', 'Kli�', '+420732128465', 'klis25@gmail.com', 'D�t�');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Petr', 'Franti�ek', 'V�lek', '+420608563259', 'valek456@seznam.cz', 'Student');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Vanesa', 'Jaroslava', 'Bedn��ovsk�', '+421702305983', 'bedna25@centrum.cz', 'Dosp�l�');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Jana', 'Marie', 'Louskav�', '+420603055396', 'louskacek@louskacek.cz', 'D�chodce');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('John', 'Cliff', 'Doe', '+421775309855', 'john.doe@oracle.com', 'ZTP');

INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Ji��', 'Emanuel', 'Kli�', '+420732128466', 'klis225@gmail.com', 'D�t�', 6851034606);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Petra', 'Franti�ka', 'V�lek', '+420608563359', 'valek456@seznam.cz', 'Student', 9710110321);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Jarmila', 'Jaroslava', 'Bedn��ovsk�', '+421702305583', 'bednarova25@centrum.cz', 'Dosp�l�', 6906302589);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Ji�ina', 'Maruna', 'Louskav�', '+420603056396', 'louskacek2@louskacek.cz', 'D�chodce', 8351293112);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Pep', 'N�mo�n�k', 'Guardiola', '+421775309857', 'pepek@mancity.com', 'Dosp�l�', 9210240546);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Jos�', 'TheSpecialOne', 'Mourinho', '+420111111111', 'itsjose@jose.com', 'Dosp�l�', 9210240546);

INSERT INTO rezervace(datum_rezervace, aktivni, id_zakaznik, id_promitani) VALUES (date '2019-03-23', 1, 1, 1);
INSERT INTO rezervace(datum_rezervace, aktivni, id_zakaznik, id_promitani) VALUES (date '2019-03-23', 1, 2, 2);
INSERT INTO rezervace(datum_rezervace, aktivni, id_zakaznik, id_promitani) VALUES (date '2019-03-23', 1, 3, 3);
INSERT INTO rezervace(datum_rezervace, aktivni, id_zakaznik, id_promitani) VALUES (date '2019-03-23', 1, 4, 4);
INSERT INTO rezervace(datum_rezervace, aktivni, id_zakaznik, id_promitani) VALUES (date '2019-03-23', 1, 5, 5);
INSERT INTO rezervace(datum_rezervace, aktivni, id_zakaznik, id_promitani) VALUES (date '2019-03-23', 1, 4, 5);


INSERT INTO listek(datum_prodeje, rada, sedadlo, cena, rodne_cislo, id_promitani, id_zakaznik, id_rezervace) VALUES (date '2019-03-24', 6, 7, 150, 6851034606, 5, 2, 2);
INSERT INTO listek(datum_prodeje, rada, sedadlo, cena, rodne_cislo, id_promitani, id_zakaznik, id_rezervace) VALUES (date '2019-03-24', 6, 7, 180, 6851034606, 5, 4, 4);
INSERT INTO listek(datum_prodeje, rada, sedadlo, cena, rodne_cislo, id_promitani, id_zakaznik, id_rezervace) VALUES (date '2019-03-24', 4, 9, 290, 9710110321, 6, 3, 3);
INSERT INTO listek(datum_prodeje, rada, sedadlo, cena, rodne_cislo, id_promitani, id_zakaznik, id_rezervace) VALUES (date '2019-03-24', 4, 8, 100, 9710110321, 8, 3, 3);

------------------------------------------------
--              SELECT QUERY
------------------------------------------------
---DVA SKRIPTY NA SPOJENI DVOU TABULEK

--seznam zamestnancu multikina a mesto ve kterem pracuji
SELECT zamestnanec.jmeno AS jmeno, zamestnanec.prijmeni AS prijmeni, multikino.mesto AS mesto_kina, multikino.id_kino AS ID
FROM zamestnanec JOIN multikino
ON zamestnanec.id_kino = multikino.id_kino;

--seznam zakazniku a datum jejich rezervaci
SELECT zakaznik.jmeno AS jmeno, zakaznik.prijmeni AS prijmeni, rezervace.datum_rezervace AS datum
FROM zakaznik JOIN rezervace
ON zakaznik.id_zakaznik = rezervace.id_zakaznik;

--SKRIPT NA SPOJENI TRI TABULEK

--seznam zakazniku s koupenym listkem + informace o listku (datum, cas, cena)
SELECT zakaznik.jmeno AS jmeno, zakaznik.prijmeni AS prijmeni, TO_CHAR(promitani.cas, 'hh24:mi') AS cas, promitani.datum AS datum, listek.cena AS cena
FROM zakaznik, listek, promitani
WHERE zakaznik.id_zakaznik = listek.id_zakaznik AND listek.id_promitani = promitani.id_promitani;

--DVA SKRIPTY S KLAUZULI GROUP BY A AGREGACNI FUNKCI

--pocet naplanovanych promitani pro jednotlive filmy
SELECT film.nazev AS nazev, COUNT(*) AS pocet_promitani
FROM promitani, film
WHERE film.id_film = promitani.id_film
GROUP BY film.nazev;

--prumerna cena listku na na jednotliva promitani 
SELECT promitani.id_promitani AS ID, promitani.datum AS DATUM, TO_CHAR(promitani.cas, 'hh24:mi') AS CAS, AVG(listek.cena) AS prumerny_cena_na_osobu
FROM promitani, listek
WHERE listek.id_promitani = promitani.id_promitani
GROUP BY promitani.id_promitani, promitani.datum, TO_CHAR(promitani.cas, 'hh24:mi');

--SKRIPT OBSAHUJICI PREDIKAT EXISTS

--seznam filmu, ktere nemaji naplanovane promitani
SELECT film.nazev AS nazev
FROM film
WHERE NOT EXISTS(
    SELECT film.nazev
    FROM promitani
    WHERE film.id_film = promitani.id_film
);

--SKRIPT OBSAHUJICI PREDIKAT IN S VNORENYM SELECTEM

--seznam filmu, ktere se promitaji mezi 26. - 27. b�eznem 2019
SELECT film.nazev AS nazev
FROM film
WHERE film.id_film IN(
    SELECT promitani.id_film
    FROM promitani
    WHERE promitani.datum BETWEEN TO_DATE('2019-03-26', 'yyyy-mm-dd') AND TO_DATE('2019-03-27', 'yyyy-mm-dd')
);

------------------------------------------------
--              DEMONSTRACE
------------------------------------------------

-- trigger na autoinkrement

SELECT MAX(promitani.id_promitani) AS POSLEDNI_ID FROM promitani;

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-10-11', timestamp '2019-10-11 21:30:00.00 +01:00', 6, 3);

SELECT MAX(promitani.id_promitani) AS POSLEDNI_ID FROM promitani;


--trigger co vypise chybu -> kolize s jin�m prom�t�n�m

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-11', timestamp '2019-04-11 21:30:00.00 +01:00', 6, 3);

-- vyvol�n� procedur
-- procedura na vypsani vekovych skupin z�kazn�k� a jejich percentu�ln� zastoupen�
EXEC vekove_skupiny_zakazniku_procentualne();
-- procedura na vyps�n� tr�eb jednotliv�ch film�
EXEC celkova_trzba_jednotlivych_filmu();

-- pou�it� explain plan pro zji�t�n� ceny dotazu
EXPLAIN PLAN FOR 
    SELECT promitani.id_promitani AS ID, promitani.datum AS DATUM, TO_CHAR(promitani.cas, 'hh24:mi') AS CAS, AVG(listek.cena) AS prumerny_cena_na_osobu
    FROM promitani, listek
    WHERE listek.id_promitani = promitani.id_promitani
    GROUP BY promitani.id_promitani, promitani.datum, TO_CHAR(promitani.cas, 'hh24:mi');
SELECT * FROM TABLE(dbms_xplan.display());

-- vytvo�en� indexu pro sn�en� ceny dotazu
CREATE INDEX promitani_index ON promitani ( id_promitani, datum, cas );

--op�tovn� vol�n� pro zji�t�n� ceny stejn�ho dotazu (cena se sn�ila)
EXPLAIN PLAN FOR 
    SELECT promitani.id_promitani AS ID, promitani.datum AS DATUM, TO_CHAR(promitani.cas, 'hh24:mi') AS CAS, AVG(listek.cena) AS prumerny_cena_na_osobu
    FROM promitani, listek
    WHERE listek.id_promitani = promitani.id_promitani
    GROUP BY promitani.id_promitani, promitani.datum, TO_CHAR(promitani.cas, 'hh24:mi');
SELECT * FROM TABLE(dbms_xplan.display());

------------------------------------------------
--              GRANT ACCESS
------------------------------------------------

GRANT ALL ON multikino TO xtramp00;
GRANT ALL ON zamestnanec TO xtramp00;
GRANT ALL ON film TO xtramp00;
GRANT ALL ON sal TO xtramp00;
GRANT ALL ON promitani TO xtramp00;
GRANT ALL ON zakaznik TO xtramp00;
GRANT ALL ON rezervace TO xtramp00;
GRANT ALL ON listek TO xtramp00;

GRANT EXECUTE ON vekove_skupiny_zakazniku_procentualne TO xtramp00;
GRANT EXECUTE ON celkova_trzba_jednotlivych_filmu TO xtramp00;

------------------------------------------------
--             MATERIALIZED VIEW
------------------------------------------------

DROP MATERIALIZED VIEW multikino_view;

CREATE MATERIALIZED VIEW LOG ON xstudn00.multikino WITH PRIMARY KEY, ROWID(mesto) INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW multikino_view
CACHE
BUILD IMMEDIATE
REFRESH ON COMMIT
ENABLE QUERY REWRITE AS SELECT xstudn00.multikino.mesto AS MESTO, COUNT(xstudn00.multikino.mesto) AS POCET
FROM xstudn00.multikino
GROUP BY xstudn00.multikino.mesto;

SELECT * FROM multikino_view;

INSERT INTO xstudn00.multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Praha', 'Pa��sk�', 1257, 10000);
COMMIT;

SELECT * FROM multikino_view;

GRANT ALL ON multikino_view TO xstudn00;
