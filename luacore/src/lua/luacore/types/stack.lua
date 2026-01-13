local types = require("luacore").types

return types.Define({
	constructor = function(self)
		self.es = {}
	end,

	Push = function(self, e)
		table.insert(self.es, e)
	end,

	Pop = function(self)
		return table.remove(self.es)
	end,

	Top = function(self)
		return self.es[self:Size()]
	end,

	Size = function(self)
		return #self.es
	end,

	IsEmpty = function(self)
		return self:Size() == 0
	end,
})
