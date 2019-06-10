<?php

/*
** IPP - treti projekt
** test.php
** Ondrej Studnicka
** xstudn00
** 2018/2019
*/

/* pocatecni nastaveni*/
$recursive = false;
$int_only = false;
$parse_only = false;
$path = ".";
$parser = "./parse.php";
$interpret = "./interpret.py";

/* pripustne argumenty */
$possible_arguments_without_path = array("--help", "--recursive", "--parse-only", "--int-only");
$possible_arguments_containing_path = array("--directory", "--parse-script", "--int-script");


/*z pole argumentu odstran prvni argument (test.php)*/
$array_of_arguments = $argv;
unset($array_of_arguments[0]);

/* pokud zadany argument obsahuje = tak jej podle nej rozdel*/
$array_of_exploded = array();
foreach($array_of_arguments as $argument){
	if(strpos($argument, "=")){
		$exploded = explode("=", $argument, 2);
		array_push($array_of_exploded, $exploded[0]);
	}
}
/* pro kazdy argument
** zjisti jestli obsahuje = 
** zjisti jestli je v poli pripustnych argumentu
** tak zjisti jestli je uvedena cesta k sobouru/slozce platna
** pokud ano tak zmen pocatecni nastaveni 
**
** pokud argument neobsahuje =
** tak zjisit jestli je v poli pripustnych argumentu
** pokud ano tak zmen pocatecni nastaveni
**
** dale se zde kontroluje jestli se argumenty nehadaji
*/
foreach($array_of_arguments as $argument){
	if(strpos($argument, "=")){
		$exploded = explode("=", $argument, 2);
		if(in_array($exploded[0], $possible_arguments_containing_path)){
			if ($exploded[0] == "--directory"){
				if(file_exists($exploded[1])){
                    $path = $exploded[1];
                }
                else{
                    exit(10);
                }
			
			}

			if ($exploded[0] == "--parse-script"){
				if(file_exists($exploded[1])){
                    $parser = $exploded[1];
                }
                else{
                    exit(10);
                }
			}

			if ($exploded[0] == "--int-script"){
				if(file_exists($exploded[1])){
                    $interpret = $exploded[1];
                }
                else{
                    exit(10);
                }
			}

		}
		else{
			exit(10);
		}		
	}

	else{
		if(in_array($argument, $possible_arguments_without_path)){

			if ($argument == "--help"){
				if ($argc == 2){
					echo "Skript slouzi pro automaticke testovani skriptu parse.php, interpret.py. Test.php projde zadany adresar, nalezne zdrojove soubory a nad nimi spusti zadany skript. O vysledcich testu pak bude uzivatel informovat v jednoduche HTML strance";
					exit(0);
				}
				else{
					exit(10);
				}
			}

			if($argument == "--recursive"){
				$recursive = true;
			}

			if($argument == "--parse-only"){
				if(in_array("--int-only", $array_of_arguments)){
					exit(10);
				}
				if(in_array("--int-script", $array_of_exploded)){
					exit(10);
				}

				$parse_only = true;
			}

			if($argument == "--int-only"){
				if(in_array("--parse-only", $array_of_arguments)){
					exit(10);
				}
				if(in_array("--parse-script", $array_of_exploded)){
					exit(10);
				}

				$int_only = true;
			}


		}
		else{
			exit(10);
		}

	}
}

/* pokud je nastaveno rekurzivni prohledavani tak musim najit vsechny soubory ve vsech podslozkach*/
/* pokud neni nastaveno rekurzivni vyhledavani tak musim najit soubory pouze v zadane slozce*/
if($recursive){
    $rii = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($path));

    $files = array();
    foreach ($rii as $file)
        if (!$file->isDir()){
            array_push($files, $file->getPathname());
        }
}
else{
    $scanned_dir = glob($path.'/*.*');
    $files = array();
    foreach($scanned_dir as $file){
        array_push($files, $file);
    }
}

/* v poli souboru hledam vsechny ktere maji koncovku src*/
$sources_to_work_with = array();
foreach($files as $source){
    if(preg_match('/.+(.src)$/', $source)){
        array_push($sources_to_work_with, $source);
    }
}


