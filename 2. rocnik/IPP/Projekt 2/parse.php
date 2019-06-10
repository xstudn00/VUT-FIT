<?php


//pokud existuji vice nez dva argumenty, tak exit
//smi se zadat maximalne --help
if(count($argv) != 1 && count($argv) != 2){
    exit(10);
}


//pokud je zadany aragument -- help, vypis napovedu
if(count($argv) == 2 && ($argv[1] == "--help")){
    echo "Skript typu filtr (parse.php v jazyce PHP 7.3) nacte ze standardniho vstupu zdrojovy kod v IPPcode19, zkontroluje lexikalni a syntaktickou spravnost kodu a vypise na standardni vystup XML reprezentaci programu dle specifikace.\n";
    exit(0);
}

if(count($argv) == 2 && ($argv[1] != "--help")){
    exit(10);
}


//cti ze standardniho vstupu
$stdin = fopen( 'php://stdin', 'r' );

$array_of_lines = array();

//dokud je neco na vstupu tak nacitej po radku
//odstran z nacteneho radku sekvenci bilych znaku
//odstran z nacteneho radku komentare
//upraveny radek uloz do pole
//uzavri stdin
if ($stdin) {
    while (($line = fgets($stdin)) !== false) {
        $line = preg_replace('/\s+/', ' ',$line);
        $line = preg_replace('/#.*/', '', $line);
        array_push($array_of_lines, $line);
    }
    fclose($stdin);
}

//orez bile znaky ze zacatku a konce radku
$array_of_lines = array_filter($array_of_lines, 'trim');
//print_r($array_of_lines);

//zjisti jestli je na prvnim radku hlavicka 
//nutno orezat, prevest na velka pismena
if(trim(strtoupper(reset($array_of_lines))) != ".IPPCODE19"){
    exit(21);
}

//odstran z pole prvni radek obsahujici hlavicku
unset($array_of_lines[0]);


