library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- popis rozhrani obvodu
-- vstupni signaly: SMCLK, RESET
-- vystupni signaly: ROW, LED
entity ledc8x8 is
port ( 
    SMCLK: IN std_logic;
    RESET: IN std_logic;
    ROW: OUT std_logic_vector (0 to 7);
    LED: OUT std_logic_vector (0 to 7)

);
end ledc8x8;

-- definice vnitrnich signalu
-- counter: promena pro ziskani cisla 1843200 (SMCLK/4) - protoze 4 stavy
-- ce_counter: promenna pro ziskani cisla 28800 (SMCLK/256)
-- leds: promenna pro urceni sloupce
-- rows: promenna pro urceni radku
-- state: promenna pro urceni stavu (jmeno/nic/prijmeni/nic)
-- eae: enable

architecture main of ledc8x8 is

    signal counter: std_logic_vector (20 downto 0) := (others => '0');
    signal ce_counter: std_logic_vector (14 downto 0) := (others => '0');
    signal leds: std_logic_vector (7 downto 0) := (others => '0');
    signal rows: std_logic_vector (7 downto 0) := (others => '0');
    signal state: std_logic_vector (1 downto 0) := (others => '0');
    signal eae: std_logic;

begin
    -- deleni signalu smclk
    -- asynchronni reset -> nastavim ce_counter do nuly
    -- pokud prijde nabezna hrana, tak zkontroluju jestli se ce_counter rovna 28800, pokud ano, tak nastavim enable do jednicky, jinak nula
    -- inkrementace ce_counter o jedna
    smclk_divider: process(SMCLK, RESET)
    begin
        if RESET = '1' then
            ce_counter <= "000000000000000";
        elsif rising_edge(SMCLK) then
            if ce_counter = "111000010000000" then
                    eae <= '1';
                else
                    eae <= '0';
            end if;

            ce_counter <= ce_counter + 1;
        end if;
    end process smclk_divider;

    -- posun radku
    -- asynchronni reset -> pokud ano, tak nastavuji prvni radek
    -- pokud prijde nabezna hrana a enable je v jednicce, tak posunu jednicku o jednu pozici doprava
    row_changer: process(SMCLK, RESET, eae)
    begin
        if RESET = '1' then
            rows <= "10000000";
        elsif rising_edge(SMCLK) and eae = '1' then
            rows <= rows(0) & rows(7 downto 1);
        end if;
    end process row_changer;

    -- zmena stavu
    -- asynchronni reset, pokud plati podminka, tak nuluju promennou counter
    -- pokud prijde nabezna hrana, tak inkrementuju counter o jednicku
    -- pokud se counter rovna 1843200, tak se posunuju do dalsiho stavu (napriklad z jmena na nic) a nuluju counter
    state_changer: process(SMCLK, RESET)
    begin
        if RESET = '1' then
            counter <= "000000000000000000000";
        elsif rising_edge(SMCLK) then
				counter <= counter + 1;
            if counter = "111000010000000000000" then
                state <= state + 1;
                counter <= "000000000000000000000";
            end if;
        end if;
    end process state_changer;

    -- decoder urcuje zobrazeni pismen
    -- nula znamena zapnuto, jednicka vypnuto
    -- 00 jmeno, 01 nic, 10 prijmeni, 11 nic
    decoder: process(rows, state)
    begin
		if state = "00" then 
			case rows is
				when "10000000" => leds <= "11111111";
                when "01000000" => leds <= "11000011";
                when "00100000" => leds <= "10111101";
                when "00010000" => leds <= "10111101";
                when "00001000" => leds <= "10111101";
                when "00000100" => leds <= "10111101";
                when "00000010" => leds <= "10111101";
                when "00000001" => leds <= "11000011";
                when others     => leds <= "11111111";
			end case;
		elsif state = "01" then
			case rows is
				when "10000000" => leds <= "11111111";
                when "01000000" => leds <= "11111111";
                when "00100000" => leds <= "11111111";
                when "00010000" => leds <= "11111111";
                when "00001000" => leds <= "11111111";
                when "00000100" => leds <= "11111111";
                when "00000010" => leds <= "11111111";
                when "00000001" => leds <= "11111111";
                when others     => leds <= "11111111";
			end case;
		elsif state = "10" then
			case rows is
				when "10000000" => leds <= "11111111";
                when "01000000" => leds <= "11000001";
                when "00100000" => leds <= "10011111";
                when "00010000" => leds <= "10011111";
                when "00001000" => leds <= "11000011";
                when "00000100" => leds <= "11111001";
                when "00000010" => leds <= "11111001";
                when "00000001" => leds <= "10000011";
                when others     => leds <= "11111111";
			end case;
		elsif state = "11" then
			case rows is
				when "10000000" => leds <= "11111111";
                when "01000000" => leds <= "11111111";
                when "00100000" => leds <= "11111111";
                when "00010000" => leds <= "11111111";
                when "00001000" => leds <= "11111111";
                when "00000100" => leds <= "11111111";
                when "00000010" => leds <= "11111111";
                when "00000001" => leds <= "11111111";
                when others     => leds <= "11111111";
			end case;
		end if;

    end process decoder;

    -- prirazeni
    ROW <= rows;
    LED <= leds;

end main;
