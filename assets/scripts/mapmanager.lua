local map = require("assets/maps/map")

local layers = {}
for i, layer in ipairs(map.layers) do
	layers[layer.name] = layer
end

function GetPos(layerName, x, y)
	return layers[layerName].data[y * layer.width + x - 1]
end

function SetPos(layerName, x, y, val)
	layers[layerName].data[y * layer.width + x - 1] = val
end

function GetLayerProperty(layerName, propertyName)
	return layers[layerName][propertyName]
end