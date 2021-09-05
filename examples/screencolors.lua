-- this example demonstrates the map display block use
-- Connection that should be made are:
--
-- programmable block:output1 <=> map display:display_input

-- initialize the display with a colorful pattern
disp = {}
-- Display needs a 196 (14*14) table filled with values 0-15
-- which represent colors from a predefined pallete. Here we just
-- make a repeating pattern that displays all the 16 colors.
-- Alternatively, you can assign colors using a predefined convenience
-- color table, by writing:
-- 
-- disp[i] = color.red
--
-- Available color constants are:
-- black, blue, light_blue,  violet, pink, green, light_green, orange,
-- yellow, red, light_red, brown, light_brown, gray, light_gray, white

for i = 1, 14*14 do
	disp[i] = (i % 16)
end

-- Put our display data on the output port. It's going to stay there
-- unchanged, so the screen will keep displaying the same picture
ports.output1 = disp

-- update function must exist in a script
function update()
end