----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.09.2021 18:21:27
-- Design Name: 
-- Module Name: FSM1 - Behavioral
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

entity FSM1 is
    Port ( Clock : in STD_LOGIC;
           Input : in STD_LOGIC;
           Reset : in STD_LOGIC;
           Detector : inout STD_LOGIC);
end FSM1;

architecture Behavioral of FSM1 is
type FSM is (Zero, One, OneOne, OneOneZero, OneOneZeroZero,OneOneZeroZeroOne);
signal CurrentState, NextState: FSM;
begin

-- Clock and Reset
process(Clock,Reset)
begin
 if(reset='1') then
  CurrentState <= Zero;
 elsif(rising_edge(clock)) then
  CurrentState <= NextState;
 end if;
end process;

-- Sequence Detection
process(CurrentState,Input)
begin
 case(CurrentState) is
 when Zero =>
  if(Input='1') then
   NextState <= One;
  else
   NextState <= Zero;
  end if;
 when One =>
  if(Input='1') then
   NextState <= OneOne;
  else
   NextState <= Zero;
  end if;  
 when OneOne => 
  if(Input='0') then
   NextState <= OneOneZero;
  else
   NextState <= OneOne;
  end if;  
 when OneOneZero =>
  if(Input='0') then
   NextState <= OneOneZeroZero;
  else
   NextState <= One;
  end if; 
 when OneOneZeroZero =>
  if(Input='1') then
   NextState <= OneOneZeroZeroOne;
  else
   NextState <= Zero;
  end if;
  when OneOneZeroZeroOne =>
  if(Input='1') then
   NextState <= OneOne;
  else
   NextState <= Zero;
  end if;
 end case;
end process;

-- Output of Detector
process(currentState)
begin 
 case CurrentState is
 when Zero =>
  Detector <= '0';
 when One =>
  Detector <= '0'; 
 when OneOne => 
  Detector <= '0'; 
 when OneOneZero =>
  Detector <= '0'; 
 when OneOneZeroZero =>
  Detector <= '0';
 when OneOneZeroZeroOne =>
  Detector <= '1'; -- Detection of the correct sequence
 end case;
end process;

end Behavioral;
