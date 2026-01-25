local core = require("luacore")

local sys = core.sys
local env = core.env
local types = core.types
local Stack = types.stack

local function Normalize(path)
	assert(type(path) == "string", "'path' must be of @type string")

	local has_prefix = path:sub(1, 1) == "/"

	local parts = Stack()
	path = path .. "/"
	for part in path:gmatch("[^/]*/") do
		part = part:sub(1, -2) -- trailing '/'
		if #part == 0 then
			goto continue
		end

		if part == ".." then
			parts:PopBack()
		elseif part ~= "." then
			parts:PushBack(part)
		end

		::continue::
	end

	local normalized = table.concat(parts.es, "/")
	if has_prefix then
		normalized = "/" .. normalized
	end
	return normalized
end

local function Resolve(parent, child)
	assert(type(parent) == "string", "'parent' must be of @type string")

	local path = parent
	if child then
		assert(type(child) == "string", "'child' must be of @type string | nil")

		if child:sub(1, 1) == "/" then
			path = child
		elseif #child ~= 0 then
			path = path .. "/" .. child
		end
	end

	local prefix = path:sub(1, 1)
	if prefix == "~" then
		path = env.HOME .. "/" .. path
	elseif prefix ~= "/" then
		path = env.Cwd() .. "/" .. path
	end

	return Normalize(path)
end

return types.Define({
	static = {
		Normalize = Normalize,
		Resolve = Resolve,
		Canonicalize = require("luacore").canonicalize,
	},

	constructor = function(self, first, ...)
		self.path = Normalize(table.concat({ first or "", ... }, "/"))
	end,

	Resolve = function(self, ...)
		local path = Resolve(self.path)
		for i, part in ipairs({ ... }) do
			if type(part) == "string" then
				path = Resolve(path, part)
			elseif part.IsInstanceOf and part:IsInstanceOf(self:Type()) then
				path = Resolve(path, part.path)
			else
				core.Panic("bad argument #%i, must be of @type Path | string", i)
			end
		end
		self.path = path
		return self
	end,

	Components = function(self)
		if self.components then
			return self.components
		end

		local components = {}
		for part in self.path:gmatch("[^/]*/") do
			part = part:sub(1, -2) -- trailing '/'
			if #part == 0 then
				goto continue
			end

			table.insert(components, part)

			::continue::
		end
		self.components = components
		return components
	end,

	Exists = function(self)
		return sys.Access(self.path)
	end,

	IsAbsolute = function(self)
		return self.path:sub(1, 1) == "/"
	end,

	IsRelative = function(self)
		return not self:IsAbsolute()
	end,

	metatable = {
		__concat = function(left, right)
			if type(left) == "string" then
				return left .. tostring(right)
			end

			if type(right) == "string" then
				return left:Type()(left.path .. "/" .. right)
			elseif right.IsInstanceOf and right:IsInstanceOf(left:Type()) then
				return left:Type()(left.path .. "/" .. right.path)
			else
				core.Panic("attempt to concatenate a %s value", type(right))
			end
		end,
		__tostring = function(self)
			return self.path
		end,
	},
})
