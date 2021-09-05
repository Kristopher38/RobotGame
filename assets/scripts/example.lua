-- write code that runs once here

function ping(x, y, w, h)
	ports.output1 = {x, y, w, h}
end

function readData()
	local v = ports.input1
	if type(v) == "table" then
		return ports.input1
	else
		return {"none"}
	end
end

local disp = {}

for i = 1, 14*14 do
	disp[i] = 0
end

function setDisp(x, y, color)
	disp[y * 14 + x + 1] = color
end

function update()
	local sizex = 5
	local sizey = 5
	ping(-1, -1, sizex, sizey)
	for i, block in pairs(readData()) do
		print(block)
		local x = (i-1) % sizex
		local y = (i-1) // sizex
		local pos = y * 14 + x + 1
		if block == "wall" then
			disp[pos] = color.red
		elseif block == "robot" then
			disp[pos] = color.green
		elseif block == "floor" then
			disp[pos] = color.blue
		elseif block == "item" then
			disp[pos] = color.yellow
		elseif block == "none" then
			disp[pos] = color.light_gray
		end
	end
	ports.output2 = disp
end