/* testovani parseru*/
/* spust parer se zadanym testem a odchyt jeho vystup a hodnotu kterou vratil*/
/* pokud se vracena hodnota rovna 0 tak zjisti jestli byla ocekavana nula -> pokud nula byla ocekavana tak spust jexamxml pro porovnani vystupu z parseru s referencnim -> pokud je vse ok tak test prosel -> jinak neprosel (to same pokud je ocekavan hodnota jina nez 0 a parser vratil 0)*/
/* pokud vystupni hodnota neni nula tak porovnej jestli se vracena a ocekavana hodnota rovnaji tak test prosel pokud se nerovnaji tak test neprosel*/
/* vrat vysledek*/

function parse_test($test_file, $rc_value, $out_value){
	global $parser;
	global $fail;
	global $succ;
	$return = array();

	exec("php7.3 $parser < $test_file", $parser_output, $parse_ret_val);

	if($parse_ret_val == 0){
		if($parse_ret_val == $rc_value){

			shell_exec('java -jar /pub/courses/ipp/jexamxml/jexamxml.jar $out_value $parser_output /pub/courses/ipp/jexamxml/options');

			$correct = shell_exec('echo $?');

			if($correct == 0){
				$succ = $succ + 1;
				array_push($return, $test_file, "OK", $parse_ret_val, $rc_value);
				return $return;
			}
			else{
				$fail = $fail + 1;
				array_push($return, $test_file, "FAIL", $parse_ret_val, $rc_value);
				return $return;
			}

		}
		else{
			$fail = $fail + 1;
			array_push($return, $test_file, "FAIL", $parse_ret_val, $rc_value);
			return $return;
		}

	}
	else{
		if($parse_ret_val == $rc_value){
			$succ = $succ + 1;
			array_push($return, $test_file, "OK", $parse_ret_val, $rc_value);
			return $return;
		}
		else{
			$fail = $fail + 1;
			array_push($return, $test_file, "FAIL", $parse_ret_val, $rc_value);
			return $return;
		}
	}
}

/* testovani interpretru*/
/* vytvor docasny soubor do ktereho se bude ukladat vystup interpretu*/
/* spust interpret se zadanymi vstupy, zachycuj vracenou hodnotu a vystup interpretu */
/* pokud je hodnota 0 a ocekava se nula tak spust diff nad vystupem z parseru a ocekavanym vystupem*/
/* zjisti jak diff dopadl*/
/* pokud diff skoncil 0 tak test prosel*/
/* jinak neprosel*/
/* pokud interpret vratil 0 a ocekavalo se neco jineho tak fail*/
/* pokud interpret vratil neco jineho nez 0 tak porovnej vystup interpretu s rc value */
/* pokud se rovnaji tak test prosel*/
/* pokud se nerovnaji tak test neprosel*/
/* vrat vysledek*/
function int_test($test_file, $rc_value, $out_value, $in_value){
	global $interpret;
	global $fail;
	global $succ;
	$return = array();

	$tmp = preg_replace('/(.src)|(.xml)$/', ".tmp", $test_file);

	$file_to_generate = fopen($tmp, "w");
    fclose($file_to_generate);

	exec("python3.6 ".$interpret." --source=".$test_file. " < ".$in_value." > " .$tmp." 2> /dev/null", $int_out, $int_ret_val);



	if($int_ret_val == 0){
		if($int_ret_val == $rc_value){

			shell_exec("diff $tmp $out_value");

			$correct = shell_exec('echo $?');

			if($correct == 0){
				$succ = $succ + 1;
				array_push($return, $test_file, "OK", $int_ret_val, $rc_value);
				unlink($tmp);
				return $return;
			}
			else{
				$fail = $fail + 1;
				array_push($return, $test_file, "FAIL", $int_ret_val, $rc_value);
				unlink($tmp);
				return $return;
			}

		}
		else{
			$fail = $fail + 1;
			array_push($return, $test_file, "FAIL", $int_ret_val, $rc_value);
			unlink($tmp);
			return $return;
		}

	}
	else{
		if($int_ret_val == $rc_value){
			$succ = $succ + 1;
			array_push($return, $test_file, "OK", $int_ret_val, $rc_value);
			unlink($tmp);
			return $return;
		}
		else{
			$fail = $fail + 1;
			array_push($return, $test_file, "FAIL", $int_ret_val, $rc_value);
			unlink($tmp);
			return $return;
		}
	}



}