//vytvoreni poli pro vsechny pripustne instrukce
$inst_no_opr = array("CREATEFRAME", "PUSHFRAME", "POPFRAME", "RETURN", "BREAK");
$instr_1_var = array("DEFVAR", "POPS");
$instr_1_label = array("CALL", "LABEL", "JUMP");
$instr_1_symbol = array("PUSHS", "WRITE", "EXIT", "DPRINT");
$instr_2_var_symb = array("MOVE", "INT2CHAR", "STRLEN", "TYPE", "NOT");
$instr_2_var_type = array("READ");
$instr_3_label_symb_symb = array("JUMPIFEQ", "JUMPIFNEQ");
$instr_3_var_symb_symb = array("ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "STRI2INT", "CONCAT", "GETCHAR", "SETCHAR");

// regularni vyrazy pro kontrolu operandu
$var_regex = '/^(GF|LF|TF)(@)([a-zA-Z]|[_\-$&%*!?])([a-zA-Z]|[0-9]|[_\-$&%*!?])*$/';
$symb_regex = '/(^(GF|LF|TF)(@)(([a-zA-Z]|[_\-$&%*!?])([a-zA-Z]|[0-9])*)$)|((^(int)(@).+)$)|((^(string)(@).*)$)|((^(bool)(@)(true|false))$)|((^(nil)(@)(nil))$)/';
$label_regex = '/^(([a-zA-Z]|[_\-$&%*!?])([a-zA-Z]|[0-9])*)$/';
$type_regex = "/^(int|bool|string)$/";

$array_of_checked_instructions = array();

foreach($array_of_lines as $word){

	//odstraneni tabulatoru
    $word = preg_replace('/^[ \t]/', '', $word);

    //rozdeleni nacteneho radku podle mezery, ziskavame instrukci a operandy
    $word = explode(" ",$word);

    //prace s bilymi znaky
    $word = array_filter($word, 'trim');
    $word = str_replace(' ', '', $word);
    //array_push($wordsInLines, $word);

    //prepis instrukci na velka pismena
    $word[0] = strtoupper($word[0]);

    //kontrola pro zpetne lomitko
    //za zpetnym lomitkem musi nasledovat tri cisla
    //kontroluji jak instrukce tak operandy
    foreach($word as $backslash_check){
        if(strpos($backslash_check, "\\")){
            if(!preg_match('/(\\\\[0-9][0-9][0-9])/', $backslash_check)){
                exit(23);
            }
        }
    }

    // kontroluji jestli zadana instrukce je spravne zapsana (jestli se nechazi v nekterem poli instrukci)
    //kontroluji jestli ma instrukce spravny pocet operandu
    //u instrukci ktere maji mit operandy kontroluji, jestli jsou operandy validne zapsany
    //pokud je vse ok, tak ukladam do pole, pokud se objevi chyba -> prislusny exit
    if(in_array($word[0], $inst_no_opr)){
        if(sizeof($word) != 1){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_1_var)){
        if(sizeof($word) != 2){
            exit(23);
        }
        if(!preg_match($var_regex, $word[1])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_1_label)){
        if(sizeof($word) != 2){
            exit(23);
        }
        if(!preg_match($label_regex, $word[1])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_1_symbol)){
        if(sizeof($word) != 2){
            exit(23);
        }
        if(!preg_match($symb_regex, $word[1])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_2_var_symb)){
        if(sizeof($word) != 3){
            exit(23);
        }
        if(!preg_match($var_regex, $word[1]) || !preg_match($symb_regex, $word[2])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_2_var_type)){
        if(sizeof($word) != 3){
            exit(23);
        }
        if(!preg_match($var_regex, $word[1]) || !preg_match($type_regex, $word[2])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_3_label_symb_symb)){
        if(sizeof($word) != 4){
            exit(23);
        }
        if(!preg_match($label_regex, $word[1]) || !preg_match($symb_regex, $word[2]) || !preg_match($symb_regex, $word[3])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    elseif(in_array($word[0], $instr_3_var_symb_symb)){
        if(sizeof($word) != 4){
            exit(23);
        }
        if(!preg_match($var_regex, $word[1]) || !preg_match($symb_regex, $word[2]) || !preg_match($symb_regex, $word[3])){
            exit(23);
        }
        array_push($array_of_checked_instructions, $word);
    }
    else{
        exit(22);
    }
}

//print_r($array_of_checked_instructions);

//------------------generovani xml
//v teto fazi uz mam zkontrolovane vsechny radky a vim ze jak instrukce, tak operandy jsou v poradku


//zacinam generovat
//zavolam XMLWritter
//nasmeruju ho na standardni vystup
//nastavim hlavicku
$writer = new XMLWriter();
$writer->openURI('php://output');
$writer->startDocument('1.0','UTF-8');
$writer->setIndent(4);
$writer->startElement('program');
$writer->writeAttribute('language', 'IPPcode19');

//cyklim pres pole zkontrolovanych instrukci
for($i = 0; $i < sizeof($array_of_checked_instructions); $i++){
	//pro kazdou instrukci musim vygenerovat jeji hlavicku
	// -> instruction, order, opcode
	//timto blokem jsem zaroven vyresil generovani instrukci bez operandu
    $writer->startElement('instruction');
    $writer->writeAttribute('order', $i+1);
    $writer->writeAttribute('opcode', $array_of_checked_instructions[$i][0]);
    $array_of_frames = array("TF", "LF", "GF");

    //pokud ma generovana instrukce 1 operand a tim je var
    //vytvor element arg1
    //do typu elementu vepis var
    //zapis samotny var
    //ukonci element
    if(in_array($array_of_checked_instructions[$i][0], $instr_1_var)){
        $writer->startElement('arg1');
        $writer->writeAttribute('type', "var");
        $writer->text($array_of_checked_instructions[$i][1]);
        $writer->endElement();
    }

    //pokud ma generovana instrukce 1 operand a tim je label
    //vytvor element arg1
    //do typu elementu vepis label
    //zapis samotny label
    //ukonci element
    if(in_array($array_of_checked_instructions[$i][0], $instr_1_label)){
        $writer->startElement('arg1');
        $writer->writeAttribute('type', "label");
        $writer->text($array_of_checked_instructions[$i][1]);
        $writer->endElement();
    }

    //pokud ma generovana instrukce 1 operand a tim je symbol
    //rozdel symbol podle @
    //zjisti jestli je symbol var nebo type (pokud je cast pred zavinacem v poli ramcu, tak se jedna o var)
    //pokud je var:
    	//vytvor element arg1
    	//do typu elementu vepis var
    	//zapis samotny var
    	//ukonci element
    //pokud je type:
        //vytvor element arg1
    	//do typu elementu vepis typ (string, int, ...)
    	//zapis samotny cast za zavinacem
    	//ukonci element
    if(in_array($array_of_checked_instructions[$i][0], $instr_1_symbol)){
    	if(preg_match('/^string/', $array_of_checked_instructions[$i][1])){
    		$expoloded_word = preg_replace('/^([^@]+)@/', "string(sekvence_neporavdepodobnych_znaku)", $array_of_checked_instructions[$i][1]);
    		$exploded_word = explode("(sekvence_neporavdepodobnych_znaku)",$expoloded_word);
    	}
    	else{
    		$exploded_word = explode("@",$array_of_checked_instructions[$i][1]);
    	}
        
        if(in_array($exploded_word[0], $array_of_frames)){
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "var");
            $writer->text($array_of_checked_instructions[$i][1]);
            $writer->endElement();
        }
        else{
            $writer->startElement('arg1');
            $writer->writeAttribute('type', $exploded_word[0]);
            $writer->text($exploded_word[1]);
            $writer->endElement();
        }

    }

    //stejny princip jako vyse
    if(in_array($array_of_checked_instructions[$i][0], $instr_2_var_symb)){
        $writer->startElement('arg1');
        $writer->writeAttribute('type', "var");
        $writer->text($array_of_checked_instructions[$i][1]);
        $writer->endElement();

        if(preg_match('/^string/', $array_of_checked_instructions[$i][2])){
    		$expoloded_word = preg_replace('/^([^@]+)@/', "string(sekvence_neporavdepodobnych_znaku)", $array_of_checked_instructions[$i][2]);
    		$exploded_word = explode("(sekvence_neporavdepodobnych_znaku)",$expoloded_word);
    	}
    	else{
    		$exploded_word = explode("@",$array_of_checked_instructions[$i][2]);
    	}

        if(in_array($exploded_word[0], $array_of_frames)){
            $writer->startElement('arg2');
            $writer->writeAttribute('type', "var");
            $writer->text($array_of_checked_instructions[$i][2]);
            $writer->endElement();
        }
        else{
            $writer->startElement('arg2');
            $writer->writeAttribute('type', $exploded_word[0]);
            $writer->text($exploded_word[1]);
            $writer->endElement();
        }


    }

    //stejny princip jako vyse
    if(in_array($array_of_checked_instructions[$i][0], $instr_2_var_type)){
        $writer->startElement('arg1');
        $writer->writeAttribute('type', "var");
        $writer->text($array_of_checked_instructions[$i][1]);
        $writer->endElement();

        $writer->startElement('arg2');
        $writer->writeAttribute('type', "type");
        $writer->text($array_of_checked_instructions[$i][2]);

        $writer->endElement();

    }

    //stejny princip jako vyse
    if(in_array($array_of_checked_instructions[$i][0], $instr_3_label_symb_symb)){

        $writer->startElement('arg1');
        $writer->writeAttribute('type', "label");
        $writer->text($array_of_checked_instructions[$i][1]);
        $writer->endElement();

        if(preg_match('/^string/', $array_of_checked_instructions[$i][2])){
    		$expoloded_word = preg_replace('/^([^@]+)@/', "string(sekvence_neporavdepodobnych_znaku)", $array_of_checked_instructions[$i][2]);
    		$exploded_word = explode("(sekvence_neporavdepodobnych_znaku)",$expoloded_word);
    	}
    	else{
    		$exploded_word = explode("@",$array_of_checked_instructions[$i][2]);
    	}

        if(in_array($exploded_word[0], $array_of_frames)){
            $writer->startElement('arg2');
            $writer->writeAttribute('type', "var");
            $writer->text($array_of_checked_instructions[$i][2]);
            $writer->endElement();
        }
        else{
            $writer->startElement('arg2');
            $writer->writeAttribute('type', $exploded_word[0]);
            $writer->text($exploded_word[1]);
            $writer->endElement();
        }

        if(preg_match('/^string/', $array_of_checked_instructions[$i][3])){
    		$exploded_word_2 = preg_replace('/^([^@]+)@/', "string(sekvence_neporavdepodobnych_znaku)", $array_of_checked_instructions[$i][3]);
    		$exploded_word_2 = explode("(sekvence_neporavdepodobnych_znaku)",$exploded_word_2);
    	}
    	else{
    		$exploded_word_2 = explode("@",$array_of_checked_instructions[$i][3]);
    	}
        if(in_array($exploded_word_2[0], $array_of_frames)){
            $writer->startElement('arg3');
            $writer->writeAttribute('type', "var");
            $writer->text($array_of_checked_instructions[$i][3]);
            $writer->endElement();
        }
        else{
            $writer->startElement('arg3');
            $writer->writeAttribute('type', $exploded_word_2[0]);
            $writer->text($exploded_word_2[1]);
            $writer->endElement();
        }

    }

    //stejny princip jako vyse
    if(in_array($array_of_checked_instructions[$i][0], $instr_3_var_symb_symb)){
        $writer->startElement('arg1');
        $writer->writeAttribute('type', "var");
        $writer->text($array_of_checked_instructions[$i][1]);
        $writer->endElement();

        if(preg_match('/^string/', $array_of_checked_instructions[$i][2])){
            $expoloded_word = preg_replace('/^([^@]+)@/', "string(sekvence_neporavdepodobnych_znaku)", $array_of_checked_instructions[$i][2]);
            $exploded_word = explode("(sekvence_neporavdepodobnych_znaku)",$expoloded_word);
        }
        else{
            $exploded_word = explode("@",$array_of_checked_instructions[$i][2]);
        }
        
        if(in_array($exploded_word[0], $array_of_frames)){
            $writer->startElement('arg2');
            $writer->writeAttribute('type', "var");
            $writer->text($array_of_checked_instructions[$i][2]);
            $writer->endElement();
        }
        else{
            $writer->startElement('arg2');
            $writer->writeAttribute('type', $exploded_word[0]);
            $writer->text($exploded_word[1]);
            $writer->endElement();
        }

        if(preg_match('/^string/', $array_of_checked_instructions[$i][3])){
            $exploded_word_2 = preg_replace('/^([^@]+)@/', "string(sekvence_neporavdepodobnych_znaku)", $array_of_checked_instructions[$i][3]);
            $exploded_word_2 = explode("(sekvence_neporavdepodobnych_znaku)",$exploded_word_2);
        }
        else{
            $exploded_word_2 = explode("@",$array_of_checked_instructions[$i][3]);
        }


        if(in_array($exploded_word_2[0], $array_of_frames)){
            $writer->startElement('arg3');
            $writer->writeAttribute('type', "var");
            $writer->text($array_of_checked_instructions[$i][3]);
            $writer->endElement();
        }
        else{
            $writer->startElement('arg3');
            $writer->writeAttribute('type', $exploded_word_2[0]);
            $writer->text($exploded_word_2[1]);
            $writer->endElement();
        }

    }
    //ukonci element instruction
    $writer->endElement();
}

// koncim element (program) a dokument
//vypisu
$writer->endElement();
$writer->endDocument();
$writer->flush();

?>