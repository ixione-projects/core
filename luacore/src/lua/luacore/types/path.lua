local core = require("luacore")

local sys = core.sys
local env = core.env
local types = core.types
local Stack = types.stack

local function Normalize(path)
	assert(type(path) == "string", "'path' must be of @type string")

	local is_absolute = path:sub(1, 1) == "/"
	if path:sub(#path) ~= "/" then
		path = path .. "/"
	end

	local parts = Stack()
	for part in path:gmatch("[^/]*/") do
		part = part:sub(1, -2) -- trailing '/'
		if #part == 0 then
			goto continue
		end

		if part == ".." then
			parts:Pop()
		elseif part ~= "." then
			parts:Push(part)
		end

		::continue::
	end

	local normalized = table.concat(parts.es, "/")
	if is_absolute then
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

	local ch = path:sub(1, 1)
	if ch == "~" then
		path = env.HOME .. "/" .. path
	elseif ch ~= "/" then
		path = env.Cwd() .. "/" .. path
	end

	return Normalize(path)
end

return types.Define({
	static = {
		Normalize = Normalize,
		Resolve = Resolve,
	},

	constructor = function(self, first, ...)
		self.path = Normalize(table.concat({ first or "", ... }, "/"))
	end,

	Resolve = function(self, ...)
		self.path = Resolve(self.path)
		for i, part in ipairs({ ... }) do
			if type(part) == "string" then
				self.path = Resolve(self.path, part)
			elseif types.IsInstanceOf(part, getmetatable(self).__type) then -- TODO: i would like path:IsInstanceOf(Path)
				self.path = Resolve(self.path, part.path)
			else
				core.Panic("bad argument #%i, must be of @type string | Path", i)
			end
		end
		return self
	end,

	Exists = function(self)
		return sys.Exists(self.path)
	end,

	IsAbsolute = function(self)
		return self.path:sub(1, 1) == "/"
	end,

	metatable = {
		__concat = function(self, other)
			-- TODO: define a topath ?
			assert(types.IsInstanceOf(other, getmetatable(self).__type), "'other' must be of @type Path") -- TODO: i would like path:IsInstanceOf(Path)
			return self.path .. other.path
		end,
		__len = function(self)
			return #self.path
		end,
	},
})
