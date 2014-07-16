local swizzleMap = {

	["x"] = 0,
	["y"] = 1,
	["z"] = 2,
	["w"] = 3,
	
	["u"] = 0,
	["v"] = 1,
	
	["r"] = 0,
	["g"] = 1,
	["b"] = 2,
	["a"] = 3
	
}

local vec_elements = { }

vec_elements[2] = { "x", "y", "r", "g", "u", "v" }
vec_elements[3] = { "x", "y", "z", "r", "g", "b", "u", "v" }
vec_elements[4] = { "x", "y", "z", "w", "r", "g", "b", "a", "u", "v" }

local min_length = 2
local max_length = 4

for i = min_length, max_length do

	vec_elements[i] = { }
	
	local j = 1
	
	for k, v in pairs(swizzleMap) do
	
		if v < i then
		
			vec_elements[i][j] = k
			j = j + 1
		
		end
	
	end

end

local function printSwizzle(f, swizzle)

	local swizzle_str = ""

	for i, x in pairs(swizzle) do
		swizzle_str = swizzle_str .. x
	end
	
	local matrix_type = "Matrix<T, " .. #swizzle .. ", 1>"
	
	local function_def = "inline " .. matrix_type .. " " .. swizzle_str .. "(void) const { return " .. matrix_type .. "("
	
	for i, x in pairs(swizzle) do
	
		if (i > 1) then
			function_def = function_def .. ", "
		end
		
		function_def = function_def .. "*(m_data + " .. swizzleMap[x] .. ")"
	end
	
	function_def = function_def .. "); }\n"
	
	f:write(function_def)

end

local function swizzleCombinations(f, elements, output, k, maxlen)

	if (k > maxlen) then
			printSwizzle(f, output)
		return
	end

	for i, x in pairs(elements) do
	
		output[k] = x
		swizzleCombinations(f, elements, output, k + 1, maxlen)
	
	end

end

for i = 2, 4 do

	local f = io.open("SwizzleVector" .. i .. ".h", "w")
	
	f:write("#pragma once\n")
	
	for j, x in pairs(vec_elements[i]) do
	
		local function_def  = "inline const T& " .. x .. "(void) const { return *(m_data + " .. swizzleMap[x] .. "); }\n"
		local function_def2 = "inline T& " .. x .. "(void) { return *(m_data + " .. swizzleMap[x] .. "); }\n"
		
		f:write(function_def)
		f:write(function_def2)
	
	end
	
	for l = 2, 4 do
	
		swizzleCombinations(f, vec_elements[i], { }, 1, l)
	
	end
	
	f:close()

end
