----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 07.08.2021 20:02:41
-- Design Name: 
-- Module Name: test_and2or - Behavioral
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
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity test_and2or is
--  Port ( );
end test_and2or;

ARCHITECTURE behavioral OF test_and2or IS

COMPONENT and2or
PORT(
in1 : IN std_logic;
in2 : IN std_logic;
in3 : IN std_logic;
in4 : IN std_logic;
outandor : OUT std_logic;
outandor_flow : OUT std_logic;
outandor_beh : OUT std_logic);
END COMPONENT;

signal inputs : std_logic_vector(3 downto 0) := "0000";
signal outputs : std_logic_vector(2 downto 0) := "000";

BEGIN

--Instantiate the Unit Under Test (UUT)
uut: and2or PORT MAP (
in1 => inputs(0),
in2 => inputs(1),
in3 => inputs(2),
in4 => inputs(3),
outandor => outputs(0),
outandor_flow => outputs(1),
outandor_beh => outputs(2));

    input_gen : process

        begin
    
            inputs <= "0000"; 
	    --this loop will  output the truth table for  an AND gate
            for I in 1 to 16 loop
                wait for 10ps;
                    if ((inputs(0) = '1' and inputs(1) = '1') or (inputs(2) = '1' and inputs(3) = '1')) then
                        assert (outputs(0) = '1') report "bad gate - stuck at 0" severity error; 
                        assert (outputs(1) = '1') report "bad gate - stuck at 0" severity error; 
                        assert (outputs(2) = '1') report "bad gate - stuck at 0" severity error; 
                    else
                        assert (outputs(0) = '0') report "bad gate - stuck at 1" severity error; 
                        assert (outputs(1) = '0') report "bad gate - stuck at 1" severity error; 
                        assert (outputs(2) = '0') report "bad gate - stuck at 1" severity error;
                    end if;
--                if (inputs = "00") then
--	                assert (outAnd = '0') report "bad gate - stuck at 1" severity error;
--                elsif (inputs = "10") then
--                    assert (outAnd = '0') report "bad  gate - stuck at 1 " severity error;
--                elsif (inputs = "01") then
--                    assert (outAnd = '0') report "bad gate - stuck at 1" severity error;
--                elsif (inputs = "11") then
--                    assert (outAnd = '1') report "bad gate - stuck at 0" severity error;
--                end if;
                inputs <= inputs + '1';
            end loop;
            wait;
    end process;


END Behavioral;
