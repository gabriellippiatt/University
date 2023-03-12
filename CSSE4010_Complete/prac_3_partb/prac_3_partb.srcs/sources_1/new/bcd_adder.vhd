----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04.09.2021 22:09:53
-- Design Name: 
-- Module Name: bcd_adder - Behavioral
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

entity bcd_adder is
    Port ( bcdA : in STD_LOGIC_VECTOR (3 downto 0);
           bcdB : in STD_LOGIC_VECTOR (3 downto 0);
           bcdCin : in STD_LOGIC;
           bcdCout : out STD_LOGIC;
           bcdSum : out STD_LOGIC_VECTOR (3 downto 0));
end bcd_adder;

architecture Behavioral of bcd_adder is
component four_bit_adder
    Port ( A : in STD_LOGIC_VECTOR (3 downto 0);
           B : in STD_LOGIC_VECTOR (3 downto 0);
           Cin : in STD_LOGIC;
           Cout : out STD_LOGIC;
           Sum : out STD_LOGIC_VECTOR (3 downto 0));
           end component;
           
SIGNAL CoutCon :  std_logic;
SIGNAL CoutCon2 :  std_logic;
SIGNAL SumCon :  std_logic_vector(3 downto 0) ;
SIGNAL SumCon2 :  std_logic_vector(3 downto 0) ;
SIGNAL BCDVariable :  std_logic_vector(3 downto 0) ;
SIGNAL CinCon :  std_logic ;
begin

A1: four_bit_adder PORT MAP (
    A => bcdA,
    B => bcdB,
    Cin => bcdCin,
    Cout => CoutCon,
    Sum => SumCon
);

A2: four_bit_adder PORT MAP (
    A => SumCon,
    B => BCDVariable, -- this is 6 or zero
    Cin => CinCon, -- should be zero
    Cout => CoutCon2,
    Sum => SumCon2
);

process(bcdA, bcdB, bcdCin, SumCon,BCDVariable, CinCon, SumCon2, CoutCon, CoutCon2)
begin

CinCon <= '0';

if (bcdA > "1001" or bcdB > "1001") then
    bcdSum <= "0000";
    bcdCout <= '0';
else
    bcdSum <= SumCon2;
    bcdCout <= CoutCon2;
end if;

if (SumCon > "1001" or CoutCon = '1') then
    BCDVariable <= "0110";
else
    BCDVariable <= "0000";
    
end if;


end process;
end Behavioral;
