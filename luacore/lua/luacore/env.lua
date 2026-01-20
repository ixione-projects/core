local sys = require("luacore").sys

local env = setmetatable({}, {
	__index = function(self, varname)
		local var = os.getenv(varname)
		if var then
			self[varname] = var
		end
		return var
	end,
})

function env.Cwd()
	return sys.GetCwd()
end

return env
