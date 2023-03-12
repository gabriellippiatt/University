----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02.09.2021 16:58:15
-- Design Name: 
-- Module Name: sixteen_bit_addr - Behavioral
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

entity sixteen_bit_addr is
    Port ( A : in STD_LOGIC_VECTOR (15 downto 0);
           B : in STD_LOGIC_VECTOR (15 downto 0);
           Cin : in STD_LOGIC_VECTOR (15 downto 0);
           Sum : inout STD_LOGIC_VECTOR (15 downto 0);
           N : out std_logic;
           Z : out std_logic;
           C : out std_logic;
           V : out std_logic;
           SatEn : in std_logic);
end sixteen_bit_addr;

architecture Behavioral of sixteen_bit_addr is

begin
process(A, B, Cin, Sum, SatEn)
variable Cout : STD_LOGIC_VECTOR (15 downto 0);
variable CinArray : STD_LOGIC_VECTOR (16 downto 0);
begin

-- Create a copy of the Cin Array
CinArray(0) := Cin(0);

-- Calculate the Carry out
for I in 0 to 15 loop
    Cout(I) := (A(I) AND B(I)) OR (CinArray(I) AND (A(I) XOR B(I)));
    
    CinArray(I+1) := Cout(I);
end loop;

-- Carry Out Flag
C <= Cout(15);

-- Over Flow Flag
if (Cout(15) /= CinArray(15)) then
    V <= '1';
else
    V <= '0';
end if;

-- First Check Saturation pin
if (SatEn = '0') then
    Sum <= A + B + Cin;
elsif (SatEn = '1') then
    -- Negative overflow
    if ((Cin(0) = '0') and (Cout(15) = '1')) then
        Sum <= x"8000";
    -- Positive overflow
    elsif ((Cin(0) = '1') and (Cout(15) = '0')) then
        Sum <= x"7FFF";
    end if;
     
end if;

-- Check if the Summation is Zero
if (Sum = std_logic_vector(to_signed(0, Sum'length))) then
    Z <= '1';
else 
    Z <= '0';
end if;

-- Negative Flag
if (Sum(15) = '1') then
    N <= '1';
else
    N <= '0';
end if;

end process;

end Behavioral;
