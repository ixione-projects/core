local Path = require("luacore").types.path

describe("Path", function()
	local cwd = require("luacore").env.Cwd()
	it("should resolve relative paths to the current working directory", function()
		assert.are.equals(cwd .. "/src/index.js", Path("src", "index.js"):Resolve().path)
		assert.are.equals(cwd .. "/test.js", Path("./test.js"):Resolve().path)
		assert.are.equals(cwd .. "/a/b", Path("a", "", "b"):Resolve().path)
	end)

	it("should resolve absolute paths by ignoring previous segments", function()
		assert.are.equals("/home/user", Path("/etc"):Resolve("nginx", "/home/user").path)
		assert.are.equals("/var/log", Path():Resolve("tmp", "/var/log").path)
	end)

	it("should resolve empty paths to the current working directory", function()
		assert.are.equals(cwd, Path():Resolve().path)
	end)

	it('should process ".." (parent directory) segments correctly', function()
		assert.are.equals("/a/b", Path("/a/b/c", "..").path)
		assert.are.equals("/a/d", Path("/a/b/c", "../../d").path)
	end)

	it('should process "." (current directory) segments by ignoring them', function()
		assert.are.equals("/a/b/c", Path("/a/b", ".", "c").path)
		assert.are.equals("test.js", Path("./test.js").path)
	end)

	it('should process "" segments by ignoring them', function()
		assert.are.equals("a/b", Path("a", "", "b").path)
	end)

	it("should normalize multiple slashes and remove trailing slashes", function()
		assert.are.equals("/a/b/c", Path("//a/b///", "c//").path)
	end)
end)
