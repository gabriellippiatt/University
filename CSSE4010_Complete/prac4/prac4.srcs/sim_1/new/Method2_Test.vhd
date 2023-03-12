----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.09.2021 21:37:12
-- Design Name: 
-- Module Name: Method2_Test - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Method2_Test is
--  Port ( );
end Method2_Test;

architecture Behavioral of Method2_Test is
   component Method2
    Port ( Clock : in STD_LOGIC;
           InputSeq : in STD_LOGIC;
           Reset : in STD_LOGIC;
           ZFlag : out STD_LOGIC);
end component;

signal inClock: std_logic;
signal inInputSeq: std_logic;
signal InReset: std_logic;
signal OutZFlag: std_logic;

constant ClockPeriod : time := 10 ps;
begin
uut: Method2 port map (
    Clock => inClock,
    InputSeq => inInputSeq,
    Reset => inReset,
    ZFlag => outZFlag
);

clock_process :process
begin
inClock <= '0';
wait for ClockPeriod/2;
inClock <= '1';
wait for ClockPeriod/2;
end process;

input_gen: process
begin

--inInputSeq <= '0';
--inReset <= '1';
--wait for 10 ps;
--inReset <= '0';
--wait for 10 ps;
--inInputSeq <= '1';
--wait for 10 ps;
--inInputSeq <= '1';
--wait for 10 ps;
--inInputSeq <= '0'; 
--wait for 10 ps;
--inInputSeq <= '0'; 
--wait for 10 ps;
--inInputSeq <= '1'; 
--wait for 10 ps;
--inInputSeq <= '1';
--wait for 10 ps;
--inInputSeq <= '0'; 
--wait for 10 ps;
--inInputSeq <= '0'; 
--wait for 10 ps;
--inInputSeq <= '1'; 
--wait for 10 ps;
--inInputSeq <= '1'; 
--wait for 10 ps;
--end process;
inInputSeq <= '0';
inReset <= '1';
wait for 10 ps;
inReset <= '0';
wait for 10 ps;
inInputSeq <= '1';
wait for 10 ps;
inInputSeq <= '1';
wait for 10 ps;
inInputSeq <= '0'; 
inReset <= '1';
wait for 10 ps;
inInputSeq <= '0'; 
inReset <= '0';
wait for 10 ps;
inInputSeq <= '1'; 
wait for 10 ps;
inInputSeq <= '1'; 
wait for 10 ps;
inInputSeq <= '1'; 
wait for 10 ps;
inInputSeq <= '0'; 
wait for 10 ps;
end process;
end Behavioral;
