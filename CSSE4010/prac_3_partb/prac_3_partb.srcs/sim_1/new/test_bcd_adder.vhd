----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04.09.2021 22:56:29
-- Design Name: 
-- Module Name: test_bcd_adder - Behavioral
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

entity test_bcd_adder is
--  Port ( );
end test_bcd_adder;

architecture Behavioral of test_bcd_adder is
    component bcd_adder 
    Port ( bcdA : in STD_LOGIC_VECTOR (3 downto 0);
           bcdB : in STD_LOGIC_VECTOR (3 downto 0);
           bcdCin : in STD_LOGIC;
           bcdCout : out STD_LOGIC;
           bcdSum : out STD_LOGIC_VECTOR (3 downto 0));
    end component;
    
    signal inbcdA: std_logic_vector(3 downto 0);
    signal inbcdB: std_logic_vector(3 downto 0);
    signal inbcdCin: std_logic;
    signal outbcdCout: std_logic;
    signal outbcdSum: std_logic_vector(3 downto 0);
    
begin
uut: bcd_adder port map (
    bcdA => inbcdA,
    bcdB => inbcdB,
    bcdCin => inbcdCin,
    bcdCout => outbcdCout,
    bcdSum => outbcdSum
);
  input_gen: process
  begin
    -- Test Carry out and no Carry in
    inbcdA <= std_logic_vector(to_signed(4, inbcdA'length));
    inbcdB <= std_logic_vector(to_signed(5, inbcdB'length));
    inbcdCin <= '1';
    wait for 5ps;
     -- Test No Carry out and Carry in
    inbcdA <= std_logic_vector(to_signed(1, inbcdA'length));
    inbcdB <= std_logic_vector(to_signed(1, inbcdB'length));
    inbcdCin <= '0';
    wait for 5ps;
    -- Test No Carry out and no carry in
    inbcdA <= std_logic_vector(to_signed(5, inbcdA'length));
    inbcdB <= std_logic_vector(to_signed(4, inbcdB'length));
    inbcdCin <= '0';
    wait for 5ps;
    -- Test Carry out and Carry in
    inbcdA <= std_logic_vector(to_signed(5, inbcdA'length));
    inbcdB <= std_logic_vector(to_signed(4, inbcdB'length));
    inbcdCin <= '1';
    wait for 5ps;
    -- Test Input larger than 9
    inbcdA <= std_logic_vector(to_signed(10, inbcdA'length));
    inbcdB <= std_logic_vector(to_signed(4, inbcdB'length));
    inbcdCin <= '0';
    wait for 5ps;
             
    end process;

end Behavioral;
