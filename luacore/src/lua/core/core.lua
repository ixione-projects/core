local sys = require("core.sys")

local function src_lua_loader(modname)
	local cwd = lfs.currentdir()
	if cwd:sub(cwd:len(), cwd:len()) == "/" then
		cwd = cwd:sub(0, cwd:len() - 1)
	end
	local modpath = modname:gsub("%.", "/")

	local paths = { cwd .. "/src/lua/" .. modpath .. ".lua", cwd .. "/src/lua/" .. modpath .. "/init.lua" }
	for _, path in ipairs(paths) do
		if lfs.attributes(path) then
			local file, err = loadfile(path)
			return file or error(err)
		end
	end

	local path = cwd .. "/src/lua" .. modpath .. ".so"
	if lfs.attributes(path) then
		local dash = modname:find("-", 1, true)
		local libname = dash and modname:sub(dash + 1) or modname
		local file, err = package.loadlib(path, "luaopen_" .. libname:gsub("%.", "_"))
		return file or error(err)
	end

	return nil
end

table.insert(package.loaders, 2, src_lua_loader)
