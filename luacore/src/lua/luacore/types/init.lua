local pack = table.pack or function(...)
	return { n = select("#", ...), ... }
end

local unpack = table.unpack or unpack

local core = require("luacore")

local types = setmetatable({}, {
	__index = function(self, modname)
		local ok, mod = pcall(require, "luacore.types." .. modname)
		if not ok then
			core.Panic(mod)
		end
		self[modname] = mod
		return mod
	end,
})

local function super(classmeta)
	return function(self, name)
		for k, v in pairs(classmeta.__methods) do
			if k == name then
				return function(...)
					if not classmeta.__supertype then
						return v(self, ...)
					end

					local save_super = self.super
					self.super = super(getmetatable(classmeta.__supertype))
					local ok, result = pcall(v, self, ...)
					self.super = save_super
					return ok and result or core.Panic(result)
				end
			end
		end
		return nil
	end
end

function types.Define(typedef)
	local methods = {}
	local attributes = {}
	for k, v in pairs(typedef) do
		if k == "static" or k == "constructor" then
			goto continue
		end

		if type(v) == "function" then
			methods[k] = v
		else
			attributes[k] = v
		end

		::continue::
	end

	local constructor = typedef.constructor or function(...) end

	local typeobject = typedef.static or {}
	local meta = { __index = methods, __type = typeobject }
	setmetatable(typeobject, {
		__call = function(_, ...)
			local instance = setmetatable({}, meta)

			for k, v in pairs(attributes) do
				instance[k] = v
			end

			local result = constructor(instance, ...)
			if result and typeobject(result) == "table" then
				instance = setmetatable(result, meta)
			end

			return instance
		end,
		__attributes = attributes,
		__methods = methods,
		__constructor = setmetatable(debug.getinfo(constructor, "u"), {
			__call = function(_, ...)
				return constructor(...)
			end,
		}),
	})
	return typeobject
end

function types.DefineSubType(suptype, subtypedef)
	local subtype = types.Define(subtypedef)

	local supmeta = getmetatable(suptype)
	local submeta = getmetatable(subtype)

	local nparams = 0
	--- @type table | nil
	local classmeta = supmeta
	while classmeta do
		nparams = nparams + classmeta.__constructor.nparams
		if classmeta.__supertype then
			classmeta = getmetatable(classmeta.__supertype)
		else
			classmeta = nil
		end
	end
	nparams = nparams - 1 -- receiver ('self')

	setmetatable(submeta.__methods, { __index = supmeta.__methods })
	local meta = {
		__index = submeta.__methods,
		__type = subtype,
	}
	submeta.__call = function(_, ...)
		local vargs = pack(...)

		local supargs = {}
		local subargs = {}

		table.move(vargs, 1, nparams, 1, supargs)
		table.move(vargs, nparams + 1, vargs.n, 1, subargs)

		local instance = setmetatable(suptype(unpack(supargs)), meta)

		for k, v in pairs(submeta.__attributes) do
			instance[k] = v
		end

		local result = submeta.__constructor(instance, unpack(subargs))
		if result and type(result) == "table" then
			instance = setmetatable(result, meta)
		end

		instance.super = super(supmeta)

		return instance
	end

	submeta.__supertype = suptype

	return subtype
end

function types.IsInstanceOf(o, type)
	if not o then
		return false
	end
	return getmetatable(o).__type == type
end

return types
