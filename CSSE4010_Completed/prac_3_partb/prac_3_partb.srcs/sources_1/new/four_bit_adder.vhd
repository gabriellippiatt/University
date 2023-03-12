----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04.09.2021 21:29:10
-- Design Name: 
-- Module Name: four_bit_adder - Behavioral
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
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity four_bit_adder is
    Port ( A : in STD_LOGIC_VECTOR (3 downto 0);
           B : in STD_LOGIC_VECTOR (3 downto 0);
           Cin : in STD_LOGIC;
           Cout : out STD_LOGIC;
           Sum : out STD_LOGIC_VECTOR (3 downto 0));
end four_bit_adder;

architecture Behavioral of four_bit_adder is

begin
process(A, B, Cin)
variable CoutArray : STD_LOGIC_VECTOR (3 downto 0);
variable CinArray : STD_LOGIC_VECTOR (4 downto 0);
begin

-- Create a copy of the Cin Array
CinArray(0) := Cin;

-- Calculate the Carry out
for I in 0 to 3 loop
    CoutArray(I) := (A(I) AND B(I)) OR (CinArray(I) AND (A(I) XOR B(I)));
    CinArray(I+1) := CoutArray(I);
end loop;

Cout <= CoutArray(3);

-- Compute Sum
Sum <= A + B + Cin;

end process;

end Behavioral;
