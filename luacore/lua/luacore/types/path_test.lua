local Path = require("luacore").types.path

describe("Path", function()
	local cwd = require("luacore").env.Cwd()
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

	it("should concatenate and normalize the two paths", function()
		assert.are.equals("/a/b/c", (Path("//a/b///") .. Path("c//")).path)
	end)

	describe(".Canonicalize", function()
		-- TODO: these test fail if run on anyone elses machine
		it("should resolve symbolic links recursively to their final path", function()
			assert.are.equals("/usr/lib/cargo/bin/fd", Path.Canonicalize("/usr/local/bin/fd"))
		end)

		it("should normalize paths before reseolving them", function()
			assert.are.equals("/home", Path.Canonicalize("//./../././../../..//./home"))
		end)

		it("should resolve relative paths (including empty paths) to the current working directory", function()
			assert.are.equals(cwd .. "/" .. "CMakeLists.txt", Path.Canonicalize("CMakeLists.txt"))
			assert.are.equals(cwd, Path.Canonicalize(""))
		end)
	end)

	describe(":Resolve", function()
		it("should resolve relative paths to the current working directory", function()
			assert.are.equals(cwd .. "/src/index.js", Path("src", "index.js"):Resolve().path)
			assert.are.equals(cwd .. "/test.js", Path("./test.js"):Resolve().path)
			assert.are.equals(cwd .. "/a/b", Path("a", "", "b"):Resolve().path)
		end)

		it("should resolve absolute paths by ignoring previous segments", function()
			assert.are.equals("/home/user", Path("/etc", "nginx"):Resolve("/home/user").path)
			assert.are.equals("/var/log", Path():Resolve("tmp", "/var/log").path)
		end)

		it("should resolve empty paths to the current working directory", function()
			assert.are.equals(cwd, Path():Resolve().path)
		end)
	end)
end)
