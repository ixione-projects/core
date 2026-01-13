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
        luacore.loader:     module to support convenient module resolution
        luacore.env:        module to support manipulating the environment
        luacore.types:      module to support object-oriented practices
        luacore.types.path: module to support unix path parsing

        luacore.sys:        module to support access to system calls
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
		CMAKE_INSTALL_PREFIX = "$(PREFIX)",
		CMAKE_BUILD_TYPE = "Release",

		LUA = "$(LUA)",
		LUADIR = "$(LUADIR)",
	},
}

test = {
	type = "busted",
}
