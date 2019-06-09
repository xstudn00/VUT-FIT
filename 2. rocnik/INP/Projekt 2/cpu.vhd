-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Ondrej Studnicka (xstudn00)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 type fsm_state is (
   stav_0, stav_01,
   stav_1, stav_2, stav_3, stav_3_1, stav_4, stav_4_1, stav_5, stav_5_1, stav_5_2, stav_5_3, stav_6, stav_6_1, stav_6_2, stav_6_3, stav_6_4, stav_6_5,
   stav_7, stav_7_1, stav_8, stav_9, stav_9_1, stav_9_2, stav_10, stav_11, stav_12, stav_13, stav_14, stav_15, stav_16, stav_17, stav_18, stav_19,
   stav_20, stav_21, stav_22, stav_23, stav_24, stav_25, stav_26, stav_27
   );

 signal pstate : fsm_state;
 signal nstate : fsm_state;


 signal data_pc : std_logic_vector(11 downto 0);
 signal inc_pc : std_logic;
 signal dec_pc : std_logic;

 signal data_ptr : std_logic_vector(9 downto 0);
 signal inc_ptr : std_logic;
 signal dec_ptr : std_logic;

 signal data_cnt : std_logic_vector(7 downto 0);
 signal inc_cnt : std_logic;
 signal dec_cnt : std_logic;

 signal mpx : std_logic_vector(1 downto 0);
 signal tmp : std_logic_vector(7 downto 0);