/* testovani parseru i interpretu*/
/* spust parer se zadanym testem a odchyt jeho vystup a hodnotu kterou vratil*/
/* pokud se vracena hodnota rovna 0 tak zjisti jestli byla ocekavana nula -> pokud nula byla ocekavana tak prejdi s vystupem do interpretu*/
/* pokud vracena hodnota je nula a neceka se nula tak fail*/
/* pokud ocekavana hodnota je ruzna od nuly tak porovnej hodnotu s hodnotou v souboru rc_value  pokud se rovnaji tak ok pokud ne tak fail*/
/* pokud mam interpretovat tak zavolam funkci int_only a odchytnu jeji vysledky*/
/* vrat vysledek*/

function test_both($test_file, $rc_value, $out_value, $in_value){
	global $interpret;
	global $parser;
	global $fail;
	global $succ;
	$return = array();

	exec("php7.3 $parser < $test_file", $parser_output, $parse_ret_val);

	if($parse_ret_val == 0){		

			$xml_file = preg_replace('/(.src)$/', ".xml", $test_file);

			$file_to_generate = fopen($xml_file, "w");
			foreach ($parser_output as $xml_line){
				fwrite($file_to_generate, $xml_line);
			}        	
        	fclose($file_to_generate);

			$y = int_test($xml_file, $rc_value, $out_value, $in_value);
			unlink($xml_file);
			$d = $y[0];
			$f = $y[1];
			$g = $y[2];
			$h = $y[3];

			array_push($return, $d, $f, $g, $h);
			return $return;

	}
	else{
		if($parse_ret_val == $rc_value){
			$succ = $succ + 1;
			array_push($return, $test_file, "OK", $parse_ret_val, $rc_value);
			return $return;
		}
		else{
			$fail = $fail + 1;
			array_push($return, $test_file, "FAIL", $parse_ret_val, $rc_value);
			return $return;
		}
	}



}


/* pocatecni nastaveni pro vystup do HTML*/
$succ = 0;
$fail = 0;
$all_tests = count($sources_to_work_with);
$test_info = array();


/* pro kazdy src soubor zkontroluj jestli existuje i stejny nazev s koncovkou in, out a rc*/
/* pokud in neexistuje tak ho vytvor*/
/* pokud out neexistuje tak ho vytvor*/
/* pokud rc neexistuje tak ho vytvor a uloz do nej 0*/

/* dale pokracuje nastaveni volani funkci (podle toho jestli chceme testovat pouze parser/interpret nebo oboje*/
/* pro kazdy test volame funkci*/
/* po navraceni se z funkce uloz do pole test_info vysledek testovani*/
if($sources_to_work_with){
	foreach($sources_to_work_with as $test_file){

		$in_file = preg_replace('/(.src)$/', ".in", $test_file);
	    $out_file = preg_replace('/(.src)$/', ".out", $test_file);
	    $rc_file = preg_replace('/(.src)$/', ".rc", $test_file);

	    if(!in_array($in_file, $files)){
	        $file_to_generate = fopen($in_file, "w");
	        fclose($file_to_generate);
	    }


	    if(!in_array($out_file, $files)){
	        $file_to_generate = fopen($out_file, "w");
	        fclose($file_to_generate);
	    }

	    if(!in_array($rc_file, $files)){
	        $rc_value = 0;
	        $file_to_generate = fopen($rc_file, "w");
	        fwrite($file_to_generate, $rc_value);
	        fclose($file_to_generate);
	    }
	    else{
	        $file_to_read = fopen($rc_file, "r");
	        $rc_value = fread($file_to_read, filesize($rc_file));
	        fclose($file_to_read);
	    }

	    if($parse_only){
	    	$x = parse_test($test_file, $rc_value, $out_file);
	    	array_push($test_info, $x);
	    }

	    if($int_only){
	    	$x = int_test($test_file, $rc_value, $out_file, $in_file);
	    	array_push($test_info, $x);
	    }

	    if(!$parse_only && !$int_only){
	    	$x = test_both($test_file, $rc_value, $out_file, $in_file);
	    	array_push($test_info, $x);
	    }

	}
}

