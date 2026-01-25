local types = require("luacore").types

return types.Define({
	constructor = function(self)
		self.es = {}
	end,

	PushBack = function(self, e)
		table.insert(self.es, e)
	end,

	PopBack = function(self)
		return table.remove(self.es)
	end,

	Back = function(self)
		return self.es[self:Size()]
	end,

	Size = function(self)
		return #self.es
	end,

	IsEmpty = function(self)
		return self:Size() == 0
	end,
})
