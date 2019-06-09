------------------------------------------------
-- IDS projekt
-- Ondøej Studnièka, Václav Trampeška
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
    vekova_skupina VARCHAR2(15) NOT NULL CHECK (vekova_skupina='Dítì' or vekova_skupina='Student' or vekova_skupina='Dospìlý' or vekova_skupina='Dùchodce' or vekova_skupina='ZTP'),
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

-- trigger na zjištìní jestli je sál již obsazen
-- pokud je sál obsazen v dobu, kdy chce uživatel naplánovat další, tak dojde k vyvolání vyjímky

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
    RAISE_APPLICATION_ERROR(-20100, 'Sál je v tuto dobu již využit.');
END IF;
END;
/

-----------------------------------
-- PROCEDURES
-----------------------------------

-- povolení výpisù
SET serveroutput ON;


-- procedura na výpis percentuálního zastoupení rùzných vìkových skupin zákazníkù v databázi

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
      ELSIF zkznk.vekova_skupina = 'Dùchodce' THEN
        duchodci := duchodci+1;
      ELSIF zkznk.vekova_skupina = 'Dospìlý' THEN
        dospeli := dospeli+1;
      ELSIF zkznk.vekova_skupina = 'Student' THEN
        studenti := studenti+1;
      ELSIF zkznk.vekova_skupina = 'Dítì' THEN
        deti := deti+1;
      END IF;
    END LOOP;  

    IF celkem = 0 THEN
      RAISE NO_DATA_FOUND;
    END IF;
    dbms_output.put_line('Procentuální rozdìlení zákazníkù podle vìkových skupin:');
    dbms_output.put_line('Deti      : '|| ROUND((deti/celkem)*100,1) ||'%');
    dbms_output.put_line('Studenti  : '|| ROUND((studenti/celkem)*100,1) ||'%');
    dbms_output.put_line('Dospeli   : '|| ROUND((dospeli/celkem)*100,1) ||'%');
    dbms_output.put_line('Duchodci  : '|| ROUND((duchodci/celkem)*100,1) ||'%');
    dbms_output.put_line('ZTP       : '|| ROUND((ZTP/celkem)*100,1) ||'%');
    CLOSE zakaznici;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      RAISE_APPLICATION_ERROR(-20101, 'Nebyli nalezeni žádní zákazníci.');
    WHEN OTHERS THEN
      RAISE_APPLICATION_ERROR(-20102, 'Chyba, co se nestane.');
  END;
/

-- procedura na zjisteni celkových výdìlkù jednotlivých filmù 

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
    dbms_output.put_line('Celkové tržby jednotlivých filmù:');
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

INSERT INTO multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Plzeò', 'Nádražní', 20, 30100);
INSERT INTO multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Praha', 'Jana Nepomuckého', 361, 10000);
INSERT INTO multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Ostrava', 'Hlavní', 1024, 70030);

INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, postaveni, id_kino) VALUES (6851034606, 'Alex', 'Petr', 'Novotný', date '1968-01-03', 'Plzeò', 'Poštovní', 321, 30100, 'novotny', '12345', 'v', 1);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, id_kino) VALUES (9710110321, 'Jakub', 'Jan', 'Slovák', date '1997-10-11', 'Plzeò', 'Sportovní', 25, 30100, 'slovak', '67890', 1);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, postaveni, id_kino) VALUES (8351293112, 'Alexandra', 'Lucie', 'Procházková', date '1983-01-29', 'Praha', 'V Koutì', 1021, 10000, 'prochazkova', '13579', 'v', 2);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, id_kino) VALUES (6906302589, 'Václav', 'David', 'Macháèek', date '1969-06-30', 'Plzeò', 'Okružní', 137, 10000, 'machacek', '24680', 2);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, postaveni, id_kino) VALUES (9210240546, 'Stanislav', 'Petr', 'Koutný', date '1992-10-24', 'Ostrava', 'Báòská', 42, 70030, 'koutny', '14702', 'v', 3);
INSERT INTO zamestnanec(rodne_cislo, jmeno, prostredni_jmeno, prijmeni, datum_narozeni, mesto, ulice, cislo_popisne, postovni_smerovaci_cislo, login, heslo, id_kino) VALUES (8358218564, 'Aneta', 'Anna', 'Bystøická', date '1983-08-21', 'Ostrava', 'Opavská', 256, 70030, 'bystricka', '35671', 3);

INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Nedotknutelní', 'Komediální drama', 113, 'èeský', 'FRA', 'Olivier Nakache, Eric Toledano', 'François Cluzet, Omar Sy, Anne Le Ny, Audrey Fleurot, Joséphine de Meaux, Clotilde Mollet, Alba Ga?a Kraghede Bellugi, Cyril Mendy, Salimata Kamate, Grégoire Oestermann, Dominique Daguier, François Caron, Christian Ameri, Thomas Solivéres, Dorothée Briere, Émilie Caen, Sylvain Lazard, Jean-François Cayrey, François Bureloup', 'Ochrnutý a bohatý aristokrat Philippe si za svého nového opatrovníka vybere Drisse, živelného mladíka z pøedmìstí, kterého právì propustili z vìzení. Jinými slovy - najde si na tuto práci tu nejménì vhodnou osobu. Podaøí se jim však propojit nemožné: Vivaldiho a populární hudbu, serióznost a žoviální vtípky, luxusní obleky a tepláky. Bláznivým, zábavným, silným, neoèekávaným a hlavnì „nedotknutelným“, pøesnì takovým se stane jejich pøátelství… Komedie s dramatickou zápletkou o tom, že ani od krku po prsty u nohou nepohyblivý èlovìk odkázaný na pomoc druhých, nemusí ztratit smysl života. A o tom, že i nejménì pravdìpodobné spojení melancholického multimilionáøe a extrovertního recidivisty mùže humornì zapùsobit na diváka a mùže se z nìj stát kasovní trhák.', 'Ano');
INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Fantastická zvíøata: Grindelwaldovy zloèiny', 'Dobrodružný', 134, 'anglický', 'UK', 'David Yates', 'Eddie Redmayne, Katherine Waterston, Dan Fogler, Johnny Depp, Jude Law, Alison Sudol, Ezra Miller, Zoë Kravitz, Claudia Kim, Carmen Ejogo, Jessica Williams, Ólafur Darri Ólafsson, Callum Turner, Victoria Yeates, Poppy Corby-Tuech, Brontis Jodorowsky, William Nadylam, Fiona Glascott, Derek Riddell, Kevin Guthrie, David Sakurai', 'Na konci prvního filmu KOKUSA (Kouzelnícký kongres USA) zajme s pomocí Mloka Scamandera (Eddie Redmayne) mocného èernoknìžníka Gellerta Grindelwalda (Johnny Depp). Grindelwald ale splnil svou hrozbu, že uteèe z vazby, a zaèal shromažïovat své stoupence, z nichž vìtšina vùbec nic netušila o jeho skuteèných cílech: pøipravit vládu èistokrevných èarodìjù nad všemi nemagickými bytostmi. Ve snaze zmaøit Grindelwaldovy plány, Albus Brumbál (Jude Law) získá svého bývalého studenta Mloka Scamandera, který souhlasí s tím, že pomùže, aniž by si uvìdomoval, jakému nebezpeèí se tím vystavuje. Bitevní linie jsou vytyèeny a ve stále více znesváøeném svìtì èarodìjù je podrobena zkoušce láska a loajalita, dokonce i mezi nejvìrnìjšími pøáteli a rodinou.', 'Ano');
INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Mlèení jehòátek', 'Filmové drama', 138, 'èeský', 'USA', 'Jonathan Demme', 'Jodie Foster, Anthony Hopkins, Scott Glenn, Anthony Heald, Ted Levine, Frankie Faison, Kasi Lemmons, Brooke Smith, Obba Babatundé, Diane Baker, Roger Corman, Charles Napier, Cynthia Ettinger, Chris Isaak, Daniel von Bargen, Ted Monte, George A. Romero, Tracey Walter, Stuart Rudin, Brent Hinkley, John W. Iwanonkiw, John Hall', 'Clarice Starlingová, nadaná studentka akademie FBI, je pøidìlena agentu Crawfordovi vyšetøujícímu pøípad masového vraha Buffalo Billa, který své obìti stahuje z kùže. Clarice navštíví v baltimorské vìzeòské nemocnici bývalého vynikajícího psychiatra Hannibala Lectera, odsouzeného na doživotí za sérii brutálních vražd a kanibalismus, který by o vrahovi mohl nìco vìdìt. Odsouzený psychiatr Clarice zároveò dìsí a fascinuje. Když je masovým vrahem unesena dcera senátorky Martinové, nabídne Clarice Lecterovi pøijatelnìjší vìzeòské podmínky výmìnou za informaci, vedoucí k dopadení Buffalo Billa...', 'Ne');

