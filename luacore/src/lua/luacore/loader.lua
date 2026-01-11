local sys = require("luacore.sys")

local M = {}

function M.SrcLuaLoader(modname)
	local cwd = sys.GetCwd()
	if cwd:sub(cwd:len(), cwd:len()) == "/" then
		cwd = cwd:sub(0, cwd:len() - 1)
	end
	local modpath = modname:gsub("%.", "/")

	local paths = { cwd .. "/src/lua/" .. modpath .. ".lua", cwd .. "/src/lua/" .. modpath .. "/init.lua" }
	for _, path in ipairs(paths) do
		if sys.Exists(path) then
			local file, err = loadfile(path)
			return file or error(err)
		end
	end

	local path = cwd .. "/src/lua" .. modpath .. ".so"
	if sys.Exists(path) then
		local dash = modname:find("-", 1, true)
		local libname = dash and modname:sub(dash + 1) or modname
		local file, err = package.loadlib(path, "luaopen_" .. libname:gsub("%.", "_"))
		return file or error(err)
	end

	return nil
end

table.insert(package.loaders, 2, M.SrcLuaLoader)

return M