begin

   -- ptr
   ptr_register: process(CLK, RESET, data_ptr, inc_ptr, dec_ptr)
   begin
      if RESET = '1' then
         data_ptr <= (others => '0');
      elsif rising_edge(CLK) then
         if inc_ptr = '1' then
            data_ptr <= data_ptr + 1;
         elsif dec_ptr = '1' then
            data_ptr <= data_ptr - 1;
         end if;
      end if;
      DATA_ADDR <= data_ptr;
   end process;

   selector: process(IN_DATA, DATA_RDATA, mpx)
   begin
      if mpx = "11" then
         DATA_WDATA <= IN_DATA;
      elsif mpx = "00" then
         DATA_WDATA <= DATA_RDATA + 1; 
      elsif mpx = "01" then
         DATA_WDATA <= DATA_RDATA - 1;
      elsif mpx = "10" then
         DATA_WDATA <= tmp;
      end if;
   end process;

   -- cnt
   cnt_register: process(CLK, RESET, data_cnt, inc_cnt, dec_cnt)
   begin
      if RESET = '1' then
         data_cnt <= (others => '0');
      elsif rising_edge(CLK) then
         if inc_cnt = '1' then
            data_cnt <= data_cnt + 1;
         elsif dec_cnt = '1' then
            data_cnt <= data_cnt - 1;
         end if;
      end if;
   end process;

   -- fsm
   fsm_register: process(CLK, RESET)
   begin
      if RESET = '1' then
         pstate <= stav_0;
      elsif rising_edge(CLK) then
         if EN = '1' then
            pstate <= nstate;
         end if;
      end if;
   end process;

      -- pc
   pc_register: process(CLK, RESET, data_pc, inc_pc, dec_pc)
   begin
      if RESET = '1' then
         data_pc <= (others => '0');
      elsif rising_edge(CLK) then
         if inc_pc = '1' then
            data_pc <= data_pc + 1;
         elsif dec_pc = '1' then
            data_pc <= data_pc - 1;
         end if;
      end if;
      CODE_ADDR <= data_pc;
   end process;

   fsm_nstate: process(CODE_DATA, DATA_RDATA, IN_VLD, OUT_BUSY, data_cnt, pstate)
   begin
      inc_ptr <= '0';
      dec_ptr <= '0';
      --   
      inc_pc <= '0';
      dec_pc <= '0';
      --
      inc_cnt <= '0';
      dec_cnt <= '0';
      --
      DATA_EN <= '0';
      IN_REQ <= '0';
      OUT_WE <= '0';
      DATA_RDWR <= '0';
      --
      CODE_EN <= '1';
      --
      mpx <= "00";
      tmp <= "00000000";

      case pstate is
         when stav_0 =>
            CODE_EN <= '1';
            nstate <= stav_01;

         when stav_01 =>
            case CODE_DATA is

               when X"3E" => 
                  nstate <= stav_1;
               when X"3C" => 
                  nstate <= stav_2;
               when X"2B" =>
                  nstate <= stav_3;
               when X"2D" =>
                  nstate <= stav_4;
               when X"5B" =>
                  nstate <= stav_5;
               when X"5D" =>
                  nstate <= stav_6;
               when X"2E" =>
                  nstate <= stav_7;
               when X"2C" =>
                  nstate <= stav_8;
               when X"23" =>
                  nstate <= stav_9;
               when X"30" =>
                  nstate <= stav_10;
               when X"31" =>
                  nstate <= stav_11;
               when X"32" =>
                  nstate <= stav_12;
               when X"33" =>
                  nstate <= stav_13;
               when X"34" =>
                  nstate <= stav_14;
               when X"35" =>
                  nstate <= stav_15;
               when X"36" =>
                  nstate <= stav_16;
               when X"37" =>
                  nstate <= stav_17;
               when X"38" =>
                  nstate <= stav_18;
               when X"39" =>
                  nstate <= stav_19;
               when X"41" =>
                  nstate <= stav_20;
               when X"42" =>
                  nstate <= stav_21;
               when X"43" =>
                  nstate <= stav_22;
               when X"44" =>
                  nstate <= stav_23;
               when X"45" =>
                  nstate <= stav_24;
               when X"46" =>
                  nstate <= stav_25;
               when X"00" =>
                  nstate <= stav_26;
               when others =>
                  nstate <= stav_27;

            end case;

            when stav_1 =>
               inc_ptr <= '1';
               inc_pc <= '1';
               nstate <= stav_0;

            when stav_2 =>
               dec_ptr <= '1';
               inc_pc <= '1';
               nstate <= stav_0;

            when stav_3 =>
               DATA_EN <= '1';
               DATA_RDWR <= '1';
               nstate <= stav_3_1;

            when stav_3_1 =>
               mpx <= "00";
               DATA_EN <= '1';
               DATA_RDWR <= '0';
               inc_pc <= '1';
               nstate <= stav_0;

            when stav_4 =>
               DATA_EN <= '1';
               DATA_RDWR <= '1';
               nstate <= stav_4_1;

            when stav_4_1 =>
               mpx <= "01";
               DATA_EN <= '1';
               DATA_RDWR <= '0';
               inc_pc <= '1';
               nstate <= stav_0;

            when stav_5 =>
               inc_pc <= '1';
               DATA_EN <= '1';
               DATA_RDWR <= '1';
               nstate <= stav_5_1;

            when stav_5_1 =>
               if DATA_RDATA = "00000000" then
                  inc_cnt <= '1';
                  nstate <= stav_5_2;
               else
                  nstate <= stav_0;
               end if;


            when stav_5_2 =>
               if data_cnt = "00000000" then
                  nstate <= stav_0;
               else
                  CODE_EN <= '1';
                  nstate <= stav_5_3;
               end if;

            when stav_5_3 =>
               if CODE_DATA = X"5D" then                  
                  dec_cnt <= '1';
               elsif CODE_DATA = X"5B" then
                  inc_cnt <= '1';
               end if;

               inc_pc <= '1';
               nstate <= stav_5_2;

            when stav_6 =>
               DATA_EN <= '1';
               DATA_RDWR <= '1';
               nstate <= stav_6_1;

            when stav_6_1 =>
               if DATA_RDATA = "00000000" then
                  inc_pc <= '1';
                  nstate <= stav_0;
               else
                  nstate <= stav_6_2;
               end if;

            when stav_6_2 =>
               inc_cnt <= '1';
               dec_pc <= '1';
               nstate <= stav_6_3;

            when stav_6_3 =>
               if data_cnt = "00000000" then
                  nstate <= stav_0;
               else
                  CODE_EN <= '1';
                  nstate <= stav_6_4;
               end if;

            when stav_6_4 =>
               if CODE_DATA = X"5B" then
                  dec_cnt <= '1';
               elsif CODE_DATA = X"5D" then                  
                  inc_cnt <= '1';
               end if;
               nstate <= stav_6_5;

            when stav_6_5 =>
               if data_cnt = "00000000" then
                  inc_pc <= '1';
               else
                  dec_pc <= '1';
               end if;
               nstate <= stav_6_3;

            when stav_7 =>
               if OUT_BUSY = '1' then
                  nstate <= stav_7;
               else
                  DATA_EN <= '1';
                  DATA_RDWR <= '1';
                  nstate <= stav_7_1;
               end if; 

            when stav_7_1 =>
               OUT_WE <= '1';
               OUT_DATA <= DATA_RDATA;
               inc_pc <= '1';
               nstate <= stav_0;

            when stav_8 =>
               IN_REQ <= '1';
               if IN_VLD = '0' then
                  nstate <= stav_8;
               else
                  mpx <= "11";
                  DATA_EN <= '1';
                  DATA_RDWR <= '0';
                  inc_pc <= '1';
                  nstate <= stav_0;
               end if;

               when stav_9 =>
                  inc_pc <= '1';
                  nstate <= stav_9_1;

               when stav_9_1 =>
                  nstate <= stav_9_2;

               when stav_9_2 =>
                  if CODE_DATA = x"23" then
                     inc_pc <= '1';
                     nstate <= stav_0;
                  else
                     nstate <= stav_9;
                  end if;

               when stav_10 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "00000000";
                  nstate <= stav_0;

               when stav_11 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "00010000";
                  nstate <= stav_0;

               when stav_12 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "00100000";
                  nstate <= stav_0;

               when stav_13 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "00110000";
                  nstate <= stav_0;

               when stav_14 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "01000000";
                  nstate <= stav_0;

               when stav_15 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "01010000";
                  nstate <= stav_0;

               when stav_16 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "01100000";
                  nstate <= stav_0;

               when stav_17 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "01110000";
                  nstate <= stav_0;

               when stav_18 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "10000000";
                  nstate <= stav_0;

               when stav_19 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "10010000";
                  nstate <= stav_0;

               when stav_20 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "10100000";
                  nstate <= stav_0;

               when stav_21 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "10110000";
                  nstate <= stav_0;

               when stav_22 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "11000000";
                  nstate <= stav_0;

               when stav_23 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "11010000";
                  nstate <= stav_0;

               when stav_24 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "11100000";
                  nstate <= stav_0;

               when stav_25 =>
                  DATA_EN <= '1';
                  inc_pc <= '1';
                  mpx <= "10";
                  tmp <= "11110000";
                  nstate <= stav_0;

               when stav_26 =>
                  nstate <= stav_26;

               when stav_27 => 
                  inc_pc <= '1';
                  nstate <= stav_0;

               when others => null;

      end case;
   end process;


end behavioral;
 
