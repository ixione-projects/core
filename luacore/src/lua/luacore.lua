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

function core.loader.SrcLuaLoader(modname)
	local cwd = sys.GetCwd()
	if cwd:sub(#cwd) == "/" then
		cwd = cwd:sub(1, -2)
	end
	local modpath = modname:gsub("%.", "/")

	local paths = { cwd .. "/src/lua/" .. modpath .. ".lua", cwd .. "/src/lua/" .. modpath .. "/init.lua" }
	for _, path in ipairs(paths) do
		if sys.Access(path, { Read = true }) then
			local file, err = loadfile(path)
			return file or core.Panic("error loading module '%s' from file '%s':\n\t%s", modname, path, err)
		end
	end

	local path = cwd .. "/src/lua" .. modpath .. ".so"
	if sys.Access(path, { Read = true, Exec = true }) then
		local dash = modname:find("-", 1, true)
		local libname = dash and modname:sub(dash + 1) or modname
		local file, err = package.loadlib(path, "luaopen_" .. libname:gsub("%.", "_"))
		return file or core.Panic("error loading module '%s' from file '%s':\n\t%s", modname, path, err)
	end

	return nil
end

table.insert(package.loaders, 2, core.loader.SrcLuaLoader)

return core
