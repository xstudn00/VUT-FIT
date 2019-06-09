-- fsm.vhd: Finite State Machine
-- Author(s): Ondrej Studnicka (xstudn00@stud.fit.vutbr.cz)
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_1, TEST_2, TEST_3, TEST_4, TEST_5, TEST_6, -- testy spolecnych znaku z kodu
      TEST_7A, TEST_8A, TEST_9A, -- testy prvniho vstupniho kodu
      TEST_7B, TEST_8B, TEST_9B, -- testy druheho vstupniho kodu
      TEST_10, TEST_11, FAIL, PRINT_MSG_FAIL, PRINT_MSG_SUCC, FINISH); -- test spolecnych znaku; stavy, do kterych se muzu dostat
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;
-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_1: zmacknuta 1 -> pokracuju na TEST_2; zmaknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_1 =>
      next_state <= TEST_1;
      if (KEY(1) = '1') then
         next_state <= TEST_2;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_2: zmacknuta 1 -> pokracuju na TEST_3; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_2 =>
      next_state <= TEST_2;
      if (KEY(1) = '1') then
         next_state <= TEST_3;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if;  

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_3: zmacknuta 5 -> pokracuju na TEST_4; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_3 =>
      next_state <= TEST_3;
      if (KEY(5) = '1') then
         next_state <= TEST_4;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_4: zmacknuta 1 -> pokracuju na TEST_5; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_4 =>
      next_state <= TEST_4;
      if (KEY(1) = '1') then
         next_state <= TEST_5;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_5: zmacknuta 1 -> pokracuju na TEST_6; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_5 =>
      next_state <= TEST_5;
      if (KEY(0) = '1') then
         next_state <= TEST_6;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_6: zmacknuta 5 -> pokracuju na TEST_7A; zmacknuta 0 -> pokracuju na TEST_7B; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_6 =>
      next_state <= TEST_6;
      if (KEY(5) = '1') then
         next_state <= TEST_7A;
      elsif (KEY(0) = '1') then
         next_state <= TEST_7B;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_7A: zmacknuta 1 -> pokracuju na TEST_8A; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_7A =>
      next_state <= TEST_7A;
      if (KEY(1) = '1') then
         next_state <= TEST_8A;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_8A: zmacknuta 3 -> pokracuju na TEST_9A; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_8A =>
      next_state <= TEST_8A;
      if (KEY(3) = '1') then
         next_state <= TEST_9A;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_9A: zmacknuta 1 -> pokracuju na TEST_10; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_9A =>
      next_state <= TEST_9A;
      if (KEY(3) = '1') then
         next_state <= TEST_10;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_7B: zmacknuta 7 -> pokracuju na TEST_8B; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_7B =>
      next_state <= TEST_7B;
      if (KEY(7) = '1') then
         next_state <= TEST_8B;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_8B: zmacknuta 9 -> pokracuju na TEST_9B; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_8B =>
      next_state <= TEST_8B;
      if (KEY(9) = '1') then
         next_state <= TEST_9B;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_9B: zmacknuta 9 -> pokracuju na TEST_10; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_9B =>
      next_state <= TEST_9B;
      if (KEY(9) = '1') then
         next_state <= TEST_10;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 

   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_10: zmacknuta 2 -> pokracuju na TEST_11; zmacknut # -> pristup odepren; zmacknuto cokoliv jineho -> FAIL
   when TEST_10 =>
      next_state <= TEST_10;
      if (KEY(2) = '1') then
         next_state <= TEST_11;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TEST_11: zmacknuta # -> pokracuju na PRINT_MSG_SUCC; zmacknuto cokoliv jineho -> FAIL
   when TEST_11 =>
      next_state <= TEST_11;
      if (KEY(15) = '1') then
         next_state <= PRINT_MSG_SUCC;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= FAIL;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- zmacknuto # -> pokracuju na PRINT_MSG_FAIL
   when FAIL =>
      next_state <= FAIL;
      if (KEY(15) = '1') then
         next_state <= PRINT_MSG_FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- CNT_OF = '1' -> skocim na FINISH
   when PRINT_MSG_FAIL =>
      next_state <= PRINT_MSG_FAIL;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- CNT_OF = '1' -> skocim na FINISH
   when PRINT_MSG_SUCC =>
      next_state <= PRINT_MSG_SUCC;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- pokud opet zmacknu #, tak se vracim na TEST_1 a cekam na zadani
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_1;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- ostatni pripady -> TEST_1
   when others =>
      next_state <= TEST_1;

   end case;
end process next_state_logic;


-- -------------------------------------------------------

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin

   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MSG_SUCC =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MSG_FAIL =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '0';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   if (KEY(14 downto 0) /= "000000000000000") then
      FSM_LCD_WR     <= '1';
   end if;
   if (KEY(15) = '1') then
      FSM_LCD_CLR    <= '1';
   end if;  
   end case;
end process output_logic;

end architecture behavioral;
