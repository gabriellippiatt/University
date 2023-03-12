----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 05.09.2021 19:35:55
-- Design Name: 
-- Module Name: two_digit_bcd_adder - Behavioral
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

entity two_digit_bcd_adder is
    Port ( A1 : in STD_LOGIC_VECTOR (3 downto 0);
           A2 : in STD_LOGIC_VECTOR (3 downto 0);
           B1 : in STD_LOGIC_VECTOR (3 downto 0);
           B2 : in STD_LOGIC_VECTOR (3 downto 0);
           Cin : in STD_LOGIC;
           Cout : out STD_LOGIC;
           Sum1 : out STD_LOGIC_VECTOR (3 downto 0);
           Sum2 : out STD_LOGIC_VECTOR (3 downto 0);
           Sum3 : out STD_LOGIC_VECTOR (3 downto 0));
end two_digit_bcd_adder;

architecture Behavioral of two_digit_bcd_adder is
component bcd_adder
    Port ( bcdA : in STD_LOGIC_VECTOR (3 downto 0);
           bcdB : in STD_LOGIC_VECTOR (3 downto 0);
           bcdCin : in STD_LOGIC;
           bcdCout : out STD_LOGIC;
           bcdSum : out STD_LOGIC_VECTOR (3 downto 0));
           end component;
           
SIGNAL A3 :  std_logic_vector(3 downto 0) ;
SIGNAL B3 :  std_logic_vector(3 downto 0) ;
SIGNAL CoutCon1 :  std_logic;
SIGNAL CoutCon2 :  std_logic;
SIGNAL CoutCon3 :  std_logic;
SIGNAL SumCon1 :  std_logic_vector(3 downto 0) ;
SIGNAL SumCon2 :  std_logic_vector(3 downto 0) ;
SIGNAL SumCon3 :  std_logic_vector(3 downto 0) ;
begin

BCD1: bcd_adder PORT MAP (
    bcdA => A1,
    bcdB => B1,
    bcdCin => Cin,
    bcdCout => CoutCon1,
    bcdSum => SumCon1
);

BCD2: bcd_adder PORT MAP (
    bcdA => A2,
    bcdB => B2,
    bcdCin => CoutCon1,
    bcdCout => CoutCon2,
    bcdSum => SumCon2
);

BCD3: bcd_adder PORT MAP (
    bcdA => A3,
    bcdB => B3,
    bcdCin => CoutCon2,
    bcdCout => CoutCon3,
    bcdSum => SumCon3
);

process(A1, A2, B1, B2, Cin, CoutCon1, CoutCon2, CoutCon3, SumCon1, SumCon2, SumCon3)
begin

-- Final adder should have zero inputs and a carry in value
A3 <= "0000";
B3 <= "0000";

-- Make sure inputs are valid
if (A1 > "1001" or A2 > "1001" or B1 > "1001" or B2 > "1001") then
    Sum1 <= "0000";
    Sum2 <= "0000";
    Sum3 <= "0000";
    Cout <= '0';
else
    Sum1 <= SumCon1;
    Sum2 <= SumCon2;
    Sum3 <= SumCon3;
    Cout <= CoutCon3;
end if;


end process;
end Behavioral;
