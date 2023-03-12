----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04.09.2021 23:18:03
-- Design Name: 
-- Module Name: test_four_bit_adder - Behavioral
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity test_four_bit_adder is
--  Port ( );
end test_four_bit_adder;

architecture Behavioral of test_four_bit_adder is
    component four_bit_adder 
    Port ( A : in STD_LOGIC_VECTOR (3 downto 0);
           B : in STD_LOGIC_VECTOR (3 downto 0);
           Cin : in STD_LOGIC;
           Cout : out STD_LOGIC;
           Sum : out STD_LOGIC_VECTOR (3 downto 0));
    end component;
    
    signal inA: std_logic_vector(3 downto 0);
    signal inB: std_logic_vector(3 downto 0);
    signal inCin: std_logic;
    signal outCout: std_logic;
    signal outSum: std_logic_vector(3 downto 0);
    
begin
uut: four_bit_adder port map (
    A => inA,
    B => inB,
    Cin => inCin,
    Cout => outCout,
    Sum => outSum
);

  input_gen: process
  begin
  
    -- Both positive numbers
    inA <= std_logic_vector(to_signed(4, inA'length));
    inB <= std_logic_vector(to_signed(5, inB'length));
    inCin <= '0';
    wait for 5ps;
    -- Positive numbers with carry out
    inA <= std_logic_vector(to_signed(11, inA'length));
    inB <= std_logic_vector(to_signed(11, inB'length));
    inCin <= '0';
    wait for 5ps;
    -- Both positive numbers and Carry in
    inA <= std_logic_vector(to_signed(4, inA'length));
    inB <= std_logic_vector(to_signed(4, inB'length));
    inCin <= '1';
    wait for 5ps;
             
    end process;

end Behavioral;
