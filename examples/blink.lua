-- Simple diode blink example
-- 
-- In this example you can see how to keep state between
-- calls to update(). Any code outside of update() (in the
-- toplevel scope) is only ran once when you enter
-- interactive mode. This means you can keep your state that
-- needs to be saved between calls to update() and do one-time
-- initialization there.
--
-- Connections that need to be made for this demo are as follows:
-- progammable block:output1 <=> diode:diode_val
--
-- This demo blinks the diode with 1Hz frequency.

local cnt = 0

function update()
	-- simulation runs at 60 ticks per second, so each state (on/off)
	-- should last for 30 ticks
	if cnt % 60 < 30 then
		ports.output1 = true
	else
		ports.output1 = false
	end
	-- increment counter each tick
	cnt = cnt + 1
end