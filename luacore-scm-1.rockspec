rockspec_format = "3.0"

package = "luacore"
version = "scm-1"

source = {
	url = "https://github.com/ixione-projects/core/archive/refs/heads/master.zip",
}

description = {
	summary = "Core Lua Library",
	detailed = [[
      Core is a Lua library developed to provide a common set of Lua function. 
      Core offers the following modules:
        core.object: module to support object-oriented practices
        core.sys:    module to access system calls
   ]],
	homepage = "https://github.com/ixione-projects/core/luacore#readme",
	license = "MIT",
}

dependencies = {
	"lua >= 5.1",
}

test_dependencies = {
	"busted",
}

build = {
	type = "cmake",
	variables = {
		BUILD_LUA = "ON",
		CMAKE_GENERATOR = "Ninja",
		LUA = "$(LUA)",
		PREFIX = "$(PREFIX)",
		LUADIR = "$(LUADIR)",
		CMAKE_FIND_DEBUG_MODE = "ON",
	},
}

test = {
	type = "busted",
}