/*generovani vysledeho HTML5 souboru -> meni se pozadi podle toho jestli vsechny testy prosly/neprosly/neuskutecnily se*/

echo '
<!DOCTYPE HTML>

<html>

<head>
	<meta charset="UTF-8">
	<meta name="description" content="Free Web tutorials">
  	<meta name="keywords" content="HTML,CSS">
    <meta name="author" content="Ondřej Studnička">
	<title>IPP report</title>
	<link href="https://fonts.googleapis.com/css?family=Roboto:400,700&amp;subset=latin-ext" rel="stylesheet">

	<style type="text/css">
';
echo 'body{';
if($sources_to_work_with){

	if ($fail != 0){
		echo 'background-color: #e84118;';
		echo 'font-family: "Roboto", sans-serif;';
		echo 'margin:0;';
		echo 'padding: 0;';
	}
	else{
		echo 'background-color: #4cd137;';
		echo 'font-family: "Roboto", sans-serif;';
		echo 'margin:0;';
		echo 'padding: 0;';
	}
}
else{

	echo 'background-color: #40739e;';
	echo 'font-family: "Roboto", sans-serif;';
	echo 'margin:0;';
	echo 'padding: 0;';

}

echo '}';



echo '
	.main{
  			margin: 80px auto;
  			width: 960px;
  			background-color: white; 
  			padding: 15px 30px;
  			box-sizing: border-box;
  			position: relative;  			
		}

		h1{
			color: #192a56;
			font-size: 60px;
		}
		.summary{
			width:100%;
			background-color: #7f8fa6;
			height: 60px;
			box-sizing: border-box;
			padding-left: 15px;
			box-sizing: border-box;
			position: absolute;
			left:0;
		}

		h2{
			display: inline-block;
			padding-left: 15px;
			margin-right: 70px;
			color:#f5f6fa;
		}

		.tests{
			margin-top: 130px;
		}

		table tr:nth-child(odd){
			background-color: #dcdde1; 
		}

		table { 
    	border-spacing: 0px;
    	border-collapse: separate;
		}



		td{
			word-break: break-all;
			height: 40px;
			box-sizing: border-box;
			padding-left:5px;
		}

		th{
			box-sizing: border-box;
			padding-left:5px;
		}

	</style>
</head>
';

echo'
<body>

	<div class="main">
		<h1>IPP report</h1>
		<div class="summary">
';

echo '
	<h2>Celkový počt testů: <span style="color:#2f3640">'.$all_tests.'</span></h2>
';

echo '
	<h2>Prošlo: <span style="color:#4cd137">'.$succ.'</span></h2>
';

echo '
	<h2>Neprošlo: <span style="color:#e84118">'.$fail.'</span></h2>
';

echo '
	</div>
		<div class="tests">

			<table style="width:100%">
  				<tr>
    				<th style="width:25%" align="left"><h3>Název</h3></th>
    				<th style="width:25%" align="left"><h3>Prošel</h3></th> 
    				<th style="width:25%" align="left"><h3>Vrácená hodnota</h3></th>
    				<th style="width:25%" align="left"><h3>Očekávaná hodnota</h3></th>
  				</tr>
';

foreach ($test_info as $info){
	if($info[1] == "OK"){
		$color = "#4cd137";
	}
	else{
		$color = "#e84118";
	}
	echo '<tr>';
	echo '<td style="width:25%" align="left">'.$info[0].'</td>';
	echo '<td style="width:25%" align="left"><span style="color:'.$color.'; font-weight:700">'.$info[1].'</span></td>';
	echo '<td style="width:25%" align="left">'.$info[2].'</td>';
	echo '<td style="width:25%" align="left">'.$info[3].'</td>';
	echo '</tr>';
}

echo '
  			</table>		
			
		</div>
	</div>




</body>

</html>

';

?>