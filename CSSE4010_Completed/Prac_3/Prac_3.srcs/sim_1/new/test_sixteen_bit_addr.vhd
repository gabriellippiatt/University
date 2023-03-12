----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02.09.2021 17:06:59
-- Design Name: 
-- Module Name: test_sixteen_bit_addr - Behavioral
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

entity test_sixteen_bit_addr is
--  Port ( );
end test_sixteen_bit_addr;

architecture Behavioral of test_sixteen_bit_addr is
    component sixteen_bit_addr 
    Port ( A : in STD_LOGIC_VECTOR (15 downto 0);
           B : in STD_LOGIC_VECTOR (15 downto 0);
           Cin : in STD_LOGIC_VECTOR (15 downto 0);
           Sum : inout STD_LOGIC_VECTOR (15 downto 0);
           N : out std_logic;
           Z : out std_logic;
           C : out std_logic;
           V : out std_logic;
           SatEn : in std_logic);
    end component;
    
    signal inA: std_logic_vector(15 downto 0);
    signal inB: std_logic_vector(15 downto 0);
    signal inCin: std_logic_vector(15 downto 0);
    signal outSum: std_logic_vector(15 downto 0);
    signal outN: std_logic;
    signal outZ: std_logic;
    signal outC: std_logic;
    signal outV: std_logic;
    signal inSatEn: std_logic;
    
begin
uut: sixteen_bit_addr port map (
    A => inA,
    B => inB,
    Cin => inCin,
    Sum => outSum,
    N => outN,
    Z => outZ,
    C => outC,
    V => outV,
    SatEn => inSatEn
);
  input_gen: process
  begin
--                        -- Test For Flags and saturation
--    -- Negative Flag
--    inA <= std_logic_vector(to_signed(-2, inA'length));
--    inB <= std_logic_vector(to_signed(1, inB'length));
--    inCin <= std_logic_vector(to_signed(0, inCin'length));
--    inSatEn <= '0';
--    wait for 5ps;
    
--    -- Zero Flag
--    inA <= std_logic_vector(to_signed(0, inA'length));
--    inB <= std_logic_vector(to_signed(0, inB'length));
--    inCin <= std_logic_vector(to_signed(0, inCin'length));
--    inSatEn <= '0';
--    wait for 5ps;
    
--    -- Carry Out Flag
--    inA <= std_logic_vector(to_signed(-32767, inA'length));
--    inB <= std_logic_vector(to_signed(-32767, inB'length));
--    inCin <= std_logic_vector(to_signed(0   , inCin'length));
--    inSatEn <= '0';
--    wait for 5ps;
    
--    -- Over Flow Flag
--    inA <= std_logic_vector(to_signed(32767, inA'length));
--    inB <= std_logic_vector(to_signed(1, inB'length));
--    inCin <= std_logic_vector(to_signed(1   , inCin'length));
--    inSatEn <= '0';
--    wait for 5ps;
    
--    -- Positive Saturation
--    inA <= std_logic_vector(to_signed(500, inA'length));
--    inB <= std_logic_vector(to_signed(500, inB'length));
--    inCin <= std_logic_vector(to_signed(1, inCin'length));
--    inSatEn <= '1';
--    wait for 5ps;
    
--    -- Negative Saturation
--    inA <= std_logic_vector(to_signed(32767, inA'length));
--    inB <= std_logic_vector(to_signed(-32767, inB'length));
--    inCin <= std_logic_vector(to_signed(0   , inCin'length));
--    inSatEn <= '1';
--    wait for 5ps;
                        -- Test Arithmatic
    -- Both Positive numbers
    inA <= std_logic_vector(to_signed(5, inA'length));
    inB <= std_logic_vector(to_signed(4, inB'length));
    inCin <= std_logic_vector(to_signed(0   , inCin'length));
    inSatEn <= '0';
    wait for 5ps;
    
    -- Both negative numbers
    inA <= std_logic_vector(to_signed(-5, inA'length));
    inB <= std_logic_vector(to_signed(-4, inB'length));
    inCin <= std_logic_vector(to_signed(0, inCin'length));
    inSatEn <= '0';
    wait for 5ps;
    
    -- Negative and Positive Number
    inA <= std_logic_vector(to_signed(5, inA'length));
    inB <= std_logic_vector(to_signed(-4, inB'length));
    inCin <= std_logic_vector(to_signed(0   , inCin'length));
    inSatEn <= '0';
    wait for 5ps;                
    
    -- Zero input
    inA <= std_logic_vector(to_signed(0, inA'length));
    inB <= std_logic_vector(to_signed(0, inB'length));
    inCin <= std_logic_vector(to_signed(0   , inCin'length));
    inSatEn <= '0';
    wait for 5ps;                
    end process;

end Behavioral;
