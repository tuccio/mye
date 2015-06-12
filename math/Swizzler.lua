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

local types = { "T", "float" }

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

local function printSwizzle(f, swizzle, T, N, define)

	local swizzle_str = ""

	for i, x in pairs(swizzle) do
		swizzle_str = swizzle_str .. x
	end
	
	local return_type = "Matrix<" .. T .. ", " .. #swizzle .. ", 1>"
	local matrix_type = "Matrix<" .. T .. ", " .. N .. ", 1>"
	
	if not define then
	
		local function_decl = "__MYE_MATH_INLINE " .. return_type .. " " .. swizzle_str .. "(void) const;\n"
		f:write(function_decl)
		
	else
	
		local function_def = "__MYE_MATH_INLINE " .. return_type .. " " .. matrix_type .. "::" .. swizzle_str .. "(void) const { return " .. return_type .. "("
		
		if T == "T" then
		
			function_def = "template <typename T> " .. function_def
			
			for i, x in pairs(swizzle) do
			
				if i > 1 then
					function_def = function_def .. ", "
				end
				
				function_def = function_def .. "*(m_data + " .. swizzleMap[x] .. ")"
				
			end
		
		elseif T == "float" then
		
			function_def = function_def .. "_mm_shuffle_ps(m_vector, m_vector, _MM_SHUFFLE("
		
			local reversed = { }
		
			for i, x in pairs(swizzle) do
				reversed[#swizzle - i + 1] = swizzleMap[x]
			end
			
			for i = 1, 4 - #swizzle do
				
				function_def = function_def .. "0, "
				
			end
			
			for i, x in pairs(reversed) do
			
				if i > 1 then
					function_def = function_def .. ", "
				end
				
				function_def = function_def .. x	
				
			end
			
			function_def = function_def .. "))"
			
		end
		
		function_def = function_def .. "); }\n"	
		f:write(function_def)
		
	end

end

local function swizzleCombinations(f, elements, output, k, maxlen, T, N, define)

	if (k > maxlen) then
			printSwizzle(f, output, T, N, define)
		return
	end

	for i, x in pairs(elements) do
	
		output[k] = x
		swizzleCombinations(f, elements, output, k + 1, maxlen, T, N, define)
	
	end

end

for t, T in ipairs(types) do

	for i = 2, 4 do
	
		local f1 = io.open("SwizzleVector" .. i .. T:sub(1, 1) .. ".h", "w")
		local f2 = io.open("SwizzleVector" .. i .. T:sub(1, 1) .. ".inl", "w")
		
		f1:write("#pragma once\n")
		
		for j, x in pairs(vec_elements[i]) do
		
			local function_def  = "__MYE_MATH_INLINE const " .. T .. " & " .. x .. "(void) const { return *(m_data + " .. swizzleMap[x] .. "); }\n"
			local function_def2 = "__MYE_MATH_INLINE " .. T .. " & " .. x .. "(void) { return *(m_data + " .. swizzleMap[x] .. "); }\n"
			
			f1:write(function_def)
			f1:write(function_def2)
		
		end
		
		for l = 2, 4 do
		
			swizzleCombinations(f1, vec_elements[i], { }, 1, l, T, i, false)
			swizzleCombinations(f2, vec_elements[i], { }, 1, l, T, i, true)
		
		end
		
		f1:close()
	
	end

end