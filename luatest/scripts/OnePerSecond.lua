function Update(self, dt)

	local max = 3
	local threshold = 2

	if (self.n < max) then

		self.n = self.n + dt
		
		local n = self:Count()

		if (n ~= self.last_n) then

			self.last_n = n
			print(tostring(self), n)

		end

		if (n == threshold and not self.child:Exists()) then

			self.child = GameObjects:CreateEntity("OnePerSec")
			print(tostring(self), "->", tostring(self.child))

		end

	else

			print(tostring(self), "done")
			self:Destroy()

	end

end

function Count(self)

	return math.floor(self.n)

end