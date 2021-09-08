-- This game is about building your own control panel and programming its components
-- in Lua. If you want to read about game objectives, read objectives.lua. If you want to
-- know how to get started with programming in-game, read on.
--
-- This is a basic example to get you started, illustrating how the UI
-- works and how to interact with other blocks with the programmable block.
-- We will make a simple repeater out of a programmable block
-- that will forward the button signal to a diode (this is just for
-- demonstration purposes; button and diode can be directly connected)
--
-- First, place a programmable block, diode and button on the grid. Now,
-- right click the button, select its only port, left click on the programmable
-- block, and select port named "input1". Then right click the diode, select its
-- only port and connect it to the programmable block's "output1" port. With that
-- set up, right click on the programmable block again, click "edit code",
-- paste the code below into the editor and close it. Now press M to switch
-- to interactive mode, and try pressing a button. You should see the diode light up.
--
-- The way below code works is simple - programmable block has access to a global named
-- ports, which contains the states of all of its input and output ports. Button provides
-- a true/false value, and diode receives a true/false value. We assign the button value
-- available at the programmable's block "input1" port to "output1" port - which is
-- connected to the diode input, so that the diode's state directly corresponds to the
-- button's state. We do this in the update() function, which is called every tick,
-- so that we can see the results immediately.
--
-- More details about how ports work and what's available in in-game Lua are provided in
-- details.lua file

function update()
	ports.output1 = ports.input1
end