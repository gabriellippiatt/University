----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 05.09.2021 19:55:56
-- Design Name: 
-- Module Name: test_two_digit_bcd_adder - Behavioral
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

entity test_two_digit_bcd_adder is
--  Port ( );
end test_two_digit_bcd_adder;

architecture Behavioral of test_two_digit_bcd_adder is
    component two_digit_bcd_adder 
    Port ( A1 : in STD_LOGIC_VECTOR (3 downto 0);
           A2 : in STD_LOGIC_VECTOR (3 downto 0);
           B1 : in STD_LOGIC_VECTOR (3 downto 0);
           B2 : in STD_LOGIC_VECTOR (3 downto 0);
           Cin : in STD_LOGIC;
           Cout : out STD_LOGIC;
           Sum1 : out STD_LOGIC_VECTOR (3 downto 0);
           Sum2 : out STD_LOGIC_VECTOR (3 downto 0);
           Sum3 : out STD_LOGIC_VECTOR (3 downto 0));
    end component;
    
    signal inA1: std_logic_vector(3 downto 0);
    signal inA2: std_logic_vector(3 downto 0);
    signal inB1: std_logic_vector(3 downto 0);
    signal inB2: std_logic_vector(3 downto 0);
    signal inCin: std_logic;
    signal outCout: std_logic;
    signal outSum1: std_logic_vector(3 downto 0);
    signal outSum2: std_logic_vector(3 downto 0);
    signal outSum3: std_logic_vector(3 downto 0);
    
begin
uut: two_digit_bcd_adder port map (
   A1 => inA1,
   A2 => inA2,
   B1 => inB1,
   B2 => inB2,
   Cin => inCin,
   Cout => outCout,
   Sum1 => outSum1,
   Sum2 => outSum2,
   Sum3 => outSum3
);
  input_gen: process
  begin
    -- 4 inputs with no carry in (Min)
    inA1 <= std_logic_vector(to_signed(1, inA1'length));
    inA2 <= std_logic_vector(to_signed(0, inA2'length));
    inB1 <= std_logic_vector(to_signed(1, inB1'length));
    inB2 <= std_logic_vector(to_signed(0, inB2'length));
    inCin <= '0';
    wait for 5ps;
    -- 4 inputs with no carry in (Max)
    inA1 <= std_logic_vector(to_signed(9, inA1'length));
    inA2 <= std_logic_vector(to_signed(9, inA2'length));
    inB1 <= std_logic_vector(to_signed(9, inB1'length));
    inB2 <= std_logic_vector(to_signed(9, inB2'length));
    inCin <= '0';
    wait for 5ps;
    -- 4 inputs with a carry in (Min)
    inA1 <= std_logic_vector(to_signed(1, inA1'length));
    inA2 <= std_logic_vector(to_signed(0, inA2'length));
    inB1 <= std_logic_vector(to_signed(1, inB1'length));
    inB2 <= std_logic_vector(to_signed(0, inB2'length));
    inCin <= '1';
    wait for 5ps;
    -- 4 inputs with a carry in (Max)
    inA1 <= std_logic_vector(to_signed(9, inA1'length));
    inA2 <= std_logic_vector(to_signed(9, inA2'length));
    inB1 <= std_logic_vector(to_signed(9, inB1'length));
    inB2 <= std_logic_vector(to_signed(9, inB2'length));
    inCin <= '1';
    wait for 5ps;
    -- 4 inputs with an invalid input
    inA1 <= std_logic_vector(to_signed(10, inA1'length));
    inA2 <= std_logic_vector(to_signed(9, inA2'length));
    inB1 <= std_logic_vector(to_signed(9, inB1'length));
    inB2 <= std_logic_vector(to_signed(9, inB2'length));
    inCin <= '0';
    wait for 5ps;
    -- 4 inputs with an invalid input and carry in
    inA1 <= std_logic_vector(to_signed(10, inA1'length));
    inA2 <= std_logic_vector(to_signed(9, inA2'length));
    inB1 <= std_logic_vector(to_signed(9, inB1'length));
    inB2 <= std_logic_vector(to_signed(9, inB2'length));
    inCin <= '1';
    wait for 5ps;
    -- 4 inputs that will cause carry out with the carry in
    inA1 <= std_logic_vector(to_signed(4, inA1'length));
    inA2 <= std_logic_vector(to_signed(0, inA2'length));
    inB1 <= std_logic_vector(to_signed(5, inB1'length));
    inB2 <= std_logic_vector(to_signed(0, inB2'length));
    inCin <= '1';
    wait for 5ps;
             
    end process;

end Behavioral;
