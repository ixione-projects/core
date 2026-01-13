local core = setmetatable({
	loader = require("luacore.loader"),
}, {
	__index = function(self, modname)
		local ok, mod = pcall(require, "luacore." .. modname)
		if not ok then
			self.Panic(mod)
		end
		self[modname] = mod
		return mod
	end,
})

function core.Panic(fmt, ...)
	error(fmt:format(...) or "", 2)
end

return core