INSERT INTO film(nazev, zanr, delka, jazyk, puvod, rezie, obsazeni, popis, pristupny) VALUES ('Forrest Gump', 'Filmové drama', 144, 'èeský', 'USA', 'Robert Zemeckis', 'Tom Hanks, Robin Wright, Gary Sinise, Mykelti Williamson, Sally Field, Haley Joel Osment, Peter Dobson, Siobhan Fallon Hogan, Hanna Hall, Brett Rice, Kirk Ward, Kurt Russell, Stephen Bridgewater, Michael Jace, Michael McFall, Joe Alaskey, Sam Anderson, Jacqueline Lovell, Mary Ellen Trainor, Vanessa Roth, Bryan Hanna, Jason McGuire', 'Zemeckisùv film je brilantním shrnutím dosavadních režisérových poznatkù o možnostech "comicsového" vyprávìní. Formálnì i obsahovì nejménì konvenèní z jeho snímkù pøesvìdèuje komediálními gagy i naléhavì patetickým tónem. Jeho hrdinou je prosáèek Forrest Gump, typický obyèejný muž, který od dìtství dìlal, co se mu øeklo. Do života si tak odnesl nìkolik ponauèení své peèlivé maminky a osvìdèené pravidlo, jež se mu hodí mnohokrát v nejrùznìjších situacích: Když se dostaneš do problémù, utíkej. Forrest proutíká školou, jako hráè amerického fotbalu i univerzitou, potom peklem vietnamské války a zoufalstvím nad matèinou smrtí. Vždycky je totiž nìkdo nebo nìco, co po nìm skuteènì èi obraznì "hází kameny". Nakonec však Forrest poznává, že jsou i jiná øešení situací než útìk. Svùj život spojuje s kamarádkou ze školy Jenny, která pro nìj zùstane provždycky jedinou láskou, s pøítelem z vojny, èernochem Bubbou, který dá smìr jeho úvahám o lovu krevet a s poruèíkem Taylorem, jemuž ve Vietnamu zachrání život. Forrestova životní pou od 50. do 80. let je koncipovaná jako totálnì "bezelstné" vyprávìní hrdiny, neschopného obecnìjšího hodnocení situace. Forrest jako sportovec i jako váleèný hrdina se setkává se slavnými lidmi, které nakonec vždycky nìkdo zastøelí (J. F. Kennedy, J. Lennon). Také jeho bližní umírají, ale on sám zùstává. Empiricky se dopracovává od impulsivního útìku pøed životem k úvahám o lidském osudu a o Bohu. Soukromý hrdinùv osud zároveò postihuje tøicet let pováleèných amerických dìjin.', 'Ano');


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


INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Jan', 'Maxmilián', 'Kliš', '+420732128465', 'klis25@gmail.com', 'Dítì');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Petr', 'František', 'Válek', '+420608563259', 'valek456@seznam.cz', 'Student');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Vanesa', 'Jaroslava', 'Bednáøovská', '+421702305983', 'bedna25@centrum.cz', 'Dospìlý');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('Jana', 'Marie', 'Louskavá', '+420603055396', 'louskacek@louskacek.cz', 'Dùchodce');
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina) VALUES ('John', 'Cliff', 'Doe', '+421775309855', 'john.doe@oracle.com', 'ZTP');

INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Jiøí', 'Emanuel', 'Kliš', '+420732128466', 'klis225@gmail.com', 'Dítì', 6851034606);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Petra', 'Františka', 'Válek', '+420608563359', 'valek456@seznam.cz', 'Student', 9710110321);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Jarmila', 'Jaroslava', 'Bednáøovská', '+421702305583', 'bednarova25@centrum.cz', 'Dospìlý', 6906302589);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Jiøina', 'Maruna', 'Louskavá', '+420603056396', 'louskacek2@louskacek.cz', 'Dùchodce', 8351293112);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('Pep', 'Námoøník', 'Guardiola', '+421775309857', 'pepek@mancity.com', 'Dospìlý', 9210240546);
INSERT INTO zakaznik(jmeno, prostredni_jmeno, prijmeni, telefon, email, vekova_skupina, rodne_cislo) VALUES ('José', 'TheSpecialOne', 'Mourinho', '+420111111111', 'itsjose@jose.com', 'Dospìlý', 9210240546);

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

--seznam filmu, ktere se promitaji mezi 26. - 27. bøeznem 2019
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


--trigger co vypise chybu -> kolize s jiným promítáním

INSERT INTO promitani(datum, cas, id_sal, id_film) VALUES (date '2019-04-11', timestamp '2019-04-11 21:30:00.00 +01:00', 6, 3);

-- vyvolání procedur
-- procedura na vypsani vekovych skupin zákazníkù a jejich percentuální zastoupení
EXEC vekove_skupiny_zakazniku_procentualne();
-- procedura na vypsání tržeb jednotlivých filmù
EXEC celkova_trzba_jednotlivych_filmu();

-- použití explain plan pro zjištìní ceny dotazu
EXPLAIN PLAN FOR 
    SELECT promitani.id_promitani AS ID, promitani.datum AS DATUM, TO_CHAR(promitani.cas, 'hh24:mi') AS CAS, AVG(listek.cena) AS prumerny_cena_na_osobu
    FROM promitani, listek
    WHERE listek.id_promitani = promitani.id_promitani
    GROUP BY promitani.id_promitani, promitani.datum, TO_CHAR(promitani.cas, 'hh24:mi');
SELECT * FROM TABLE(dbms_xplan.display());

-- vytvoøení indexu pro snížení ceny dotazu
CREATE INDEX promitani_index ON promitani ( id_promitani, datum, cas );

--opìtovné volání pro zjištìní ceny stejného dotazu (cena se snížila)
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

INSERT INTO xstudn00.multikino(mesto, ulice, cislo_popisne, postovni_smerovaci_cislo) VALUES ('Praha', 'Paøížská', 1257, 10000);
COMMIT;

SELECT * FROM multikino_view;

GRANT ALL ON multikino_view TO xstudn00;
