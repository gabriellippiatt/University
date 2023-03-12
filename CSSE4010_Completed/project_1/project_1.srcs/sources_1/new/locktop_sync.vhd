----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 21.08.2021 21:02:58
-- Design Name: 
-- Module Name: locktop_async - Behavioral
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

entity locktop_sync is
    Port ( DABus : in STD_LOGIC_VECTOR (3 downto 0);
           DBBus : in STD_LOGIC_VECTOR (3 downto 0);
           But1: in std_logic;
           Clock: in std_logic;
           But2: in std_logic;
           Clr: in std_logic;
           LockClosed : out STD_LOGIC;
           LockOpen : out STD_LOGIC;
           SSD1Bus : out STD_LOGIC_VECTOR (6 downto 0);
           SSD2Bus : out STD_LOGIC_VECTOR (6 downto 0);
           SSD3Bus : out STD_LOGIC_VECTOR (6 downto 0);
           SSD4Bus : out STD_LOGIC_VECTOR (6 downto 0));
           
end locktop_sync;

architecture Behavioral of locktop_sync is
component flip_flop_reg_sync
 Port ( 
           DBus : in STD_LOGIC_VECTOR (3 downto 0);
           Reset : in STD_LOGIC;
           Set : in STD_LOGIC;
           QBus : inout STD_LOGIC_VECTOR (3 downto 0);
           Clk : in STD_LOGIC);
           end component;
component lock
Port (
           A1Bus : in STD_LOGIC_VECTOR (3 downto 0);
           A2Bus : in STD_LOGIC_VECTOR (3 downto 0);
           B1Bus : in STD_LOGIC_VECTOR (3 downto 0);
           B2Bus : in STD_LOGIC_VECTOR (3 downto 0);
           Reset : in STD_LOGIC;
           Lock : out STD_LOGIC;
           Unlock : out STD_LOGIC );
           end component;
component sev_seg
Port (
           DBus : in STD_LOGIC_VECTOR (3 downto 0);
           SSDBus : out STD_LOGIC_VECTOR (6 downto 0);
           Reset : in STD_LOGIC );
           end component;
           
SIGNAL A1Con :  std_logic_vector(3 downto 0) ;
SIGNAL A2Con :  std_logic_vector(3 downto 0) ;
SIGNAL B1Con :  std_logic_vector(3 downto 0) ;
SIGNAL B2Con :  std_logic_vector(3 downto 0) ;

begin
F1: flip_flop_reg_sync PORT MAP (
    DBus => DABus,
    Reset => Clr,
    Set => But1,
    QBus => A1Con,
    Clk => Clock
);

F2: flip_flop_reg_sync PORT MAP (
    DBus => DBBus,
    Reset => Clr,
    Set => But1,
    QBus => A2Con,
    Clk => Clock
);


F3: flip_flop_reg_sync PORT MAP (
    DBus => DABus,
    Reset => Clr,
    Set => But2,
    QBus => B1Con,
    Clk => Clock
);


F4: flip_flop_reg_sync PORT MAP (
    DBus => DBBus,
    Reset => Clr,
    Set => But2,
    QBus => B2Con,
    Clk => Clock
);

L0: lock port map (
    A1Bus => A1Con,
    A2Bus => A2Con,
    B1Bus => B1Con,
    B2Bus => B2Con,
    Lock => LockClosed,
    Unlock => LockOpen,
    Reset => Clr
    
 );
 
 S1: sev_seg port map (
    DBus => A1Con,
    SSDBus => SSD1Bus,
    Reset => Clr
    
 );

 S2: sev_seg port map (
    DBus => A2Con,
    SSDBus => SSD2Bus,
    Reset => Clr
    
 );
 
  S3: sev_seg port map (
    DBus => B1Con,
    SSDBus => SSD3Bus,
    Reset => Clr
    
 );
 
  S4: sev_seg port map (
    DBus => B2Con,
    SSDBus => SSD4Bus,
    Reset => Clr
    
 );
end Behavioral;
