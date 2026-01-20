local sys = require("luacore.sys")

local core = setmetatable({ loader = {} }, {
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
	error(fmt and fmt:format(...) or "", 2)
end

function core.Assert(expr, fmt, ...)
	if not expr then
		core.Panic(fmt, ...)
	end
end

function core.loader.LuaLoader(modname)
	local cwd = sys.GetCwd()
	if cwd:sub(#cwd) == "/" then
		cwd = cwd:sub(1, -2)
	end
	local modpath = modname:gsub("%.", "/")

	local paths = { cwd .. "/lua/" .. modpath .. ".lua", cwd .. "/lua/" .. modpath .. "/init.lua" }
	for _, path in ipairs(paths) do
		if sys.Access(path, "r") then
			local file, err = loadfile(path)
			return file or core.Panic("error loading module '%s' from file '%s':\n\t%s", modname, path, err)
		end
	end

	local path = cwd .. "/lua" .. modpath .. ".so" -- TODO: move to seperate LibLoader
	if sys.Access(path, "rx") then
		local dash = modname:find("-", 1, true)
		local libname = dash and modname:sub(dash + 1) or modname
		local file, err = package.loadlib(path, "luaopen_" .. libname:gsub("%.", "_"))
		return file or core.Panic("error loading module '%s' from file '%s':\n\t%s", modname, path, err)
	end

	return nil
end

table.insert(package.loaders, 2, core.loader.LuaLoader)

return core
