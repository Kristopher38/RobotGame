-- This example demonstrates use of the map display block
-- together with the radar block. It displays 5x5 area around
-- the robot on the screen.
-- The connections should be made as follows:
-- programmable block:input1 <=> radar:data_output
-- programmable block:output1 <=> radar:position_input
-- programmable block:output2 <=> map display:display_input
-- 
-- You can also hook buttons to the motor controller block to
-- move around and see how the displayed area changes. Items
-- (displayed in yellow) are blocks that are added to your
-- inventory, and are automatically picked up when you move
-- to their position.

-- initialize display data
local disp = {}
for i = 1, 14*14 do
	disp[i] = 0
end

-- area size - you can query the radar for a maximum 5x5 area
-- this can be improved by doing several queries to the radar
-- over multiple update() calls and rendering a bigger area
local sizex = 5
local sizey = 5

-- this gets called every tick
function update()
	-- query the radar by putting data on its input,
	-- offset -2, -2, means two blocks to the left (first -2) and
	-- two blocks up (second -2)
	ports.output1 = {-2, -2, sizex, sizey}
	
	-- radar returns the values on the next update() call,
	-- but we can just ignore the results if the data isn't
	-- a table
	local values = ports.input1
	if type(values) ~= "table" then
		values = {}
	end
	
	for i, block in pairs(values) do
		-- calculate correct screen coordinates
		local x = (i-1) % sizex
		local y = (i-1) // sizex
		local pos = y * 14 + x + 1
		
		-- possible values for elements returned from
		-- the radar are "wall", "robot", "floor" and "item"
		if block == "wall" then
			disp[pos] = color.red
		elseif block == "robot" then
			disp[pos] = color.green
		elseif block == "floor" then
			disp[pos] = color.blue
		elseif block == "item" then
			disp[pos] = color.yellow
		end
	end
	-- put our display data on the display
	ports.output2 = disp
end